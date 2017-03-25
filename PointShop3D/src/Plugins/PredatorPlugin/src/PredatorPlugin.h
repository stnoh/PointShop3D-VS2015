// Title:   PredatorPlugin.h
// Created: Thu Sep 25 14:45:34 2003
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


#ifndef __PREDATORPLUGIN_H_
#define __PREDATORPLUGIN_H_

#include "../../../Core/Interfaces/src/PluginInterface.h"

/**
 * This class implements a plugin which converts the color of the <code>SurfelInterface</code>s of
 * the <em>active</em> <code>Object</code> to a heat map, like in the movie <a href="http://us.imdb.com/Title?0093773">Predator"</a>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class PredatorPlugin : public PluginInterface {

public:
	
	PLUGIN_API PredatorPlugin();
	PLUGIN_API virtual ~PredatorPlugin();

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

#endif  // __PREDATORPLUGIN_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
