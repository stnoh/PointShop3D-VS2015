// Title:   ToolManager.cpp
// Created: Thu Sep 25 14:10:08 2003
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

#include <qtoolbar.h>
#include <qstring.h>
#include <qevent.h>
#include <qmainwindow.h>

#include "../../Interfaces/src/RendererInterface.h"
#include "../../Interfaces/src/ToolInterface.h"
#include "../../Configuration/src/Configuration.h"
#include "../../RendererManager/src/RendererManager.h"
#include "../../CoreResourceTools/src/CoreResourceTools.h"
#include "../../CoreTools/src/CoreTools.h"
#include "../../CoreTools/NavigationTool/src/NavigationTool.h"
#include "../../CoreTools/ColorPickerTool/src/ColorPickerTool.h"
#include "../../CoreTools/SelectionTool/src/SelectionTool.h"
#include "../../../Utilities/src/Common.h"
#include "ToolDirWalk.h"
#include "ToolRegistry.h"
#include "ToolLoader.h"
#include "ToolManager.h"

// ******
// global
// ******

// the single ToolManager instance, only
// accesible by getInstance() and
// destroyInstance()
static ToolManager *instance = 0;

// **************
// public methods
// **************

ToolManager *ToolManager::getInstance() {

	if (instance == 0) {
		instance = new ToolManager();
	}
	return instance;

}

void ToolManager::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void ToolManager::initialize (const QString toolPluginDirectory, QMainWindow *mainWindow) {

	ToolDirWalk  toolDirWalk (toolPluginDirectory);
	ToolUserData *toolUserData;

	// create the toolbar only for the first time initialize is called
	if (toolBar == 0) {
		toolBar = new QToolBar (mainWindow);
	}
	else if (mainWindow != this->mainWindow) {
		// the old toolbar is managed and deleted by the old parent, no need to delete it here
		toolBar = new QToolBar (mainWindow);
	}
	this->mainWindow = mainWindow;
	mainWindow->moveToolBar (toolBar, QMainWindow::Left);
	toolBar->setPalette (QPalette (Constants::TOOLBAR_BACKGROUND));

	// clear the button group and the toolbar first, unregister all previous tool
	toolBar->clear();
	ToolRegistry::getInstance()->unregisterAllTools();

	// delete the old button group, if necessary
	if (buttonGroup != 0) {
		delete buttonGroup;
	}
	buttonGroup = new QButtonGroup();
	buttonGroup->setExclusive (true);
	// get informed whenever a tool button has been clicked
	this->connect (buttonGroup, SIGNAL (clicked (int)),
	               this, SLOT (handleToolChanged (int)));

	toolUserData = new ToolUserData (toolBar, buttonGroup);

	// sets also the current tool
	this->addDefaultTools();

	if (toolDirWalk.execute (toolUserData) == false) {
		qDebug ("ToolManager: initialize: WARNING: tool plugin directory not found, ignoring plugin tools.");
	}

	delete toolUserData;

}

void ToolManager::checkToolAvailability() {
	
	std::vector<ToolInterface*> *tools;
	std::vector<ToolInterface*>::iterator iter_tools;

	tools = ToolRegistry::getInstance()->getTools();

	for (iter_tools = tools->begin(); iter_tools != tools->end(); ++iter_tools) {

		(*iter_tools)->getToolButton()->setEnabled ((*iter_tools)->isAvailable());			
	}

	// remove unused resource
	delete tools;

}

ToolInterface *ToolManager::getCurrentTool() const {
	return currentTool;
}


void ToolManager::switchToNavigationTool() {
	if (navigationTool->isAvailable() == true) {
		this->handleToolChanged (navigationToolID);

		// since the button was not clicked, we have to explicitly set it
			buttonGroup->setButton (navigationToolID);
	}
}


void ToolManager::handleKeyPressEvent (QKeyEvent *ke) {

	switch (ke->key()) {

		// Color Picker Tool
	case Qt::Key_C:
		{
			if (colorPickerTool->isAvailable() == true) {
				this->handleToolChanged (colorPickerToolID);
				// since the button was not clicked, we have to explicitly set it
				buttonGroup->setButton (colorPickerToolID);
			}
		}
		break;

		// Selection Tool
	case Qt::Key_S:
		{
			if (selectionTool->isAvailable() == true) {
				this->handleToolChanged (selectionToolID);
				// since the button was not clicked, we have to explicitly set it
				buttonGroup->setButton (selectionToolID);
			}
		}
		break;

		// Navigation Tool
	case Qt::Key_N:
		{
			if (navigationTool->isAvailable() == true) {
				this->handleToolChanged (navigationToolID);
				// since the button was not clicked, we have to explicitly set it
				buttonGroup->setButton (navigationToolID);
			}
		}
		break;

	default:
		{
			if (currentTool->getToolButton()->isEnabled() == true) {
				currentTool->handleKeyPressEvent (ke);
			}
		}
		break;

	}  // switch (key)

}

