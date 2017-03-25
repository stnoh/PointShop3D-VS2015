// Title:   ClipBoard.cpp
// Created: Thu Sep 25 14:09:08 2003
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

#include "../Scene/src/Object.h"
#include "../Scene/src/Scene.h"
#include "../CoreTools/src/CoreTools.h"

#include "ClipBoard.h"

// ******
// global
// ******

// the single ClipBoard instance, only
// accesible by getInstance() and
// destroyInstance()
static ClipBoard *instance = 0; 

// **************
// public methods
// **************

ClipBoard *ClipBoard::getInstance() {

	if (instance == 0) {
		instance = new ClipBoard();
	}
	return instance;

}

void ClipBoard::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

Object *ClipBoard::getContent() const {
	return content;
}

void ClipBoard::clear() {

	if (content != 0) {
		delete content;
		content = 0;
	}

}

void ClipBoard::cut() {

	std::vector<SurfelInterface*> *selection;

	// delete the previous content, if necessary
	if (content != 0) {
		delete content;
	}
	
	content = new Object ("NewObject");
	selection = CoreTools::getInstance()->getSelectionTool()->getSelection();
	content->setSurfelCollection (selection);

	// cut the selection
	Scene::getInstance()->removeSurfels (selection);

	CoreTools::getInstance()->getSelectionTool()->updateSelectionCounters();

}

void ClipBoard::cutObject() {

	Scene  *scene;

	scene = Scene::getInstance();

	// delete the previous copy, if necessary
	if (content != 0) {
		delete content;
	}
	content = scene->getActiveObject()->copy();

	// cut the active object
	scene->removeObject (scene->getActiveObject());

}

void ClipBoard::copy() {

	std::vector<SurfelInterface*> *selection;
	SurfelInterface              *surfel;
	Scene                        *scene;
	SurfelCollection             *surfelCollection;

	// delete the previous content, if necessary
	if (content != 0) {
		delete content;
	}
	
	content = new Object ("NewObject");
	selection = CoreTools::getInstance()->getSelectionTool()->getSelection();
	content->setSurfelCollection (selection);

	// we have to turn off the SELECTED and EMPHASISE flags in the content's surfelCollection
	surfelCollection = content->getSurfelCollection();
	scene = Scene::getInstance();
	for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {		
		scene->setSurfelFlagOn (surfel, (SurfelInterface::Flag)(SurfelInterface::getSelectionFlags() | SurfelInterface::EMPHASISE), false, false);
	}

}

void ClipBoard::copyObject() {

	Scene  *scene;

	scene = Scene::getInstance();

	// delete the previous copy, if necessary
	if (content != 0) {
		delete content;
	}
	content = scene->getActiveObject()->copy();

}

void ClipBoard::paste() {

	Object *pastedObject;

	if (content != 0) {
		pastedObject = content->copy();
		Scene::getInstance()->addObject (pastedObject);
	}

}

// ***************
// private methods
// ***************

ClipBoard::ClipBoard() {
	content = 0;
}

ClipBoard::~ClipBoard() {
	
	if (content != 0) {
		delete content;
	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
