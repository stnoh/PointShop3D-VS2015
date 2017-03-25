// Title:   BitmapResourceTool.cpp
// Created: Thu Sep 25 14:22:49 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003 Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

#include <limits.h>

#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qwhatsthis.h>

#include "BitmapDirWalk.h"

#include "BitmapResourceTool.h"

// *********
// constants
// *********

const uint BitmapResourceTool::NO_BITMAPS = UINT_MAX;

// **************
// public methods
// **************

BitmapResourceTool::BitmapResourceTool (const QString bitmapDirectory, const BitmapUserData::BitmapProperties bitmapProperties) {
	
	currentBitmapID        = NO_BITMAPS;
	currentBitmap          = 0;
	this->bitmapDirectory  = bitmapDirectory;
	this->bitmapProperties = bitmapProperties;

}

BitmapResourceTool::~BitmapResourceTool() {

	clearRegistry();

}

void BitmapResourceTool::clearRegistry() {

	std::vector<QImage*>::iterator iter_images;

	for (iter_images = registry.begin(); iter_images != registry.end(); ) {
		QImage *image = *iter_images;
		++iter_images;
		delete image;
	}

	registry.clear();
}

QString BitmapResourceTool::getBitmapDirectory() const {
	return bitmapDirectory;
}

void BitmapResourceTool::setBitmapProperties (const BitmapUserData::BitmapProperties newBitmapProperties) {
	bitmapProperties = newBitmapProperties;
	this->updateBitmaps();
}

BitmapUserData::BitmapProperties BitmapResourceTool::getBitmapProperties() const {
	return bitmapProperties;
}

uint BitmapResourceTool::getNofBitmaps() const {
	return registry.size();
}

void BitmapResourceTool::setBitmapSelected (uint id) {

	if (registry.size() > 0) {

		if (id >= registry.size()) {
			id = registry.size() - 1;
		}

		currentBitmapID = id;
		currentBitmap   = registry.at (currentBitmapID);
		emit bitmapChanged (currentBitmap);

	}

}

const QImage *BitmapResourceTool::getSelectedBitmap() const {
	return currentBitmap;
}

uint BitmapResourceTool::getSelectedBitmapID() const {
	return currentBitmapID;
}

QToolButton *BitmapResourceTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;

	bitmapButton = new QToolButton (iconSet, "Bitmap Resource Tool", "Choose a bitmap",
	                                0, 0, toolBar, "Bitmap Resource Tool");
	

	// initialize the bitmap menu, check if any bitmaps are available
	this->updateBitmaps();
	if (registry.size() == 0) {
		bitmapButton->setEnabled (false);
	}	
	else {
		bitmapButton->setEnabled (true);
	}


	bitmapButton->setPopup (&popupMenu);
	// instant reaction of popup menu
	bitmapButton->setPopupDelay(0);
	return bitmapButton;

}

void BitmapResourceTool::addMenuToPushButton (QPushButton *pushButton) {

	this->updateBitmaps();
	if (registry.size() == 0) {
		pushButton->setEnabled (false);
	}	
	else {
		pushButton->setEnabled (true);
	}

	pushButton->setPopup (&popupMenu);

}

// ************
// public slots
// ************

void BitmapResourceTool::handleBitmapDirectoryChanged (const QString newBitmapDirectory) {
	bitmapDirectory = newBitmapDirectory;
	this->updateBitmaps();
}

// ***************
// private methods
// ***************

void BitmapResourceTool::updateBitmaps() {

	BitmapDirWalk  bitmapDirWalk (bitmapDirectory);
	BitmapUserData bitmapUserData (bitmapProperties, &registry, &popupMenu, this, SLOT (handleBitmapSelected (int)));

	// remove the old entries
	popupMenu.clear();
	clearRegistry();

	popupMenu.insertTearOffHandle();
	popupMenu.setCaption ("Bitmaps");

	if ((bitmapDirWalk.execute (&bitmapUserData) == true) && (registry.size() > 0)) {
		// select the first available bitmap as current bitmap
		currentBitmapID = 0;
		currentBitmap   = registry.at (currentBitmapID);
	}
	else {
		currentBitmapID = NO_BITMAPS;
		currentBitmap   = 0;		
	}


	// bitmap may be 0
	emit bitmapChanged (currentBitmap);

}

// *************
// private slots
// *************

void BitmapResourceTool::handleBitmapSelected (int id) {

	currentBitmap   = registry.at (id);
	currentBitmapID = id;
	emit bitmapChanged (currentBitmap);

}



// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
