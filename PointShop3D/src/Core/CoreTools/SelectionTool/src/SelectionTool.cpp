// Title:   SelectionTool.cpp
// Created: Thu Sep 25 14:21:56 2003
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

#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qcursor.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qwhatsthis.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qrect.h>
#include <qcolor.h>
#include <qpoint.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

#include <vector>

#include "../../../DataStructures/src/NeighbourHood.h"
#include "../../../Interfaces/src/ToolInterface.h"
#include "../../../Interfaces/src/RendererInterface.h"
#include "../../../Interfaces/src/FrameBufferInterface.h"
#include "../../../Interfaces/src/SurfelInterface.h"
#include "../../../CoreResourceTools/src/CoreResourceTools.h"
#include "../../../Scene/src/Scene.h"
#include "../../../Transformer/src/Transformer.h"
#include "../../../Configuration/src/Configuration.h"
#include "../../../RendererManager/src/RendererManager.h"
#include "../../../Painter/src/Painter.h"
#include "../UI/SelectionConfigurationWidget.h"
#include "ScanConversion.h"
#include "SelectionTool.h"

// *********
// constants
// *********

// each time the currentLassoLine vector is full, MORE_POINT_ELEMENTS elements are additionally allocated
// must be >= 4, since for the rectangle tool 4 points are added at once 
static const int MORE_POINT_ELEMENTS = 128;



// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"selection\"> "
"Click this button to activate the selection tool."
"Hold down the mouse button to choose a selection tool.";

int selectionFunction (const int y,
					   const int leftX,
					   const int rightX,
					   SurfelInterface::Flag selectionFlag,
					   bool select) {

	int                             x;
	const FrameBufferInterface      *frameBuffer;
	const std::vector<SurfelInterface *> *visibleSurfels;
	uint                            nofVisible,
		                            i;
	SurfelInterface                 *surfel;
	Scene                           *scene;
	int								numCoveredSurfels;

	numCoveredSurfels = 0;

	frameBuffer = RendererManager::getInstance()->getMainRenderer()->getFrameBuffer();
	scene       = Scene::getInstance();
	visibleSurfels = 0;
	for (x = leftX; x <= rightX; x++) {
		
		// mark the selected splats
		frameBuffer->getVisibleSurfels (x, y, &visibleSurfels, &nofVisible);
		for (i = 0; i < nofVisible; i++) {

			surfel = visibleSurfels->at(i);
			// if not selected yet...
			if (surfel->isFlagOn (selectionFlag) != select) {
				
				// ...mark this surfel as selected, but DON'T emit the signal here
				scene->setSurfelFlagOn (surfel, (SurfelInterface::Flag)(selectionFlag | SurfelInterface::EMPHASISE), select, false);
				if (select) {
					numCoveredSurfels++;
				}
				else {
					numCoveredSurfels--;
				}
			}

		}

	}
	return numCoveredSurfels;
}

// **********
// Tool class
// **********

SelectionTool::SelectionTool() {

	int i;
	toolCursor = new QCursor (CrossCursor);

	QPixmap  normalEllipseIcon ("Resources/EllipseIcon.png"),
		     disabledEllipseIcon ("Resources/EllipseIcon_disabled.png"),
			 normalLassoIcon ("Resources/LassoIcon.png"),
			 disabledLassoIcon ("Resources/LassoIcon_disabled.png"),
			 normalPolygonIcon ("Resources/PolygonIcon.png"),
			 disabledPolygonIcon ("Resources/PolygonIcon_disabled.png"),
			 normalRectangleIcon ("Resources/RectangleIcon.png"),
			 disabledRectangleIcon ("Resources/RectangleIcon_disabled.png"),
			 normalBrushIcon ("Resources/BrushIcon.png"),
			 disabledBrushIcon ("Resources/BrushIcon_disabled.png"),
			 normalFloodFillIcon ("Resources/FloodFillIcon.png"),
			 disabledFloodFillIcon ("Resources/FloodFillIcon_disabled.png");

	ellipseIconSet.setPixmap (normalEllipseIcon, QIconSet::Automatic, QIconSet::Normal);
	ellipseIconSet.setPixmap (disabledEllipseIcon, QIconSet::Automatic, QIconSet::Disabled);

	lassoIconSet.setPixmap (normalLassoIcon, QIconSet::Automatic, QIconSet::Normal);
	lassoIconSet.setPixmap (disabledLassoIcon, QIconSet::Automatic, QIconSet::Disabled);

	polygonIconSet.setPixmap (normalPolygonIcon, QIconSet::Automatic, QIconSet::Normal);
	polygonIconSet.setPixmap (disabledPolygonIcon, QIconSet::Automatic, QIconSet::Disabled);

	rectangleIconSet.setPixmap (normalRectangleIcon, QIconSet::Automatic, QIconSet::Normal);
	rectangleIconSet.setPixmap (disabledRectangleIcon, QIconSet::Automatic, QIconSet::Disabled);

	brushIconSet.setPixmap (normalBrushIcon, QIconSet::Automatic, QIconSet::Normal);
	brushIconSet.setPixmap (disabledBrushIcon, QIconSet::Automatic, QIconSet::Disabled);

	floodFillIconSet.setPixmap (normalFloodFillIcon, QIconSet::Automatic, QIconSet::Normal);
	floodFillIconSet.setPixmap (disabledFloodFillIcon, QIconSet::Automatic, QIconSet::Disabled);



	nofPolyPoints                 = 0;
	isPolygonLineBeingConstructed = false;

	selectionLists          = new std::vector<SurfelInterface*>[SurfelInterface::getNumOfSelections()];
	selectionsValid         = new bool [SurfelInterface::getNumOfSelections()];
	selectionsNumSurfels    = new int [SurfelInterface::getNumOfSelections()];

	for (i = 0; i < SurfelInterface::getNumOfSelections(); i++) {
		selectionsValid[i] = false;
		selectionsNumSurfels[i] = 0;
	}

	orthoMapper = 0;
	painter = 0;

	configurationWidget = new SelectionConfigurationWidget;

}

SelectionTool::~SelectionTool() {
	delete toolCursor;
}

