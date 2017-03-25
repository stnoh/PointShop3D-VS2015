// Title:   LightTool.cpp
// Created: Thu Sep 25 14:55:53 2003
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

#include "../../../../Utilities/src/TrackBall.h"
#include "../../../../Utilities/src/Common.h"
#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Core/Scene/src/Object.h"
#include "../../../../Core/Configuration/src/Configuration.h"
#include "../../../../Core/RendererManager/src/RendererManager.h"
#include "../UI/LightConfigurationWidget.h"
#include "LightTool.h"

// ******************
// 'What's this' text
// ******************

const char *lightText = "<img source=\"light\"> "
"Click this button to control the scene lighting using the mouse:"
"<ul><li>Press left mouse button to set light direction</ul>";

LightTool::LightTool() {
	toolCursor         = new QCursor (QPixmap ("Resources/LightToolCursor.png"));
	usePreviewRenderer = true;

	configurationWidget = new LightConfigurationWidget;
	configurationWidget->previewRendererCheckBox->setChecked (usePreviewRenderer);
}

LightTool::~LightTool() {
	delete toolCursor;
	delete configurationWidget;
}

QToolButton *LightTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	QPixmap  normalIcon ("Resources/LightToolIcon.png"),
		     disabledIcon ("Resources/LightToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);
	toolButton = new QToolButton (iconSet, "Light Tool", "Control the lighting of the scene",
	                        0, 0, toolBar, "Light Tool");
	toolButton->setToggleButton (true);
	toolButton->setEnabled (this->checkToolAvailability());
	QWhatsThis::add (toolButton, lightText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("light", normalIcon);

	this->connect (configurationWidget->previewRendererCheckBox, SIGNAL (toggled (bool)),
		           this, SLOT (handlePreviewRendererToggled (bool)));

	return toolButton;
}

QToolButton *LightTool::getToolButton() {
	return toolButton;
}

QCursor *LightTool::getToolCursor() const {
	return toolCursor;
}

QWidget *LightTool::getToolConfigurationWidget() const {
	return configurationWidget;
}

bool LightTool::isAvailable() const {
	return this->checkToolAvailability();
}

void LightTool::handleMousePressEvent (QMouseEvent *me) {

	if (Configuration::getInstance()->isPreviewRendererEnabled() == false && 
		configurationWidget->previewRendererCheckBox->isChecked()) {
		// second parameter means: do not trigger render frame event
		RendererManager::getInstance()->setPreviewRendererEnabled (true, false);
	}

	// left mouse button
	if (me->button() == QMouseEvent::LeftButton) {
		// this method also triggers a render frame signal 
		// (via Configuration::getInstance()->setLightPosition(direction);)
		setLightDirection(QPoint(me->x(), me->y()));
	}

}

void LightTool::handleMouseMoveEvent (QMouseEvent *me) {

	// left mouse button
	if (me->state() == QMouseEvent::LeftButton) {

		// this method also triggers a render frame signal 
		// (via Configuration::getInstance()->setLightPosition(direction);)
		setLightDirection(QPoint(me->x(), me->y()));
	}
}

void LightTool::handleMouseReleaseEvent (QMouseEvent *) {
	RendererManager::getInstance()->setPreviewRendererEnabled (Configuration::getInstance()->isPreviewRendererEnabled(), true);
}

void LightTool::handleKeyPressEvent (QKeyEvent *ke) {
	ke->ignore();		
}

void LightTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	ke->ignore();		
}

void LightTool::selectTool() {}

void LightTool::deselectTool() {}

// ************
// public slots
// ************

void LightTool::handleRendererWidgetRepainted() {}

// ***************
// private methods
// ***************

void LightTool::setLightDirection(const QPoint &p) {
	
	Vector3D direction;
	QSize viewPortSize;
	Scene *scene;

	viewPortSize = Configuration::getInstance()->getViewPortSize();

	scene = Scene::getInstance();

	// normalize mouse positions into rectangle, [-1.0, 1.0] x [-1.0, 1.0]
	// NOTE: flip x and y directions, since the light vector has the same orientation
	// as the light. however, we also have to flip the y coordinate from window coords. 
	// to camera coords, therefore -y*-y = +y!
	direction[0] = -((float)p.x() / (float)viewPortSize.width()  * 2.0f - 1.0f);
	direction[1] = ((float)p.y() / (float)viewPortSize.height() * 2.0f - 1.0f);
	
	// compute light direciton
	float l = (float)sqrt(direction[0]*direction[0] + direction[1]*direction[1]);
	if (l > 1) {
		direction[0] /= l;
		direction[1] /= l;
	}

	l = 1 - direction[0]*direction[0] - direction[1]*direction[1];
	if (l > 0) {
		direction[2] = -(float)sqrt(l);
	} 
	else {
		direction[2] = 0;
	}

	direction.normalize();

	Configuration::getInstance()->setLightDirection(direction);

}

bool LightTool::checkToolAvailability() const {
	return true;
}

// *************
// private slots
// *************

void LightTool::handlePreviewRendererToggled (bool isOn) {
	usePreviewRenderer = isOn;
}

// ****************
// access functions
// ****************

ToolInterface *createTool() {
	return new LightTool();
}

const char *getToolName() {
	return "LightTool";
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
