// Title:   PluginRegistry.h
// Created: Thu Sep 25 14:28:31 2003
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

#ifndef __PLUGINREGISTRY_H_
#define __PLUGINREGISTRY_H_

#include <qmap.h>

#include "../../Interfaces/src/PluginInterface.h"
#include "PluginLoader.h"

// *****************
// class definitions
// *****************

/**
 * This class registers all loaded plugins, so they can be unloaded when not
 * needed anymore.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class PluginRegistry {

public:

	// ********
	// typedefs
	// ********

	/**
	 * A <code>struct</code> storing the access <code>PluginAccess</code> functions
	 * and the <code>pluginPath</code>.
	 */
	typedef struct {
		PluginLoader::PluginAccess pluginAccess;
		QString                    pluginPath;
	} DynamicAccess;

	/**
	 * This method creates an instance of this <code>PluginRegistry</code>, if necessary,
	 * and returns it. Only one instance at a time exists.
	 *
	 * @return an instance of this <code>PluginRegistry</code>
	 */
	static PluginRegistry *getInstance();
	
	/**
	 * Destroys the instance of this <code>PluginRegistry</code>, if there
	 * is one.
	 */
	static void destroyInstance();

	// ****************
	// Registry methods
	// ****************

	/**
	 * Registers a plugin, accessible with <code>DynamicAccess</code> and identified by ID <code>id</code>.
	 *
	 * @param id
	 *        an ID which identifies the plugin to be registered
	 * @param dynamicAccess
	 *        a pointer to the <code>DynamicAccess</code>, which allows to unload dynamically loaded plugins
	 * @see #unregisterPlugin
	 */
	void registerPlugin (const int id, const DynamicAccess dynamicAccess);
	
	/**
	 * Unregisters and unloads the plugin given by its ID <code>id</code>.
	 *
	 * @param id
	 *        the ID of the plugin to be unregistered and unloaded
	 * @see #registerTool
	 */ 
	void unregisterPlugin (const int id);

	/**
	 * Unregisters all plugins, unloading the corresponding dynamic libraries.
	 */
	void unregisterAllPlugins();

	/**
	 * Returns a <code>PluginAccess</code> identified by its ID <code>id</code>. Check the field
	 * <code>isValid</code> to see if the requested <code>PluginAccess</code> was found.
	 * 
	 * @param id
	 *        the ID of the <code>PluginAccess</code> which is to be returned
	 * @return the <code>PluginAccess</code> with ID <code>id</code>; the field <code>isValid</code>
	 *         is <code>true</code> if the <code>PluginAccess</code> identified by <code>id</code> was
	 *         found, <code>false</code> else
	 */
	PluginLoader::PluginAccess getPlugin (const int id) const;

private:

	// ********
	// typedefs
	// ********

	typedef QMap<int,DynamicAccess> Registry;

	Registry registry;

	PluginRegistry();
	virtual ~PluginRegistry();

	// unloads the corresponding library
	void deleteEntry (Registry::Iterator it);	

};




#endif  // __PLUGINREGISTRY_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
