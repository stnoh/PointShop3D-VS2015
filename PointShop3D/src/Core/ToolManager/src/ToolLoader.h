// Title:   ToolLoader.h
// Created: Thu Sep 25 14:28:01 2003
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

#ifndef __TOOLLOADER_H_
#define __TOOLLOADER_H_

#include "../../Interfaces/src/ToolInterface.h"

/**
 * This namespace provides functions for accessing dynamic link libraries
 * (<em>DLLs</em> under win32) or shared objects (<em>*.so</em> under Unix/Linux).
 * The corresponding functions try to open a file and check if it is a valid
 * tool, that is if it implements the <code>ToolInterface</code> and provides
 * the specific access functions.
 * 
 * <b>Note:</b> This implementation is <em>platform specific</em>. Implement
 *              the necessary changes here as to access dynamic libraries.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see ToolInterface
 */
namespace ToolLoader {

	// ********
	// typedefs
	// ********

	/**
	 * <code>CreatePluginFunction</code>: pointer to a function returning a pointer to a <code>ToolInterface</code>.
	 */
	typedef ToolInterface *(*CreateToolFunction)();

	/**
	 * <code>GetToolNameFunction</code>: pointer to a function returning a pointer to a <code>const char</code> string.
	 */
	typedef const char *(*GetToolNameFunction)();

	/**
	 * A struct which contains the <code>createToolFunction</code> which creates the <code>ToolInterface</code>
	 * and the <code>getToolNameFunction</code> which
	 * returns the name of the <code>ToolInterface</code> and a field <code>isValid</code> which is set to
	 * <code>true</code> if the function pointers are valid.
	 */
	typedef struct {
		CreateToolFunction  createToolFunction;
		GetToolNameFunction getToolNameFunction;
		bool isValid;
	} ToolAccess;

	/**
	 * Loads the tool specified by <code>toolPath</code>. The tool is linked
	 * dynamically to the application executable.
	 *
	 * @param pluginPath
	 *        a pointer to a string specifying the plugin path
	 * @return a <code>ToolAccess</code> structure; check the field <code>isValid</code>
	 *         first to see if loading was succesful before accessing any function pointers
	 */
	ToolAccess loadTool (const char *toolPath);

	/**
	 * Unloads the tool specified by <code>toolPath</code>. The tool is removed
	 * from memory if it is not used anymore by another process.
	 *
	 * @param toolPath
	 *        a pointer to a string specifying the tool path
	 * @return <code>true</code>, if succesful; <code>false</code>, if <code>toolPath</code> is wrong or
	 *         tool was not loaded previously
	 */
	bool unloadTool (const char *toolPath);

};

#endif  // __PLUGINLOADER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
