// Title:   BitmapDirWalk.cpp
// Created: Thu Sep 25 14:22:51 2003
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

#include <qstring.h>
#include <qimage.h>
#include <qpopupmenu.h>
#include <qobject.h>

#include "../../../../Utilities/DirWalk/src/DirWalk.h"
#include "BitmapDirWalk.h"

// *********
// constants
// *********

// each time the registry vector is full, MORE_REGISTRY_ELEMENTS are additionally allocated 
static const uint MORE_REGISTRY_ELEMENTS = 16;

// the maximum width / height of a pixmap to be inserted into the menu
static const int MAX_SIZE = 64;

// **************
// public methods
// **************

BitmapUserData::BitmapUserData (BitmapProperties bitmapProperties, std::vector<QImage*> *registry, QPopupMenu *popupMenu, QObject *signalReceiver, const char *signalHandler) {

	this->bitmapProperties = bitmapProperties;
	this->registry         = registry;
	this->popupMenu        = popupMenu;
	this->signalReceiver   = signalReceiver;
	this->signalHandler    = signalHandler;

}

BitmapUserData::~BitmapUserData() {}

UserDataInterface *BitmapUserData::copy() {
	return new BitmapUserData (bitmapProperties, registry, popupMenu, signalReceiver, signalHandler);
}

void BitmapUserData::copy (UserDataInterface *source) {

	BitmapUserData *bitmapUserData;

	bitmapUserData = (BitmapUserData *)source;

	bitmapProperties = bitmapUserData->bitmapProperties;
	popupMenu        = bitmapUserData->popupMenu;
	registry         = bitmapUserData->registry;
	signalReceiver   = bitmapUserData->signalReceiver;
	signalHandler    = bitmapUserData->signalHandler;

}

BitmapDirWalk::BitmapDirWalk (QString BitmapDirectory)
: DirWalk (BitmapDirectory, QDir::Files | QDir::Dirs | QDir::NoSymLinks, QDir::Name | QDir::DirsFirst) {}

BitmapDirWalk::~BitmapDirWalk() {}

DirWalk *BitmapDirWalk::createNewInstance (const QString directoryName) {
	return new BitmapDirWalk (directoryName);
}

void BitmapDirWalk::applyFileMethod() {

	BitmapUserData                   *bitmapUserData;
	QImage                           *bitmapImage,
		                             *scaledBitmapImage;
	QPixmap                          bitmapPixmap;
	int                              id;
	std::vector<QImage*>              *registry;
	int                              depth,
		                             scaledWidth,
									 scaledHeight;
	BitmapUserData::BitmapProperties bitmapProperties;
	QString                          filePath;
	bool                             isGrayScale;
	QSize                            bitmapSize;

	bitmapUserData = (BitmapUserData *)userData;

	bitmapProperties = bitmapUserData->bitmapProperties;
	bitmapImage      = new QImage();

	filePath = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1();
	
	if (bitmapImage->load (filePath) == true) {
		
		// check the depth of the image
		depth = bitmapImage->depth();

		// check the colors of the image
		isGrayScale = bitmapImage->isGrayscale();
		if (((depth == 1)           && (bitmapProperties & BitmapUserData::DEPTH_1)   ||
			 (depth == 8)           && (bitmapProperties & BitmapUserData::DEPTH_8)   ||
			 (depth == 16)          && (bitmapProperties & BitmapUserData::DEPTH_16)  ||
			 (depth == 32)          && (bitmapProperties & BitmapUserData::DEPTH_32)) &&
			((isGrayScale == true)  && (bitmapProperties & BitmapUserData::GRAYSCALE) ||
			 (isGrayScale == false) && (bitmapProperties & BitmapUserData::COLOR))) {

			// make sure the pixmap is small enough, so it fits nicely into the menu
			bitmapSize = bitmapImage->size();
			if (bitmapSize.width() > bitmapSize.height()) {

				if (bitmapSize.width() > MAX_SIZE) {
					// scale
					scaledWidth  = MAX_SIZE;
					scaledHeight = (int)(((float)MAX_SIZE / (float)bitmapSize.width()) * bitmapSize.height() + 0.5f);
					scaledBitmapImage = new QImage (scaledWidth, scaledHeight, depth);
					*scaledBitmapImage = bitmapImage->smoothScale (scaledWidth, scaledHeight);
					bitmapPixmap.convertFromImage (*scaledBitmapImage);
					delete scaledBitmapImage;

				}
				else {
					// bitmap is small enough			
					bitmapPixmap.convertFromImage (*bitmapImage);
				}

			}
			else {

				if (bitmapSize.height() > MAX_SIZE) {
					// scale
					scaledWidth  = (int)(((float)MAX_SIZE / (float)bitmapSize.height()) * bitmapSize.width() + 0.5f);
					scaledHeight = MAX_SIZE;
					scaledBitmapImage = new QImage (scaledWidth, scaledHeight, depth);
					*scaledBitmapImage = bitmapImage->smoothScale (scaledWidth, scaledHeight);
					bitmapPixmap.convertFromImage (*scaledBitmapImage);
					delete scaledBitmapImage;
				}
				else {
					// bitmap is small enough			
					bitmapPixmap.convertFromImage (*bitmapImage);
				}

			}

			// generate the id according to the number of already stored bitmaps in the registry
			registry = bitmapUserData->registry;
			id = registry->size();
			bitmapUserData->popupMenu->insertItem (bitmapPixmap,
												   bitmapUserData->signalReceiver,
		   										   bitmapUserData->signalHandler,
												   0, id);

			// now store the bitmap in the registry, allocate space first if needed
			registry->push_back(bitmapImage);
			
		}
		else {
			delete bitmapImage;
		}
		
	}
	else {
		delete bitmapImage;
	}

}

void BitmapDirWalk::applyDirectoryMethod() {

	QPopupMenu *popupMenu;

	BitmapUserData *bitmapUserData;

	bitmapUserData = (BitmapUserData *)userData;

	popupMenu = new QPopupMenu (bitmapUserData->popupMenu);
	popupMenu->insertTearOffHandle();
	bitmapUserData->popupMenu->insertItem (currentFileName, popupMenu);

	// modify user data
	bitmapUserData->popupMenu = popupMenu;


}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
