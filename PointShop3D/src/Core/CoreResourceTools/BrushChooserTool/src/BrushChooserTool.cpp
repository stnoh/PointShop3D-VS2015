// Title:   BrushChooserTool.cpp
// Created: Thu Sep 25 14:22:46 2003
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
#include <qiconset.h>

#include "BrushDirWalk.h"

#include "BrushChooserTool.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"brush\"> "
"Choose a brush.";

// *********
// constants
// *********

const uint BrushChooserTool::NO_BRUSHES = UINT_MAX;

// **************
// public methods
// **************

BrushChooserTool::BrushChooserTool (const QString brushDirectory) {
	
	currentBrushID        = NO_BRUSHES;
	currentBrush          = 0;
	this->brushDirectory  = brushDirectory;

	bitmapWidth = 32;
	bitmapHeight = 32;
	showBitmaps = true;
	showNames = true;
	useAlphaMasks = true;

}


BrushChooserTool::~BrushChooserTool() {
	
	clearRegistry();
}

void BrushChooserTool::clearRegistry() {

	std::vector<Brush*>::iterator iter_brushes;

	for (iter_brushes = registry.begin(); iter_brushes != registry.end(); ) {
		Brush *brush = *iter_brushes;
		++iter_brushes;
		delete brush;
	}

	registry.clear();
}


QString BrushChooserTool::getBrushDirectory() const {
	return brushDirectory;
}

uint BrushChooserTool::getNofBrushes() const {
	return registry.size();
}

void BrushChooserTool::setBrushSelected (uint id) {

	if (registry.size() > 0) {

		if (id >= registry.size()) {
			id = registry.size() - 1;
		}

		currentBrushID = id;
		currentBrush   = registry.at (currentBrushID);
		emit brushChanged (currentBrush);

	}

}

Brush *BrushChooserTool::getSelectedBrush() {
	return currentBrush;
}

uint BrushChooserTool::getSelectedBrushID() const {
	return currentBrushID;
}

void BrushChooserTool::addAndSelectBrush (Brush *brush) {

	// add brush to first level popup menu
	uint id = registry.size();

	QIconSet menuIcon;
	QString menuText;

	if (showBitmaps) {
		QPixmap bitmapPixmap;
		QImage *previewImage = brush->createPreviewImage (bitmapWidth, bitmapHeight, useAlphaMasks);
		bitmapPixmap.convertFromImage (*previewImage);
		delete previewImage;
	
		menuIcon.setPixmap (bitmapPixmap, QIconSet::Small, QIconSet::Normal);
		menuIcon.setPixmap (bitmapPixmap, QIconSet::Large, QIconSet::Normal);			
	}

	if (showNames) {
		menuText = brush->getName();
	}
	else {
		menuText = "";
	}


	if(brush->getPopupMenu() == 0) {
		popupMenu.insertItem (menuIcon, menuText, this, SLOT (handleBrushSelected (int)), 0, id);
		brush->setPopupMenu (&popupMenu);
	}
	else {
		brush->getPopupMenu()->insertItem (menuIcon, menuText, this, SLOT (handleBrushSelected (int)), 0, id);
	}
	brush->setMenuId (id);
	
	registry.push_back(brush);

	// save the brush on disk
	brush->save (this->getBrushDirectory());

	// enable the tool button since now at least one brush is available
	brushButton->setEnabled (true);

	// select new brush, this method also emits the brushChanged signal
	this->handleBrushSelected (id);

}


void BrushChooserTool::setDisplayParameters (unsigned int bitmapWidth,
											 unsigned int bitmapHeight,
											 bool showBitmaps,
											 bool showNames,
											 bool useAlphaMasks) {

	if ((this->bitmapWidth != bitmapWidth) ||
		(this->bitmapHeight != bitmapHeight) ||
		(this->showBitmaps != showBitmaps) ||
		(this->showNames != showNames) ||
		(this->useAlphaMasks != useAlphaMasks)) {

		this->bitmapWidth = bitmapWidth;
		this->bitmapHeight = bitmapHeight;
		
		// display at least names
		if ((showBitmaps == false) && (showNames == false)) {
			showNames = true;
		}

		this->showBitmaps = showBitmaps;
		this->showNames = showNames;
		this->useAlphaMasks = useAlphaMasks;

		updateBrushes();
	}
}