void SelectionTool::selectAllVisible (const bool visualizeSelection) {

	int renderWidth,
		renderHeight;
	FrameBufferInterface *frameBuffer;
	RendererInterface    *renderer;

	renderer    = RendererManager::getInstance()->getMainRenderer();
	frameBuffer = renderer->getFrameBuffer();	

	renderWidth  = Configuration::getInstance()->getViewPortSize().width();
	renderHeight = Configuration::getInstance()->getViewPortSize().height();

	// clear the previous points and selection
	nofPolyPoints = 0;
	currentPolyLine.resize(4);
	this->clearSelection (false, false);

	// create and insert the new points, but check
	// first if there's enough memory available
	if (nofPolyPoints < currentPolyLine.size() - 3) {

		// at the 4 points of the rectangle
		currentPolyLine.setPoint (nofPolyPoints, QPoint (0, 0));
		nofPolyPoints++;
		currentPolyLine.setPoint (nofPolyPoints, QPoint (renderWidth, 0));
		nofPolyPoints++;
		currentPolyLine.setPoint (nofPolyPoints, QPoint (renderWidth, renderHeight));
		nofPolyPoints++;
		currentPolyLine.setPoint (nofPolyPoints, QPoint (0, renderHeight));
		nofPolyPoints++;

	}
	else {

		// allocate more memory first
		currentPolyLine.resize (nofPolyPoints + MORE_POINT_ELEMENTS);
		// at the 4 points of the rectangle
		currentPolyLine.setPoint (nofPolyPoints, QPoint (0, 0));
		nofPolyPoints++;
		currentPolyLine.setPoint (nofPolyPoints, QPoint (renderWidth, 0));
		nofPolyPoints++;
		currentPolyLine.setPoint (nofPolyPoints, QPoint (renderWidth, renderHeight));
		nofPolyPoints++;
		currentPolyLine.setPoint (nofPolyPoints, QPoint (0, renderHeight));
		nofPolyPoints++;

	}

	renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_SURFELLISTS);

	selectionsNumSurfels [getActiveSelectionIndex()] = ScanConversion::concavePolygon (nofPolyPoints,
		&currentPolyLine, QRect (QPoint (0, 0), Configuration::getInstance()->getViewPortSize()),								    
		selectionFunction, getCurrentSelectionFlag(), true);

	selectionsValid [getActiveSelectionIndex()] = false;
	
	// visualize the selection, if necessary
	if (visualizeSelection == true) {
		renderer->renderFrame();
	}

	emit selectionChanged (getCurrentSelectionFlag());

}

void SelectionTool::selectAllActiveObject (const bool visualizeSelection) {

	Object                *activeObject;
	SurfelCollection      *activeSurfelCollection;
	SurfelInterface       *surfel;
	Scene                 *scene;
	SurfelInterface::Flag  selectionFlag;

	scene = Scene::getInstance();
	activeObject = scene->getActiveObject();

	if (activeObject == 0) {
		return;
	}


	activeSurfelCollection = activeObject->getSurfelCollection();
	selectionFlag = getCurrentSelectionFlag();


	for (surfel = activeSurfelCollection->getFirstSurfel(); surfel != 0; surfel = activeSurfelCollection->getNextSurfel()) {
		// ...mark this surfel as selected, but DON'T emit the signal here
		scene->setSurfelFlagOn (surfel, (SurfelInterface::Flag)(selectionFlag | SurfelInterface::EMPHASISE), true, false);

	}
	selectionsNumSurfels [getActiveSelectionIndex()] = activeSurfelCollection->getNofSurfels();	
	selectionsValid [getActiveSelectionIndex()] = false;

	// visualize the selection, if necessary
	if (visualizeSelection == true) {
		RendererManager::getInstance()->getMainRenderer()->renderFrame();
	}

	emit selectionChanged (selectionFlag);

}

std::vector<SurfelInterface*> *SelectionTool::getSelection (SurfelInterface::Flag selectionFlag) {


	int selectionIndex = getSelectionIndex (selectionFlag);
	SurfelInterface *surfel;
	Object* activeObject;
	int index;
	

	activeObject = Scene::getInstance()->getActiveObject();
	
	if (activeObject) {
		if (selectionsValid [selectionIndex] == false) {
			
			qDebug ("getSelection : recompute ...");
			SurfelCollection *surfels = activeObject->getSurfelCollection();
			selectionLists [selectionIndex].resize (surfels->getNofSurfels());
			index = 0;
			for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {
				if (surfel->isFlagOn (selectionFlag)) {
					(selectionLists [selectionIndex])[index] = surfel;
					index++;

				}
			}
			selectionsValid [selectionIndex] = true;
			selectionsNumSurfels [selectionIndex] = index;
			selectionLists [selectionIndex].resize (index);

			qDebug ("getSelection : ... finished");

		}
	}
	else {
		return 0;
	}
	
	return &selectionLists [selectionIndex];
}


void SelectionTool::getAllNotSelected (std::vector<SurfelInterface*>& allNotSelected, SurfelInterface::Flag selectionFlag) {
	
	SurfelInterface *surfel;
	Object* activeObject;
	int index;
		
	activeObject = Scene::getInstance()->getActiveObject();
	
	if (activeObject) {
		
		SurfelCollection *surfels = activeObject->getSurfelCollection();
		allNotSelected.resize (surfels->getNofSurfels());
		
		index = 0;
		for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {
			if (!surfel->isFlagOn (selectionFlag)) {
				allNotSelected[index] = surfel;
				index++;
			}
		}
		
		allNotSelected.resize (index);
		
	}
	else {
		allNotSelected.clear();
	}
}







int SelectionTool::getSelectionSize (SurfelInterface::Flag selectionFlag) {

	return selectionsNumSurfels [getSelectionIndex (selectionFlag)];
}


