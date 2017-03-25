// Title:   MarkerManager.cpp
// Created: Thu Sep 25 14:15:50 2003
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

#include <qwidget.h>

#include "../../Scene/src/Scene.h"
#include "../../RendererManager/src/RendererManager.h"
#include "Marker2D.h"
#include "Marker3D.h"
#include "MarkerUtils.h"

#include "MarkerManager.h"

// ******
// global
// ******

// the single MarkerManager instance, only
// accesible by getInstance() and
// destroyInstance()
static MarkerManager *instance = 0;

// **************
// public methods
// **************

MarkerManager *MarkerManager::getInstance() {

	if (instance == 0) {
		instance = new MarkerManager();
	}
	return instance;

}

void MarkerManager::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void MarkerManager::initialize (QWidget *widget2D, QWidget *widget3D) {
	this->widget2D = widget2D;
	this->widget3D = widget3D;
}

void MarkerManager::setWidget2D (QWidget *newWidget2D) {
	widget2D = newWidget2D;
}

QWidget *MarkerManager::getWidget2D() const {
	return widget2D;
}

void MarkerManager::setWidget3D (QWidget *newWidget3D) {
	widget3D = newWidget3D;
}

QWidget *MarkerManager::getWidget3D() const {
	return widget3D;
}

// ***********
// list access
// ***********

const QList<Marker2D> *MarkerManager::getFittingConstraints2D() const {
	return &fittingConstraints2D;
}

const QList<Marker3D> *MarkerManager::getFittingConstraints3D() const {
	return &fittingConstraints3D;
}

uint MarkerManager::getMinimumFittingConstraintListCount() const {
	return ((fittingConstraints2D.count() < fittingConstraints3D.count()) ? fittingConstraints2D.count() : fittingConstraints3D.count());
}

// *****************
// list modification
// *****************

void MarkerManager::addFittingConstraint2D (const Marker2D *marker2D) {
	fittingConstraints2D.append (marker2D);
	emit fittingConstraints2DChanged (&fittingConstraints2D);
}

void MarkerManager::addFittingConstraints2D (const QList<Marker2D> *markers2D) {

	QListIterator<Marker2D> it(*markers2D);

	for ( ; it.current() != 0; ++it) {
		fittingConstraints2D.append (it.current());
	}
	emit fittingConstraints2DChanged (&fittingConstraints2D);

}

void MarkerManager::removeFittingConstraint2D (Marker2D *marker2D) {

	fittingConstraints2D.removeRef (marker2D);
	MarkerUtils::reallocateIDs ((QList<MarkerInterface> *)&fittingConstraints2D, widget2D);
	emit fittingConstraints2DChanged (&fittingConstraints2D);

}

void MarkerManager::removeSelectedFittingConstraints2D() {

	MarkerInterface *marker2D;
	bool            hasListChanged;

	marker2D = fittingConstraints2D.first();
	hasListChanged = false;
	while (marker2D != 0) {

		if (marker2D->isSelected() == true) {

			marker2D->togglePainting (widget2D);
			// remove the current item, the current item is automatically the next item
			// note: if the last item is removed
			//       - in Qt 2.x: the new last item (the one just before the deleted item) becomes the 
			//                    new current item, if there is one
			//       - in Qt 2.x: the new current item is set to 0
			//       -> in Qt 2.x the second last item might be checked twice
			fittingConstraints2D.remove();
			marker2D = fittingConstraints2D.current();
			hasListChanged = true;

		}
		else {
			// check the next item
			marker2D = fittingConstraints2D.next();
		}

	}

	// check if list has changed, re-enumerate and notify event receivers
	if (hasListChanged == true) {	
		MarkerUtils::reallocateIDs ((QList<MarkerInterface> *)&fittingConstraints2D, widget2D);
		emit fittingConstraints2DChanged (&fittingConstraints2D);
	}

}

void MarkerManager::removeAllFittingConstraints2D() {

	this->toggleAllFittingConstraint2DVisualization();
	fittingConstraints2D.clear();
	emit fittingConstraints2DChanged (&fittingConstraints2D);

}

void MarkerManager::addFittingConstraint3D (const Marker3D *marker3D) {
	fittingConstraints3D.append (marker3D);
	emit fittingConstraints3DChanged (&fittingConstraints3D);
}

