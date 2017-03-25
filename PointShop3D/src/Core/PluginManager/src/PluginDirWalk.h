// Title:   PluginDirWalk.h
// Created: Thu Sep 25 14:28:43 2003
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

#ifndef __PLUGINDIRWALK_H_
#define __PLUGINDIRWALK_H_

#include <qpopupmenu.h>
#include <qobject.h>
#include <qstring.h>

#include "../../../Utilities/DirWalk/src/DirWalk.h"

/**
 * The user data needed for the <code>PluginDirWalk</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see PluginDirWalk
 */
class PluginUserData : public UserDataInterface {

public:

	/**
	 * Creates this <code>PluginUserData</code>, storing the <code>popupMenu</code> to
	 * which the entries should be added and the <code>signalReceiver</code> and its
	 * <code>signalHandler</code> which should be called whenever an entry in the
	 * <code>popupMenu</code> has been selected.
	 *
	 * @param popupMenu
	 *        a <code>QPopupMenu</code> to which the entries are added while traversing
	 *        the directory structure
	 * @param signalReceiver
	 *        the <code>QObject</code> which is to be notified whenever an entry in the
	 *        <code>popupMenu</code> has been selected
	 * @param signalHandler
	 *        the method of the <code>signalReceiver</code> which is called whenever an
	 *        entry in the <code>popupMenu</code> has been selected
	 * @see   PluginDirWalk
	 */
	PluginUserData (QPopupMenu *popupMenu, QObject *signalReceiver, const char *signalHandler);
	virtual ~PluginUserData();

	UserDataInterface *copy ();
	void copy (UserDataInterface *source);

 private:
	
	// access for...
	friend class PluginDirWalk;

	QPopupMenu     *popupMenu;
	QObject        *signalReceiver;
	const char     *signalHandler;

};

/**
 * This class implements the <code>DirWalk</code>. It iterates recursively over the given
 * directory and looks for plugins which appear in the plugin menu. Entries are
 * then generated and added to the <code>QPopupMenu</code> which has been specified in
 * the <code>BrushUserData</code> object.
 * 
 * @author Oliver Knoll
 * @version 1.2
 * @see PluginUserData
 */
class PluginDirWalk : public DirWalk {

public:

	/**
	 * Creates this <code>PluginDirWalk</code>, specifying the <code>pluginDirectory</code>
	 * which is to be traversed.
	 *
	 * @param pluginDirectory
	 *        a <code>QString</code> specifying the path to the plugin directory
	 */
	PluginDirWalk (QString pluginDirectory);
	virtual ~PluginDirWalk();

	// *****************
	// interface DirWalk
	// *****************

	DirWalk *createNewInstance (const QString directoryName);

protected:

	void applyFileMethod();
	void applyDirectoryMethod();

};



#endif  // __PLUGINDIRWALK_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
