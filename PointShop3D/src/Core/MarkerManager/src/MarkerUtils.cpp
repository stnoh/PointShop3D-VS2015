// Title:   MarkerUtils.cpp
// Created: Thu Sep 25 14:15:39 2003
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

#include <float.h>

#include <qpoint.h>
#if QT_VERSION >= 0x030000
# include <qptrlist.h>
# ifndef QList
#  define QList QPtrList
# endif
# ifndef QListIterator
#  define QListIterator QPtrListIterator
# endif
#else
# include <qlist.h>
#endif

#include "../../../Utilities/src/MyMathTools.h"
#include "../../../DataTypes/src/Vector2D.h"
#include "../../Interfaces/src/FrameBufferInterface.h"
#include "../../RendererManager/src/RendererManager.h"
#include "../../Configuration/src/Configuration.h"
#include "../../Transformer/src/Transformer.h"
#include "../../Scene/src/Scene.h"
#include "MarkerInterface.h"
#include "MarkerUtils.h"

// *********
// constants
// *********

// squared distance of mouse pointer to marker
static const float THRESHOLD = 25.0f;

// *********
// functions
// *********

MarkerInterface *MarkerUtils::pickMarker (const QPoint mousePosition,
									      QList<MarkerInterface> *markerList,
										  const QWidget *widget) {

	MarkerInterface *marker;
	Vector2D        mousePos,
		            markerPos;

	mousePos[0] = mousePosition.x();
	mousePos[1] = mousePosition.y();
	for (marker = markerList->last(); marker != 0; marker = markerList->prev()) {

		markerPos[0] = marker->getPosition (widget).x();
		markerPos[1] = marker->getPosition (widget).y();

		if (MyMathTools::squaredEuklideanDistance2D (markerPos, mousePos) < THRESHOLD) {
			break;
		}

	}

	return marker;

}

SurfelInterface *MarkerUtils::getClosestSurfel (const QPoint mousePosition) {

	FrameBufferInterface                 *frameBuffer;
	RendererInterface                    *renderer;
				 
	Vector2D                             splattedCoordinate;
	Configuration                        *configuration;
	Transformer                          *transformer;
	MyDataTypes::TransformationMatrix16f sceneViewMatrix;
	SurfelInterface                      *surfel;

	renderer = RendererManager::getInstance()->getMainRenderer();
	frameBuffer = renderer->getFrameBuffer();

	configuration = Configuration::getInstance();
	Scene::getInstance()->getTransformationMatrix (sceneViewMatrix);
	transformer = Transformer::getSceneTransformer();

	// NOTE: the per pixel object pointer in the frame buffer is needed to determine whether a surfel belongs to the 
	// active object (markers are only allowed on the active object)
	renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_SURFEL | FrameBufferInterface::PERPIXEL_OBJECTPOINTER);

	if(mousePosition.x() >= 0 && mousePosition.x() < configuration->getViewPortSize().width() &&
		mousePosition.y() >= 0 && mousePosition.y() < configuration->getViewPortSize().height() ) {

		if(frameBuffer->getObjectAtPixel(mousePosition.x(), mousePosition.y()) == Scene::getInstance()->getActiveObject()) {
			surfel = frameBuffer->getSurfel (mousePosition.x(), mousePosition.y());
		}
		else {
			surfel = 0;
		}

	}
	else {
		surfel = 0;
	}

	return surfel;

}

void MarkerUtils::selectAll (QList<MarkerInterface> *markerList, QWidget *widget) {

	MarkerInterface *marker;

	for (marker = markerList->first(); marker != 0; marker = markerList->next()) {

		if (marker->isSelected() == false) {

			// erase old state
			marker->togglePainting (widget);
			marker->setSelected (true);
			marker->togglePainting (widget);

		}

	}

}

void MarkerUtils::deselectAll (QList<MarkerInterface> *markerList, QWidget *widget) {

	MarkerInterface *marker;

	for (marker = markerList->first(); marker != 0; marker = markerList->next()) {

		if (marker->isSelected() == true) {

			// erase old state
			marker->togglePainting (widget);
			marker->setSelected (false);
			marker->togglePainting (widget);

		}
		
	}

}

void MarkerUtils::reallocateIDs (QList<MarkerInterface> *markerList, QWidget *widget) {

	MarkerInterface *marker;
	uint            id;

	id = 1;
	for (marker = markerList->first(); marker != 0; marker = markerList->next()) {

		// erase old state
		marker->togglePainting (widget);
		marker->setID (id);
		id++;
		marker->togglePainting (widget);
		
	}

}

void MarkerUtils::toggleSelection (MarkerInterface *marker, QWidget *widget) {

	if (marker->isSelected() == true) {

		// erase old state
		marker->togglePainting (widget);
		marker->setSelected (false);
		marker->togglePainting (widget);

	}
	else {

		// erase old state
		marker->togglePainting (widget);
		marker->setSelected (true);
		marker->togglePainting (widget);

	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
