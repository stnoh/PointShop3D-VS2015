// Title:   DirWalk.h
// Created: Thu Sep 25 15:02:58 2003
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

#ifndef __DIRWALK_H_
#define __DIRWALK_H_

#include <qdir.h>
#include <qfileinfo.h>

#include "../../src/UtilitiesDLL.h"

/**
 * This class is the base class for any other class which holds user data which
 * is required during the directory walk. It also specifies the methods which
 * have to be implemented by its subclasses.
 */
class UTILITIES_API UserDataInterface {

public:

	/**
	 * Creates a deep copy of <code>source</code>, which must be of the same
	 * dynamic type as this <code>UserDataInterface</code>, and stores the copied
	 * user data into this <code>UserDataInterface</code>.
	 *
	 * @param source
	 *        the source <code>UserDataInterface</code> to be copied     
	 */
	virtual void copy (UserDataInterface *source) = 0;

	/**
	 * Creates and returns a deep copy of this <code>UserData</code>. This copy
	 * has to be <code>delete</code>d after use.
	 *
	 * @return a deep copy of this <code>UserDataInterface</code>; has to be <code>delete</code>d
	 *         after use
	 */
	virtual UserDataInterface *copy() = 0;

};

/**
 * This is the base class for any other class which needs to traverse a directory structure
 * and apply specific methods on files and subdirectories.
 */
class DirWalk {

public:

	/**
	 * Creates this <code>DirWalk</code>, which must be subclassed, specifying the <code>directoryName</code> to
	 * be walked through, <code>filterSpec</code> specifies which files/directories should be considered and
	 * <code>sortSpec</code> defines in which order the files/directories should be looked at.
	 *
	 * @param directoryName
	 *        a <code>QString</code> specifying the absolute or relative path of the directory to be walked through
	 * @param filterSpec
	 *        specifies if files and/or directories and/or symbolic links should be looked at
	 * @param sortSpec
	 *        specifies in which order the files/directories are processed
	 */
	UTILITIES_API DirWalk (QString directoryName, int filterSpec = QDir::Files | QDir::Dirs | QDir::NoSymLinks, int sortSpec = QDir::Unsorted);
	
	/**
	 * Destroys this <code>DirWalk</code>.
	 */
	UTILITIES_API virtual ~DirWalk();

	/**
	 * Walks through the directory specified in the c'tor. Returns false, if this directory does not exist.
	 * The <code>currentUserData</code> carries user data which is used by subclasses of this <code>DirWalk</code>.
	 *
	 * @param currentUserData
	 *        a <code>UserDataInterface</code>, which must be implemented by subclasses of this
	 *        <code>DirWalk</code> as well, which carries user data which may be used in the methods
	 *        <code>applyFileMethod</code> and/or <code>applyDirectoryMethod</code>; may be 0
	 * @return <code>true</code> if the directory specified by <code>directoryName</code> in the constructor
	 *         exists; <code>false</code> else
	 * @see #applyFileMethod
	 * @see #applyDirectoryMethod
	 */
	UTILITIES_API virtual bool execute (UserDataInterface *currentUserData);

	/**
	 * Creates a new instance, which is a concrete subclass of this <code>DirWalk</code>. This is used
	 * for recursion when travelling into subdirectories. It's up to the concrete subclasses if they
	 * create a new instance of themselves with the same parameters for the constructor or not.
	 *
	 * @param directoryName
	 *        a <code>QString</code> specifying the absolute or relative path of the directory to be walked through
	 */
	UTILITIES_API virtual DirWalk *createNewInstance (const QString directoryName) = 0;

protected:

	/**
	 * The current file name, such as "Foo".
	 */
	QString currentFileName;

	/**
	 * The current directory, such as "../Foo/Bar"
	 */
	QString currentDirectory; 
	
	/** 
     * The user data which is accessible for subclasses of this <code>DirWalk</code>.
	 */
	UserDataInterface *userData;

	/**
	 * This method is applied for each file encountered, except "." (the current directory)
	 * and ".." (the parent directory).
	 */
	virtual void applyFileMethod() = 0;

	/**
	 * This method is applied for each directory encountered, except "." (the current directory)
	 * and ".." (the parent directory).
	 */
	virtual void applyDirectoryMethod() = 0;

private:

	QDir *dir;
 
};

#endif  // __DIRWALK_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
