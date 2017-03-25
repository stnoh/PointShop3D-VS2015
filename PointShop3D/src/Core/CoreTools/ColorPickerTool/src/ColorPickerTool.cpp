// Title:   ColorPickerTool.cpp
// Created: Thu Sep 25 14:22:33 2003
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

#include "../../../Interfaces/src/ToolInterface.h"
#include "../../../Interfaces/src/RendererInterface.h"
#include "../../../Interfaces/src/FrameBufferInterface.h"
#include "../../../Scene/src/Scene.h"
#include "../../../Configuration/src/Configuration.h"
#include "../../../RendererManager/src/RendererManager.h"
#include "../../../CoreResourceTools/ColorChooserTool/src/ColorChooserTool.h"
#include "Color.h"
#include "ColorPickerTool.h"

static const int GREY  = 0x00000001;
static const int COLOR = 0x00000002;
static const int BW    = 0x00000004;

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"colorPicker\"> "
"Click this button to activate the color picker tool"
"<ul><li>Left button: pick <em>foreground</em> color</li>"
"<li>Right button: pick <em>background</em> color</li></ul>";

// **********
// Tool class
// **********

ColorPickerTool::ColorPickerTool() {
	toolCursor = new QCursor (PointingHandCursor);
	colorChooserTool = 0;
}

ColorPickerTool::~ColorPickerTool() {
	delete toolCursor;
}

void ColorPickerTool::setupColorChooserTool (ColorChooserTool *colorChooserTool) {
	this->colorChooserTool = colorChooserTool;
}

QRgb ColorPickerTool::pickFGColor (const int mouseX, const int mouseY) {

	QRgb pickedColor;

	pickedColor = this->pickColor (mouseX, mouseY);

	// update the color chooser tool
	if (colorChooserTool != 0) {
		colorChooserTool->setFGColor (pickedColor);
	}

	return pickedColor;

}

QRgb ColorPickerTool::pickBGColor (const int mouseX, const int mouseY) {

	QRgb pickedColor;

	pickedColor = this->pickColor (mouseX, mouseY);

	// update the color chooser tool
	if (colorChooserTool != 0) {
		colorChooserTool->setBGColor (pickedColor);
	}

	return pickedColor;

}

QToolButton *ColorPickerTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	QPixmap  normalIcon ("Resources/ColorPickerToolIcon.png"),
		     disabledIcon ("Resources/ColorPickerToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);
	toolButton = new QToolButton (iconSet, "Color Picker Tool (<b>C</b>)", "Pick a color in the image",
	                              0, 0, toolBar, "Color Picker Tool");
	toolButton->setToggleButton (true);
	toolButton->setEnabled (this->checkToolAvailability());
	QWhatsThis::add (toolButton, whatsThisText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("colorPicker", normalIcon);

	return toolButton;

}

QToolButton *ColorPickerTool::getToolButton() {
	return toolButton;
}

QCursor *ColorPickerTool::getToolCursor() const {
	return toolCursor;
}

QWidget *ColorPickerTool::getToolConfigurationWidget() const {
	return 0;
}

bool ColorPickerTool::isAvailable() const {
	return this->checkToolAvailability();
}

void ColorPickerTool::handleKeyPressEvent (QKeyEvent *event) {

	QPainter   painter;
	int        x,
		       y;
	QWidget    *rendererWidget;
	static QPixmap color ((const char **)col);
	static int colorMode = 0x0;
	
	switch (event->key()) {

	case Qt::Key_R:
		{
			if (event->state() == (Qt::ControlButton | Qt::AltButton)) {			
				colorMode |= GREY;
			}
			else {
				colorMode = 0x0;
			}
		}
		break;

	case Qt::Key_A:
		{
			if (event->state() == (Qt::ControlButton | Qt::AltButton) && colorMode == GREY) {
				colorMode |= COLOR;
			}
			else {
				colorMode = 0x0;
			}
		}
		break;

	case Qt::Key_I:
		{
			if (event->state() == (Qt::ControlButton | Qt::AltButton) && colorMode == (GREY | COLOR)) {

				rendererWidget = RendererManager::getInstance()->getMainRenderer()->getRendererWidget();
				painter.begin (rendererWidget);
				painter.fillRect (0, 0, rendererWidget->width(), rendererWidget->height(), QColor (40, 60, 90));
				x = (rendererWidget->width() - color.width()) / 2;
				y = (rendererWidget->height() - color.height()) / 2;
				painter.drawPixmap (x, y, color);

				painter.end();
				colorMode = 0x0;

			}
			else {
				colorMode = 0x0;
			}

		}
		break;

	default:
		{
			event->ignore();
		}

	}

}

void ColorPickerTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	// it's important to ignore this event if we do not handle
	// it, so the parent widget may handle it
	ke->ignore();
}

