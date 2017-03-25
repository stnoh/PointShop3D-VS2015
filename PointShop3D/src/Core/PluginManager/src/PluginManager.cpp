// Title:   PluginManager.cpp
// Created: Thu Sep 25 14:12:50 2003
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

#define INCLUDE_MENUITEM_DEF
#include <qmenudata.h>

#include <qstring.h>
#include <qwidget.h>
#include <qpopupmenu.h> 


#include "../../Interfaces/src/PluginInterface.h"
#include "../../Configuration/src/Configuration.h"

#include "PluginRegistry.h"
#include "PluginManager.h"

// ******
// global
// ******

// the single PluginManager instance, only
// accesible by getInstance() and
// destroyInstance()
static PluginManager *instance = 0;

// **************
// public methods
// **************

PluginManager *PluginManager::getInstance() {

	if (instance == 0) {
		instance = new PluginManager();
	}
	return instance;

}

void PluginManager::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void PluginManager::initialize (const QString pluginDirectory, QMenuBar *menuBar, int *pluginMenuID) {

	PluginDirWalk pluginDirWalk (pluginDirectory);
	PluginUserData *pluginUserData;

	// create the toolbar only for the first time initialize is called
	if (pluginMenu == 0) {
		pluginMenu = new QPopupMenu (menuBar);
	}
	else if (this->menuBar != menuBar) {
		// the old popupmenu is managed and deleted by the old parent, no need to delete it here
		pluginMenu = new QPopupMenu (menuBar);
	}
	this->menuBar = menuBar;
	*pluginMenuID = menuBar->insertItem ("&Plugins", pluginMenu);

	pluginUserData = new PluginUserData (pluginMenu, this, SLOT (handlePluginSelected(int)));

	// unregister all previous plugins first, clear the menu
	PluginRegistry::getInstance()->unregisterAllPlugins();
	pluginMenu->clear();

	if (pluginDirWalk.execute (pluginUserData) == false) {
		qDebug ("PluginManager: initialize: WARNING: plugin directory %s not found, ignoring plugins.", pluginDirectory.latin1());
	};
	
	delete pluginUserData;

}

void PluginManager::checkPluginAvailability() {	
	this->enablePluginItems (pluginMenu);
}

// ***************
// private methods
// ***************

PluginManager::PluginManager() {

	pluginMenu = 0;
	menuBar    = 0;

	// get informed whenever the plugin directory changed
	this->connect (Configuration::getInstance(), SIGNAL (pluginDirectoryChanged (const QString)),
		           this, SLOT (handlePluginDirectoryChanged (const QString)));

}

PluginManager::~PluginManager() {

	// IMPORTANT: first clear...
	if (pluginMenu != 0) {
		pluginMenu->clear();
	}

	// ... then unload DLLs
	PluginRegistry::destroyInstance();

}

void PluginManager::enablePluginItems (QPopupMenu *menu) {

	PluginLoader::PluginAccess pluginAccess;
	PluginInterface            *plugin;
	int                        index,
		                       nofItems,
							   itemID;
	QMenuItem                  *menuItem;

	// temporarily create all plugins, check if they are available and enable/disable
	// their corresponding menu entries
	nofItems = menu->count();
	for (index = 0; index < nofItems; index++) {

		itemID = menu->idAt (index);

		pluginAccess = (PluginRegistry::getInstance())->getPlugin (itemID);

		if (pluginAccess.isValid == true) {

			// this is a plugin
			plugin = pluginAccess.createPluginFunction();		
			pluginMenu->setItemEnabled (itemID, plugin->isAvailable());		
			delete plugin;

		}
		else {
			// this is a submenu
			menuItem = menu->findItem (itemID);

			// go into this submenu recursively
			this->enablePluginItems (menuItem->popup());

		}

	}

}

// *************
// private slots
// *************

void PluginManager::handlePluginSelected (int id) {

	PluginLoader::PluginAccess pluginAccess;
	PluginInterface            *plugin;

	pluginAccess = (PluginRegistry::getInstance())->getPlugin (id);
	plugin = pluginAccess.createPluginFunction();	
	plugin->execute();
	delete plugin;

}

void PluginManager::handlePluginDirectoryChanged (const QString newPluginDirectory) {

	int pluginMenuID;

	// initialize again, adding default tools and dynamically linked tools
	this->initialize (newPluginDirectory, menuBar, &pluginMenuID);

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
