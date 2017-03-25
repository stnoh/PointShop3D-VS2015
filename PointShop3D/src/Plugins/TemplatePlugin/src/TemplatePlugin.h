// Title:   TemplatePlugin.h
// Created: Thu Sep 25 14:45:12 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003, Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General
// Public License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330,
// Boston, MA 02111-1307 USA
// 
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//


#ifndef __TEMPLATEPLUGIN_H_
#define __TEMPLATEPLUGIN_H_

// **********************************************
// To compile plugins (win32):
// 
// define PLUGIN_EXPORTS on the command line
//
// in Visual Studio:
// Project->Settings->C/C++, Category Preprocessor
// add PLUGIN_EXPORTS to 'preprocessor definitions'
// ***********************************************

#include "../../../Core/Interfaces/src/PluginInterface.h"

/**
 * This class implements a template plugin which may be used
 * to implement more plugins. It does nothing but show a message
 * on <code>std err</code> on execution.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class TemplatePlugin : public PluginInterface {

public:
	
	PLUGIN_API TemplatePlugin();
	PLUGIN_API virtual ~TemplatePlugin();

	// *************************
	// interface PluginInterface
	// *************************

	PLUGIN_API void execute();
	PLUGIN_API bool isAvailable() const;

private:
	
	bool checkPluginAvailability() const;
	
};

// ****************
// access functions
// ****************

// extern "C" is needed so those functions can be accessed
// with GetProcAddress() (WIN32) and dlsym() (UNIX) respective
extern "C" {
	PLUGIN_API PluginInterface *createPlugin();
	PLUGIN_API const char *getPluginName();
	PLUGIN_API const char *getPluginDescription();
}

#endif  // __TEMPLATEPLUGIN_H_


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
