// Title:   ToolDirWalk.h
// Created: Thu Sep 25 14:28:07 2003
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

#ifndef __TOOLDIRWALK_H_
#define __TOOLDIRWALK_H_

#include <qpopupmenu.h>
#include <qobject.h>
#include <qstring.h>
#include <qbuttongroup.h>
#include <qwidget.h>

#include "../../../Utilities/DirWalk/src/DirWalk.h"

/**
 * This class extends the <code>UserDataInterface</code> needed for the
 * <code>DirWalk</code>. It contains the <code>toolBar</code> and the
 * <code>buttonGroup</code> to which the tool buttons belong.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see DirWalk
 */
class ToolUserData : public UserDataInterface {

public:

	/**
	 * Creates this <code>ToolUserData</code>, storing the <code>toolBar</code> and
	 * the <code>buttonGroup</code> to which the entries should be added.
	 *
	 * @param toolBar
	 *        a pointer to a <code>QToolBar</code> to which the <code>QToolButton</code>s are added while traversing
	 *        the directory structure
	 * @param buttonGroup
	 *        a pointer to a <code>QButtonGroup</code> to which the <code>QToolButton</code>s are added while traversing
	 *        the directory structure
	 * @see   ToolDirWalk
	 */
	ToolUserData (QToolBar *toolBar, QButtonGroup *buttonGroup);
	virtual ~ToolUserData();

	// ***************************
	// interface UserDataInterface
	// ***************************

	/**
	 * builds and returns a copy of this <code>ToolUserData</code>
	 */
	UserDataInterface *copy ();
	
	/**
	 * copies the data from <code>source</code>
	 *
	 * <b>Note</b>:source needs to be of type <code>ToolUserData</code>
	 */
	void copy (UserDataInterface *source);

private:	

	// access for...
	friend class ToolDirWalk;

	QToolBar     *toolBar;
	QButtonGroup *buttonGroup;

};

/**
 * This class extends the <code>DirWalk</code>. It browes through the
 * tool directory and tries to load each file encountered as valid
 * tool, adding it to the toolbar.
 *
 * @author Oliver Knoll
 * @version 1.2
 */ 
class ToolDirWalk : public DirWalk {

public:

	/**
	 * Creates this <code>ToolDirWalk</code>, specifying the <code>toolDirectory</code>
	 * which is to be recursively traversed.
	 *
	 * @param toolDirectory
	 *        a <code>QString</code> specifying the absolute or relative path to the tool directory
	 */
	ToolDirWalk (QString toolDirectory);
	virtual ~ToolDirWalk();

	// *****************
	// interface DirWalk
    // *****************

	DirWalk *createNewInstance (const QString directoryName);

protected:

	void applyFileMethod();
	void applyDirectoryMethod();

};



#endif  // __TOOLDIRWALK_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
