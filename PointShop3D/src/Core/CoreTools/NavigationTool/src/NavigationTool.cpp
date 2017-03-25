// Title:   NavigationTool.cpp
// Created: Thu Sep 25 14:25:53 2003
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

#include <math.h>

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qwhatsthis.h>
#include <qcheckbox.h>
#include <qpopupmenu.h>

#include "../../../../Utilities/src/TrackBall.h"
#include "../../../../Utilities/src/Common.h"
#include "../../../../Utilities/src/Matrix.h"
#include "../../../Scene/src/Scene.h"
#include "../../../Scene/src/Object.h"
#include "../../../Configuration/src/Configuration.h"
#include "../../../RendererManager/src/RendererManager.h"
#include "../UI/NavigationConfigurationWidget.h"
#include "NavigationTool.h"

// ******************
// 'What's this' text
// ******************

const char *navigationText = "<img source=\"navigation\"> "
"Click this button to rotate, scale and translate the scene or individual objects:"
"<ul><li>left and hold mouse button over button: choose tool</li>"
"<li>left mouse button: rotate the scene/object</li>"
"<li>middle mouse button: translate the scene/object in z-direction</li>"
"<li>right mouse button: translate the scene/object in x/y-direction</li>"
"<li>right mouse button + SHIFT: translate the scene/object in z-direction</li>"
"<li>right mouse button + CONTROL: scale the scene/object</li></ul>";

NavigationTool::NavigationTool() {

	QPixmap normalNavigationIcon ("Resources/NavigationIcon.png"),
		    disabledNavigationIcon ("Resources/NavigationIcon_disabled.png"),
			normalBuilderIcon ("Resources/BuilderIcon.png"),
			disabledBuilderIcon ("Resources/BuilderIcon_disabled.png");

	toolCursor         = new QCursor (SizeAllCursor);
	usePreviewRenderer = true;	

	navigationIconSet.setPixmap (normalNavigationIcon, QIconSet::Automatic, QIconSet::Normal);
	navigationIconSet.setPixmap (disabledNavigationIcon, QIconSet::Automatic, QIconSet::Disabled);

	builderIconSet.setPixmap (normalBuilderIcon, QIconSet::Automatic, QIconSet::Normal);
	builderIconSet.setPixmap (disabledBuilderIcon, QIconSet::Automatic, QIconSet::Disabled);

}

NavigationTool::~NavigationTool() {
	delete toolCursor;
}

