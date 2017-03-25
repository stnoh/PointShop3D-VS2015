// Title:   LightTool.h
// Created: Thu Sep 25 14:55:50 2003
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

#ifndef __LIGHTTOOL_H_
#define __LIGHTTOOL_H_

#include <qwidget.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qpoint.h>
#include <qcursor.h>
#include <qobject.h>

#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../UI/LightConfigurationWidget.h"

/**
 * This class implements a light tool that allows to control 
 * the scene lighting using the mouse.
 *
 * @author Matthias Zwicker
 * @version 1.2
 */
class LightTool : public ToolInterface {

	Q_OBJECT

public:

	TOOL_API LightTool();
	TOOL_API virtual ~LightTool();

	// ***********************
	// interface ToolInterface
	// ***********************

	TOOL_API QToolButton *addButtonToToolBar (QToolBar *toolBar);
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

	QCursor                       *toolCursor;
	QToolButton                   *toolButton;
	bool                          usePreviewRenderer;
	LightConfigurationWidget      *configurationWidget;

	// this method also triggers a render frame signal, see implementation
	// (via Configuration::getInstance()->setLightPosition(direction);)
	void setLightDirection(const QPoint &p);
	bool checkToolAvailability() const;

private slots:

	void handlePreviewRendererToggled (bool isOn);

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

#endif  // __LIGHTTOOL_H_





// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
