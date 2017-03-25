// Title:   RendererRegistry.cpp
// Created: Thu Sep 25 14:11:21 2003
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

#include "RendererLoader.h"
#include "RendererRegistry.h"

// ******
// global
// ******

// the single RendererRegistry instance, only
// accesible by getInstance() and
// deleteInstance()
static RendererRegistry *instance = 0;

// ***********
// c'tor/d'tor
// ***********

RendererRegistry::RendererRegistry() {}

RendererRegistry::~RendererRegistry() {
	this->unregisterAllRendererPlugins();
}

// *****************
// Singleton Pattern
// *****************

RendererRegistry *RendererRegistry::getInstance() {

	if (instance == 0) {
		instance = new RendererRegistry();
	}
	return instance;

}

void RendererRegistry::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void RendererRegistry::registerRendererPlugin (const QString rendererType, const DynamicAccess dynamicAccess) {
	registry.insert (rendererType, dynamicAccess);
}

void RendererRegistry::unregisterRendererPlugin (const QString rendererType) {

	Registry::Iterator it;
	
	it = registry.find (rendererType);
	if (it != registry.end()) {
		this->deleteEntry (it);
    	registry.remove (rendererType);
	}

}

void RendererRegistry::unregisterAllRendererPlugins() {

	Registry::Iterator it;

	for (it = registry.begin(); it != registry.end(); ++it) {
		this->deleteEntry (it);
	}

	registry.clear();

}

RendererLoader::RendererAccess RendererRegistry::getRendererAccess (const QString rendererType) const {

	Registry::ConstIterator        it;
	RendererLoader::RendererAccess rendererAccess;

	it = registry.find (rendererType);

	if (it != registry.end()) {
		rendererAccess = it.data().rendererAccess;
		rendererAccess.isValid = true;
	}
	else {
		rendererAccess.isValid = false;
	}
	
	return rendererAccess;

}

// ***************
// private methods
// ***************

void RendererRegistry::deleteEntry (Registry::Iterator it) {
	// unload the library
	RendererLoader::unloadRendererPlugin (it.data().rendererPluginPath);
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