QToolButton *NavigationTool::addButtonToToolBar (QToolBar *toolBar) {

	toolButton = new QToolButton (navigationIconSet, "Navigation Tool (<b>N</b>)", "Rotate, translate and zoom",
	                              0, 0, toolBar, "Navigation Tool");
	toolButton->setToggleButton (true);
	QWhatsThis::add (toolButton, navigationText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("navigation", navigationIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal));

	navigationID = navigationMenu.insertItem (navigationIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	builderID    = navigationMenu.insertItem (builderIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	
	currentTool = navigationID;

	toolButton->setPopup (&navigationMenu);
	toolButton->setEnabled (this->checkToolAvailability());

	configurationWidget = new NavigationConfigurationWidget;
	configurationWidget->previewRendererCheckBox->setChecked (usePreviewRenderer);
	configurationWidget->objectCoordinateSystemCheckBox->setEnabled (false);
	this->connect (configurationWidget->previewRendererCheckBox, SIGNAL (toggled (bool)),
		           this, SLOT (handlePreviewRendererToggled (bool)));

	return toolButton;

}

QToolButton *NavigationTool::getToolButton() {
	return toolButton;
}

QCursor *NavigationTool::getToolCursor() const {
	return toolCursor;
}

QWidget *NavigationTool::getToolConfigurationWidget() const {
	return configurationWidget;
}

bool NavigationTool::isAvailable() const {
	return this->checkToolAvailability();
}

void NavigationTool::handleMousePressEvent (QMouseEvent *me) {

	ToolInterface::handleMousePressEvent(me);

	lastMouseDragPosition = me->pos();
	// enable preview renderer while rotating etc.
	RendererManager::getInstance()->setPreviewRendererEnabled (usePreviewRenderer, true);

}

void NavigationTool::handleMouseMoveEvent (QMouseEvent *me) {

	float                                normalizedMousePositions[4];
	float                                rotationVector[4],
		                                 sx,
		                                 sy,
		                                 sz;
	QSize                                viewPortSize;
	Scene                                *scene;
	Vector3D                             transformation,
		                                 finalTransformation;
	MyDataTypes::TransformationMatrix16f sceneRotationMatrix,
		                                 inverseSceneRotationMatrix;

	viewPortSize = Configuration::getInstance()->getViewPortSize();
	scene = Scene::getInstance();

	setGlobalTransformationActive(currentTool == navigationID);
	
	ToolInterface::handleMouseMoveEvent(me);

	// ************
	// builder tool
	// ************

	if (currentTool == builderID) {
		// left mouse button
		if (me->state() == QMouseEvent::LeftButton) {

			// rotate

			// normalize mouse positions into rectangle, [-1.0, 1.0] x [-1.0, 1.0]
			normalizedMousePositions[0] = (float)lastMouseDragPosition.x() / (float)viewPortSize.width()  * 2.0f - 1.0f;
			normalizedMousePositions[1] = (float)lastMouseDragPosition.y() / (float)viewPortSize.height() * 2.0f - 1.0f;
			normalizedMousePositions[2] = (float)me->x() / (float)viewPortSize.width()  * 2.0f - 1.0f;
			normalizedMousePositions[3] = (float)me->y() / (float)viewPortSize.height() * 2.0f - 1.0f;

			if (TrackBall::calculateRotationMatrix (normalizedMousePositions, rotationVector) == true) {

				if (configurationWidget->objectCoordinateSystemCheckBox->isChecked() == false) {

					transformation[0] = rotationVector[1];
					transformation[1] = rotationVector[2];
					transformation[2] = rotationVector[3];

					scene->getRotationMatrix (sceneRotationMatrix);
					MtrInverse4x4f (sceneRotationMatrix, inverseSceneRotationMatrix);
					MtrMultVector3D (inverseSceneRotationMatrix, transformation, &finalTransformation);
					scene->getActiveObject()->rotate (rotationVector[0] * PI_180, finalTransformation[0], finalTransformation[1], finalTransformation[2]);
				
				}
				else {
					scene->getActiveObject()->rotate (rotationVector[0] * PI_180, rotationVector[1], rotationVector[2], rotationVector[3]);
				}
				
			}
			
		}

		// middle mouse button
		else if ((me->state() == QMouseEvent::MidButton)) {

			// translate in z direction
			
			transformation[0] = 0.0f;
			transformation[1] = 0.0f;
			transformation[2] = -2.0f * (me->y() - lastMouseDragPosition.y());
 			
			if (configurationWidget->objectCoordinateSystemCheckBox->isChecked() == false) {

				scene->getRotationMatrix (sceneRotationMatrix);
				MtrInverse4x4f (sceneRotationMatrix, inverseSceneRotationMatrix);
				MtrMultVector3D (inverseSceneRotationMatrix, transformation, &finalTransformation);
				scene->getActiveObject()->translate (finalTransformation);
				
			}
			else {
				scene->getActiveObject()->translate (transformation);
			}

		}

		// right mouse button
		else if (me->state() == QMouseEvent::RightButton) {

			// translate in x/y direction

			transformation[0] = me->x() - lastMouseDragPosition.x();
			transformation[1] = lastMouseDragPosition.y() - me->y();
			transformation[2] = 0.0f;

			if (configurationWidget->objectCoordinateSystemCheckBox->isChecked() == false) {

				scene->getRotationMatrix (sceneRotationMatrix);
				MtrInverse4x4f (sceneRotationMatrix, inverseSceneRotationMatrix);
				MtrMultVector3D (inverseSceneRotationMatrix, transformation, &finalTransformation);
				scene->getActiveObject()->translate (finalTransformation);
			
			}
			else {
				scene->getActiveObject()->translate (transformation);
			}
			
		}

		// right mouse button + CONTROL
		else if (me->state() == (QMouseEvent::RightButton | QMouseEvent::ControlButton)) {

			// uniform scaling in x, y and z-direction

			sy = 1.0f + 0.025f * (lastMouseDragPosition.y() - me->y());
			sy = (sy > 0.1f) ? sy : 0.1f;
			sx = sy;
			sz = sy;
			scene->getActiveObject()->scale (sx, sy, sz);

		}

		// right mouse button + SHIFT
		else if (me->state() == (QMouseEvent::RightButton | QMouseEvent::ShiftButton)) {
			
			// translate in z direction
			
			transformation[0] = 0.0f;
			transformation[1] = 0.0f;
			transformation[2] = -2.0f * (me->y() - lastMouseDragPosition.y());
 			
			if (configurationWidget->objectCoordinateSystemCheckBox->isChecked() == false) {

				scene->getRotationMatrix (sceneRotationMatrix);
				MtrInverse4x4f (sceneRotationMatrix, inverseSceneRotationMatrix);
				MtrMultVector3D (inverseSceneRotationMatrix, transformation, &finalTransformation);
				scene->getActiveObject()->translate (finalTransformation);
				
			}
			else {
				scene->getActiveObject()->translate (transformation);
			}

		}		

	}

	lastMouseDragPosition = me->pos();	

}

void NavigationTool::handleMouseReleaseEvent (QMouseEvent *) {
	RendererManager::getInstance()->setPreviewRendererEnabled (Configuration::getInstance()->isPreviewRendererEnabled(), true);
}

void NavigationTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	ke->ignore();		
}

void NavigationTool::selectTool() {}

void NavigationTool::deselectTool() {}

// ************
// public slots
// ************

void NavigationTool::handleRendererWidgetRepainted() {}

void NavigationTool::setGlobalTransformationActive(bool isOn) {
	if (!isOn) {
		handleToolSelected(builderID);
	}
	else {
		handleToolSelected(navigationID);
	}
}

// ***************
// private methods
// ***************

bool NavigationTool::checkToolAvailability() const {
	return (Scene::getInstance()->getNofObjects() > 0);
}

// *************
// private slots
// *************

void NavigationTool::handleToolSelected (int id) {

	currentTool = id;

	if (id == navigationID) {
		ToolInterface::setGlobalTransformationActive(true);
		toolButton->setIconSet (navigationIconSet);
		configurationWidget->objectCoordinateSystemCheckBox->setEnabled (false);
	}    
	else if (id == builderID) {
		ToolInterface::setGlobalTransformationActive(false);
		toolButton->setIconSet (builderIconSet);
		configurationWidget->objectCoordinateSystemCheckBox->setEnabled (true);
	}

}

void NavigationTool::handlePreviewRendererToggled (bool isOn) {
	usePreviewRenderer = isOn;
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
