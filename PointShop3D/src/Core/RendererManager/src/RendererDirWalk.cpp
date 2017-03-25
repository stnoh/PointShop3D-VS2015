// Title:   RendererDirWalk.cpp
// Created: Thu Sep 25 14:12:28 2003
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

#include <qstring.h>

#include "../../../Utilities/UserMessages/src/SplashScreen.h"
#include "../../../Utilities/DirWalk/src/DirWalk.h"
#include "RendererRegistry.h"
#include "RendererLoader.h"

#include "RendererDirWalk.h"

RendererUserData::RendererUserData (QStringList *availableRenderers) {
	this->availableRenderers = availableRenderers;
}

RendererUserData::~RendererUserData() {}

UserDataInterface *RendererUserData::copy() {
	return new RendererUserData (availableRenderers);
}

void RendererUserData::copy (UserDataInterface *source) {
	
	RendererUserData *rendererUserData;

	rendererUserData = (RendererUserData *)source;
	availableRenderers = rendererUserData->availableRenderers;

}

RendererDirWalk::RendererDirWalk (QString rendererPluginDirectory)
	: DirWalk (rendererPluginDirectory, QDir::Files | QDir::NoSymLinks) {}

RendererDirWalk::~RendererDirWalk() {}

DirWalk *RendererDirWalk::createNewInstance (const QString directoryName) {
	return new RendererDirWalk (directoryName);
}

void RendererDirWalk::applyFileMethod() {

	RendererUserData                *rendererUserData;
	RendererLoader::RendererAccess  pluginAccess;
	QString                         rendererType;
	RendererRegistry::DynamicAccess dynamicAccess;

	rendererUserData = (RendererUserData *)userData;

	// try and see if the current file is a renderer plugin
	pluginAccess = RendererLoader::loadRendererPlugin (QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1());
	if (pluginAccess.isValid == true) {
		
		// register the plugin
		dynamicAccess.rendererAccess     = pluginAccess;
		dynamicAccess.rendererPluginPath = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1();
		rendererType = QString (pluginAccess.getRendererTypeFunction());
		RendererRegistry::getInstance()->registerRendererPlugin (rendererType, dynamicAccess);
		rendererUserData->availableRenderers->append (pluginAccess.getRendererTypeFunction());
		SplashScreen::getInstance()->showText (QString ("Renderer %1 registered").arg (pluginAccess.getRendererTypeFunction()));
	
	}
	else {
		qDebug ("RendererDirWalk: applyFileMethod: File %s is not a valid renderer plugin, ignoring it.", currentFileName.latin1());
	}

}

void RendererDirWalk::applyDirectoryMethod() {}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
