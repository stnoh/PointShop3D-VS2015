// Title:   StatisticsDialogImpl.cpp
// Created: Thu Sep 25 14:42:57 2003
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

#include <qlabel.h>

#include "../Core/Scene/src/Scene.h"
#include "../Core/CoreTools/src/CoreTools.h"
#include "StatisticsDialogImpl.h"

// ******
// global
// ******

// the single StatisticsDialogImpl instance, only
// accesible by getInstance() and
// destroyInstance()
static StatisticsDialogImpl *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

StatisticsDialogImpl::StatisticsDialogImpl (QWidget *parent, const char *name, bool modal, WFlags flags)
    : StatisticsDialogBase (parent, name, modal, flags) {

	this->initDialog();

}

StatisticsDialogImpl::~StatisticsDialogImpl() {
    // no need to delete child widgets, Qt does it all for us
}

// **************
// public methods
// **************

StatisticsDialogImpl *StatisticsDialogImpl::getInstance() {

	if (instance == 0) {
		instance = new StatisticsDialogImpl();
	}
	return instance;

}

void StatisticsDialogImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}

// ***************
// private methods
// ***************

void StatisticsDialogImpl::initDialog() {

	this->updateLabels();

}

void StatisticsDialogImpl::updateLabels() {

	Scene  *scene;
	uint   nofObjects;
	Object *activeObject;
	SelectionTool* selectionTool;

	scene = Scene::getInstance();
	selectionTool = CoreTools::getInstance()->getSelectionTool();

	nofObjects = scene->getNofObjects();
	nofObjectsLabel->setText (QString::number (nofObjects));
	nofSurfelsLabel->setText (QString::number (scene->getNofSurfels()));
		
	nofSelected1SurfelsLabel->setText (QString::number (selectionTool->getSelectionSize (SurfelInterface::SELECTED1)));
	nofSelected2SurfelsLabel->setText (QString::number (selectionTool->getSelectionSize (SurfelInterface::SELECTED2)));
	nofSelected3SurfelsLabel->setText (QString::number (selectionTool->getSelectionSize (SurfelInterface::SELECTED3)));

	mainRendererLabel->setText (RendererManager::getInstance()->getMainRendererType() + ": ");
	mainRendererFPSLabel->setText (QString::number (RendererManager::getInstance()->getMainRendererFPS(), 'g', 3) + " FPS");
	previewRendererLabel->setText (RendererManager::getInstance()->getPreviewRendererType() + ": ");
	previewRendererFPSLabel->setText (QString::number (RendererManager::getInstance()->getPreviewRendererFPS(), 'g', 3) + " FPS");
	if (nofObjects > 0) {

		activeObject = scene->getActiveObject();
		activeObjectNameLabel->setText (activeObject->getName());
		activeObjectNofSurfelsLabel->setText (QString::number (activeObject->getSurfelCollection()->getNofSurfels()));

	}
	else {

		activeObjectNameLabel->setText ("-");
		activeObjectNofSurfelsLabel->setText ("0");

	}

}

// ************
// public slots
// ************

bool StatisticsDialogImpl::close(bool autoDelete) {

	RendererManager *rendererManager;
	// get informed whenever the number of objects in the scene changes
	this->disconnect (Scene::getInstance(), SIGNAL (objectAdded()),
		           this, SLOT (handleChanges()));

	// get informed whenever the number of objects in the scene changes
	this->disconnect (Scene::getInstance(), SIGNAL (objectRemoved()),
		           this, SLOT (handleChanges()));

	this->disconnect (Scene::getInstance(), SIGNAL (objectModified(bool)),
					this, SLOT (handleChanges()));

	// get informed whenever the objects in the scene change
	this->disconnect (Scene::getInstance(), SIGNAL (activeObjectChanged ()),
		           this, SLOT (handleChanges()));

	// get informed whenever the selection changed
	this->disconnect (CoreTools::getInstance()->getSelectionTool(), SIGNAL (selectionChanged (SurfelInterface::Flag)),
		           this, SLOT (handleSelectionChanged (SurfelInterface::Flag)));

	rendererManager = RendererManager::getInstance();
	// get informed whenever the main renderer changed
	this->disconnect (rendererManager, SIGNAL (mainRendererChanged()),
		           this, SLOT (handleMainRendererChanged()));

	this->disconnect (rendererManager, SIGNAL (previewRendererChanged()),
		           this, SLOT (handlePreviewRendererChanged()));

	this->disconnect (rendererManager, SIGNAL (mainRendererFPSChanged (const float)),
		           this, SLOT (handleMainRendererFPSChanged (const float)));

	this->disconnect (rendererManager, SIGNAL (previewRendererFPSChanged (const float)),
		           this, SLOT (handlePreviewRendererFPSChanged (const float)));

	return StatisticsDialogBase::close(autoDelete);
}

void StatisticsDialogImpl::show() {

	RendererManager *rendererManager;
	// get informed whenever the number of objects in the scene changes
	this->connect (Scene::getInstance(), SIGNAL (objectAdded()),
		           this, SLOT (handleChanges()));

	// get informed whenever the number of objects in the scene changes
	this->connect (Scene::getInstance(), SIGNAL (objectRemoved()),
		           this, SLOT (handleChanges()));

	this->connect (Scene::getInstance(), SIGNAL (objectModified(bool)),
					this, SLOT (handleChanges()));

	// get informed whenever the objects in the scene change
	this->connect (Scene::getInstance(), SIGNAL (activeObjectChanged ()),
		           this, SLOT (handleChanges()));

	// get informed whenever the selection changed
	this->connect (CoreTools::getInstance()->getSelectionTool(), SIGNAL (selectionChanged (SurfelInterface::Flag)),
		           this, SLOT (handleSelectionChanged (SurfelInterface::Flag)));

	rendererManager = RendererManager::getInstance();
	// get informed whenever the main renderer changed
	this->connect (rendererManager, SIGNAL (mainRendererChanged()),
		           this, SLOT (handleMainRendererChanged()));

	this->connect (rendererManager, SIGNAL (previewRendererChanged()),
		           this, SLOT (handlePreviewRendererChanged()));

	this->connect (rendererManager, SIGNAL (mainRendererFPSChanged (const float)),
		           this, SLOT (handleMainRendererFPSChanged (const float)));

	this->connect (rendererManager, SIGNAL (previewRendererFPSChanged (const float)),
		           this, SLOT (handlePreviewRendererFPSChanged (const float)));

	StatisticsDialogBase::show();
}

// *************
// private slots
// *************

void StatisticsDialogImpl::handleChanges() {	
	CoreTools::getInstance()->getSelectionTool()->updateSelectionCounters();
	this->updateLabels();
}

void StatisticsDialogImpl::handleSelectionChanged (SurfelInterface::Flag selectionFlag) {

	(void)selectionFlag;
	this->updateLabels();
}

void StatisticsDialogImpl::handleMainRendererChanged() {
	mainRendererLabel->setText (RendererManager::getInstance()->getMainRendererType() + ": ");
}

void StatisticsDialogImpl::handlePreviewRendererChanged() {
	previewRendererLabel->setText (RendererManager::getInstance()->getPreviewRendererType() + ": ");
}

void StatisticsDialogImpl::handleMainRendererFPSChanged (const float mainRendererFPS) {
	mainRendererFPSLabel->setText (QString::number (mainRendererFPS, 'g', 3) + " FPS");
}

void StatisticsDialogImpl::handlePreviewRendererFPSChanged (const float previewRendererFPS) {
	previewRendererFPSLabel->setText (QString::number (previewRendererFPS, 'g', 3) + " FPS");
}



// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
