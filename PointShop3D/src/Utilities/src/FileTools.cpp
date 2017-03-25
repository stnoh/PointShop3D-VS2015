// Title:   FileTools.cpp
// Created: Thu Sep 25 15:01:53 2003
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
#include <qdir.h>

#include "FileTools.h"

// ****************
// public functions
// ****************

QString FileTools::autoCompleteSuffix (const QString fileName, const QString suffix) {

	int  position,
	     suffixLength;

	suffixLength = (int)suffix.length();

	// check the last 'suffixLength' characters
	position = fileName.find (suffix, -suffixLength, false);

	if ((position > 0) &&
		((int)fileName.length() > suffixLength) &&
		(fileName.at ((uint)(fileName.length() - suffixLength - 1)) == ".")) {
		// suffix already exists and is separated by a dot
		return fileName;
	}
	else {
		return fileName + "." + suffix;
	}

}

QString FileTools::replaceSuffix (const QString fileName, const QString newSuffix) {

	int     position;
	uint    oldSuffixLength;
	QString newFileName;

	// find the last dot, if any
	position = fileName.findRev (".");
	
	if (position >= 0) {

		oldSuffixLength = fileName.length() - position;
		if (newSuffix.isEmpty() != true) {
			newFileName = fileName;
			newFileName.replace (position + 1, oldSuffixLength, newSuffix);

		}
		else {
			newFileName = fileName;
			newFileName.replace (position, oldSuffixLength + 1, "");
		}

	}
	else {
		if (newSuffix.isEmpty() != true) {
			newFileName = fileName + "." + newSuffix;
		}
	}

	return newFileName;

}

QString FileTools::getSuffix (const QString fileName) {

	int     position;
	QString suffix = "";

	// find the last dot, if any
	position = fileName.findRev (".");
	
	if (position >= 0) {
		suffix = fileName.mid ((position + 1));
	}
	return suffix;

}

QString FileTools::getFileName (const QString filePath) {

	int position;

	position = filePath.findRev ("/");
	if (position != -1) {
		return filePath.right (filePath.length() - position - 1);
	}
	else {
		return filePath;
	}

}

QString FileTools::getFileNameWithoutSuffix (const QString filePath) {

	int position;
	QString fileName = getFileName(filePath);
	QString fileNameWithoutSuffix;

	// remove suffix
	position = fileName.findRev (".");
	
	if (position >= 0) {
		 fileNameWithoutSuffix = fileName.left ((position));
	}
	return fileNameWithoutSuffix;
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
