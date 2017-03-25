// Title:   FileFormatPluginManager.cpp
// Created: Thu Sep 25 14:24:55 2003
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

#include "../../Interfaces/src/FileFormatPluginInterface.h"
#include "../../Configuration/src/Configuration.h"

#include "FileFormatPluginRegistry.h"
#include "FileFormatPluginManager.h"

// ******
// global
// ******

// the single FileFormatPluginManager instance, only
// accesible by getInstance() and
// destroyInstance()
static FileFormatPluginManager *instance = 0;

// **************
// public methods
// **************

FileFormatPluginManager *FileFormatPluginManager::getInstance() {

	if (instance == 0) {
		instance = new FileFormatPluginManager();
	}
	return instance;

}

void FileFormatPluginManager::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

bool FileFormatPluginManager::initialize (const QString fileFormatPluginDirectory) {

	FileFormatPluginRegistry *fileFormatPluginRegistry;
	FileFormatPluginDirWalk  fileFormatPluginDirWalk (fileFormatPluginDirectory);

	fileFormatPluginRegistry = FileFormatPluginRegistry::getInstance();

	// unregister all previous plugins first
	fileFormatPluginRegistry->unregisterAllFileFormatPlugins();

	if (fileFormatPluginDirWalk.execute(0) == false) {
		qDebug ("FileFormatPluginManager: initialize: WARNING: file format plugin directory %s not found, ignoring file format plugins.", fileFormatPluginDirectory.latin1());
		return false;
	};

	// check if any file format plugins where found
	return (fileFormatPluginRegistry->getNofFileFormatPlugins() > 0);

}

FileFormatPluginInterface *FileFormatPluginManager::getFileFormatPlugin (const QString fileFilter) {

	FileFormatPluginLoader::FileFormatPluginAccess fileFormatPluginAccess;

	if (fileFormatPlugin != 0) {
		delete fileFormatPlugin;
		fileFormatPlugin = 0;
	}

	fileFormatPluginAccess = FileFormatPluginRegistry::getInstance()->getFileFormatPlugin (fileFilter);

	if (fileFormatPluginAccess.isValid == true) {
		fileFormatPlugin = fileFormatPluginAccess.createFileFormatPluginFunction();
	}

	return fileFormatPlugin;

}

const QList<FileFormatPluginInterface> *FileFormatPluginManager::getFileFormatPlugins() const {
	return FileFormatPluginRegistry::getInstance()->getFileFormatPlugins();
}

FileFormatPluginInterface *FileFormatPluginManager::probe (const FileFormatPluginInterface::FileHeader fileHeader) {

	if (fileFormatPlugin != 0) {
		delete fileFormatPlugin;
		fileFormatPlugin = 0;
	}

	fileFormatPlugin = FileFormatPluginRegistry::getInstance()->probe (fileHeader);

	return fileFormatPlugin;

}


// ***************
// private methods
// ***************

FileFormatPluginManager::FileFormatPluginManager() {

	fileFormatPlugin = 0;

	// get informed whenever the plugin directory changed
	this->connect (Configuration::getInstance(), SIGNAL (fileFormatPluginDirectoryChanged (const QString)),
		           this, SLOT (handleFileFormatPluginDirectoryChanged (const QString)));

}

FileFormatPluginManager::~FileFormatPluginManager() {
	
	if (fileFormatPlugin != 0) {
		delete fileFormatPlugin;
		fileFormatPlugin = 0;
	}
	FileFormatPluginRegistry::destroyInstance();

}

// *************
// private slots
// *************

void FileFormatPluginManager::handleFileFormatPluginDirectoryChanged (const QString newFileFormatPluginDirectory) {

	// initialize again
	this->initialize (newFileFormatPluginDirectory);

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