void BrushChooserTool::getDisplayParameters (unsigned int &bitmapWidth,
											 unsigned int &bitmapHeight,
											 bool &showBitmaps,
											 bool &showNames,
											 bool &useAlphaMasks) {

	bitmapWidth   = this->bitmapWidth;
	bitmapHeight  = this->bitmapHeight;
	showBitmaps   = this->showBitmaps;
	showNames     = this->showNames;
	useAlphaMasks = this->useAlphaMasks;
}



void BrushChooserTool::emitBrushChanged() {
	emit brushChanged (currentBrush);
}

QToolButton *BrushChooserTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	QPixmap  normalIcon ("Resources//BrushChooserToolIcon.png"),
		     disabledIcon ("Resources//BrushChooserToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);

	brushButton = new QToolButton (iconSet, "Brush Chooser Tool", "Choose a brush",
	                               0, 0, toolBar, "Brush Chooser Tool");

	QWhatsThis::add (brushButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("brush", normalIcon);
	

	// initialize the brush menu, check if any brushs are available
	this->updateBrushes();
	if (registry.size() == 0) {
		brushButton->setEnabled (false);
	}	
	else {
		brushButton->setEnabled (true);
	}

	brushButton->setPopup (&popupMenu);
	// instant reaction of popup menu
	brushButton->setPopupDelay(0);
	return brushButton;
}

void BrushChooserTool::addMenuToPushButton (QPushButton *pushButton) {

	this->updateBrushes();
	if (registry.size() == 0) {
		pushButton->setEnabled (false);
	}	
	else {
		pushButton->setEnabled (true);
	}

	pushButton->setPopup (&popupMenu);

}

void BrushChooserTool::updateBrushBitmap (Brush* brush) {

	QIconSet menuIcon;
	QString  menuText;

	if (showBitmaps) {
		QPixmap bitmapPixmap;
		QImage *previewImage = brush->createPreviewImage (bitmapWidth, bitmapHeight, useAlphaMasks);
		bitmapPixmap.convertFromImage (*previewImage);
		delete previewImage;
		
		menuIcon.setPixmap (bitmapPixmap, QIconSet::Small, QIconSet::Normal);
		menuIcon.setPixmap (bitmapPixmap, QIconSet::Large, QIconSet::Normal);			
	}
	
	if (showNames) {
		menuText = brush->getName();
	}
	else {
		menuText = "";
	}

	popupMenu.changeItem (brush->getMenuId(), menuIcon, menuText);

	emitBrushChanged();
}

// ************
// public slots
// ************

void BrushChooserTool::handleBrushDirectoryChanged (const QString newBrushDirectory) {
	brushDirectory = newBrushDirectory;
	this->updateBrushes();
}

// ***************
// private methods
// ***************

void BrushChooserTool::updateBrushes() {

	BrushDirWalk  brushDirWalk (brushDirectory);
	BrushUserData brushUserData (&registry, &popupMenu, this, SLOT (handleBrushSelected (int)),
		this->bitmapWidth, this->bitmapHeight, this->showBitmaps, this->showNames, this->useAlphaMasks);

	// remove the old entries
	popupMenu.clear();
	clearRegistry();

	popupMenu.insertTearOffHandle();
	popupMenu.setCaption ("Brushes");

	if ((brushDirWalk.execute (&brushUserData) == true) && (registry.size() > 0)) {
		// select the first available brush as current brush
		currentBrushID = 0;
		currentBrush   = registry.at (currentBrushID);
		brushButton->setEnabled (true);
	}
	else {
		currentBrushID = NO_BRUSHES;
		currentBrush   = 0;
		brushButton->setEnabled (false);
	}

	// brush may be 0
	emit brushChanged (currentBrush);

}

// *************
// private slots
// *************

void BrushChooserTool::handleBrushSelected (int id) {

	currentBrush   = registry.at (id);
	currentBrushID = id;
	emit brushChanged (currentBrush);
}





// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
