// Title:   TemplateTool.h
// Created: Thu Sep 25 14:56:33 2003
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

#ifndef __TEMPLATETOOL_H_
#define __TEMPLATETOOL_H_

// **********************************************
// To compile plugins (win32):
// 
// define TOOL_EXPORTS on the command line
//
// in Visual Studio:
// Project->Settings->C/C++, Category Preprocessor
// add TOOL_EXPORTS to 'preprocessor definitions'
// ***********************************************

#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qcursor.h>
#include <qevent.h>
#include <qwidget.h>
#include <qobject.h>

#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../UI/TemplateConfigurationWidget.h"

/**
 * This class implements a template tool which may be used
 * to implement more plugins. It provides a configuration
 * widget, which specifies a text to be drawn into the renderer widget
 * at the position of the mouse click.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class TemplateTool : public ToolInterface {

	Q_OBJECT

public:
	
	TOOL_API TemplateTool();
	TOOL_API virtual ~TemplateTool();

	// ***********************
	// interface ToolInterface
	// ***********************

	TOOL_API QToolButton* addButtonToToolBar (QToolBar *toolBar);
	TOOL_API QToolButton *getToolButton();
	TOOL_API QCursor *getToolCursor() const;
	TOOL_API QWidget *getToolConfigurationWidget() const;
	TOOL_API bool isAvailable() const;
	
	TOOL_API void handleMousePressEvent   (QMouseEvent *me);
	TOOL_API void handleMouseReleaseEvent (QMouseEvent *me);
	TOOL_API void handleMouseMoveEvent    (QMouseEvent *me);
	TOOL_API void handleKeyPressEvent     (QKeyEvent   *ke);
	TOOL_API void handleKeyReleaseEvent   (QKeyEvent   *ke);

	TOOL_API void selectTool();
	TOOL_API void deselectTool();

public slots:

	void handleRendererWidgetRepainted();

private:

	QCursor                     *toolCursor;
	QToolButton                 *toolButton;
	TemplateConfigurationWidget *templateConfigurationWidget;

	bool checkToolAvailability() const;
	
};

// ****************
// access functions
// ****************

// extern "C" is needed so those functions can be accessed
// with GetProcAddress() (WIN32) and dlsym() (UNIX) respective
extern "C" {
	TOOL_API ToolInterface *createTool();
	TOOL_API const char *getToolName();
}

#endif  // __TEMPLATETOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