void SelectionTool::clearSelection (const bool visualizeSelection, const bool emitSignal) {

	SurfelInterface       *surfel;
	Scene                 *scene;
	SurfelInterface::Flag selectionFlag;		
	SurfelCollection      *surfels;
	

	scene = Scene::getInstance();
	selectionFlag = getCurrentSelectionFlag();
	surfels = scene->getActiveObject()->getSurfelCollection();

	
	for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {
		scene->setSurfelFlagOn (surfel, (SurfelInterface::Flag)(selectionFlag | SurfelInterface::EMPHASISE), false, false);
		if (surfel->isSelected()) {
			scene->setSurfelFlagOn (surfel, SurfelInterface::EMPHASISE, true, false);
		}

	}	
	
	selectionsValid [getSelectionIndex (selectionFlag)] = false;
	selectionsNumSurfels [getSelectionIndex (selectionFlag)] = 0;	

	// visualize the selection, if necessary
	if (visualizeSelection == true) {
		RendererManager::getInstance()->getMainRenderer()->renderFrame();
	}
	
	if (emitSignal) {
		emit selectionChanged (selectionFlag);
	}	
}



void SelectionTool::invertSelection (const bool visualizeSelection) {

	SurfelInterface       *surfel;
	Scene                 *scene;
	SurfelCollection      *surfels;
	SurfelInterface::Flag selectionFlag;	

	scene = Scene::getInstance();
	surfels = scene->getActiveObject()->getSurfelCollection();
	selectionFlag = (SurfelInterface::Flag) (getCurrentSelectionFlag() | SurfelInterface::EMPHASISE);	
	

	for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {
		if (surfel->isFlagOn (selectionFlag)) {
			scene->setSurfelFlagOn (surfel, selectionFlag, false, false);
		}
		else {
			scene->setSurfelFlagOn (surfel, selectionFlag, true, false);
		}

		if (surfel->isSelected()) {
			scene->setSurfelFlagOn (surfel, SurfelInterface::EMPHASISE, true, false);
		}
	}		

	selectionsNumSurfels [getSelectionIndex (selectionFlag)] = 	surfels->getNofSurfels() - selectionsNumSurfels [getSelectionIndex (selectionFlag)];
	selectionsValid [getSelectionIndex (selectionFlag)] = false;

	// visualize the selection, if necessary
	if (visualizeSelection == true) {
		RendererManager::getInstance()->getMainRenderer()->renderFrame();
	}
	
	emit selectionChanged (getCurrentSelectionFlag());

}



void SelectionTool::shuffleSelection (const bool visualizeSelection) {

	SurfelInterface       *surfel;
	Scene                 *scene;
	SurfelCollection      *surfels;
	SurfelInterface::Flag selectionFlag;


	scene = Scene::getInstance();
	surfels = scene->getActiveObject()->getSurfelCollection();
	

	for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {

		selectionFlag = SurfelInterface::NO_FLAGS;

		if (surfel->isFlagOn (SurfelInterface::SELECTED1)) {
			selectionFlag = (SurfelInterface::Flag) (selectionFlag | SurfelInterface::SELECTED2);
		}

		if (surfel->isFlagOn (SurfelInterface::SELECTED2)) {
			selectionFlag = (SurfelInterface::Flag) (selectionFlag | SurfelInterface::SELECTED3);
		}

		if (surfel->isFlagOn (SurfelInterface::SELECTED3)) {
			selectionFlag = (SurfelInterface::Flag) (selectionFlag | SurfelInterface::SELECTED1);
		}

		scene->setSurfelFlagOn (surfel, SurfelInterface::SELECTED1, selectionFlag & SurfelInterface::SELECTED1, false);
		scene->setSurfelFlagOn (surfel, SurfelInterface::SELECTED2, selectionFlag & SurfelInterface::SELECTED2, false);
		scene->setSurfelFlagOn (surfel, SurfelInterface::SELECTED3, selectionFlag & SurfelInterface::SELECTED3, false);

	}		

	int numSelected3 = selectionsNumSurfels [getSelectionIndex (SurfelInterface::SELECTED3)];
	selectionsNumSurfels [getSelectionIndex (SurfelInterface::SELECTED3)] = selectionsNumSurfels [getSelectionIndex (SurfelInterface::SELECTED2)];
	selectionsNumSurfels [getSelectionIndex (SurfelInterface::SELECTED2)] = selectionsNumSurfels [getSelectionIndex (SurfelInterface::SELECTED1)];
	selectionsNumSurfels [getSelectionIndex (SurfelInterface::SELECTED1)] = numSelected3;

	
	selectionsValid [getSelectionIndex (SurfelInterface::SELECTED1)] = false;
	selectionsValid [getSelectionIndex (SurfelInterface::SELECTED2)] = false;
	selectionsValid [getSelectionIndex (SurfelInterface::SELECTED3)] = false;

	// visualize the selection, if necessary
	if (visualizeSelection == true) {
		RendererManager::getInstance()->getMainRenderer()->renderFrame();
	}	
	emitAllSelectionsChanged();
}



bool SelectionTool::hasSelection() {
	
	return (getSelection (getCurrentSelectionFlag())->size() > 0);
}



void SelectionTool::setActiveSelection (int activeSelection) {

	if ((activeSelection >=0) && (activeSelection < SurfelInterface::getNumOfSelections())) {
		switch (activeSelection) {
		case 0:
			configurationWidget->selection1RadioButton->setChecked (true);
			break;
		case 1:
			configurationWidget->selection2RadioButton->setChecked (true);
			break;
		case 2:
			configurationWidget->selection3RadioButton->setChecked (true);
			break;				
		}
	}
}

int SelectionTool::getActiveSelectionIndex () const {

	if (configurationWidget->selection1RadioButton->isChecked()) {
		return 0;
	}
	else if (configurationWidget->selection2RadioButton->isChecked()) {
		return 1;
	}
	else if (configurationWidget->selection3RadioButton->isChecked()) {
		return 2;
	}

	return 0;
}



