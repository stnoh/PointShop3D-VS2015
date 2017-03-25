// Title:   TemplateTool.cpp
// Created: Thu Sep 25 14:56:40 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003, Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General
// Public License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330,
// Boston, MA 02111-1307 USA
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
#include <qcolor.h>
#include <qlineedit.h>

#include "../../../../Core/RendererManager/src/RendererManager.h"
#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../UI/TemplateConfigurationWidget.h"

#include "TemplateTool.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "<img source=\"template\"> "
"Click this button to activate the template tool"
"<ul><li>Enter here a short description of this tool</li>"
"<li>You may even use simple <i>HTML formated</i> text</li></ul>";

// **********
// Tool class
// **********

TemplateTool::TemplateTool() {
	toolCursor = new QCursor (PointingHandCursor);
	templateConfigurationWidget = new TemplateConfigurationWidget();
}

TemplateTool::~TemplateTool() {
	delete toolCursor;
	delete templateConfigurationWidget;
}

QToolButton *TemplateTool::addButtonToToolBar (QToolBar *toolBar) {

	QIconSet iconSet;
	// create a new icon set for the button
	// path must be relative to the executable (i.e. PointShop.exe)
	// all Icons, Cursors, Bitmaps etc. which are needed by this tool should
	// go into the subdirectory 'Resources'
	QPixmap  normalIcon ("Resources/TemplateToolIcon.png"),
		     disabledIcon ("Resources/TemplateToolIcon_disabled.png");

	iconSet.setPixmap (normalIcon, QIconSet::Automatic, QIconSet::Normal);
	iconSet.setPixmap (disabledIcon, QIconSet::Automatic, QIconSet::Disabled);

	// add the button to the toolbar, including 'tool tip text'
	// and 'status bar text'
	toolButton = new QToolButton (iconSet, "Tool tip text", "This is the status bar text",
	                              0, 0, toolBar, "Template Tool");

	// make it a toggle button
	toolButton->setToggleButton (true);

	// enable/disable it
	toolButton->setEnabled (this->checkToolAvailability());

	// add the 'What's this'-text to this button, a short description
	QWhatsThis::add (toolButton, whatsThisText);

	// make the icon visible in the 'What's this'-text, 'template' is the name of
	// the placeholder specified in the 'What's this'-text above
	QMimeSourceFactory::defaultFactory()->setPixmap ("template", normalIcon);

	return toolButton;

}

QToolButton *TemplateTool::getToolButton() {
	return toolButton;
}

QCursor *TemplateTool::getToolCursor() const {
	return toolCursor;
}

QWidget *TemplateTool::getToolConfigurationWidget() const {
	return templateConfigurationWidget;
}

bool TemplateTool::isAvailable() const {
	return this->checkToolAvailability();
}

void TemplateTool::handleKeyPressEvent (QKeyEvent *ke) {
	// it's important to ignore this event if we do not handle
	// it, so the parent widget may handle it
	ke->ignore();
}

void TemplateTool::handleKeyReleaseEvent (QKeyEvent *ke) {
	// it's important to ignore this event if we do not handle
	// it, so the parent widget may handle it
	ke->ignore();
}

void TemplateTool::handleMousePressEvent (QMouseEvent *me) {

	QPainter painter;		
	
	// we paint "PointShop" into the renderWidget if the left mouse
	// button was clicked
	if (me->button() == Qt::LeftButton) {

		painter.begin (RendererManager::getInstance()->getMainRenderer()->getRendererWidget());
		painter.setPen (Qt::white);
		painter.drawText (me->x(), me->y(), templateConfigurationWidget->textLineEdit->text());

		painter.end();

	}

}

void TemplateTool::handleMouseReleaseEvent (QMouseEvent *) {
	// handle mouse button release events,
	// me->button() gives the button which was released
}

void TemplateTool::handleMouseMoveEvent (QMouseEvent *) {
	// handle mouse moves events - this event is also received
	// if no mouse button is pressed. To query the mouse button
	// state, including modifiers such as Qt::ControlButton, use
	// me->state()
}

void TemplateTool::selectTool() {
	// this tool was just seleceted by the user:
	// aquire resources if necessary
}

void TemplateTool::deselectTool() {
	// the user selected a different tool now:
	// release resoures if necessary
}

// ************
// public slots
// ************

void TemplateTool::handleRendererWidgetRepainted() {
	// the RenderWidget has repainted its content, so here this tool
	// can draw its content with a QPainter, if necessary
}

// ***************
// private methods
// ***************

bool TemplateTool::checkToolAvailability() const {
	
	// TODO: check here any conditions, e.g.
	//       - if an interactive main renderer is present
	//       - if an Object is in the Scene
	//       - etc.
	return true;

}

// ****************
// access functions
// ****************

ToolInterface *createTool() {
	return new TemplateTool();
}

const char *getToolName() {
	return "Template";
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
