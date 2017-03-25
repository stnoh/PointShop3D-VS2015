// Title:   BrushTool.cpp
// Created: Thu Sep 25 14:54:37 2003
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
#include <qspinbox.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qmessagebox.h>


#include "../../../../Core/Scene/src/Object.h"
#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Core/Transformer/src/Transformer.h"
#include "../../../../Core/Configuration/src/Configuration.h"
#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "../../../../Utilities/src/Common.h"
#include "../../../../Utilities/src/MyMathTools.h"
#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Core/CoreTools/src/CoreTools.h"
#include "../../../../Core/Painter/src/Painter.h"
#include "../UI/BrushConfigurationWidget.h" 
#include "../UI/ImageViewer.h" 
#include "BrushTool.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"brush\"> "
"Click this button to draw on the 3D-surface:"
"<ul><li>Control + middle mouse button: change brush height</li>"
"<li>Shift + middle mouse button: change brush width</li>"
"<li>Alt + middle mouse button: change scale </li>"
"<li>Shift + right mouse button: change intrusion</li></ul>";

BrushTool::BrushTool() {

	standardCursor     = new QCursor (CrossCursor);
	toolCursor         = standardCursor;
	orthoMapper        = 0;
	painter            = 0;
	newSurfelPositions = 0;
	
	configurationWidget = new BrushConfigurationWidget(0);

	this->connect (configurationWidget->orientationNormalRadioButton, SIGNAL (toggled (bool)),
		           this, SLOT (handleToolOrientationChanged()));

	this->connect (configurationWidget->touchingSurfaceRadioButton, SIGNAL (toggled (bool)),
		           this, SLOT (handleTouchingAlignmentChanged()));


}


BrushTool::~BrushTool() {

	delete standardCursor;
	delete configurationWidget;

	this->cleanUp();

}

// **************
// public methods
// **************

QToolButton *BrushTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	QPixmap  normalIcon ("Resources/BrushToolIcon.png"),
		     disabledIcon ("Resources/BrushToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);
	brushButton = new QToolButton (iconSet, "Brush Tool", "Paint with a brush",
	                               0, 0, toolBar, "Brush Tool");
	brushButton->setToggleButton (true);
	brushButton->setEnabled (this->checkToolAvailability());

	brushButton->setEnabled (this->checkToolAvailability());
	QWhatsThis::add (brushButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("brush", normalIcon);

	return brushButton;

}

QCursor *BrushTool::getToolCursor() const {
	return toolCursor;
}

QToolButton* BrushTool::getToolButton () {
	return brushButton;
}

QWidget *BrushTool::getToolConfigurationWidget() const {
	return configurationWidget;
}

bool BrushTool::isAvailable() const {
	return this->checkToolAvailability();
}

void BrushTool::handleMousePressEvent (QMouseEvent *me) {

	if (currentPositionValid == true && currentBrush != 0) {

		if (me->button() == Qt::LeftButton && me->state() == Qt::NoButton) {
			this->initiateBrushOperation();
		}
		else if ((me->button() == Qt::MidButton) || (me->button() == Qt::RightButton)){			
			orthoMapper->initAdjustCursorShape();
			lastMouseDragPosition = me->pos();			
		}

	}

}

void BrushTool::handleMouseMoveEvent (QMouseEvent *me) {

	float mouseMoveDistance = me->y() - lastMouseDragPosition.y();

	if (currentBrush == 0) {
		return;
	}

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
	
	currentPositionValid = orthoMapper->drawCursor (screenPosition,
		(float) configurationWidget->tilingDiscreteSpinBox->value() / 100.0f);

	if (me->state() == Qt::LeftButton) {
		this->initiateBrushOperation();
	}

}

void BrushTool::handleMouseReleaseEvent (QMouseEvent *) {}

void BrushTool::handleKeyPressEvent (QKeyEvent *ke) {

	Vector3D cartNormal = orthoMapper->getFixedToolNormal();
	Vector3D polarNormal = Vector3D::cartesianToPolar (cartNormal);

	switch (ke->key()) {


	case Qt::Key_P:
		{
			SurfelCollection *surfels = Scene::getInstance()->getActiveObject()->getSurfelCollection();
			painter->paintWholeObject (surfels);
			Scene::getInstance()->emitObjectModifiedSignal (true);
			Scene::getInstance()->emitRenderFrameSignal ();
		}
		break;


	case Qt::Key_I:
		{
			orthoMapper->invertDepth();
		}
		break;


	// adjusting fixed carving direction
	case Qt::Key_8:
		{
			polarNormal[1] += 0.01f;
		}
		break;

	case Qt::Key_2:
		{
			polarNormal[1] -= 0.01f;
		}
		break;
	case Qt::Key_6:
		{
			polarNormal[2] += 0.01f;
		}
		break;
	case Qt::Key_4:
		{
			polarNormal[2] -= 0.01f;
		}
		break;
	}

	cartNormal = Vector3D::polarToCartesian (polarNormal);
	cartNormal.normalize();

	orthoMapper->setFixedToolNormal (cartNormal);
	
	currentPositionValid = orthoMapper->drawCursor (screenPosition,
		(float) configurationWidget->tilingDiscreteSpinBox->value() / 100.0f);
}

void BrushTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	ke->ignore();
}

