// Title:   EraserTool.h
// Created: Thu Sep 25 14:55:26 2003
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

#ifndef __ERASERTOOL_H_
#define __ERASERTOOL_H_

#include <qwidget.h>
#include <qtoolbutton.h>
#include <qpoint.h>
#include <qpointarray.h>
#include <qcursor.h>
#include <qobject.h>
#include <qimage.h>


#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../../../../DataTypes/src/MyDataTypes.h"
#include "../../../../DataTypes/src/Vector3D.h"
#include "../../../../Utilities/src/Matrix.h"

#include "../../../../Core/RendererManager/src/RendererManager.h"
#include "../../../../Core/Transformer/src/Transformer.h"
#include "../../../../Core/OrthoMapper/src/OrthoMapper.h"
#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../UI/EraserConfigurationWidget.h"

/**
 * This class implements a tool with which the user can draw with eraseres onto
 * a 3D-object. If a selection is active, only the selected area is affected.
 *
 * @author Mark Pauly
 * @version 1.2
 */
class EraserTool : public ToolInterface {

	Q_OBJECT

public:

	TOOL_API EraserTool();
	TOOL_API virtual ~EraserTool();

	// ***********************
	// interface ToolInterface
	// ***********************

	TOOL_API QToolButton *addButtonToToolBar (QToolBar *toolBar);	
	TOOL_API QCursor *getToolCursor() const;
	TOOL_API QToolButton *getToolButton();
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

	// the last position of the mouse before the
	// current mouse drag event
	QPoint                   lastMouseDragPosition;
	QCursor                  *toolCursor;
	QCursor                  *standardCursor;
	QToolButton              *eraserButton;	

	bool                     isSelectionActive;
	bool                     currentPositionValid;

	// the configuration widget
	EraserConfigurationWidget *configurationWidget;

	OrthoMapper              *orthoMapper;
	Vector2D                 screenPosition;
	

	void removeSurfels();
	bool checkToolAvailability() const;

private slots:

	void handleSceneMoved();
	void handleViewPortSizeChanged ();


};

// ****************
// access functions
// ****************

// extern "C" is needed so those functions can be accessed
// with GetProcAddress() (WIN32) and dlsym() (UNIX) respective
extern "C" {
	TOOL_API ToolInterface *createTool();
	TOOL_API void destroyTool (ToolInterface *tool);
	TOOL_API const char *getToolName();
}

#endif  // __ERASERTOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