void SelectionTool::updateSelectionCounters () {

	qDebug ("SelectionTool::updateSelectionCounters ... ");

	int                   selectionIndex;
	SurfelInterface       *surfel;
	Object                *activeObject;
	SurfelInterface::Flag *selectionFlags;
	int                   numSelections;

	activeObject = Scene::getInstance()->getActiveObject();
	
	if (activeObject) {
		
		numSelections = SurfelInterface::getNumOfSelections();
		selectionFlags = new SurfelInterface::Flag [numSelections];
		
		for (selectionIndex = 0; selectionIndex < numSelections; selectionIndex++) {
			selectionsNumSurfels [selectionIndex] = 0;
			selectionFlags [selectionIndex] = getSelectionFlag (selectionIndex);
		}
		
		
		SurfelCollection *surfels = activeObject->getSurfelCollection();		
		for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {			
			for (selectionIndex = 0; selectionIndex < numSelections; selectionIndex++) {								
				if (surfel->isFlagOn (selectionFlags [selectionIndex])) {
					selectionsNumSurfels [selectionIndex]++;
				}
			}
		}		
	}
	qDebug ("SelectionTool::updateSelectionCounters ... finished");

	emitAllSelectionsChanged();
}


void SelectionTool::emitAllSelectionsChanged() {

	int i;
	for (i = 0; i < SurfelInterface::getNumOfSelections(); i++) {
		selectionsValid[i] = false;
		emit selectionChanged (getSelectionFlag(i));
	}
}

QToolButton *SelectionTool::addButtonToToolBar (QToolBar *toolBar) {

	
	toolButton = new QToolButton (rectangleIconSet, "Selection Tool (<b>S</b>)", "Select an area",
	                              0, 0, toolBar, "Selection Tool");
	toolButton->setToggleButton (true);
	QWhatsThis::add (toolButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("selection", rectangleIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal));

	// initialize the popup menu
	ellipseID   = selectionMenu.insertItem (ellipseIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	lassoID     = selectionMenu.insertItem (lassoIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	polygonID   = selectionMenu.insertItem (polygonIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	rectangleID = selectionMenu.insertItem (rectangleIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	brushID     = selectionMenu.insertItem (brushIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	floodFillID = selectionMenu.insertItem (floodFillIconSet.pixmap (QIconSet::Automatic, QIconSet::Normal), this, SLOT (handleToolSelected(int)));
	currentTool = rectangleID;

	toolButton->setPopup (&selectionMenu);
	toolButton->setEnabled (this->checkToolAvailability());

	return toolButton;

}

QToolButton *SelectionTool::getToolButton() {
	return toolButton;
}

QCursor *SelectionTool::getToolCursor() const {
	return toolCursor;
}

QWidget *SelectionTool::getToolConfigurationWidget() const {
	return configurationWidget;
}

bool SelectionTool::isAvailable() const {
	return this->checkToolAvailability();
}

void SelectionTool::handleKeyPressEvent (QKeyEvent *ke) {

	switch (ke->key()) {
		
	case Qt::Key_1:
		{
			configurationWidget->selection1RadioButton->setChecked (true);
		}
		break;
		
		
	case Qt::Key_2:
		{
			configurationWidget->selection2RadioButton->setChecked (true);
		}
		break;
		
	case Qt::Key_3:
		{
			configurationWidget->selection3RadioButton->setChecked (true);
		}
		break;
	}
}

void SelectionTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	ke->ignore();
}

void SelectionTool::handleMousePressEvent (QMouseEvent *me) {

	QPainter qPainter;

	if ((me->button() == Qt::LeftButton) || (me->button() == Qt::MidButton)) {
		// select
		select = true;
	}
	else if (me->button() == Qt::RightButton) {
		// de-select
		select = false;
	}


	// **********
	// lasso tool
	// **********

	if (currentTool == lassoID) {

		// clear the previous points
		nofPolyPoints = 0;
		currentPolyLine.resize(0);
		//this->clearSelection (false, false);
		
		// create and insert the first new point, but check
		// first if there's enough memory available
		if (nofPolyPoints < currentPolyLine.size()) {
			currentPolyLine.setPoint (nofPolyPoints, me->pos());
			nofPolyPoints++;
		}
		else {
			// allocate more memory first
			currentPolyLine.resize (nofPolyPoints + MORE_POINT_ELEMENTS);
			currentPolyLine.setPoint (nofPolyPoints, me->pos());
			nofPolyPoints++;
		}
		
		

	}  // lasso tool

	// **************
	// rectangle tool
	// **************

	else if (currentTool == rectangleID) {

		
		// clear the previous points and selection
		nofPolyPoints = 0;
		currentPolyLine.resize(4);
		//			this->clearSelection (false, false);
		
		// create and insert the new points, but check
		// first if there's enough memory available
		if (nofPolyPoints < currentPolyLine.size() - 3) {
			
			// at the point 4 times
			currentPolyLine.setPoint (nofPolyPoints, me->pos());
			nofPolyPoints++;
			currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), me->y()));
			nofPolyPoints++;
			currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), me->y()));
			nofPolyPoints++;
			currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), me->y()));
			nofPolyPoints++;
			
		}
		else {
			
			// allocate more memory first
			currentPolyLine.resize (nofPolyPoints + MORE_POINT_ELEMENTS);
			// at the point 4 times
			currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), me->y()));
			nofPolyPoints++;
			currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), me->y()));
			nofPolyPoints++;
			currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), me->y()));
			nofPolyPoints++;
			currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), me->y()));
			nofPolyPoints++;			
		}		
		startMousePosition = me->pos();
			
	}  // rectangle tool

	// ************
	// ellipse tool
	// ************

	else if (currentTool == ellipseID) {
		
		// clear the previous selection
		//			this->clearSelection (false, false);
		startMousePosition = me->pos();

	}

	// ************
	// brush tool
	// ************

	else if (currentTool == brushID) {

		if (currentPositionValid) {
			
			if (me->button() == Qt::LeftButton && me->state() == Qt::NoButton) {				
				initiateSelectOperation (true);				
			}
			if (me->button() == Qt::RightButton && me->state() == Qt::NoButton) {				
				initiateSelectOperation (false);				
			}
			else if (me->button() == Qt::MidButton){
				
				orthoMapper->initAdjustCursorShape();
				lastMouseDragPosition = me->pos();			
			}
		}
	}

	// ************
	// floodFill tool
	// ************

	else if (currentTool == floodFillID) {
		initiateFloodFillOperation (me->x(), me->y(), select);				
	}
	oldMousePosition = me->pos();

}