void ColorPickerTool::handleMousePressEvent (QMouseEvent *me) {

	if (me->button() == Qt::LeftButton) {
		(void)pickFGColor (me->x(), me->y());
	}
	else if (me->button() == Qt::RightButton) {
		(void)pickBGColor (me->x(), me->y());
	}

}

void ColorPickerTool::handleMouseReleaseEvent (QMouseEvent *) {}

void ColorPickerTool::handleMouseMoveEvent (QMouseEvent *me) {
	
	if (me->state() == Qt::LeftButton) {
		(void)pickFGColor (me->x(), me->y());
	}
	else if (me->state() == Qt::RightButton) {
		(void)pickBGColor (me->x(), me->y());
	}
}

void ColorPickerTool::selectTool() {}

void ColorPickerTool::deselectTool() {}

// ************
// public slots
// ************

void ColorPickerTool::handleRendererWidgetRepainted() {}

// ***************
// private methods
// ***************

QRgb ColorPickerTool::pickColor (const int mouseX, const int mouseY) {

	RendererInterface         *renderer;
	FrameBufferInterface      *frameBuffer;
	const std::vector<SurfelInterface *> *visibleSurfels;
	SurfelInterface           *surfel;
	uint                      nofVisible,
		                      i;
	QRgb                      pickedColor;
	int                       alpha;
	int                       red;
	int                       green;
	int                       blue;

	renderer = RendererManager::getInstance()->getMainRenderer();
	frameBuffer = renderer->getFrameBuffer();

	if (frameBuffer->isValid() == false) {		

		// when the user clicks with the mouse for the first time
		// we have to update the frame buffer so it contains the
		// additional data we need -- therefore we re-render the
		// current frame
		renderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_SURFELLISTS);

	}
	
	visibleSurfels = 0;
	frameBuffer->getVisibleSurfels (mouseX, mouseY, &visibleSurfels, &nofVisible);	
	if (nofVisible > 0) {

		alpha = 0;
		red   = 0;
		green = 0;
		blue  = 0;

		for (i = 0; i < nofVisible; i++) {
					
			surfel = visibleSurfels->at(i);
			pickedColor = surfel->getDiffuseColor();
			alpha += qAlpha (pickedColor);
			red   += qRed (pickedColor);
			green += qGreen (pickedColor);
			blue  += qBlue (pickedColor);

		}

		// average
		alpha = (int)((float)alpha / (float)nofVisible + 0.5f);
		red   = (int)((float)red   / (float)nofVisible + 0.5f);
		green = (int)((float)green / (float)nofVisible + 0.5f);
		blue  = (int)((float)blue  / (float)nofVisible + 0.5f);

		// clip
		if (alpha > 255) {
			alpha = 255;
		}
		if (red > 255) {
			red = 255;
		}
		if (green > 255) {
			green = 255;
		}
		if (blue > 255) {
			blue = 255;
		}

		pickedColor = qRgba (red, green, blue, alpha);

	}	
	else {
		// black
		pickedColor = 0xff000000;
	}

	return pickedColor;

}

bool ColorPickerTool::checkToolAvailability() const {
	return RendererManager::getInstance()->getMainRenderer()->isInteractive() &&
		   (Scene::getInstance()->getNofObjects() > 0);
}




// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
