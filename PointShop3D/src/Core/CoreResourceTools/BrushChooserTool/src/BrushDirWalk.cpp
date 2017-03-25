// Title:   BrushDirWalk.cpp
// Created: Thu Sep 25 14:22:43 2003
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
#include <qgrid.h>

#include "../../../../Utilities/DirWalk/src/DirWalk.h"
#include "../../../../Utilities/src/FileTools.h"
#include "BrushDirWalk.h"


// *********
// constants
// *********

// each time the registry vector is full, MORE_REGISTRY_ELEMENTS are additionally allocated 
const uint BrushDirWalk::MORE_REGISTRY_ELEMENTS = 16;



// **************
// public methods
// **************

BrushUserData::BrushUserData (std::vector<Brush*> *registry, QPopupMenu *popupMenu, QObject *signalReceiver, const char *signalHandler, 
							  unsigned int bitmapWidth, unsigned int bitmapHeight, bool showBitmaps, bool showNames, bool useAlphaMasks) {

	this->registry       = registry;
	this->popupMenu      = popupMenu;
	this->signalReceiver = signalReceiver;
	this->signalHandler  = signalHandler;
	this->bitmapWidth    = bitmapWidth;
	this->bitmapHeight   = bitmapHeight;
	this->showBitmaps    = showBitmaps;
	this->showNames      = showNames;
	this->useAlphaMasks  = useAlphaMasks;


}

BrushUserData::~BrushUserData() {}

UserDataInterface *BrushUserData::copy() {
	return new BrushUserData (registry, popupMenu, signalReceiver, signalHandler, bitmapWidth, bitmapHeight, showBitmaps, showNames, useAlphaMasks);
}

void BrushUserData::copy (UserDataInterface *source) {

	BrushUserData *brushUserData;

	brushUserData = (BrushUserData *)source;

	popupMenu      = brushUserData->popupMenu;
	registry       = brushUserData->registry;
	signalReceiver = brushUserData->signalReceiver;
	signalHandler  = brushUserData->signalHandler;
	bitmapWidth    = brushUserData->bitmapWidth;
	bitmapHeight   = brushUserData->bitmapHeight;
	showBitmaps    = brushUserData->showBitmaps;
	showNames      = brushUserData->showNames;

}

BrushDirWalk::BrushDirWalk (QString BrushDirectory)
: DirWalk (BrushDirectory, QDir::Files | QDir::Dirs | QDir::NoSymLinks, QDir::Name | QDir::DirsFirst) {}

BrushDirWalk::~BrushDirWalk() {}

DirWalk *BrushDirWalk::createNewInstance (const QString directoryName) {
	return new BrushDirWalk (directoryName);
}

void BrushDirWalk::applyFileMethod() {

	BrushUserData *brushUserData;
	Brush *brush;
	int id;
	std::vector<Brush*> *registry;

	brushUserData = (BrushUserData *)userData;	

	// load .pbd (pointshop brush descriptor) files only
	if(FileTools::getSuffix (currentFileName) == "pbd") {

		brush = new Brush();

		QPixmap bitmapPixmap;


		// try to generate a brush from the .pbd file
		if ( brush->load(currentDirectory, currentFileName) != -1 ) {
		
			// generate the id according to the number of already stored brushes in the registry
			registry = brushUserData->registry;
			id = registry->size();

			brush->setMenuId (id);
			brush->setPopupMenu (brushUserData->popupMenu);
	
			QIconSet menuIcon;
			QString menuName;

			if (brushUserData->showBitmaps) {
				QImage *previewImage = brush->createPreviewImage (brushUserData->bitmapWidth, brushUserData->bitmapHeight, brushUserData->useAlphaMasks);
				bitmapPixmap.convertFromImage (*previewImage);
				delete previewImage;
				
				menuIcon.setPixmap (bitmapPixmap, QIconSet::Small, QIconSet::Normal);
				menuIcon.setPixmap (bitmapPixmap, QIconSet::Large, QIconSet::Normal);			
			}

			if (brushUserData->showNames) {
				menuName = brush->getName();
			}
			else {
				menuName = "";
			}

			brushUserData->popupMenu->insertItem (menuIcon, menuName, brushUserData->signalReceiver,
	   											  brushUserData->signalHandler, 0, id);
		


			registry->push_back (brush);

		} else {
			delete brush;
		}
	}
}

void BrushDirWalk::applyDirectoryMethod() {

	QPopupMenu *popupMenu;

	BrushUserData *brushUserData;

	brushUserData = (BrushUserData *)userData;

	popupMenu = new QPopupMenu (brushUserData->popupMenu);
	popupMenu->insertTearOffHandle();
	brushUserData->popupMenu->insertItem (currentFileName, popupMenu);

	// modify user data
	brushUserData->popupMenu = popupMenu;
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
