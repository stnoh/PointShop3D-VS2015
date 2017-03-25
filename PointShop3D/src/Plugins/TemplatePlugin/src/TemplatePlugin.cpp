// Title:   TemplatePlugin.cpp
// Created: Thu Sep 25 14:45:18 2003
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


#include <qobject.h>

#include "TemplatePlugin.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "This is a template plugin"
"<ul><li>Enter here a short description of this plugin</li>"
"<li>You may even use simple <i>HTML formated</i> text</li></ul>";

// **************
// public methods
// **************

TemplatePlugin::TemplatePlugin() {}
TemplatePlugin::~TemplatePlugin() {}

void TemplatePlugin::execute() {
	// TODO: implement plugin code here
	qDebug ("Template plugin has been executed");
}

bool TemplatePlugin::isAvailable() const {
	return this->checkPluginAvailability();
}

// ***************
// private methods
// ***************

bool TemplatePlugin::checkPluginAvailability() const {
	// TODO: check here any conditions, e.g.
	//       - if an interactive main renderer is present
	//       - if an Object is in the Scene
	//       - etc.
	return true;
}

// ****************
// access functions
// ****************

PLUGIN_API PluginInterface *createPlugin() {
	return new TemplatePlugin();
}

PLUGIN_API const char *getPluginName() {
	// TODO: return a plugin name here which is to appear in the plugin menu
	return "Template";
}

PLUGIN_API const char *getPluginDescription() {
	return whatsThisText;
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
