// Title:   ToolInterface.cpp
// Created: Thu Sep 25 14:16:28 2003
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

#include "ToolInterface.h"

void ToolInterface::handleKeyPressEvent (QKeyEvent *ke) {
	if (ke->key() == Qt::Key_Q) {
		setGlobalTransformationActive(!globalTransform);
	}
}

void ToolInterface::setGlobalTransformationActive(bool isOn) {
	this->globalTransform = isOn;
}

void ToolInterface::handleMouseMoveEvent (QMouseEvent *me) {

	float                                normalizedMousePositions[4];
	float                                rotationVector[4],
		                                 sx,
		                                 sy,
		                                 sz;
	QSize                                viewPortSize;
	Scene                                *scene;
	Vector3D                             transformation,
		                                 finalTransformation;


	if (isGlobalTransformationActive()) {
		viewPortSize = Configuration::getInstance()->getViewPortSize();
		scene = Scene::getInstance();

		// left mouse button
		if (me->state() == QMouseEvent::LeftButton) {
			
			// rotate
			
			// normalize mouse positions into rectangle, [-1.0, 1.0] x [-1.0, 1.0]
			normalizedMousePositions[0] = (float)lastMouseDragPosition.x() / (float)viewPortSize.width()  * 2.0f - 1.0f;
			normalizedMousePositions[1] = (float)lastMouseDragPosition.y() / (float)viewPortSize.height() * 2.0f - 1.0f;
			normalizedMousePositions[2] = (float)me->x() / (float)viewPortSize.width()  * 2.0f - 1.0f;
			normalizedMousePositions[3] = (float)me->y() / (float)viewPortSize.height() * 2.0f - 1.0f;
			
			if (TrackBall::calculateRotationMatrix (normalizedMousePositions, rotationVector) == true) {
				scene->rotate (rotationVector[0] * PI_180, rotationVector[1], rotationVector[2], rotationVector[3]);
			}
			
		}
		
		// middle mouse button
		else if (me->state() == QMouseEvent::MidButton) {
			// translate in z-direction
			scene->translate (0.0f, 0.0f, -2.0f * (me->y() - lastMouseDragPosition.y()));
		}
		
		// right mouse button
		else if (me->state() == QMouseEvent::RightButton) {
			// translate in x/y direction
			scene->translate (me->x() - lastMouseDragPosition.x(), lastMouseDragPosition.y() - me->y(), 0.0f);	
		}
		
		// right button AND Control
		else if ((me->state() == (QMouseEvent::RightButton | QMouseEvent::ControlButton))) {
			
			// uniform scaling in x, y and z-direction
			
			sy = 1.0f + 0.025f * (lastMouseDragPosition.y() - me->y());
			sy = (sy > 0.1f) ? sy : 0.1f;
			sx = sy;
			sz = sy;
			scene->scale (sx, sy, sz);
			
		}
		
		// right button AND Shift
		else if ((me->state() == (QMouseEvent::RightButton | QMouseEvent::ShiftButton))) {
			// translate in z-direction
			scene->translate (0.0f, 0.0f, -2.0f * (me->y() - lastMouseDragPosition.y()));
		}
	}
	

	lastMouseDragPosition = me->pos();	

}

void ToolInterface::handleMousePressEvent(QMouseEvent *event) {

	lastMouseDragPosition = event->pos();
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
