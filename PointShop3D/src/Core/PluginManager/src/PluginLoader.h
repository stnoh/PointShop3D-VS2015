// Title:   PluginLoader.h
// Created: Thu Sep 25 14:28:38 2003
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

#ifndef __PLUGINLOADER_H_
#define __PLUGINLOADER_H_

#include <qstring.h>

#include "../../Interfaces/src/PluginInterface.h"

/**
 * This namespace provides functions for accessing dynamic link libraries
 * (<em>DLLs</em> under win32) or shared objects (<em>*.so</em> under Unix/Linux).
 * The corresponding functions try to open a file and check if it is a valid
 * plugin, that is if it implements the <code>PluginInterface</code> and provides
 * the specific access functions.
 * 
 * <b>Note:</b> This implementation is <em>platform specific</em>. Implement
 *              the necessary changes here as to access dynamic libraries.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see PluginInterface
 */
namespace PluginLoader {

	/**
	 * <code>CreatePluginFunction</code>: pointer to a function returning a pointer to a <code>PluginInterface</code>.
	 */
	typedef PluginInterface *(*CreatePluginFunction)();
	
	/**
	 * <code>GetPluginNameFunction</code>: pointer to a function returning a pointer to a <code>const char</code> string.
	 */
	typedef const char *(*GetPluginNameFunction)();
	
	/**
	 * <code>GetPluginDescriptionFunction</code>: pointer to a function returning a pointer to a <code>const char string</code>.
	 */
	typedef const char *(*GetPluginDescriptionFunction)();

	/**
	 * A structure storing function pointers to plugin access functions. The field <code>isValid</code>
	 * indicates whether the plugin could be loaded succesfully (<code>true</code>).
	 */
	typedef struct {
		CreatePluginFunction         createPluginFunction;
		GetPluginNameFunction        getPluginNameFunction;
		GetPluginDescriptionFunction getPluginDescriptionFunction;
		bool                         isValid;
	} PluginAccess;

	/**
	 * Loads the plugin specified by <code>pluginPath</code>. The plugin is linked
	 * dynamically to the application executable.
	 *
	 * @param pluginPath
	 *        a pointer to a string specifying the plugin path
	 * @return a <code>PluginAccess</code> structure; check the field <code>isValid</code>
	 *         first to see if loading was succesful before accessing any function pointers
	 */
	PluginAccess loadPlugin (const char *pluginPath);

	/**
	 * Unloads the plugin specified by <code>pluginPath</code>. The plugin is removed
	 * from memory if it is not used anymore by another process.
	 *
	 * @param pluginPath
	 *        a pointer to a string specifying the plugin path
	 * @return <code>true</code>, if succesful; <code>false</code>, if <code>pluginPath</code> is wrong or
	 *         the plugin was not loaded previously
	 */
	bool unloadPlugin (const char *pluginPath);

};

#endif  // __PLUGINLOADER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
