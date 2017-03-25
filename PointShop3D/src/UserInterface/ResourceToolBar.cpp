// Title:   ResourceToolBar.cpp
// Created: Thu Sep 25 14:43:01 2003
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

#include <qtoolbar.h>
#include <qmainwindow.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

#include "../Core/Configuration/src/Configuration.h"
#include "../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "../Core/CoreResourceTools/BitmapResourceTool/src/BitmapDirWalk.h"
#include "../Core/CoreResourceTools/BitmapManagerTool/src/BitmapManagerTool.h"
#include "../Core/CoreResourceTools/ColorChooserTool/src/ColorChooserTool.h"

#include "ResourceToolBar.h"


// **************
// public methods
// **************

ResourceToolBar::ResourceToolBar (const QString &label,
		                          QMainWindow *mainWindow,
					              QMainWindow::ToolBarDock toolBarDock,
					              bool newLine,
					              const char *name)
								  : QToolBar (label, mainWindow, toolBarDock, newLine, name) {

	colorChooserTool                 = 0;
	bitmapManagerTool                = 0;
	brushChooserTool				 = 0;

}

ResourceToolBar::~ResourceToolBar() {
	this->cleanUp();	
}

void ResourceToolBar::initialize() {
	this->cleanUp();
	this->addDefaultTools();
}

// ***************
// private methods
// ***************

void ResourceToolBar::addDefaultTools() {

	CoreResourceTools *coreResourceTools;
	Configuration     *configuration;

	configuration = Configuration::getInstance();


	// ***********************
	// setup the default tools
	// ***********************

	// *****************
	// create core tools
	// *****************


	// Color Chooser Tool
	colorChooserTool = new ColorChooserTool();
	colorChooserTool->addButtonToToolBar (this);


	// bitmap manager tool
	bitmapManagerTool = new BitmapManagerTool();
	bitmapManagerTool->addButtonToToolBar (this);

	// brush chooser tool
	brushChooserTool = new BrushChooserTool (configuration->getBrushDirectory());
	brushChooserTool->addButtonToToolBar (this);

	// ******************
	// install core tools
	// ******************

	coreResourceTools = CoreResourceTools::getInstance();
	coreResourceTools->setColorChooserTool (colorChooserTool);		
	coreResourceTools->setBitmapManagerTool (bitmapManagerTool);
	coreResourceTools->setBrushChooserTool (brushChooserTool);

	// let the brushChooserTool know whenever the brush directory has changed
	this->connect (configuration, SIGNAL (brushDirectoryChanged (const QString)),
		           brushChooserTool, SLOT (handleBrushDirectoryChanged (const QString)));



}

void ResourceToolBar::cleanUp() {

	if (colorChooserTool != 0) {
		delete colorChooserTool;
		colorChooserTool = 0;
	}

	if (bitmapManagerTool != 0) {
		delete bitmapManagerTool;
		bitmapManagerTool = 0;
	}

	if (brushChooserTool != 0) {
		delete brushChooserTool;
		brushChooserTool = 0;
	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
