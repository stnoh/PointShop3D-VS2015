// Title:   ParameterizationTool.cpp
// Created: Thu Sep 25 14:58:15 2003
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

#include <limits.h>

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qwhatsthis.h>
#include <qwidget.h>
#include <qstring.h>
#include <qpainter.h>
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
#include <qfont.h>

#include "../../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../../Core/RendererManager/src/RendererManager.h"
#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Core/MarkerManager/src/Marker2D.h"
#include "../../../../Core/MarkerManager/src/Marker3D.h" 
#include "../../../../Core/MarkerManager/src/MarkerUtils.h"
#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "ParameterizationDialogImpl.h"
#include "ParameterizationTool.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"parameterization\"> "
"Generate a UV parameterization (i.e. texture coordinates) for the 3D-surface.";

// **************
// public methods
// **************

ParameterizationTool::ParameterizationTool() {

	ParameterizationDialogImpl *parameterizationDialog;

	toolCursor          = new QCursor (CrossCursor);
	configurationWidget = new ParameterizationConfigurationWidget();
	parameterization    = new Parameterization();

	parameterizationDialog = ParameterizationDialogImpl::getInstance (this);

	// store initial position and size of the dialog, as to restore it for the next time the dialog is opened
	parameterizationDialogPosition = parameterizationDialog->pos();
	parameterizationDialogSize     = parameterizationDialog->size();

	isDragging = false;

	MarkerManager::getInstance()->initialize (BitmapMarker::getInstance(), RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
	
}

ParameterizationTool::~ParameterizationTool() {
	delete toolCursor;
	delete configurationWidget;
	delete parameterization;

	ParameterizationDialogImpl::destroyInstance();
}

QToolButton *ParameterizationTool::addButtonToToolBar (QToolBar *toolBar) {
	
	QIconSet iconSet;
	QPixmap  normalIcon ("Resources/ParameterizationToolIcon.png"),
		     disabledIcon ("Resources/ParameterizationToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);
	parameterizationButton = new QToolButton (iconSet, "Parameterization Tool", "Parameterize Texture",
	                                          0, 0, toolBar, "Parameterization Tool");
	parameterizationButton->setToggleButton (true);
	parameterizationButton->setEnabled (this->checkToolAvailability());
	QWhatsThis::add (parameterizationButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("parameterization", normalIcon);

	return parameterizationButton;

}

QToolButton *ParameterizationTool::getToolButton() {
	return parameterizationButton;
}

QCursor *ParameterizationTool::getToolCursor() const {
	return toolCursor;
}

QWidget *ParameterizationTool::getToolConfigurationWidget() const {
	return configurationWidget;
}

bool ParameterizationTool::isAvailable() const {
	return this->checkToolAvailability();
}

void ParameterizationTool::handleMousePressEvent (QMouseEvent *me) {

	MarkerManager *markerManager;
	QWidget       *rendererWidget;

	// NOTE: no need to call renderFrame, since the markers call this method themselves to guarantee
	// that the frame buffer contains the necessary information (i.e., per pixel surfel lists)

	markerManager = MarkerManager::getInstance();
	if (me->button() == QMouseEvent::LeftButton) {

		// **********************************
		// modify the fitting constraint list
		// **********************************

		rendererWidget = RendererManager::getInstance()->getMainRenderer()->getRendererWidget();
		selectedMarker = (Marker3D *)MarkerUtils::pickMarker (me->pos(), (QList<MarkerInterface> *)markerManager->getFittingConstraints3D(), rendererWidget);
		if (selectedMarker == 0) {
			MarkerUtils::deselectAll ((QList<MarkerInterface> *)markerManager->getFittingConstraints3D(), rendererWidget);
		}

	}

}

void ParameterizationTool::handleMouseMoveEvent (QMouseEvent *me) {

	QWidget *rendererWidget;

	if (me->state() == QMouseEvent::LeftButton) {

		if (selectedMarker != 0) {
			isDragging = true;
			// from now on this marker ignores external calls to 'togglePainting', e.g. when
			// the render widget updates its contents
			selectedMarker->setDragged (true);
			rendererWidget = RendererManager::getInstance()->getMainRenderer()->getRendererWidget();
			keepMarker = selectedMarker->moveTo (me->pos(), rendererWidget);
		}

	}

}

void ParameterizationTool::handleMouseReleaseEvent (QMouseEvent *me) {

	Marker3D        *marker;
	SurfelInterface *surfel;
	QWidget         *rendererWidget;
	MarkerManager   *markerManager;
	
	rendererWidget = RendererManager::getInstance()->getMainRenderer()->getRendererWidget();
	markerManager  = MarkerManager::getInstance();
	if (me->button() == QMouseEvent::LeftButton) {

		// **********************************
		// modify the fitting constraint list
		// **********************************

		if (selectedMarker == 0 && isDragging == false) {

			// *****************
			// create new marker
			// *****************

			surfel = MarkerUtils::getClosestSurfel (me->pos());
			if (surfel != 0) {

				marker = new Marker3D (surfel, true, markerManager->getFittingConstraints3D()->count() + 1,
									   MarkerInterface::FITTING_CONSTRAINT_STYLE);

				markerManager->addFittingConstraint3D (marker);
				marker->togglePainting (rendererWidget);

			}

		}
		else if (selectedMarker != 0 && isDragging == false) {

			// ****************
			// toggle selection
			// ****************

			MarkerUtils::toggleSelection (selectedMarker, rendererWidget);

		}
		else {

			// ********************
			// check dragged marker
			// ********************

			if (keepMarker == false) {
				markerManager->removeFittingConstraint3D (selectedMarker);
			}
			else {
				selectedMarker->setDragged (false);
			}
			isDragging = false;

		}

	}
	else if (me->button() == QMouseEvent::RightButton) {
		MarkerUtils::deselectAll ((QList<MarkerInterface> *)markerManager->getFittingConstraints3D(), rendererWidget);
	}

}

void ParameterizationTool::handleKeyPressEvent (QKeyEvent *ke) {
	ke->ignore();		
}

void ParameterizationTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	ke->ignore();		
}

void ParameterizationTool::selectTool() {

	MarkerManager              *markerManager;
	ParameterizationDialogImpl *parameterizationDialog;

	
	parameterizationDialog = ParameterizationDialogImpl::getInstance (this);

	// this method of restoring a QWidget's position is according to the Qt documentation:
	// "This method works on both MS-Windows and most existing X11 window managers"
	parameterizationDialog->resize (parameterizationDialogSize);
	parameterizationDialog->move (parameterizationDialogPosition);
	parameterizationDialog->show();

	markerManager = MarkerManager::getInstance();
	markerManager->toggleAllFittingConstraint3DVisualization();

}

void ParameterizationTool::deselectTool() {

	MarkerManager              *markerManager;
	ParameterizationDialogImpl *parameterizationDialog;

	parameterizationDialog = ParameterizationDialogImpl::getInstance (this);

	// store current position and size of the dialog, as to restore it for the next time the dialog is opened
	parameterizationDialogPosition = parameterizationDialog->pos();
	parameterizationDialogSize     = parameterizationDialog->size();

	ParameterizationDialogImpl::destroyInstance();

	markerManager = MarkerManager::getInstance();
	markerManager->toggleAllFittingConstraint3DVisualization();

}

// ************
// public slots
// ************

void ParameterizationTool::handleRendererWidgetRepainted() {

	MarkerManager *markerManager;

	markerManager = MarkerManager::getInstance();
	markerManager->toggleAllFittingConstraint3DVisualization();
	
}

// ***************
// private methods
// ***************

bool ParameterizationTool::checkToolAvailability() const {
	return RendererManager::getInstance()->getMainRenderer()->isInteractive() &&
		   (Scene::getInstance()->getNofObjects() > 0);
}

// ****************
// access functions
// ****************

ToolInterface *createTool() {
	return new ParameterizationTool();
}

const char *getToolName() {
	return "Parameterization Tool";
}




// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
