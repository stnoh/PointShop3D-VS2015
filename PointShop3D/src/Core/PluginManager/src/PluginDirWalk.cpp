// Title:   PluginDirWalk.cpp
// Created: Thu Sep 25 14:14:38 2003
// Authors: Oliver Knoll, Mark Pauly, Matthias Zwicker
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

#include "../../../Utilities/UserMessages/src/SplashScreen.h"
#include "../../../Utilities/DirWalk/src/DirWalk.h"
#include "PluginRegistry.h"
#include "PluginLoader.h"

#include "PluginDirWalk.h"

PluginUserData::PluginUserData (QPopupMenu *popupMenu, QObject *signalReceiver, const char *signalHandler) {

	this->popupMenu = popupMenu;
	this->signalReceiver = signalReceiver;
	this->signalHandler = signalHandler;

}

PluginUserData::~PluginUserData() {}

UserDataInterface *PluginUserData::copy() {
	return new PluginUserData (popupMenu, signalReceiver, signalHandler);
}

void PluginUserData::copy (UserDataInterface *source) {
	
	PluginUserData *pluginUserData;

	pluginUserData = (PluginUserData *)source;

	popupMenu      = pluginUserData->popupMenu;
	signalReceiver = pluginUserData->signalReceiver;
	signalHandler  = pluginUserData->signalHandler;

}

PluginDirWalk::PluginDirWalk (QString pluginDirectory)
	: DirWalk (pluginDirectory) {}

PluginDirWalk::~PluginDirWalk() {}

DirWalk *PluginDirWalk::createNewInstance (const QString directoryName) {
	return new PluginDirWalk (directoryName);
}

void PluginDirWalk::applyFileMethod() {

	PluginUserData                *pluginUserData;
	PluginLoader::PluginAccess    pluginAccess;
	int                           id;
	PluginRegistry::DynamicAccess dynamicAccess;

	pluginUserData = (PluginUserData *)userData;

	// try and see if the current file is a plugin
	pluginAccess = PluginLoader::loadPlugin (QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1());
	if (pluginAccess.isValid == true) {
		// make a menu entry ...
		id = pluginUserData->popupMenu->insertItem (pluginAccess.getPluginNameFunction(),
		                                            pluginUserData->signalReceiver,
		                                            pluginUserData->signalHandler);
		pluginUserData->popupMenu->setWhatsThis (id, pluginAccess.getPluginDescriptionFunction());
		// ... and register the plugin
		dynamicAccess.pluginAccess = pluginAccess;
		dynamicAccess.pluginPath   = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1();
		PluginRegistry::getInstance()->registerPlugin (id, dynamicAccess);

		SplashScreen::getInstance()->showText (QString ("Plugin %1 registered").arg (pluginAccess.getPluginNameFunction()));
	}
	else {
		qDebug ("PluginDirWalk: applyFileMethod: File %s is not a valid plugin, ignoring it.", currentFileName.latin1());
	}

}

void PluginDirWalk::applyDirectoryMethod() {

	QPopupMenu *popupMenu;

	PluginUserData *pluginUserData;

	pluginUserData = (PluginUserData *)userData;

	popupMenu = new QPopupMenu (pluginUserData->popupMenu);
	pluginUserData->popupMenu->insertItem (currentFileName, popupMenu);

	// modify user data
	pluginUserData->popupMenu = popupMenu;

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
