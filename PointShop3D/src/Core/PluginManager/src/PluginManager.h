// Title:   PluginManager.h
// Created: Thu Sep 25 14:28:33 2003
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

#ifndef __PLUGINMANAGER_H_
#define __PLUGINMANAGER_H_

#include <qstring.h>
#include <qevent.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qobject.h>

#include "../../Interfaces/src/PluginInterface.h"
#include "../../src/CoreDLL.h"

#include "PluginDirWalk.h"

/**
 * This class manages the available <code>PluginInterface</code>s which are loaded dynamically.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see PluginInterface
 */
class PluginManager : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates an instance of this <code>PluginManager</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>PluginManager</code>
	 */
	CORE_API static PluginManager *getInstance();

	/**
	 * Destroys the instance of this <code>PluginManager</code>, if there is any.
	 */
	CORE_API static void destroyInstance();

	/**
	 * Initializes this <code>PluginManager</code>: the <code>pluginDirectory</code> ist scanned for
	 * valid plugins. This method must be called before any other plugin access methods.
	 * Another call re-initializes this <code>PluginManager</code>.
	 *
	 * @param pluginDirectory
	 *        a <code>QString</code> specifying the relative or absolute path of the
	 *        plugin directory; this directory is scanned for
	 *        valid plugins - invalid plugins are ignored, but may lead to a system
	 *        error message, depending on the platform
	 * @param parent
	 *        a pointer to a <code>QWidget</code> which will be the parent of the <code>QPopupMenu</code>
	 *        which is initialized by this method
	 */
	CORE_API void initialize (const QString pluginDirectory, QMenuBar *menuBar, int *menuID);

	/**
	 * Checks the availability of the <code>PluginInterface</code>s and enables/disables the
	 * respective menu entries accordingly.
	 */
	CORE_API void checkPluginAvailability();

signals:

	/**
	 * Emitted whenever a plugin has been executed.
	 */
	void pluginExecuted();

private:

	QPopupMenu *pluginMenu;
	QMenuBar   *menuBar;  // the parent of the 'pluginMenu'

	PluginManager();
protected:  // To suppress warnings...
	virtual ~PluginManager();
private:

	// recursively goes through the plugin menu and enables/disables the plugin entries
	void enablePluginItems (QPopupMenu *menu);

private slots:

	void handlePluginSelected (int id);
	void handlePluginDirectoryChanged (const QString newPluginDirectory);

};

#endif  // __PLUGINMANAGER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