void SelectionTool::handleMouseMoveEvent (QMouseEvent *me) {

	QPainter qPainter;
	int      width,
		     height;

	// **********
	// lasso tool
	// **********

	if (currentTool == lassoID) {
		
		if (me->state() & (Qt::LeftButton | Qt::MidButton | Qt::RightButton)) {
			
			// create and insert the first new point, but check
			// first if there's enough memory available
			if (nofPolyPoints < currentPolyLine.size()) {
				currentPolyLine.setPoint (nofPolyPoints, me->pos());
				nofPolyPoints++;
			}
			else {
				// allocate more memory first
				currentPolyLine.resize (nofPolyPoints + MORE_POINT_ELEMENTS);
				currentPolyLine.setPoint (nofPolyPoints, me->pos());
				nofPolyPoints++;
			}
			
			// draw the last line segment
			
			qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
			qPainter.setPen (QPen (Qt::white));
			// XOR mode
			qPainter.setRasterOp (Qt::XorROP);
			
			qPainter.drawLine (currentPolyLine.point (nofPolyPoints - 2), currentPolyLine.point (nofPolyPoints - 1));
			
			qPainter.end();				
		}

	}  // lasso tool

	// ************
	// polygon tool
	// ************

	else if (currentTool == polygonID) {
		
		if (me->state() & (Qt::LeftButton | Qt::MidButton | Qt::RightButton)) {
			
			// draw the preview line, if possible
			if (isPolygonLineBeingConstructed == true) {		
				
				qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
				qPainter.setPen (QPen (Qt::white));
				// XOR mode
				qPainter.setRasterOp (Qt::XorROP);
				
				// erase the old line
				qPainter.drawLine (currentPolyLine.point (nofPolyPoints - 1), oldMousePosition);
				// draw the new line
				qPainter.drawLine (currentPolyLine.point (nofPolyPoints - 1), me->pos());
				
				qPainter.end();					
			}
		}
	}  // polygon tool

	// **************
	// rectangle tool
	// **************

	else if (currentTool == rectangleID) {

		if (me->state() & (Qt::LeftButton | Qt::MidButton | Qt::RightButton)) {
			// clear the previous points and selection
			nofPolyPoints = 0;
			
			// create and insert the new points, but check
			// first if there's enough memory available
			if (nofPolyPoints < currentPolyLine.size() - 3) {
				
				// at the 4 points of the rectangle
				currentPolyLine.setPoint (nofPolyPoints, startMousePosition);
				nofPolyPoints++;
				currentPolyLine.setPoint (nofPolyPoints, QPoint (startMousePosition.x(), me->y()));
				nofPolyPoints++;
				currentPolyLine.setPoint (nofPolyPoints, me->pos());
				nofPolyPoints++;
				currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), startMousePosition.y()));
				nofPolyPoints++;
				
			}
			else {
				
				// allocate more memory first
				currentPolyLine.resize (nofPolyPoints + MORE_POINT_ELEMENTS);
				// at the 4 points of the rectangle
				currentPolyLine.setPoint (nofPolyPoints, startMousePosition);
				nofPolyPoints++;
				currentPolyLine.setPoint (nofPolyPoints, QPoint (startMousePosition.x(), me->y()));
				nofPolyPoints++;
				currentPolyLine.setPoint (nofPolyPoints, me->pos());
				nofPolyPoints++;
				currentPolyLine.setPoint (nofPolyPoints, QPoint (me->x(), startMousePosition.y()));
				nofPolyPoints++;
			}			
			
			qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
			qPainter.setPen (QPen (Qt::white));
			// XOR mode
			qPainter.setRasterOp (Qt::XorROP);
			
			// erase the old rectangle
			width  = oldMousePosition.x() - startMousePosition.x();
			height = oldMousePosition.y() - startMousePosition.y();
			width  = (width  > 0) ? width  + 1 : width  - 1;
			height = (height > 0) ? height + 1 : height - 1;
			qPainter.drawRect (startMousePosition.x(),
				startMousePosition.y(),
				width,
				height);
			// draw the new rectangle
			width  = me->x() - startMousePosition.x();
			height = me->y() - startMousePosition.y();
			width  = (width  > 0) ? width  + 1 : width  - 1;
			height = (height > 0) ? height + 1 : height - 1;
			qPainter.drawRect (startMousePosition.x(),
				startMousePosition.y(),
				width,
				height);
			
			qPainter.end();			
		}	
	}  // rectangle tool

	// ************
	// ellipse tool
	// ************

	else if (currentTool == ellipseID) {

		if (me->state() & (Qt::LeftButton | Qt::MidButton | Qt::RightButton)) {
			
			qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
			qPainter.setPen (QPen (Qt::white));
			// XOR mode
			qPainter.setRasterOp (Qt::XorROP);
			
			// erase the old ellipse
			width  = oldMousePosition.x() - startMousePosition.x();
			height = oldMousePosition.y() - startMousePosition.y();
			qPainter.drawEllipse (startMousePosition.x(),
				startMousePosition.y(),
				width,
				height);
			// draw the new ellipse
			width  = me->x() - startMousePosition.x();
			height = me->y() - startMousePosition.y();
			qPainter.drawEllipse (startMousePosition.x(),
				startMousePosition.y(),
				width,
				height);
			
			qPainter.end();			
		}
	}

	// **********
	// brush tool
	// **********

	else if (currentTool == brushID) {

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
			orthoMapper->adjustCursorShape (OrthoMapper::ORIENTATION, (int)mouseMoveDistance);
		}		
		else {		
			screenPosition[0] = me->x();
			screenPosition[1] = me->y();
		}
	
		currentPositionValid = orthoMapper->drawCursor (screenPosition, 0);
		
		if (me->state() == Qt::LeftButton) {
			initiateSelectOperation (true);
		}
		else if (me->state() == Qt::RightButton) {
			initiateSelectOperation (false);
		}
	}

	oldMousePosition = me->pos();

}

