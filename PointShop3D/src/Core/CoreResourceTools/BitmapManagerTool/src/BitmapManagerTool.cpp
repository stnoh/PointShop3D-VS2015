// Title:   BitmapManagerTool.cpp
// Created: Thu Sep 25 14:23:05 2003
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
#include <qpainter.h>
#include <qstring.h>
#include <qstringlist.h>



#include "BitmapManagerDialogImpl.h"
#include "BitmapManagerTool.h"

#include "../../../CoreResourceTools/src/CoreResourceTools.h"
#include "../../../Scene/src/Scene.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"bitmap\"> "
"Choose and combine the resource bitmaps.";

// **************
// public methods
// **************

BitmapManagerTool::BitmapManagerTool() {

	CoreResourceTools *coreResourceTools;

	coreResourceTools = CoreResourceTools::getInstance();


}

BitmapManagerTool::~BitmapManagerTool() {
	BitmapManagerDialogImpl::destroyInstance();
}

// **************
// public methods
// **************

QToolButton *BitmapManagerTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	QPixmap  normalIcon ("Resources/BitmapManagerToolIcon.png"),
		     disabledIcon ("Resources/BitmapManagerToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);
	bitmapButton = new QToolButton (iconSet, "Bitmap Manager Tool", "Choose a set of bitmaps as brush",
	                                0, 0, toolBar, "Bitmap  Manager Tool");
	QWhatsThis::add (bitmapButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("bitmap", normalIcon);
	bitmapButton->connect (bitmapButton, SIGNAL (clicked()),
		                   this, SLOT (handleBitmapManagerDialog()));

	return bitmapButton;

}

void BitmapManagerTool::addMenuToPushButton (QPushButton *pushButton) { (void)pushButton; }


void BitmapManagerTool::readBrushBitmaps () {

	// calling getInstance for the first time calls the constructor that
	// reads all the brush bitmaps and connects signals
	BitmapManagerDialogImpl *bitmapManagerDialog = BitmapManagerDialogImpl::getInstance ();
	(void)bitmapManagerDialog;
}


// *************
// private slots
// *************

void BitmapManagerTool::handleBitmapManagerDialog() {

	BitmapManagerDialogImpl *bitmapManagerDialog;

	bitmapManagerDialog = BitmapManagerDialogImpl::getInstance ();
	bitmapManagerDialog->show();
	bitmapManagerDialog->raise();

}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
