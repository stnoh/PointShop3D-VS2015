// Title:   BitmapMarker.cpp
// Created: Thu Sep 25 14:58:38 2003
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

#include <qpixmap.h>
#include <qlabel.h>
#include <qcursor.h>

#include "ParameterizationDialogImpl.h"
#include "../../../../Core/MarkerManager/src/MarkerUtils.h"
#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "../../../../Core/MarkerManager/src/Marker2D.h"
#include "BitmapMarker.h"

// ******
// global
// ******

// the single BitmapMarker instance, only
// accesible by getInstance() and
// destroyInstance()
static BitmapMarker *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

BitmapMarker::BitmapMarker (QWidget* parent,
							const char* name,
							WFlags flags)
    : QLabel (parent, name, flags) {

	MarkerManager   *markerManager;

	this->insertBoundaryCondition = insertBoundaryCondition;

	this->setScaledContents (true);
	this->setMinimumSize (QSize (64, 64));
	this->setCursor (Qt::crossCursor);

	isDragging = false;
	
	markerManager = MarkerManager::getInstance();
	markerManager->setWidget2D (this);

}

BitmapMarker::~BitmapMarker() {
    // no need to delete child widgets, Qt does it all for us
}

// **************
// public methods
// **************

BitmapMarker *BitmapMarker::getInstance() {

	if (instance == 0) {
		instance = new BitmapMarker (0, 0);
	}
	return instance;

}

void BitmapMarker::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}

void BitmapMarker::setBoundaryConditionEnabled (const bool enable) {
	insertBoundaryCondition = enable;
}

bool BitmapMarker::isBoundaryConditionEnabled() const {
	return insertBoundaryCondition;
}

void BitmapMarker::displayPixmap (const QPixmap *pixmap) {

	if (pixmap != 0) {		
		this->setPixmap (*pixmap);
		this->adjustSize();
	}
	else {
		this->clear();
	}

}

// *****************
// protected methods
// *****************

void BitmapMarker::mousePressEvent (QMouseEvent *me) {

	MarkerManager *markerManager;

	markerManager = MarkerManager::getInstance();

	if (me->button() == QMouseEvent::LeftButton) {

		// **********************************
		// modify the fitting constraint list
		// **********************************

		selectedMarker = (Marker2D *)MarkerUtils::pickMarker (me->pos(), (QList<MarkerInterface> *)markerManager->getFittingConstraints2D(), this);
		if (selectedMarker == 0) {
			MarkerUtils::deselectAll ((QList<MarkerInterface> *)markerManager->getFittingConstraints2D(), this);
		}

	}

}

void BitmapMarker::mouseMoveEvent (QMouseEvent *me) {

	if (me->state() == QMouseEvent::LeftButton) {

		if (selectedMarker != 0) {
			isDragging = true;
			keepMarker = selectedMarker->moveTo (me->pos(), this);
		}

	}

}

void BitmapMarker::mouseReleaseEvent (QMouseEvent *me) {

	Marker2D                       *marker;
	MarkerManager                  *markerManager;
	MyDataTypes::TextureCoordinate textureCoordinate;

	markerManager = MarkerManager::getInstance();
	if (me->button() == QMouseEvent::LeftButton) {

		// **********************************
		// modify the fitting constraint list
		// **********************************

		if (selectedMarker == 0 && isDragging == false) {

			// *****************
			// create new marker
			// *****************

			textureCoordinate.u = (float)me->x() / (float)(this->size().width());
			textureCoordinate.v = (float)me->y() / (float)(this->size().height());
			marker = new Marker2D (textureCoordinate, true, markerManager->getFittingConstraints2D()->count() + 1,
								   MarkerInterface::FITTING_CONSTRAINT_STYLE);

			markerManager->addFittingConstraint2D (marker);
			marker->togglePainting (this);

		}
		else if (selectedMarker != 0 && isDragging == false) {

			// ****************
			// toggle selection
			// ****************

			MarkerUtils::toggleSelection (selectedMarker, this);

		}
		else {

			// ********************
			// check dragged marker
			// ********************

			if (keepMarker == false) {
				markerManager->removeFittingConstraint2D (selectedMarker);
			}
			isDragging = false;

		}

	}
	else if (me->button() == QMouseEvent::RightButton) {
		MarkerUtils::deselectAll ((QList<MarkerInterface> *)markerManager->getFittingConstraints2D(), this);
	}

}

void BitmapMarker::paintEvent (QPaintEvent *pe) {
	QLabel::paintEvent (pe);
	MarkerManager::getInstance()->toggleAllFittingConstraint2DVisualization();
}

// ***************
// private methods
// ***************



// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