void SelectionTool::handleMouseReleaseEvent (QMouseEvent *me) {

	QPainter             qPainter;
	FrameBufferInterface *frameBuffer;
	RendererInterface    *renderer;
	int                  width,
		                 height;
	QPoint               topLeft,
		                 bottomRight;

	renderer = RendererManager::getInstance()->getMainRenderer();
	frameBuffer = renderer->getFrameBuffer();	
	
	// **********
	// lasso tool
	// **********

	if (currentTool == lassoID) {
	
		// draw the closing line of the polyline
		
		qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
		qPainter.setPen (QPen (Qt::white));
		
		qPainter.drawLine (currentPolyLine.point (nofPolyPoints - 1), currentPolyLine.point (0));
		
		qPainter.end();				
		
		renderer = RendererManager::getInstance()->getMainRenderer();
		frameBuffer = renderer->getFrameBuffer();			
		
		renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_SURFELLISTS);
		
		selectionsNumSurfels [getActiveSelectionIndex()] = ScanConversion::concavePolygon (nofPolyPoints,
			&currentPolyLine,
			QRect (QPoint (0, 0), Configuration::getInstance()->getViewPortSize()),
			selectionFunction,
			getCurrentSelectionFlag(), select);
		
		
		selectionsValid [getActiveSelectionIndex()] = false;
		
		// visualize the (empty) selection
		renderer->renderFrame();
		
		emit selectionChanged (getCurrentSelectionFlag());
			
	}

	// ************
	// polygon tool
	// ************

	else if (currentTool == polygonID) {

		// left mouse button released
		if (me->button() == Qt::LeftButton)  {
		
			// clear the previous points and selection, if we start
			// a new polygon line
			if (isPolygonLineBeingConstructed == false) {
				nofPolyPoints = 0;
				currentPolyLine.resize(0);
				this->clearSelection (false);
			}
			
			// create and insert the first new point, but check
			// first if there's enough memory available
			if (nofPolyPoints < currentPolyLine.size()) {
				currentPolyLine.setPoint (nofPolyPoints, me->pos());
				nofPolyPoints++;
			}
			else {
				// allocate more memory first
				currentPolyLine.resize (nofPolyPoints + MORE_POINT_ELEMENTS);
				currentPolyLine.setPoint (nofPolyPoints, me->pos());
				nofPolyPoints++;
			}

			// draw the line segment, if possible
			if (isPolygonLineBeingConstructed == true) {
				
				qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
				qPainter.setPen (QPen (Qt::white));
				// XOR mode
				//qPainter.setRasterOp (Qt::XorROP);

				qPainter.drawLine (currentPolyLine.point (nofPolyPoints - 2), currentPolyLine.point (nofPolyPoints - 1));
					
				qPainter.end();				

			}
			
			isPolygonLineBeingConstructed = true;

		}  // left mouse button

		// right mouse button clicked
		else if (me->button() == Qt::RightButton)  {

			// draw the closing line of the polyline
				
			qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
			qPainter.setPen (QPen (Qt::white));
			

			// erase the preview line
			// XOR mode
			qPainter.setRasterOp (Qt::XorROP);
			qPainter.drawLine (currentPolyLine.point (nofPolyPoints - 1), oldMousePosition);

			// close the polyline
			qPainter.setRasterOp (Qt::CopyROP);
			qPainter.drawLine (currentPolyLine.point (nofPolyPoints - 1), currentPolyLine.point (0));
				
			qPainter.end();					

			renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_SURFELLISTS);		
			
			selectionsNumSurfels [getActiveSelectionIndex()] = ScanConversion::concavePolygon (nofPolyPoints,
				                            &currentPolyLine,
								            QRect (QPoint (0, 0), Configuration::getInstance()->getViewPortSize()),
								            selectionFunction,
											getCurrentSelectionFlag(), true);

			selectionsValid [getActiveSelectionIndex()] = false;

			isPolygonLineBeingConstructed = false;

			// visualize the (empty) selection
			renderer->renderFrame();

			emit selectionChanged (getCurrentSelectionFlag());

		}  // right mouse button

	}  // polygon tool

	// **************
	// rectangle tool
	// **************
	
	else if (currentTool == rectangleID) {
		
		
		qPainter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
		qPainter.setPen (QPen (Qt::white));
		
		// draw the final rectangle
		width  = me->x() - startMousePosition.x();
		height = me->y() - startMousePosition.y();
		width  = (width  > 0) ? width  + 1 : width  - 1;
		height = (height > 0) ? height + 1 : height - 1;
		qPainter.drawRect (startMousePosition.x(),
			startMousePosition.y(),
			width,
			height);
		
		qPainter.end();				
		
		renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_SURFELLISTS);
		
		selectionsNumSurfels [getActiveSelectionIndex()] += ScanConversion::concavePolygon (nofPolyPoints,
			&currentPolyLine,
			QRect (QPoint (0, 0), Configuration::getInstance()->getViewPortSize()),
			selectionFunction,
			getCurrentSelectionFlag(), select);
		
		selectionsValid [getActiveSelectionIndex()] = false;
		
		
		// visualize the (empty) selection
		renderer->renderFrame();
		
		emit selectionChanged (getCurrentSelectionFlag());
		

	}  // rectangle tool

	// ************
	// ellipse tool
	// ************

	else if (currentTool == ellipseID) {

		// calculate bounding box of ellipse
		if (me->x() > startMousePosition.x()) {
			topLeft.setX (startMousePosition.x());
			bottomRight.setX (me->x());
		}
		else {
			topLeft.setX (me->x());
			bottomRight.setX (startMousePosition.x());
		}
		
		if (me->y() > startMousePosition.y()) {
			topLeft.setY (startMousePosition.y());
			bottomRight.setY (me->y());
		}
		else {
			topLeft.setY (me->y());
			bottomRight.setY (startMousePosition.y());
		}
		
		width  = bottomRight.x() - topLeft.x();
		height = bottomRight.y() - topLeft.y();			
		
		renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_SURFELLISTS);
		
		selectionsNumSurfels [getActiveSelectionIndex()] += ScanConversion::ellipse (width / 2, height / 2,
			topLeft.x() + width / 2, topLeft.y() + height / 2,
			QRect (QPoint (0, 0), Configuration::getInstance()->getViewPortSize()),
			selectionFunction,
			getCurrentSelectionFlag(), select);
		
		selectionsValid [getActiveSelectionIndex()] = false;
		
		// visualize the (empty) selection
		renderer->renderFrame();
		
		emit selectionChanged (getCurrentSelectionFlag());

	}	

}

