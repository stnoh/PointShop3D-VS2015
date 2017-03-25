// Title:   SurfelGPRendererWidget.cpp
// Created: Thu Sep 25 14:51:16 2003
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

#include <qpainter.h>
#include <qtimer.h>

#include "../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../Core/Configuration/src/Configuration.h"
#include "../../../Core/CoreTools/src/CoreTools.h"
#include "../../../Core/src/DefaultMouseHandler.h"
#include "SurfelGPRendererWidget.h"

SurfelGPRendererWidget::SurfelGPRendererWidget (const FrameBufferInterface *frameBuffer, QWidget *parent, const char *name) 
                                               : QWidget (parent, name) {

	this->frameBuffer = frameBuffer;

	// disable clearing the background before each paint event
	this->setBackgroundMode (NoBackground);

	resizeTimer = new QTimer (this);
    this->connect (resizeTimer, SIGNAL (timeout()),
                   this, SLOT (handleResizeTimerDone()));
	isResizing = false;

}

SurfelGPRendererWidget::~SurfelGPRendererWidget() {}

void SurfelGPRendererWidget::setFrameBuffer (const FrameBufferInterface *newFrameBuffer) {
	frameBuffer = newFrameBuffer;
}

const FrameBufferInterface *SurfelGPRendererWidget::getFrameBuffer() const {
	return frameBuffer;
}

QSize SurfelGPRendererWidget::sizeHint() const {
	return Configuration::getInstance()->getViewPortSize();
}

QSize SurfelGPRendererWidget::minimumSizeHint() const {
	return Configuration::getInstance()->getMinimumViewPortSize();
}

QSizePolicy SurfelGPRendererWidget::sizePolicy() const {
	return QSizePolicy (QSizePolicy::Preferred , QSizePolicy::Preferred);
}

void SurfelGPRendererWidget::mousePressEvent (QMouseEvent *me) {	
	DefaultMouseHandler::handleMousePressEvent (me);
}

void SurfelGPRendererWidget::mouseMoveEvent (QMouseEvent *me) {
	DefaultMouseHandler::handleMouseMoveEvent (me);
}

void SurfelGPRendererWidget::mouseReleaseEvent (QMouseEvent *me) {
	DefaultMouseHandler::handleMouseReleaseEvent (me);
}

void SurfelGPRendererWidget::keyPressEvent (QKeyEvent *ke) {
	ke->ignore();
}

void SurfelGPRendererWidget::wheelEvent (QWheelEvent *we) {

	we->accept();
	if (we->state() & QWheelEvent::ControlButton) {
		Scene::getInstance()->getActiveObject()->translate (0.0f, 0.0f, we->delta());
	}
	else {
		Scene::getInstance()->translate (0.0f, 0.0f, we->delta());
	}
	
}

void SurfelGPRendererWidget::paintEvent (QPaintEvent *) {

	QPainter painter;

	if (isResizing == true) {
		// nothing to do, resized image is already drawn in 'resizeEvent'
		return;
	}
	painter.begin (this);
	painter.drawImage (QPoint (0, 0), *(frameBuffer->getImage()));	
	painter.end();
	emit widgetRepainted();

}

void SurfelGPRendererWidget::resizeEvent (QResizeEvent *re) {

	QImage       resizedImage;
	const QImage *originalImage;
	QPainter     painter;
		   
	qDebug ("SurfelGPRendererWidget: resizeEvent: RECEIVED");
	// we do not re-render the image yet (expensive)...
	resizeTimer->start (250, true);
	isResizing = true;

	// ..instead we go for the cheap solution, just scale the rendered image
	originalImage = frameBuffer->getImage();
	resizedImage = originalImage->smoothScale (re->size().width(), re->size().height());

	painter.begin (this);
	painter.drawImage (QPoint (0, 0), resizedImage);	
	painter.end();
	emit widgetRepainted();
	
}

void SurfelGPRendererWidget::handleResizeTimerDone() {	

	isResizing = false;
	// this finally triggers the re-rendering of the image
	Configuration::getInstance()->setViewPortSize (this->size());	

}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
