// Title:   ToolDirWalk.cpp
// Created: Thu Sep 25 14:10:50 2003
// Authors:  Oliver Knoll, Mark Pauly, Matthias Zwicker
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
#include <qtoolbutton.h>
#include <qbuttongroup.h>

#include "../../../Utilities/DirWalk/src/DirWalk.h"
#include "../../../Utilities/UserMessages/src/SplashScreen.h"
#include "../../RendererManager/src/RendererManager.h"
#include "../../Interfaces/src/ToolInterface.h"
#include "ToolLoader.h"
#include "ToolRegistry.h"

#include "ToolDirWalk.h"


ToolUserData::ToolUserData (QToolBar *toolBar, QButtonGroup *buttonGroup) {
	this->toolBar      = toolBar;
	this->buttonGroup  = buttonGroup;
}

ToolUserData::~ToolUserData() {}

UserDataInterface *ToolUserData::copy() {
	return new ToolUserData (toolBar, buttonGroup);
}

void ToolUserData::copy (UserDataInterface *source) {

	ToolUserData *toolUserData;

	toolUserData = (ToolUserData *)source;

	toolBar      = toolUserData->toolBar;
	buttonGroup  = toolUserData->buttonGroup;

}

ToolDirWalk::ToolDirWalk (QString ToolDirectory)
	: DirWalk (ToolDirectory, QDir::Files | QDir::NoSymLinks) {}

ToolDirWalk::~ToolDirWalk() {}

DirWalk *ToolDirWalk::createNewInstance (const QString directoryName) {
	return new ToolDirWalk (directoryName);
}

void ToolDirWalk::applyFileMethod() {

	ToolUserData                *toolUserData;
	ToolLoader::ToolAccess      toolAccess;
	QToolButton                 *toolButton;
	int                         id;
	ToolInterface               *tool;
	ToolRegistry::DynamicAccess dynamicAccess;

	toolUserData = (ToolUserData *)userData;

	// try and see if the current file is a Tool
	toolAccess = ToolLoader::loadTool (QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1());
	if (toolAccess.isValid == true) {

		tool = toolAccess.createToolFunction();
		toolButton = tool->addButtonToToolBar (toolUserData->toolBar);
		id = toolUserData->buttonGroup->insert (toolButton);

		// register the dynamically loaded tool
		dynamicAccess.toolAccess = toolAccess;
		dynamicAccess.toolPath   = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1();

		ToolRegistry::getInstance()->registerTool (id, tool, true, &dynamicAccess);
		SplashScreen::getInstance()->showText (QString ("Tool %1 registered").arg (toolAccess.getToolNameFunction()));

	}
	else {
		qDebug ("ToolDirWalk: applyFileMethod: File %s is not a valid tool, ignoring it.", currentFileName.latin1());
	}

}

void ToolDirWalk::applyDirectoryMethod() {}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
