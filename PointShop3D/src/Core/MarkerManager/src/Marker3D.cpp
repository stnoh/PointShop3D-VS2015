// Title:   Marker3D.cpp
// Created: Thu Sep 25 14:16:02 2003
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
#include <qpoint.h>
#include <qpainter.h>
#include <qfont.h>
#include <qpen.h>
#include <qcolor.h>

#include "../../../Utilities/src/MyMathTools.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../Transformer/src/Transformer.h"
#include "../../Scene/src/Scene.h"
#include "../../RendererManager/src/RendererManager.h"
#include "../../Configuration/src/Configuration.h"
#include "MarkerUtils.h"
#include "Marker3D.h"

static const float THRESHOLD = 8.0f;

Marker3D::Marker3D (SurfelInterface *surfel, const bool selected, const uint id, const Style style) {

	this->surfel   = surfel;
	this->selected = selected;
	this->id       = id;
	this->style    = style;
	
	dragged          = false;
	eraseOldPosition = true;	

}

Marker3D::~Marker3D() {}

SurfelInterface *Marker3D::getSurfel() const {
	return surfel;
}

void Marker3D::setSurfel(SurfelInterface* surfel) {
	this->surfel = surfel;
}

void Marker3D::setDragged (const bool dragged) {
	this->dragged = dragged;
}

bool Marker3D::isDragged() const {
	return dragged;
}

bool Marker3D::isClicked (const QPoint mousePosition, const QWidget *) {

	Vector3D mousePos,
		     markerPos;

	// the position is updated each time the visualization of 
	// this marker is toggled - and since a marker must be visible
	// as to be selected, the marker position is up to date
	markerPos[0] = position.x();
	markerPos[1] = position.y();
	mousePos[0]  = mousePosition.x();
	mousePos[1]  = mousePosition.y();

	return (MyMathTools::squaredEuklideanDistance3D (markerPos, mousePos) < THRESHOLD);

}

void Marker3D::setID (const uint newID) {
	id = newID;
}

uint Marker3D::getID() const {
	return id;
}

QPoint Marker3D::getPosition (const QWidget *) const {
	return position;
}

void Marker3D::setSelected (const bool selected) {
	this->selected = selected;
}

bool Marker3D::isSelected() const {
	return selected;
}

void Marker3D::setStyle (const Style newStyle) {
	style = newStyle;
}

MarkerInterface::Style Marker3D::getStyle() const {
	return style;
}

void Marker3D::togglePainting (QWidget *widget) {

	if (dragged == false) {
		this->paint (widget);
	}	

}

bool Marker3D::moveTo (const QPoint newPosition, QWidget *widget) {

	// erase the old position
	if (eraseOldPosition == true) {
		this->paint (widget);
		eraseOldPosition = false;
	}

	// move to new position
	position = newPosition;

	surfel = MarkerUtils::getClosestSurfel (position);

	if (surfel != 0) {
		// redraw at new position
		this->paint (widget);
		eraseOldPosition = true;
		return true;
	}
	else {
		return false;
	}
	
}

// ***************
// private methods
// ***************

void Marker3D::updatePosition() {

	Vector2D    viewPortPoint;
	Vector3D    surfelPosition,
		        transformedSurfelPosition;
	Transformer *transformer;
	MyDataTypes::TransformationMatrix16f objectTrafo;

	surfelPosition = surfel->getPosition();
	
	transformer = Transformer::getSceneTransformer();

	Scene *scene = Scene::getInstance();
	Object *object = scene->getActiveObject();
	object->getTransformationMatrix(objectTrafo);

	transformer->setObjectTransformationMatrix(objectTrafo);

	viewPortPoint = transformer->objectToViewPort (surfelPosition);
	position.setX ((int)(viewPortPoint[0] + 0.5f));
	position.setY ((int)(viewPortPoint[1] + 0.5f));
	transformedSurfelPosition = transformer->objectToCamera (surfelPosition);
	markerZ = transformedSurfelPosition[2];
	
}

void Marker3D::paint (QWidget *widget) {

	QPainter             painter;
	QFont                font;
	FrameBufferInterface *frameBuffer;
	float                bufferZ;
	QSize                frameBufferSize;

	// update the position
	this->updatePosition();

	// check if we need to paint this marker
	frameBuffer = RendererManager::getInstance()->getMainRenderer()->getFrameBuffer();

	// non-interactive renderers don't have a frame buffer
	if (frameBuffer == 0) {
		return;
	}

	// only draw the marker if it is inside the frame buffer
	frameBufferSize = frameBuffer->getSize();
	if(position.x() >= 0 && position.x() < frameBufferSize.width() && 
		position.y() >= 0 && position.y() < frameBufferSize.height()) {

		bufferZ = frameBuffer->getDepth (position.x(), position.y());
	
		if (markerZ + 5.0f > bufferZ) {

			painter.begin (widget);
			painter.setPen (QPen (Qt::white));
			// XOR mode
			painter.setRasterOp (Qt::XorROP);

			switch (style) {

			case BOUNDARY_CONDITION_STYLE:
				{

					font = painter.font();
					font.setPointSize (12);
					font.setBold (true);
					

					if (selected == true) {
						painter.drawRect (position.x() - 5, position.y() - 5, 11, 11);
						font.setUnderline (true);
					}

					// draw a rectangle, 5 pixel border
					painter.setPen (QPen (QColor (255, 255, 255), 5));
					painter.drawRect (position.x() - 5, position.y() - 5, 11, 11);	

					painter.setFont (font);
					// draw the id
					painter.drawText (position.x() + 7, position.y() - 7, QString::number (id));

				}
				break;

			case FITTING_CONSTRAINT_STYLE:
				{	

					font = painter.font();
					font.setPointSize (12);
					font.setBold (true);
					

					if (selected == true) {
						painter.drawEllipse (position.x() - 5, position.y() - 5, 11, 11);
						font.setUnderline (true);
					}

					// draw a circle, 5 pixel border
					painter.setPen (QPen (QColor (255, 255, 255), 5));
					painter.drawEllipse (position.x() - 5, position.y() - 5, 11, 11);	

					painter.setFont (font);
					// draw the id
					painter.drawText (position.x() + 7, position.y() - 7, QString::number (id));

				}
				break;

			default:
				break;

			}

			painter.end();	

		}  // z-Buffer test
	}
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
