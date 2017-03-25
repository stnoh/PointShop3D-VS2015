// Title:   PluginRegistry.cpp
// Created: Thu Sep 25 14:12:38 2003
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

#include "PluginLoader.h"

#include "PluginRegistry.h"

// ******
// global
// ******

// the single PluginRegistry instance, only
// accesible by getInstance() and
// deleteInstance()
static PluginRegistry *instance = 0;

// ***********
// c'tor/d'tor
// ***********

PluginRegistry::PluginRegistry() {}

PluginRegistry::~PluginRegistry() {
	this->unregisterAllPlugins();
}

// *****************
// Singleton Pattern
// *****************

PluginRegistry *PluginRegistry::getInstance() {

	if (instance == 0) {
		instance = new PluginRegistry();
	}
	return instance;

}

void PluginRegistry::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void PluginRegistry::registerPlugin (const int id, const DynamicAccess dynamicAccess) {
	
	registry.insert (id, dynamicAccess);

}

void PluginRegistry::unregisterPlugin (const int id) {

	Registry::Iterator it;
	
	it = registry.find (id);
	if (it != registry.end()) {
		this->deleteEntry (it);
    	registry.remove (id);
	}

}

void PluginRegistry::unregisterAllPlugins() {

	Registry::Iterator it;

	for (it = registry.begin(); it != registry.end(); ++it) {
		deleteEntry (it);
	}

	registry.clear();

}

PluginLoader::PluginAccess PluginRegistry::getPlugin (const int id) const {

	Registry::ConstIterator    it;
	PluginLoader::PluginAccess pluginAccess;

	it = registry.find (id);

	if (it != registry.end()) {
		pluginAccess = it.data().pluginAccess;
		pluginAccess.isValid = true;
	}
	else {
		pluginAccess.isValid = false;
	}
	
	return pluginAccess;

}

// ***************
// private methods
// ***************

void PluginRegistry::deleteEntry (Registry::Iterator it) {

	// unload the library
	PluginLoader::unloadPlugin (it.data().pluginPath);


}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
