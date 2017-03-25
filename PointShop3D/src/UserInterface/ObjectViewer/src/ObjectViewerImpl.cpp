// Title:   ObjectViewerImpl.cpp
// Created: Thu Sep 25 14:42:12 2003
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
#include <qlistbox.h>
#include <qstring.h>
#include <qpushbutton.h>

#include "../../../Core/Scene/src/Scene.h"
#include "../../../Core/Scene/src/Object.h"


#include "ObjectViewerImpl.h"

// the single instance, only accesible by getInstance() and destroyInstance()
static ObjectViewerImpl *instance = 0;

ObjectViewerImpl::ObjectViewerImpl (QWidget* parent, const char* name, bool modal, WFlags flags) :
	                              ObjectViewer(parent, name, modal, flags) {


	// get informed whenever the number of objects in the scene changes
	this->connect (Scene::getInstance(), SIGNAL (objectAdded()),
		           this, SLOT (handleObjectListChanged()));

	// get informed whenever the number of objects in the scene changes
	this->connect (Scene::getInstance(), SIGNAL (objectRemoved()),
		           this, SLOT (handleObjectListChanged()));

	handleObjectListChanged();


}

ObjectViewerImpl::~ObjectViewerImpl() {


}

ObjectViewerImpl *ObjectViewerImpl::getInstance () {

	if (instance == 0) {
		instance = new ObjectViewerImpl ();
	}
	return instance;

}

void ObjectViewerImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}



void ObjectViewerImpl::showEvent (QShowEvent *se) {


}


// ***************
// private methods
// ***************





// *************
// private slots
// *************


void ObjectViewerImpl::hideShow() {

	Scene            *scene;
	Object           *object;
	int				 i;

	scene = Scene::getInstance();

	for (object = scene->getFirstObject(), i = 0; (i < this->objectsListBox->currentItem()) && (object !=0); object = scene->getNextObject(), i++);


	object->setFlag (Object::VISIBLE, !object->isFlagOn (Object::VISIBLE));

	if (object->isFlagOn (Object::VISIBLE)) {
		this->hideShowPushButton->setText ("Hide");
	}
	else {
		this->hideShowPushButton->setText ("Show");
	}

	handleObjectListChanged();
	this->objectsListBox->setCurrentItem (i);

	scene->emitRenderFrameSignal();

}



void ObjectViewerImpl::activateDeactivate() {


}



void ObjectViewerImpl::handleObjectListChanged () {


	Scene            *scene;
	Object           *object;
	QString          text;

	scene = Scene::getInstance();

	this->objectsListBox->clear();

	for (object = scene->getFirstObject(); object !=0; object = scene->getNextObject()) {

		text = "";

		if (object->isFlagOn (Object::VISIBLE)) {
			text += "V ";
		}
		else {
			text += "  ";
		}

		if (object->isFlagOn (Object::ENABLED)) {
			text += "A ";
		}
		else {
			text += "  ";
		}

		text += object->getName();

		this->objectsListBox->insertItem (text);
	}

	this->objectsListBox->setCurrentItem (0);

}
	
// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