void SelectionTool::selectTool() {

	// create ortho mapper for brush selection & flood Fill tool
	orthoMapper = new OrthoMapper (QSize (100, 100), OrthoMapper::PLAIN);


	this->handleSelectedBrushChanged (CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush());
		
	this->connect (Scene::getInstance(), SIGNAL (sceneMoved (const float*)),
		           this, SLOT (handleSceneMoved()));

	this->connect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)),
		           this, SLOT (handleViewPortSizeChanged()));

	// get informed whenever the selected brush changes
	this->connect (CoreResourceTools::getInstance()->getBrushChooserTool(), SIGNAL (brushChanged (const Brush *)),
		           this, SLOT (handleSelectedBrushChanged (const Brush *)));
	
	// get informed whenever the number of objects has changed
	this->connect (Scene::getInstance(), SIGNAL (activeObjectChanged()),
		           this, SLOT (handleActiveObjectChanged()));

	this->connect (Scene::getInstance(), SIGNAL (objectModified (bool)),
		           this, SLOT (handleObjectModified (bool)));


	// turn mouse tracking on
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (true);
	currentPositionValid = false;

}

void SelectionTool::deselectTool() {


	delete orthoMapper;

	// turn mouse tracking off
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (false);

	RendererManager::getInstance()->disconnect (this);


	this->disconnect (Scene::getInstance(), SIGNAL (sceneMoved (const float*)), this, 0);
	this->disconnect (Scene::getInstance(), SIGNAL (activeObjectChanged()), this, 0);
	this->disconnect (Scene::getInstance(), SIGNAL (objectModified(bool)), this, 0);
	this->disconnect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)), this, 0);
	this->disconnect (CoreResourceTools::getInstance()->getBrushChooserTool(), SIGNAL (brushChanged (const Brush *)), this, 0);


}

// ************
// public slots
// ************

void SelectionTool::handleRendererWidgetRepainted() {
	currentPositionValid = orthoMapper->drawCursor (screenPosition, 0);
}

// ***************
// private methods
// ***************

void SelectionTool::initiateSelectOperation (bool select) {

			int                       i;
			std::vector<SurfelInterface *> *surfels = NULL;
			int                       nofSurfels;
			Scene                     *scene;
			int                       selectionIndex;
			bool allPixelCovered;

	if (currentPositionValid) {

		if (this->configurationWidget->useBrushCheckBox->isChecked()) {


			// return immediately if the ortho mapper could not reconstruct the surface
			if( orthoMapper->reconstructSurfaceFunction(allPixelCovered) == false ) return;


			FrameBufferInterface *frameBuffer = RendererManager::getInstance()->getAuxiliaryRenderer()->getFrameBuffer();
			
			surfels = frameBuffer->getSurfelsInViewFrustum();
			nofSurfels = frameBuffer->getNofSplatInfoEntries();
			
			// de-render cursor
			orthoMapper->renderCursor();

			// der-ender old surfels
			RendererManager::getInstance()->getMainRenderer()->subtractFromFrame (surfels, nofSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W, false);
			
			painter->select (surfels, nofSurfels, frameBuffer->getTextureCoordinatesInViewFrustum(),
				selectionsNumSurfels);				
			
		}
		else {

												
			scene = Scene::getInstance();
			selectionIndex = getActiveSelectionIndex();
			
			orthoMapper->computeAllVisibleSurfels (&surfels, nofSurfels);
			
			// de-render cursor
			orthoMapper->renderCursor();
			
			// der-ender surfels
			RendererManager::getInstance()->getMainRenderer()->subtractFromFrame (surfels, nofSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W, false);
			
			SurfelInterface::Flag selectionFlag = getCurrentSelectionFlag();;
			
			
			// select
			if (select) {
				for (i = 0; i < nofSurfels; i++) {
					
					// if not selected yet...
					if ((*surfels)[i]->isFlagOn (selectionFlag) == false) {
						
						// ...mark this surfel as selected, but DON'T emit the signal here
						scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(selectionFlag | SurfelInterface::EMPHASISE), true, false);					
						selectionsValid [selectionIndex] = false;
						selectionsNumSurfels [selectionIndex]++;
					}
				}	
			}
			// deselect
			else {
				for (i = 0; i < nofSurfels; i++) {
					
					// if selected ...
					if ((*surfels)[i]->isFlagOn (selectionFlag) == true) {
						
						// ...mark this surfel as deselected, but DON'T emit the signal here
						scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(selectionFlag | SurfelInterface::EMPHASISE), false, false);					
						if ((*surfels)[i]->isSelected()) {
							scene->setSurfelFlagOn ((*surfels)[i], SurfelInterface::EMPHASISE, true, false);
						}
						
						selectionsValid [selectionIndex] = false;
						selectionsNumSurfels [selectionIndex]--;						
					}
				}
			}
		}			
			
		// re-render new surfels
		RendererManager::getInstance()->getMainRenderer()->addToFrame (surfels, nofSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W);
		
		// re-render cursor
		orthoMapper->renderCursor();
		
		emit selectionChanged (getCurrentSelectionFlag());			
			
	}
	
}



