// Title:   FileFormatPluginDirWalk.cpp
// Created: Thu Sep 25 14:18:11 2003
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
#include "FileFormatPluginRegistry.h"
#include "FileFormatPluginLoader.h"

#include "FileFormatPluginDirWalk.h"

FileFormatPluginDirWalk::FileFormatPluginDirWalk (QString fileFormatPluginDirectory)
	: DirWalk (fileFormatPluginDirectory, QDir::Files | QDir::NoSymLinks) {}

FileFormatPluginDirWalk::~FileFormatPluginDirWalk() {}

DirWalk *FileFormatPluginDirWalk::createNewInstance (const QString directoryName) {
	return new FileFormatPluginDirWalk (directoryName);
}

void FileFormatPluginDirWalk::applyFileMethod() {

	FileFormatPluginLoader::FileFormatPluginAccess pluginAccess;
	FileFormatPluginRegistry::DynamicAccess        dynamicAccess;
	FileFormatPluginInterface                      *fileFormatPlugin;

	// try and see if the current file is a file format plugin
	pluginAccess = FileFormatPluginLoader::loadFileFormatPlugin (QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1());
	if (pluginAccess.isValid == true) {
		
		// register the plugin
		dynamicAccess.fileFormatPluginAccess = pluginAccess;
		dynamicAccess.fileFormatPluginPath   = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1();
		
		// temporarily create the file format plugin as to query the file filter
		fileFormatPlugin = pluginAccess.createFileFormatPluginFunction();
		
		FileFormatPluginRegistry::getInstance()->registerFileFormatPlugin (fileFormatPlugin->getFileFilter(), dynamicAccess);
		SplashScreen::getInstance()->showText (QString ("File format plugin %1 registered").arg (fileFormatPlugin->getFileFilter()));
	
		// remove unused resources
		delete fileFormatPlugin;

	}
	else {
		qDebug ("FileFormatPluginDirWalk: applyFileMethod: File %s is not a valid file format plugin, ignoring it.", currentFileName.latin1());
	}

}

void FileFormatPluginDirWalk::applyDirectoryMethod() {}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
