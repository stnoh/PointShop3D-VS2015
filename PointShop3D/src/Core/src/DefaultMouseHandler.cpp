// Title:   DefaultMouseHandler.cpp
// Created: Thu Sep 25 14:08:43 2003
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

#include <qevent.h>

#include "../Interfaces/src/ToolInterface.h"
#include "../ToolManager/src/ToolManager.h"
#include "../Scene/src/Scene.h"

#include "DefaultMouseHandler.h"

void DefaultMouseHandler::handleMousePressEvent (QMouseEvent *me) {

	ToolInterface *currentTool;
	ToolManager   *toolManager;

	toolManager = ToolManager::getInstance();
	currentTool = toolManager->getCurrentTool();
	if (currentTool->isAvailable() == true) {
		currentTool->handleMousePressEvent (me);
	}
	
}

void DefaultMouseHandler::handleMouseMoveEvent (QMouseEvent *me) {

	ToolInterface *currentTool;
	ToolManager   *toolManager;

	toolManager = ToolManager::getInstance();
	currentTool = toolManager->getCurrentTool();
	if (currentTool->isAvailable() == true) {
		currentTool->handleMouseMoveEvent (me);
	}

}

void DefaultMouseHandler::handleMouseReleaseEvent (QMouseEvent *me) {
	
	ToolInterface *currentTool;
	ToolManager   *toolManager;

	toolManager = ToolManager::getInstance();
	currentTool = toolManager->getCurrentTool();
	if (currentTool->isAvailable() == true) {
		currentTool->handleMouseReleaseEvent (me);
	}

}

void DefaultMouseHandler::handleMouseWheelEvent (QWheelEvent *we) {

	we->accept();
	if (we->state() & QWheelEvent::ControlButton) {
		Scene::getInstance()->getActiveObject()->translate (0.0f, 0.0f, we->delta());
	}
	else {
		Scene::getInstance()->translate (0.0f, 0.0f, we->delta());
	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