void SelectionTool::initiateFloodFillOperation (int x, int y, bool select) {
	
	unsigned int                i,
		                        selectionIndex,
								stackIndex;
	Scene                       *scene;
	FrameBufferInterface        *frameBuffer;
	Vector3D                    seedPoint;
	Transformer                 *sceneTransformer;		
	NeighbourHood               *neighbourHood;
	std::vector<SurfelInterface*>::iterator iter_begin;
	std::vector<SurfelInterface*>   *surfelStack;

	SurfelInterface             *surfel;
	SurfelInterface::Flag       selectionFlag;
	MyDataTypes::TransformationMatrix16f objectMatrix;

	(void)select;
	
	scene = Scene::getInstance();
	frameBuffer = RendererManager::getInstance()->getMainRenderer()->getFrameBuffer();
	sceneTransformer = Transformer::getSceneTransformer();
	
	selectionIndex = getActiveSelectionIndex();
	selectionFlag = getCurrentSelectionFlag();;
	
	
	
	// build search structure and corresponding surfel index vector 
	neighbourHood = scene->getActiveObject()->getSurfelCollection()->buildNeighbourHood();
	
	std::vector<SurfelInterface*>* surfels = scene->getActiveObject()->getSurfelCollection()->getSurfelVector();
	iter_begin = surfels->begin();
	
		// put seed point on stack
	scene->getActiveObject()->getTransformationMatrix (objectMatrix);
	sceneTransformer->setObjectTransformationMatrix (objectMatrix);
	seedPoint = sceneTransformer->viewPortToObject (Vector2D (x, y), frameBuffer->getDepth (x, y));
	neighbourHood->setNofNeighbours (8);
	neighbourHood->setSourcePoint (seedPoint);	
	surfel = *(iter_begin + neighbourHood->getNeighbourPositionIndex (0));		
	scene->setSurfelFlagOn (surfel, (SurfelInterface::Flag)(selectionFlag | SurfelInterface::EMPHASISE), true, false);					

	surfelStack = new std::vector<SurfelInterface*>(surfels->size());
	stackIndex = 0;
	(*surfelStack)[stackIndex++] = surfel;

	selectionsNumSurfels [selectionIndex]++;
	
	while (stackIndex > 0) {
		
		stackIndex--;
		surfel = (*surfelStack)[stackIndex];
		
		neighbourHood->setSourcePoint (surfel->getPosition());
		
		// check all neighbors of current point and put them on stack if not already marked as selected
		for (i = 1; i < neighbourHood->getNofNeighbours(); i++) {
			surfel = *(iter_begin + neighbourHood->getNeighbourPositionIndex (i));
			if (surfel->isFlagOn (selectionFlag) == false) {
				(*surfelStack)[stackIndex++] = surfel;
				scene->setSurfelFlagOn (surfel, (SurfelInterface::Flag)(selectionFlag | SurfelInterface::EMPHASISE), true, false);					
				selectionsNumSurfels [selectionIndex]++;
			}
		}
	}		
	selectionsValid [selectionIndex] = false;

	RendererManager::getInstance()->getMainRenderer()->renderFrame();
	
	delete surfelStack;


	emit selectionChanged (getCurrentSelectionFlag());			
}





SurfelInterface::Flag SelectionTool::getCurrentSelectionFlag() {

	if (configurationWidget->selection1RadioButton->isChecked()) {
		return SurfelInterface::SELECTED1;
	}
	else if (configurationWidget->selection2RadioButton->isChecked()) {
		return SurfelInterface::SELECTED2;
	}
	else {
		return SurfelInterface::SELECTED3;
	}
}



int SelectionTool::getSelectionIndex (SurfelInterface::Flag selectionFlag) {

	if (selectionFlag == SurfelInterface::SELECTED1) {
		return 0;
	}
	else if (selectionFlag == SurfelInterface::SELECTED2) {
		return 1;
	}
	else if (selectionFlag == SurfelInterface::SELECTED3) {
		return 2;
	}
	return 0;
}


SurfelInterface::Flag SelectionTool::getSelectionFlag (int selectionIndex) {

	if (selectionIndex == 0) {
		return SurfelInterface::SELECTED1;
	}
	else if (selectionIndex == 1) {
		return SurfelInterface::SELECTED2;
	}
	else if (selectionIndex == 2) {
		return SurfelInterface::SELECTED3;
	}
	return SurfelInterface::SELECTED1;
}

bool SelectionTool::checkToolAvailability() const {
	return RendererManager::getInstance()->getMainRenderer()->isInteractive() &&
		   (Scene::getInstance()->getNofObjects() > 0);
}

// *************
// private slots
// *************

void SelectionTool::handleToolSelected (int id) {

	currentTool = id;

	if (id == ellipseID) {
		toolButton->setIconSet (ellipseIconSet);
	}    
	else if (id == lassoID) {
		toolButton->setIconSet (lassoIconSet);
	}
	else if (id == rectangleID) {
		toolButton->setIconSet (rectangleIconSet);
	}
	else if (id == polygonID) {
		toolButton->setIconSet (polygonIconSet);
	}
	else if (id == brushID) {
		toolButton->setIconSet (brushIconSet);
	}
	else if (id == floodFillID) {
		toolButton->setIconSet (floodFillIconSet);
	}


}

void SelectionTool::handleActiveObjectChanged() {

	updateSelectionCounters();
	if (orthoMapper != 0) {
		orthoMapper->reset();
		currentPositionValid = orthoMapper->drawCursor (screenPosition, 0.0f);
	}
}


void SelectionTool::handleObjectModified (bool samplingPatternAltered) {

	if (samplingPatternAltered) {
		updateSelectionCounters();
		emitAllSelectionsChanged ();
	}
}



void SelectionTool::handleSceneMoved () {

	if (orthoMapper != 0) {
		orthoMapper->reset();
		currentPositionValid = orthoMapper->drawCursor (screenPosition, 0.0f);
	}

}

void SelectionTool::handleViewPortSizeChanged () {
	if (orthoMapper != 0) {
		orthoMapper->reset();
	}
}



void SelectionTool::handleSelectedBrushChanged (const Brush *newBrush) {
	
	currentBrush = newBrush;
	if (currentBrush != 0) {		
		if (orthoMapper != 0) {
			orthoMapper->handleBufferSizeChanged (currentBrush->getSize());
		}
		else {
			orthoMapper = new OrthoMapper (currentBrush->getSize(), OrthoMapper::PLAIN);
		}
		
		if (painter != 0) {
			painter->setBrush (currentBrush);
		}
		else {
			painter = new Painter (currentBrush);
		}		
	}
}





// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
