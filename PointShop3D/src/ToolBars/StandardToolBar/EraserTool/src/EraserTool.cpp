// Title:   EraserTool.cpp
// Created: Thu Sep 25 14:55:29 2003
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

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qwhatsthis.h>
#include <qwidget.h>
#include <qstring.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qapplication.h>

#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Core/Transformer/src/Transformer.h"
#include "../../../../Core/Configuration/src/Configuration.h"
#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../../../../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "../UI/EraserConfigurationWidget.h" 

#include "../../../../Utilities/src/Common.h"
#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../../Utilities/src/MyMathTools.h"
#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Core/CoreTools/src/CoreTools.h"

#include "EraserTool.h"


// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"eraser\"> "
"Click this button to erase surfels"
"<ul><li>Control + middle mouse button: change eraser height</li>"
"<li>Shift + middle mouse button: change eraser width</li>"
"<li>Alt + middle mouse button: change scale </li>";

EraserTool::EraserTool() {

	standardCursor = new QCursor (CrossCursor);
	toolCursor     = standardCursor;
	
	configurationWidget = new EraserConfigurationWidget(0);

}


EraserTool::~EraserTool() {

	delete standardCursor;
	delete configurationWidget;

}

// **************
// public methods
// **************

QToolButton *EraserTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	QPixmap  normalIcon ("Resources/EraserToolIcon.png"),
		     disabledIcon ("Resources/EraserToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);
	eraserButton = new QToolButton (iconSet, "Eraser Tool", "Erase Surfels",
	                               0, 0, toolBar, "Eraser Tool");
	eraserButton->setToggleButton (true);
	eraserButton->setEnabled (this->checkToolAvailability());
	QWhatsThis::add (eraserButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("eraser", normalIcon);

	return eraserButton;

}

QCursor *EraserTool::getToolCursor() const {
	return toolCursor;
}


QToolButton* EraserTool::getToolButton () {

	return eraserButton;
}



QWidget *EraserTool::getToolConfigurationWidget() const {
	return configurationWidget;
}

bool EraserTool::isAvailable() const {
	return this->checkToolAvailability();
}

void EraserTool::handleMouseMoveEvent (QMouseEvent *me) {


	float mouseMoveDistance = me->y() - lastMouseDragPosition.y();

	if (me->state() == Qt::MidButton) {		
		orthoMapper->adjustCursorShape (OrthoMapper::GLOBAL_SIZE, (int)mouseMoveDistance);
	}
	else if (me->state() == (Qt::MidButton | Qt::ShiftButton)) {
		orthoMapper->adjustCursorShape (OrthoMapper::X_SIZE, (int)mouseMoveDistance);
	}	
	else if (me->state() == (Qt::MidButton | Qt::ControlButton)) {
		orthoMapper->adjustCursorShape (OrthoMapper::Y_SIZE, (int)mouseMoveDistance);
	}	
	else if (me->state() == (Qt::MidButton | Qt::AltButton)) {
		orthoMapper->adjustCursorShape (OrthoMapper::Z_SIZE, (int)mouseMoveDistance);
	}
	else if (me->state() == Qt::RightButton) {
		orthoMapper->adjustCursorShape (OrthoMapper::ORIENTATION, (int)mouseMoveDistance);
	}
	else {	
		screenPosition[0] = me->x();
		screenPosition[1] = me->y();
	}
	
	currentPositionValid = orthoMapper->drawCursor (screenPosition, 0);

	if (me->state() == Qt::LeftButton) {
		removeSurfels();
	}
}

void EraserTool::handleMouseReleaseEvent (QMouseEvent *) {}

void EraserTool::handleMousePressEvent (QMouseEvent *me) {
	
	if (currentPositionValid) {

		if (me->button() == Qt::LeftButton && me->state() == Qt::NoButton) {			

			removeSurfels();
		}
		else if ((me->button() == Qt::MidButton) || (me->button() == Qt::RightButton)){
			
			orthoMapper->initAdjustCursorShape();
			lastMouseDragPosition = me->pos();			
		}
	}

}

void EraserTool::handleKeyPressEvent (QKeyEvent *ke) {
	ke->ignore();
}

void EraserTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	ke->ignore();
}


void EraserTool::handleRendererWidgetRepainted () {

	orthoMapper->renderCursor();
}



void EraserTool::selectTool() {

	RendererInterface         *renderer;

	QApplication::setOverrideCursor (Qt::waitCursor);

	renderer = RendererManager::getInstance()->getMainRenderer();
	renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::ALL_SPLATS);	

	orthoMapper = new OrthoMapper (QSize(100, 100), OrthoMapper::PLAIN);

	this->connect (Scene::getInstance(), SIGNAL (sceneMoved (const float*)),
		           this, SLOT (handleSceneMoved ()));

	this->connect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)),
		           this, SLOT (handleViewPortSizeChanged ()));

	QApplication::restoreOverrideCursor ();

	// turn mouse tracking on
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (true);
}


void EraserTool::deselectTool() {

	// turn mouse tracking off
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (false);
	
	this->disconnect (Scene::getInstance(), SIGNAL (sceneMoved (const float *)), this, 0);
	this->disconnect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)), this, 0);


	delete orthoMapper;
}


// ************
// public slots
// ************


// ***************
// private methods
// ***************

void EraserTool::removeSurfels () {
	
	if (currentPositionValid) {
		
		Object* object = Scene::getInstance()->getActiveObject();
		if (object) {
			
			std::vector<SurfelInterface*>* removeSurfels = NULL;
			int numOfSurfels;
			
			orthoMapper->computeAllVisibleSurfels (&removeSurfels, numOfSurfels);
			
			object->getSurfelCollection()->removeSurfels (removeSurfels, false);
			Scene::getInstance()->emitObjectModifiedSignal ();
			Scene::getInstance()->emitRenderFrameSignal();
		}
	}	
}

bool EraserTool::checkToolAvailability() const {
	return RendererManager::getInstance()->getMainRenderer()->isInteractive() &&
		   (Scene::getInstance()->getNofObjects() > 0);
}

// *************
// private slots
// *************

void EraserTool::handleSceneMoved () {

	orthoMapper->reset();
	currentPositionValid = orthoMapper->drawCursor (screenPosition, 0);
}


void EraserTool::handleViewPortSizeChanged () {
	orthoMapper->reset();
}


// ****************
// access functions
// ****************

ToolInterface *createTool() {
	return new EraserTool();
}

void destroyTool (ToolInterface *tool) {
	delete tool;
}

const char *getToolName() {
	return "EraserTool";
}









	

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