void BrushTool::selectTool() {
	
	mainRenderer = RendererManager::getInstance()->getMainRenderer();

	this->handleSelectedBrushChanged (CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush());
		
	this->connect (Scene::getInstance(), SIGNAL (sceneMoved (const float*)),
		           this, SLOT (handleSceneMoved()));

	this->connect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)),
		           this, SLOT (handleViewPortSizeChanged()));

	// get informed whenever the selected brush changes
	this->connect (CoreResourceTools::getInstance()->getBrushChooserTool(), SIGNAL (brushChanged (const Brush *)),
		           this, SLOT (handleSelectedBrushChanged (const Brush *)));

	this->connect (Scene::getInstance(), SIGNAL (activeObjectChanged()),
		           this, SLOT (handleActiveObjectChanged()));

	resamplingRenderer = RendererManager::getInstance()->getAuxiliaryRenderer();
	resamplingFrameBuffer = resamplingRenderer->getFrameBuffer();

	// turn mouse tracking on
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (true);

	currentPositionValid = false;
	samplingPatternAltered = false;


}


void BrushTool::deselectTool() {

	Scene::getInstance()->emitObjectModifiedSignal (samplingPatternAltered);

	// turn mouse tracking off
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (false);
	
	this->cleanUp();

	if (newSurfelPositions != 0) {
		delete[] newSurfelPositions;
		newSurfelPositions = 0;
	}

	this->disconnect (Scene::getInstance(), SIGNAL (sceneMoved (const float*)), this, 0);
	this->disconnect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)), this, 0);
	this->disconnect (CoreResourceTools::getInstance()->getBrushChooserTool(), SIGNAL (brushChanged (const Brush *)), this, 0);
	
}

// ************
// public slots
// ************

void BrushTool::handleRendererWidgetRepainted () {
	
	if (orthoMapper != 0) {
		orthoMapper->renderCursor();
	}
	
}

// ***************
// private methods
// ***************