void MarkerManager::addFittingConstraints3D (const QList<Marker3D> *markers3D) {

	QListIterator<Marker3D> it(*markers3D);

	for ( ; it.current() != 0; ++it) {
		fittingConstraints3D.append (it.current());
	}
	emit fittingConstraints3DChanged (&fittingConstraints3D);

}

void MarkerManager::removeFittingConstraint3D (Marker3D *marker3D) {

	fittingConstraints3D.removeRef (marker3D);
	MarkerUtils::reallocateIDs ((QList<MarkerInterface> *)&fittingConstraints3D, widget3D);
	emit fittingConstraints3DChanged (&fittingConstraints3D);

}

void MarkerManager::removeSelectedFittingConstraints3D() {

	MarkerInterface *marker3D;
	bool            hasListChanged;

	marker3D = fittingConstraints3D.first();
	hasListChanged = false;
	while (marker3D != 0) {

		if (marker3D->isSelected() == true) {

			marker3D->togglePainting (widget3D);
			// remove the current item, the current item is automatically the next item
			// note: if the last item is removed
			//       - in Qt 2.x: the new last item (the one just before the deleted item) becomes the 
			//                    new current item, if there is one
			//       - in Qt 2.x: the new current item is set to 0
			//       -> in Qt 2.x the second last item might be checked twice
			fittingConstraints3D.remove();
			marker3D = fittingConstraints3D.current();
			hasListChanged = true;

		}
		else {
			// check the next item
			marker3D = fittingConstraints3D.next();
		}

	}

	// check if list has changed, re-enumerate and notify event receivers
	if (hasListChanged == true) {	
		MarkerUtils::reallocateIDs ((QList<MarkerInterface> *)&fittingConstraints3D, widget3D);
		emit fittingConstraints3DChanged (&fittingConstraints3D);
	}

}

void MarkerManager::removeAllFittingConstraints3D (const bool visualize) {

	if (visualize == true) {
		this->toggleAllFittingConstraint3DVisualization();
	}
	fittingConstraints3D.clear();
	emit fittingConstraints3DChanged (&fittingConstraints3D);

}

Marker3D* MarkerManager::getMarker3D(const SurfelInterface *surfel) {

	Marker3D* marker3D;

	for (marker3D = fittingConstraints3D.first(); marker3D != 0; marker3D = fittingConstraints3D.next()) {
		if(marker3D->getSurfel() ==  surfel) {
			return marker3D;
		}
	}

	return 0;
}

// ******************
// list visualization
// ******************

void MarkerManager::toggleAllFittingConstraint2DVisualization() {

	MarkerInterface *marker2D;

	for (marker2D = fittingConstraints2D.first(); marker2D != 0; marker2D = fittingConstraints2D.next()) {
		marker2D->togglePainting (widget2D);
	}

}

void MarkerManager::toggleAllFittingConstraint3DVisualization() {

	MarkerInterface *marker3D;

	for (marker3D = fittingConstraints3D.first(); marker3D != 0; marker3D = fittingConstraints3D.next()) {
		marker3D->togglePainting (widget3D);
	}

}

// ***************
// private methods
// ***************

MarkerManager::MarkerManager() {

	fittingConstraints2D.setAutoDelete (true);
	fittingConstraints3D.setAutoDelete (true);

	// get informed whenever the renderer changes
	this->connect (RendererManager::getInstance(), SIGNAL (mainRendererChanged()),
		           this, SLOT (handleMainRendererChanged()));

	// get informed whenever the active object changes. since 3D markers are only allowed on the active object,
	// this signal causes all 3D markers to be deleted.
	this->connect (Scene::getInstance(), SIGNAL (activeObjectAboutToChange()),
		           this, SLOT (handleActiveObjectAboutToChange()));

}

MarkerManager::~MarkerManager() {}

// *************
// private slots
// *************

void MarkerManager::handleMainRendererChanged() {	
	widget3D = RendererManager::getInstance()->getMainRenderer()->getRendererWidget();
}

void MarkerManager::handleActiveObjectAboutToChange() {	

	Scene *scene = Scene::getInstance();
	Object *object = scene->getActiveObject();
	
	// if an object with markers is loaded, there is no active object when this method
	// is called!
	if(object != 0) {
		this->removeAllFittingConstraints3D(true);
	}
}




// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
