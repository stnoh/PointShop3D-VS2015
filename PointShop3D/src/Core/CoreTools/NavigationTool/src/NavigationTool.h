// Title:   NavigationTool.h
// Created: Thu Sep 25 14:30:42 2003
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

#ifndef __NAVIGATIONTOOL_H_
#define __NAVIGATIONTOOL_H_

#include <qwidget.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qpoint.h>
#include <qcursor.h>
#include <qobject.h>
#include <qpopupmenu.h>

#include "../../../Interfaces/src/ToolInterface.h"
#include "../UI/NavigationConfigurationWidget.h"

/**
 * This class implements a navigation tool which allows the user to
 * translate and rotate the scene with the mouse.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class NavigationTool : public ToolInterface {

	Q_OBJECT

public:

	TOOL_API NavigationTool();
	TOOL_API virtual ~NavigationTool();

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
	TOOL_API void handleKeyReleaseEvent   (QKeyEvent   *ke);

	TOOL_API void selectTool();
	TOOL_API void deselectTool();

	void setGlobalTransformationActive(bool isOn);

public slots:

	void handleRendererWidgetRepainted();

private:

	// the last position of the mouse before the
	// current mouse drag event
	QPoint                        lastMouseDragPosition;
	QCursor                       *toolCursor;
	QIconSet                      navigationIconSet,
                                  builderIconSet;
	QToolButton                   *toolButton;
	QPopupMenu                    navigationMenu;
	int                           navigationID,
	                              builderID,
					              currentTool;
	NavigationConfigurationWidget *configurationWidget;
	bool                          usePreviewRenderer;

	bool checkToolAvailability() const;

private slots:

	void handleToolSelected (int id);
	void handlePreviewRendererToggled (bool isOn);

};

#endif  // __NAVIGATIONTOOL_H_





// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