void BrushTool::initiateBrushOperation() {
	

	if (currentPositionValid == true) {

		int              x,                     //	coordinates in brush
			             y,
			             sizeX,                 //  size of brush
		    	         sizeY; 
		SurfelCollection *objectSurfels;
		Scene            *scene;
  		
		bool allPixelCovered;
		
		// return immediately if the ortho mapper could not reconstruct the surface
		if( orthoMapper->reconstructSurfaceFunction(allPixelCovered) == false ) return;
			
		scene = Scene::getInstance();
		
		sizeX = currentBrush->getSize().width();
		sizeY = currentBrush->getSize().height();
		
		
		/////////////////////////////////////////////////////////////////////////////////
		// resampling according to brush distribution
		//
		
		if (allPixelCovered && configurationWidget->samplingBrushRadioButton->isChecked()) {
			
			// create new surfels
			std::vector<SurfelInterface*> surfelGrid (sizeX * sizeY);
			objectSurfels = scene->getActiveObject()->getSurfelCollection();
			// reserve memory
			objectSurfels->reserve(objectSurfels->getNofSurfels() + sizeX * sizeY);
			
			for (x = 0; x < sizeX; x++) {
				for (y = 0; y < sizeY; y++) {											
					surfelGrid[y * sizeX + x] = orthoMapper->insertNewSurfel (objectSurfels, x, y);																									
				}
				
			}


			Vector3D *carveDirection = 0;
			if (currentBrush->hasChannel (SurfelInterface::POSITION)) {
				if (configurationWidget->displCarvingRadioButton->isChecked()) {
					Vector3D carveDir = orthoMapper->setDistancesToToolPlane (currentBrush->getChannel (SurfelInterface::POSITION)->getComponent(1));
					carveDirection = &carveDir;
				}
			}
			
			// modify new surfels
			std::vector<MyDataTypes::TextureCoordinate>* empty = NULL;
			if (painter->paint (&surfelGrid, sizeX * sizeY, empty, orthoMapper->getZScale(), carveDirection)) {
				
				const std::vector<SurfelInterface *>* surfels = resamplingFrameBuffer->getSurfelsInViewFrustum();
				int numSurfels = resamplingFrameBuffer->getNofSplatInfoEntries();

				// der-ender old surfels
				RendererManager::getInstance()->getMainRenderer()->subtractFromFrame (surfels, numSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W, false);

				// remove old surfels
				objectSurfels->removeSurfels (surfels, false);

				// re-render new surfels
				RendererManager::getInstance()->getMainRenderer()->addToFrame (&surfelGrid, sizeX * sizeY, 
					FrameBufferInterface::PERPIXEL_C_Z_N_W);

				samplingPatternAltered = true;
				scene->emitObjectModifiedSignal();

			}
			else {
				// painting failed, remove surfels			
				for (x = 0; x < sizeX; x++) {
					for (y = 0; y < sizeY; y++) {
						objectSurfels->removeSurfel (surfelGrid[y * sizeX + x], false);
					}
				}
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////////
		// sample on old surface positions
		//
		
		else if (configurationWidget->samplingSurfaceRadioButton->isChecked()) {
			
			if (resamplingFrameBuffer->getNofSplatInfoEntries() > 0) {
				
				
				if (configurationWidget->samplingConstantRadioButton->isChecked()) {
					
					std::vector<SurfelInterface *> *surfels = resamplingFrameBuffer->getSurfelsInViewFrustum();
					int numSurfels = resamplingFrameBuffer->getNofSplatInfoEntries();
					
					// der-ender old surfels
					RendererManager::getInstance()->getMainRenderer()->subtractFromFrame (surfels, numSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W, false);

					painter->paint (surfels, numSurfels, resamplingFrameBuffer->getTextureCoordinatesInViewFrustum());				
										
					// re-render new surfels
					RendererManager::getInstance()->getMainRenderer()->addToFrame (surfels, numSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W);

					scene->emitObjectModifiedSignal();
				}			
				else if (configurationWidget->samplingGaussianRadioButton->isChecked()) {					
				}									
			}	
		}				
	}	
}



void BrushTool::initBrushDataStructures (void) {

	if (newSurfelPositions != 0) {
		delete[] newSurfelPositions;
	}
	newSurfelPositions = new Vector3D [currentBrush->getSize().height() * currentBrush->getSize().width()];

}

bool BrushTool::checkToolAvailability() const {
	return CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush() != 0 &&
		   RendererManager::getInstance()->getMainRenderer()->isInteractive() &&
		   (Scene::getInstance()->getNofObjects() > 0);
}

void BrushTool::cleanUp() {

	if (orthoMapper != 0) {
		delete orthoMapper;
		orthoMapper = 0;
	}

	if (painter != 0) {
		delete painter;
		painter = 0;
	}

}

// *************
// private slots
// *************

void BrushTool::handleSelectedBrushChanged (const Brush *newBrush) {
	
	currentBrush = newBrush;
	if (currentBrush != 0) {

		if (brushButton->isOn() == true) {
		
			if (orthoMapper != 0) {
				orthoMapper->handleBufferSizeChanged (currentBrush->getSize());
			}
			else {
				orthoMapper = new OrthoMapper (currentBrush->getSize(), OrthoMapper::WITH_DEPTH);
			}

			if (painter != 0) {
				painter->setBrush (currentBrush);
			}
			else {
				painter = new Painter (currentBrush);
			}

			this->initBrushDataStructures();
			this->handleToolOrientationChanged();
			this->handleTouchingAlignmentChanged();

		}		
		
	}
	else {

		if (brushButton->isOn() == true) {
			this->cleanUp();
		}

	}

	brushButton->setEnabled (this->checkToolAvailability());

}

void BrushTool::handleActiveObjectChanged() {

	if (orthoMapper != 0) {
		orthoMapper->reset();
		currentPositionValid = orthoMapper->drawCursor (screenPosition,
			(float) configurationWidget->tilingDiscreteSpinBox->value() / 100.0f);
	}
}

void BrushTool::handleSceneMoved () {

	if (orthoMapper != 0) {
		orthoMapper->reset();
		currentPositionValid = orthoMapper->drawCursor (screenPosition,
			(float) configurationWidget->tilingDiscreteSpinBox->value() / 100.0f);
	}

}

void BrushTool::handleViewPortSizeChanged () {
	orthoMapper->reset();
}

void BrushTool::handleToolOrientationChanged () {
	orthoMapper->setToolOrientation (configurationWidget->orientationNormalRadioButton->isChecked());
}

void BrushTool::handleTouchingAlignmentChanged () {
	orthoMapper->setTouchingPointAlignment (configurationWidget->touchingReferenceRadioButton->isChecked());
}

// ****************
// access functions
// ****************

ToolInterface *createTool() {
	return new BrushTool();
}

const char *getToolName() {
	return "BrushTool";
}









	

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
