// Title:   ToolRegistry.h
// Created: Thu Sep 25 14:27:51 2003
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

#ifndef __TOOLREGISTRY_H_
#define __TOOLREGISTRY_H_

#include <qmap.h>
#include <qstring.h>

#include "../../Interfaces/src/ToolInterface.h"
#include "ToolLoader.h"

// *****************
// class definitions
// *****************

/**
 * This class registers all loaded tools, so they can be unloaded when not
 * needed anymore.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ToolRegistry {

public:

	// ********
	// typedefs
	// ********

	typedef struct dynamicAccess {
		ToolLoader::ToolAccess toolAccess;
		QString                toolPath;
	} DynamicAccess;

	/**
	 * This method creates an instance of this <code>ToolRegistry</code>, if necessary,
	 * and returns it. Only one instance at a time exists.
	 *
	 * @return an instance of this <code>ToolRegistry</code>
	 */
	static ToolRegistry *getInstance();

	/**
	 * Destroys the instance of this <code>ToolRegistry</code>, if there
	 * is one. Also <code>delete</code>s all the registered <code>ToolInterface</code>s.
	 */
	static void destroyInstance();

	// ****************
	// Registry methods
	// ****************

	/**
	 * Registers an instance of <code>ToolInterface</code> with ID <code>id</code>. If the tool is
	 * loaded from a dynamically loaded library then <code>isDynamicallyLoaded</code> must be set
	 * to <code>true</code> and a <code>DynamicAccess</code> must be provided.
	 *
	 * @param id
	 *        an ID which identifies the <code>tool</code> to be registered
	 * @param tool
	 *        a pointer to the <code>ToolInterface</code> which is to be registered
	 * @param isDynamicallyLoaded
	 *        set to <code>true</code> if this <code>tool</code> was loaded dynamically, that
	 *        is from a dynamic library; in this case a <code>DynamicAccess</code> must also
	 *        be provided; set to <code>false</code> else, in which case the <code>DynamicAccess</code>
	 *        is ignored, if provided
	 * @param dynamicAccess
	 *        a pointer to the <code>DynamicAccess</code>, which allows to unload dynamically loaded tools
	 * @see #unregisterTool
	 */
	void registerTool (const int id, ToolInterface *tool, const bool isDynamicallyLoaded = false, const DynamicAccess *dynamicAccess = 0);
	
	/**
	 * Unregisters the instance of <code>ToolInterface</code> with ID <code>id</code>. The
	 * tool instance itself is <code>delete</code>d if it was allocated from within PointShop,
	 * or it is destroyed and the corresponding shared library is unloaded if the tool
	 * was allocated from a dynamically loaded library, that is, if <code>isDynamicallyLoaded</code>
	 * was set to <code>true</code> when registering this tool.
	 *
	 * @param id
	 *        the ID of the <code>ToolInterface</code> to be unregistered
	 * @see #registerTool
	 */ 
	void unregisterTool (const int id);

	/**
	 * Unregisters all registered instances of <code>ToolInterface</code>. The
	 * tool instances themselves are <code>delete</code>d if they were allocated from within PointShop,
	 * or they are destroyed and the corresponding shared libraries are unloaded if the tools
	 * were allocated from a dynamically loaded library, that is, if <code>isDynamicallyLoaded</code>
	 * was set to <code>true</code> when registering the tools.
	 */ 
	void unregisterAllTools();

	/**
	 * Returns an instance of <code>ToolInterface</code> with the ID <code>id</code>. The instance
	 * is 0 if the tool with ID <code>id</code> was not found in this <code>ToolRegistry</code>.
	 * 
	 * @param id
	 *        the ID of the <code>ToolInterface</code> which is to be returned
	 * @return the <code>ToolInterface</code> with ID <code>id</code>; may be 0, which means
	 *         that the <code>id</code> does not denote a registered <code>ToolInterface</code>
	 */
	ToolInterface *getTool (const int id) const;
	
	/**
	 * Returns a vector containing references to instances of the registered tools. The
	 * returned vector must be <code>delete</code>d after use.
	 *
	 * @return a <code>std::vector<ToolInterface*></code> containing all registered <code>ToolInterface</code>s; must
	 *         be <code>delete</code>d after use
	 */
	std::vector<ToolInterface*> *getTools() const;

private:

	// ********
	// typedefs
	// ********

	typedef struct {
		ToolInterface *instance;
		bool          isDynamicallyLoaded;
		DynamicAccess dynamicAccess;
	} RegistryEntry;

	typedef QMap<int,RegistryEntry> Registry;

	Registry registry;

	ToolRegistry();
	virtual ~ToolRegistry();

	// deletes an entry, taking care of instances from libraries,
	// undloading the necessary library if appropriate
	void deleteEntry (Registry::Iterator it);

	

};




#endif  // __PLUGINREGISTRY_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
