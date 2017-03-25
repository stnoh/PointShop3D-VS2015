// Title:   ToolRegistry.cpp
// Created: Thu Sep 25 14:09:57 2003
// Authors: Oliver Knoll, Mark Pauly, Matthias Zwicker
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

#include <qmap.h>

#include "../../Interfaces/src/ToolInterface.h"
#include "ToolLoader.h"

#include "ToolRegistry.h"

// ******
// global
// ******

// the single ToolRegistry instance, only
// accesible by getInstance() and
// destroyInstance()
static ToolRegistry *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

ToolRegistry::ToolRegistry() {}

ToolRegistry::~ToolRegistry() {
	this->unregisterAllTools();
}

// *****************
// Singleton Pattern
// *****************

ToolRegistry *ToolRegistry::getInstance() {

	if (instance == 0) {
		instance = new ToolRegistry();
	}
	return instance;

}

void ToolRegistry::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void ToolRegistry::registerTool (const int id, ToolInterface *tool, const bool isDynamicallyLoaded, const DynamicAccess *dynamicAccess) {
	
	RegistryEntry entry;

	entry.instance = tool;
	if (isDynamicallyLoaded == true) {
		entry.isDynamicallyLoaded = true;
		entry.dynamicAccess = *dynamicAccess;
	}
	else {
		entry.isDynamicallyLoaded = false;
	}
	registry.insert (id, entry);

}

void ToolRegistry::unregisterTool (const int id) {

	Registry::Iterator it;
	
	it = registry.find (id);
	if (it != registry.end()) {
		this->deleteEntry (it);
    	registry.remove (id);
	}

}

void ToolRegistry::unregisterAllTools() {

	Registry::Iterator it;

	for (it = registry.begin(); it != registry.end(); ++it) {
		deleteEntry (it);
	}

	registry.clear();

}

ToolInterface *ToolRegistry::getTool (const int id) const {

	if (registry.find (id) != registry.end()) {
		return (registry.find (id)).data().instance;
	}
	else {
		return 0;
	}

}

std::vector<ToolInterface*> *ToolRegistry::getTools() const {

	std::vector<ToolInterface*>  *tools;
	Registry::ConstIterator it;

	tools = new std::vector<ToolInterface*> ();	
	tools->reserve(registry.count());

	for (it = registry.begin(); it != registry.end(); ++it) {
		tools->push_back(it.data().instance);
	}

	return tools;

}

// ***************
// private methods
// ***************

void ToolRegistry::deleteEntry (Registry::Iterator it) {

	delete it.data().instance;
	if (it.data().isDynamicallyLoaded == true) {
		// unload the library as well
		ToolLoader::unloadTool (it.data().dynamicAccess.toolPath);
	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
