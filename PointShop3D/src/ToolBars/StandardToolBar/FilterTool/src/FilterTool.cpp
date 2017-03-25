// Title:   FilterTool.cpp
// Created: Thu Sep 25 14:57:20 2003
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

#include "../../../../Core/Scene/src/Object.h"
#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Core/Transformer/src/Transformer.h"
#include "../../../../Core/Configuration/src/Configuration.h"
#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../../../../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "../UI/FilterConfigurationWidget.h" 
#include "../UI/ImageViewer.h" 

#include "../../../../Utilities/src/Common.h"
#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../../Utilities/src/MyMathTools.h"
#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Core/CoreTools/src/CoreTools.h"
#include "../../../../Core/Painter/src/Painter.h"

#include "FilterTool.h"


// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"Filter\"> "
"Click this button to filter the 3D-surface:"
"<ul><li>Control + middle mouse button: change filter height</li>"
"<li>Shift + middle mouse button: change filter width</li>"
"<li>Alt + middle mouse button: change scale </li>";

FilterTool::FilterTool() {

	standardCursor = new QCursor (CrossCursor);
	toolCursor     = standardCursor;
	orthoMapper    = 0;
	painter        = 0;
	
	configurationWidget = new FilterConfigurationWidget(0);

}


FilterTool::~FilterTool() {

	delete standardCursor;
	delete configurationWidget;
	this->cleanUp();

}

// **************
// public methods
// **************

QToolButton *FilterTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	QPixmap  normalIcon ("Resources/FilterToolIcon.png"),
		     disabledIcon ("Resources/FilterToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);
	filterButton = new QToolButton (iconSet, "Filter Tool", "Paint with a filter",
	                                0, 0, toolBar, "Filter Tool");
	filterButton->setToggleButton (true);
	filterButton->setEnabled (this->checkToolAvailability());
	QWhatsThis::add (filterButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("filter", normalIcon);

	return filterButton;

}

QCursor *FilterTool::getToolCursor() const {
	return toolCursor;
}


QToolButton* FilterTool::getToolButton () {
	return filterButton;
}

QWidget *FilterTool::getToolConfigurationWidget() const {
	return configurationWidget;
}

bool FilterTool::isAvailable() const {
	return this->checkToolAvailability();
}

void FilterTool::handleMousePressEvent (QMouseEvent *me) {


	if (currentPositionValid == true) {

		if (me->button() == Qt::LeftButton && me->state() == Qt::NoButton) {			
			this->initiateFilterOperation();
		}
		else if ((me->button() == Qt::MidButton) || (me->button() == Qt::RightButton)) {			
			orthoMapper->initAdjustCursorShape();
			lastMouseDragPosition = me->pos();			
		}

	}

}

void FilterTool::handleMouseMoveEvent (QMouseEvent *me) {


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
		this->initiateFilterOperation();
	}
	

}

void FilterTool::handleMouseReleaseEvent (QMouseEvent *) {

}

void FilterTool::handleKeyPressEvent (QKeyEvent *ke) {
	ke->ignore();
}

void FilterTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	ke->ignore();
}

void FilterTool::selectTool() {

	QApplication::setOverrideCursor (Qt::waitCursor);

	mainRenderer = RendererManager::getInstance()->getMainRenderer();
	
	orthoMapper = new OrthoMapper (QSize (configurationWidget->resolutionXSpinBox->value(), configurationWidget->resolutionYSpinBox->value()),
		OrthoMapper::PLAIN);
	orthoMapper->setToolOrientation (true);
	orthoMapper->setTouchingPointAlignment (false);


	this->connect (Scene::getInstance(), SIGNAL (sceneMoved (const float*)),
		this, SLOT (handleSceneMoved ()));


	this->connect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)),
		this, SLOT (handleViewPortSizeChanged ()));


	resamplingRenderer = RendererManager::getInstance()->getAuxiliaryRenderer();
	resamplingFrameBuffer = resamplingRenderer->getFrameBuffer();

	// turn mouse tracking on
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (true);

	currentPositionValid = false;

	QApplication::restoreOverrideCursor ();
}


void FilterTool::deselectTool() {
//	CoreResourceTools::getInstance()->getBitmapResourceManagerTool()->disconnect (this);

	// turn mouse tracking off
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setMouseTracking (false);
	
	this->cleanUp();
	
	this->disconnect (Scene::getInstance(), SIGNAL (sceneMoved (const float*)), this, 0);
	this->disconnect (Configuration::getInstance(), SIGNAL (viewPortSizeChanged (const QSize)), this, 0);
	
}

// ************
// public slots
// ************

void FilterTool::handleRendererWidgetRepainted () {

	if (orthoMapper != 0) {
		currentPositionValid = orthoMapper->drawCursor (screenPosition, 0);
	}

}

// ***************
// private methods
// ***************

