// Title:   FileFormatPluginRegistry.cpp
// Created: Thu Sep 25 14:24:50 2003
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

#include "FileFormatPluginLoader.h"
#include "FileFormatPluginRegistry.h"

// ******
// global
// ******

// the single FileFormatPluginRegistry instance, only
// accesible by getInstance() and
// deleteInstance()
static FileFormatPluginRegistry *instance = 0;

// ***********
// c'tor/d'tor
// ***********

FileFormatPluginRegistry::FileFormatPluginRegistry() {}

FileFormatPluginRegistry::~FileFormatPluginRegistry() {
	this->unregisterAllFileFormatPlugins();
}

// *****************
// Singleton Pattern
// *****************

FileFormatPluginRegistry *FileFormatPluginRegistry::getInstance() {

	if (instance == 0) {
		instance = new FileFormatPluginRegistry();
	}
	return instance;

}

void FileFormatPluginRegistry::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void FileFormatPluginRegistry::registerFileFormatPlugin (const QString fileFilter, const DynamicAccess dynamicAccess) {
	registry.insert (fileFilter, dynamicAccess);
}

void FileFormatPluginRegistry::unregisterFileFormatPlugin (const QString fileFilter) {

	Registry::Iterator it;
	
	it = registry.find (fileFilter);
	if (it != registry.end()) {
		this->deleteEntry (it);
    	registry.remove (fileFilter);
	}

}

void FileFormatPluginRegistry::unregisterAllFileFormatPlugins() {

	Registry::Iterator it;

	for (it = registry.begin(); it != registry.end(); ++it) {
		this->deleteEntry (it);
	}

	registry.clear();

}

FileFormatPluginLoader::FileFormatPluginAccess FileFormatPluginRegistry::getFileFormatPlugin (const QString fileFilter) const {

	Registry::ConstIterator                        it;
	FileFormatPluginLoader::FileFormatPluginAccess fileFormatPluginAccess;

	it = registry.find (fileFilter);

	if (it != registry.end()) {
		fileFormatPluginAccess = it.data().fileFormatPluginAccess;
		fileFormatPluginAccess.isValid = true;
	}
	else {
		fileFormatPluginAccess.isValid = false;
	}
	
	return fileFormatPluginAccess;

}

const QList<FileFormatPluginInterface> *FileFormatPluginRegistry::getFileFormatPlugins() const {

	Registry::ConstIterator it;
	QList<FileFormatPluginInterface> *fileFormatPlugins;
	FileFormatPluginInterface        *fileFormatPlugin;

	fileFormatPlugins = new QList<FileFormatPluginInterface>();
	// the instances of the file format plugins must be deleted as well as soon as this QList
	// is deleted
	fileFormatPlugins->setAutoDelete (true);
	for (it = registry.begin(); it != registry.end(); ++it) {

		fileFormatPlugin = it.data().fileFormatPluginAccess.createFileFormatPluginFunction();
		fileFormatPlugins->append (fileFormatPlugin);
	}

	return fileFormatPlugins;

}

FileFormatPluginInterface *FileFormatPluginRegistry::probe (const FileFormatPluginInterface::FileHeader fileHeader) const {

	FileFormatPluginInterface                *fileFormatPlugin;
	bool                                     isFound;
	Registry::ConstIterator                  it;

	isFound          = false;
	it               = registry.begin();
	fileFormatPlugin = 0;
	while ((isFound == false) && (it != registry.end())) {

		// temporarily create the file format plugin
		fileFormatPlugin = it.data().fileFormatPluginAccess.createFileFormatPluginFunction();

		if (fileFormatPlugin->isReadable (fileHeader) == true) {
			isFound = true;
		}
		else {
			++it;
			delete fileFormatPlugin;
			fileFormatPlugin = 0;
		}

	}

	return fileFormatPlugin;

}

uint FileFormatPluginRegistry::getNofFileFormatPlugins() const {
	return registry.count();
}

// ***************
// private methods
// ***************

void FileFormatPluginRegistry::deleteEntry (Registry::Iterator it) {
	// unload the library
	FileFormatPluginLoader::unloadFileFormatPlugin (it.data().fileFormatPluginPath);
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