void ToolManager::handleKeyReleaseEvent (QKeyEvent *ke) {
	if (currentTool->getToolButton()->isEnabled() == true) {
		currentTool->handleKeyReleaseEvent (ke);
	}
}

// ***************
// private methods
// ***************

ToolManager::ToolManager () {

	buttonGroup     = 0;
	toolBar         = 0;
	mainWindow      = 0;
	colorPickerTool = 0;
	navigationTool  = 0;
	selectionTool   = 0;

	// get informed whenever the tool directory changed
	this->connect (Configuration::getInstance(), SIGNAL (toolDirectoryChanged (const QString)),
		           this, SLOT (handleToolDirectoryChanged (const QString)));

	// get informed whenever the renderer instance changed
	this->connect (RendererManager::getInstance(), SIGNAL (mainRendererChanged()),
		           this, SLOT (handleMainRendererChanged()));	

}

ToolManager::~ToolManager() {

	// it is IMPORTANT to delete the tool bar first...
	if (toolBar != 0) {
		delete toolBar;
	}

	if (buttonGroup != 0) {
		delete buttonGroup;
	}
	// ... then unload the DLLs. This call also deletes the registered tools
	// (ColorPickerTool, NavigationTool, SelectionTool)
	ToolRegistry::destroyInstance();

}

void ToolManager::addDefaultTools() {

	QToolButton  *toolButton;
	CoreTools    *coreTools;
	ToolRegistry *toolRegistry;

	toolRegistry = ToolRegistry::getInstance();

	// ***********************
	// setup the default tools
	// ***********************

	// *****************
	// create core tools
	// *****************

	// Color Picker Tool
	// this one needs the ColorChooserTool, which must already be setup
	// at this point! ->initialize the ResourceToolBar first.
	colorPickerTool = new ColorPickerTool();
	toolButton = colorPickerTool->addButtonToToolBar (toolBar);
	colorPickerTool->setupColorChooserTool (CoreResourceTools::getInstance()->getColorChooserTool());
	colorPickerToolID = buttonGroup->insert (toolButton);
	toolRegistry->registerTool (colorPickerToolID, colorPickerTool);

	// Selection Tool
	selectionTool = new SelectionTool();
	toolButton = selectionTool->addButtonToToolBar (toolBar);
	selectionToolID = buttonGroup->insert (toolButton);
	toolRegistry->registerTool (selectionToolID, selectionTool);

	// Navigation Tool
	navigationTool = new NavigationTool();
	toolButton = navigationTool->addButtonToToolBar (toolBar);
	navigationToolID = buttonGroup->insert (toolButton);
	toolRegistry->registerTool (navigationToolID, navigationTool);

	// set Navigation Tool as currently selected tool
	toolButton->setOn (true);
	currentTool = navigationTool;
	this->handleToolChanged (navigationToolID);

	// ******************
	// install core tools
	// ******************

	coreTools = CoreTools::getInstance();
	coreTools->setColorPickerTool (colorPickerTool);
	coreTools->setSelectionTool (selectionTool);

	// ******************
	// create other tools
	// ******************
	// note that these tools may depend on the core tools!
	

}

// *************
// private slots
// *************

void ToolManager::handleMainRendererChanged() {

	RendererInterface *renderer;

	// *********************
	// reconnect the signals
	// *********************

	renderer = RendererManager::getInstance()->getMainRenderer();
	if (renderer != 0) {

		if (renderer->isInteractive() == false && currentTool->isAvailable() == false) {
			// if the renderer is non-interactive (no frame buffer) and the current tool has therefore
			// become unavailable switch
			// to the NavigationTool (which doesn't need an interactive renderer) per default...
			this->handleToolChanged (navigationToolID);
			currentTool->getToolButton()->setOn (true);
		}
		else {
			// .. else we simply reconnect the signal
			currentTool->connect (renderer, SIGNAL (rendererWidgetRepainted()),
								  currentTool, SLOT (handleRendererWidgetRepainted()));
		}

	}

}

void ToolManager::handleToolChanged (int id) {
	
	RendererInterface *renderer;
	
	renderer = RendererManager::getInstance()->getMainRenderer();

	// deselect the current tool, so it can release
	// any resources
	currentTool->deselectTool();
	renderer->disconnect (currentTool);

	// choose the new current tool according to its id
	currentTool = ToolRegistry::getInstance()->getTool (id);
	// tell this new tool that it is selected
	currentTool->selectTool();

	// re-connect the tool to the renderer
	currentTool->connect (renderer, SIGNAL (rendererWidgetRepainted()),
						  currentTool, SLOT (handleRendererWidgetRepainted()));
	
	emit toolChanged (currentTool);

}

void ToolManager::handleToolDirectoryChanged (const QString newToolDirectory) {

	// initialize again, adding default tools and dynamically linked tools
	this->initialize (newToolDirectory, mainWindow);

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