void FilterTool::initiateFilterOperation () {	
	
	if (currentPositionValid) {
		
		int   x,                     //	coordinates in brush
			  y,
			  sizeX,                 //  size of frameBuffer
			  sizeY; 
		Scene *scene;
		int   bufferX,
			  bufferY;				
		bool  allPixelCovered;
		QSize filterSize;

		filterSize = QSize (configurationWidget->resolutionXSpinBox->value(), configurationWidget->resolutionYSpinBox->value());

		orthoMapper->handleBufferSizeChanged (filterSize, false);
		// return immediately if the ortho mapper could not reconstruct the surface
		if( orthoMapper->reconstructSurfaceFunction(allPixelCovered) == false ) return;
		
		scene = Scene::getInstance();
		
		sizeX = filterSize.width();
		sizeY = filterSize.height();
		
		const std::vector<MyDataTypes::TextureCoordinate>* texCoords = resamplingFrameBuffer->getTextureCoordinatesInViewFrustum();
		
		
		/////////////////////////////////////////////////////////////////////////////////
		// right now only pixel sampling!
		// this is just an example of an oil-paint filter
		//
		
		if (resamplingFrameBuffer->getNofSplatInfoEntries() > 0) {
									
			const std::vector<SurfelInterface *>* surfels = resamplingFrameBuffer->getSurfelsInViewFrustum();
			int numSurfels = resamplingFrameBuffer->getNofSplatInfoEntries();

			
			// de-render cursor
			orthoMapper->renderCursor();
			
			// der-ender old surfels
			RendererManager::getInstance()->getMainRenderer()->subtractFromFrame (surfels, numSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W, false);
									
			
			// oil paint filter
			
			QRgb *colorGrid = new QRgb [sizeX * sizeY];
			int masksize = 5;
			int index;
			int i;
			int left, right, bottom, top;
			
			int intensityHisto[256];
			int averageRed[256], averageGreen[256], averageBlue[256];
			float red, green, blue;
			QRgb color;
			
			
			for (x = 0; x < sizeX; x++) {				
				for (y = 0; y < sizeY; y++) {
					left   = (x - masksize < 0) ? 0 : x - masksize;
					right  = (x + masksize > sizeX - 1) ? sizeX - 1 : x + masksize;
					bottom = (y - masksize < 0) ? 0 : y - masksize;
					top    = (y + masksize > sizeY - 1) ? sizeY - 1 : y + masksize;
					
					for (i = 0; i < 256; i++)
						intensityHisto[i] = 0;
					
					for (bufferX = left; bufferX <= right; bufferX++) {
						for (bufferY = bottom; bufferY <= top; bufferY++) {	
							
							if (resamplingFrameBuffer->isPixelCovered (bufferX, bufferY)) {
								color = resamplingFrameBuffer->getColor (bufferX, bufferY);						
								index = qGray (color);	
								intensityHisto[index]++;
								
								if (intensityHisto [index] == 1) {
									averageRed   [index] = qRed (color);
									averageGreen [index] = qGreen (color);
									averageBlue  [index] = qBlue (color);
								}
								else {
									averageRed   [index] += qRed (color);
									averageGreen [index] += qGreen (color);
									averageBlue  [index] += qBlue (color);
								}
							}
						}
					}					
					
					// find maximum intensity
					index = 0;
					int maxIntensity = 0;
					
					for (i = 0; i < 256; i++) {
						if (intensityHisto[i] > maxIntensity) {
							index = i;
							maxIntensity = intensityHisto[i];
						}
					}				
					
					red   = averageRed  [index] / (float) maxIntensity;
					green = averageGreen[index] / (float) maxIntensity;
					blue  = averageBlue [index] / (float) maxIntensity;
					colorGrid [y * sizeX + x] = qRgb ((int)red, (int)green, (int)blue);										
				}
			}						
			
			
			for (i = 0; i < numSurfels; i++) {
				
				color = colorGrid [(int) (((int) (*texCoords)[i].v) * sizeX + (*texCoords)[i].u)];
				
					
				scene->setSurfelDiffuseColor ((*surfels)[i], color, false);
			}
			
			delete [] colorGrid;
			
			
			// re-render new surfels
			RendererManager::getInstance()->getMainRenderer()->addToFrame (surfels, numSurfels, FrameBufferInterface::PERPIXEL_C_Z_N_W);
			
			// re-render cursor
			orthoMapper->renderCursor();
			
			scene->emitObjectModifiedSignal ();
			
		}
		
	}	

}

bool FilterTool::checkToolAvailability() const {
	return RendererManager::getInstance()->getMainRenderer()->isInteractive() &&
		   (Scene::getInstance()->getNofObjects() > 0);
}

void FilterTool::cleanUp() {

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



void FilterTool::handleModelChanged() {}

void FilterTool::handleSceneMoved () {

	if (orthoMapper != 0) {
		orthoMapper->reset();
	}

}

void FilterTool::handleViewPortSizeChanged () {
	
	if (orthoMapper != 0) {
		orthoMapper->reset();
	}

}

// ****************
// access functions
// ****************

ToolInterface *createTool() {
	return new FilterTool();
}

void destroyTool (ToolInterface *tool) {
	delete tool;
}

const char *getToolName() {
	return "FilterTool";
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
