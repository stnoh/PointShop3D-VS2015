// Title:   DirWalk.cpp
// Created: Thu Sep 25 15:03:01 2003
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

#include <qdir.h>
#include <qfileinfo.h>

#include "DirWalk.h"

DirWalk::DirWalk (QString directoryName, int filterSpec, int sortSpec) {

	currentFileName  = directoryName;
	currentDirectory = directoryName;

	dir = new QDir (directoryName);
	dir->setFilter (filterSpec);
	dir->setSorting (sortSpec);

}

DirWalk::~DirWalk() {
	delete dir;	
}

bool DirWalk::execute (UserDataInterface *currentUserData) {

	DirWalk               *dirWalk;
	UserDataInterface     *copiedData;

	const QFileInfoList   *fileInfoList;
	QFileInfoListIterator *it;
	QFileInfo             *fileInfo;

	if (dir->exists() == false) {
		qDebug ("DirWalk: execute: WARNING, plugin directory %s does not exist!", currentFileName.latin1());
		return false;
	}

	userData   = currentUserData;
	copiedData = 0;
	
	fileInfoList = dir->entryInfoList();
	it = new QFileInfoListIterator (*fileInfoList);
	

	// for each file...
	while ((fileInfo = it->current()) != 0) {

		currentFileName = fileInfo->fileName();

		// handle files
		if (fileInfo->isFile() == true) {
			this->applyFileMethod();
		}

		// handle subdirs
		else if (fileInfo->isDir() == true &&
		         fileInfo->fileName() != "." &&
		         fileInfo->fileName() != "..") {

			// backup user data
			if (userData != 0) {
				copiedData = userData->copy();
			}

			// this method may modify the user data
			this->applyDirectoryMethod();
			dirWalk = this->createNewInstance (currentDirectory + "/" + currentFileName);
			dirWalk->execute (userData);
			delete dirWalk;

			// restore original data
			if (copiedData != 0) {
				userData->copy (copiedData);
				// delete copiedData;
				delete copiedData;				
				copiedData = 0;
			}

		}

		++(*it);                               // goto next list element
	
	}

	delete it;

	return true;

}
   

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
