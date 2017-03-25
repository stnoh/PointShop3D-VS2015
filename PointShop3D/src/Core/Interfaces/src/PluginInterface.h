// Title:   PluginInterface.h
// Created: Thu Sep 25 14:29:09 2003
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

#ifndef __PLUGININTERFACE_H_
#define __PLUGININTERFACE_H_

#include <qobject.h>

// ***********
// definitions
// ***********

#if defined _WIN32

	// The following ifdef block is the standard way of creating macros which make exporting
	// from a DLL simpler. All files within this DLL are compiled with the PLUGIN_EXPORTS
	// symbol defined on the command line. this symbol should not be defined on any project
	// that uses this DLL. This way any other project whose source files include this file see
	// PLUGIN_API functions as being imported from a DLL, wheras this DLL sees symbols
	// defined with this macro as being exported.
	#ifdef PLUGIN_EXPORTS
		#define PLUGIN_API __declspec(dllexport)
	#else
		#define PLUGIN_API __declspec(dllimport)
	#endif

#else

  // for all other platforms PLUGIN_API is defined to be "nothing"
	#define PLUGIN_API
	
#endif

/**
 * This interface defines the API for plugins, that is, modules which appear
 * in the plugin menu. Note that apart from this <code>PluginInterface</code> the plugin modules
 * must also implement some special C-style functions for dynamic access:
 * <ul>
 *   <li><code>PLUGIN_API PluginInterface *createPlugin();</code></li>
 *	 <li><code>PLUGIN_API const char *getPluginName();</code></li>
 *	 <li><code>PLUGIN_API const char *getPluginDescription();</code></li>
 * </ul>
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see TemplatePlugin
 */
class PLUGIN_API PluginInterface {

public:

	/**
	 * This method is called whenever the plugin is to be executed.
	 */
	virtual void execute() = 0;

	/**
	 * Returns <code>true</code> if this <code>PluginInterface</code> is selectable, <code>false</code>
	 * if any conditions this <code>PluginInterface</code> requires are not met. Conditions for
	 * the availability of a <code>PluginInterface</code> might include:
	 * <ul>
	 *   <li>an <code>Object</code> in the <code>Scene</code> is available</li>
	 *   <li>the <em>main</em> <code>RendererInterface</code> is an <em>interactive</em> renderer</li>
	 *   <li>a <code>Brush</code> is available</li>
	 * </ul>
	 *
	 * @return <code>true</code> if this <code>PluginInterface</code> is available; <code>false</code> else
	 */
	virtual bool isAvailable() const = 0;

};

#endif  // __PLUGININTERFACE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
