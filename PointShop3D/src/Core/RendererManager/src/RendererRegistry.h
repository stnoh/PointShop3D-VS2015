// Title:   RendererRegistry.h
// Created: Thu Sep 25 14:28:19 2003
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

#ifndef __RENDERERREGISTRY_H_
#define __RENDERERREGISTRY_H_

#include <qstring.h>
#include <qmap.h>

#include "../../Interfaces/src/RendererInterface.h"
#include "RendererLoader.h"

// *****************
// class definitions
// *****************

/**
 * This class registers all loaded renderer plugins, so they can be unloaded when not
 * needed anymore.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class RendererRegistry {

public:

	// ********
	// typedefs
	// ********

	/**
	 * A <code>struct</code> storing the access <code>RendererAccess</code> functions
	 * and the <code>rendererPluginPath</code>.
	 */
	typedef struct {
		RendererLoader::RendererAccess rendererAccess;
		QString                        rendererPluginPath;
	} DynamicAccess;

	/**
	 * This method creates an instance of this <code>RendererRegisty</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>RendererRegisty</code>
	 */
	static RendererRegistry *getInstance();
	
	/**
	 * Destroys the instance of this <code>RendererRegisty</code>, if there
	 * is one. Also unregisters and unloads all renderer plugins.
	 */
	static void destroyInstance();

	// ****************
	// Registry methods
	// ****************

	/**
	 * Registers a renderer plugin, accessible with <code>DynamicAccess</code> and identified by
	 * <code>rendererType</code>.
	 *
	 * @param rendererType
	 *        a <code>QString</code> specifying the renderer type which identifies the renderer
	 * @param dynamicAccess
	 *        a <code>DynamicAccess</code> which allows to unload dynamically loaded renderer plugins
	 * @see #unregisterRendererPlugin
	 */
	void registerRendererPlugin (const QString rendererType, const DynamicAccess dynamicAccess);
	
	/**
	 * Unregisters and unloads the renderer plugin given by its <code>rendererType</code>
	 *
	 * @param rendererType
	 *        a <code>QString</code> specifying the renderer type which identifies the renderer to be unloaded
	 * @see #registerRendererPlugin
	 */ 
	void unregisterRendererPlugin (const QString rendererType);

	/**
	 * Unregisters all renderer plugins, unloading the corresponding dynamic libraries.
	 */
	void unregisterAllRendererPlugins();

	/**
	 * Returns a <code>RendererAccess</code> identified by its <code>rendererType</code>. Check the field
	 * <code>isValid</code> to see if the requested <code>RendererAccess</code> was found.
	 * 
	 * @param rendererType
	 *        a <code>QString</code> specifying the renderer type of the renderer to be returned
	 * @return the <code>RendererAccess</code> of type <code>rendererType</code>; the field <code>isValid</code>
	 *         is <code>true</code> if the <code>RendererAccess</code> identified by <code>rendererType</code> was
	 *         found, <code>false</code> else
	 */
	RendererLoader::RendererAccess getRendererAccess (const QString rendererType) const;

private:

	// ********
	// typedefs
	// ********

	typedef QMap<QString,DynamicAccess> Registry;

	Registry registry;

	RendererRegistry();
	virtual ~RendererRegistry();

	// unloads the corresponding library
	void deleteEntry (Registry::Iterator it);	

};




#endif  // __RENDERERREGISTRY_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
