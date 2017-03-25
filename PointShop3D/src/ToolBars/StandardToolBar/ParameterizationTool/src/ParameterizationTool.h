// Title:   ParameterizationTool.h
// Created: Thu Sep 25 14:58:13 2003
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

#ifndef __PARAMETERIZATIONTOOL_H_
#define __PARAMETERIZATIONTOOL_H_

#include <qwidget.h>
#include <qtoolbutton.h>
#include <qpoint.h>
#include <qcursor.h>
#include <qobject.h>
#include <qstring.h>
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
#include <qintdict.h>

#include "../../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../../Core/Interfaces/src/ToolInterface.h"
#include "../../../../Core/Transformer/src/Transformer.h"
#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "../../../../Core/MarkerManager/src/Marker2D.h"
#include "../../../../Core/MarkerManager/src/Marker3D.h"
#include "../UI/ParameterizationConfigurationWidget.h"
#include "Parameterization.h"
#include "ParameterizationDialogImpl.h"

/**
 * This class implements a tool with which corresponding markers in the 3D-image and the 2D-bitmaps (textures)
 * can be set, as to generate a mapping between texture and object.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ParameterizationTool : public ToolInterface {

	Q_OBJECT

public:

	TOOL_API ParameterizationTool();
	TOOL_API virtual ~ParameterizationTool();

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

	// direct access for...
	friend class ParameterizationDialogImpl;
	friend class BitmapMarker;

	// the last position of the mouse before the
	// current mouse drag event
	QPoint                    lastMouseDragPosition;
	QCursor                   *toolCursor;
	QToolButton               *parameterizationButton;

	// the configuration widget
	ParameterizationConfigurationWidget *configurationWidget;

	Parameterization          *parameterization;

	Marker3D                  *selectedMarker;          // the 3D marker which was clicked

	bool                      isDragging,               // true, if we are dragging a marker with the mouse
	                          keepMarker;               // false, if the marker is dragged over area where
	                                                    // no splats are available
	        
    QPoint                    parameterizationDialogPosition;
    QSize                     parameterizationDialogSize;

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

#endif  // __PARAMETERIZATIONTOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
