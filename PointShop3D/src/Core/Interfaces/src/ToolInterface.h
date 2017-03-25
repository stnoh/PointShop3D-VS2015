// Title:   ToolInterface.h
// Created: Thu Sep 25 14:28:59 2003
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

#ifndef __TOOLINTERFACE_H_
#define __TOOLINTERFACE_H_

#include <qobject.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qtoolbar.h>
#include <qcursor.h>
#include <qwidget.h>

#include "../../../DataTypes/src/Vector3D.h"
#include "../../Scene/src/Scene.h"
#include "../../Configuration/src/Configuration.h"
#include "../../../Utilities/src/TrackBall.h"


// ***********
// definitions
// ***********

#if defined _WIN32

	// The following ifdef block is the standard way of creating macros which make exporting
	// from a DLL simpler. All files within this DLL are compiled with the TOOL_EXPORTS
	// symbol defined on the command line. this symbol should not be defined on any project
	// that uses this DLL. This way any other project whose source files include this file see
	// TOOL_API  functions as being imported from a DLL, wheras this DLL sees symbols
	// defined with this macro as being exported.
	#ifdef TOOL_EXPORTS
		#define TOOL_API __declspec(dllexport)
	#else
		#define TOOL_API __declspec(dllimport)
	#endif

#else

	// for all other platforms TOOL_API is defined to be "nothing"
	#define TOOL_API
	
#endif

/**
 * This interface defines the API for tools, that is, modules which appear
 * in the standard toolbar.
 * They are informed as soon as they are selected and deselected respective by
 * the methods <code>selectTool</code> and <code>deselectTool</code>. The
 * currently selected tool also receives any mouse- and key-events which are not
 * handled by the application itself.
 * 
 * If the tool doesn't want to modify the image buffer or the 3D-model but merely
 * provide resources such as brushes or textures, it implements the <code>ResourceToolInterface</code>
 * instead, which has much less methods.
 * 
 * Note that apart from this <code>ToolInterface</code> the tool modules
 * must also implement some special C-style functions, if they are to be
 * dynamically loaded:
 * <ul>
 *   <li><code>TOOL_API ToolInterface *createTool();</code></li>
 *   <li><code>TOOL_API const char *getToolName();</code></li>
 * </ul>
 * See the <code>TemplateTool</code> for details.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see ResourceToolInterface
 * @see TemplateTool
 */
class TOOL_API ToolInterface : public QObject {

	Q_OBJECT

public:

	/**
	 * initializes the tool.
	 * Should be called by deriving class
	 */
	ToolInterface() {
		globalTransform = false;
	}

	/**
	 * Creates a tool button and adds it to the <code>toolBar</code>. The <code>QToolButton</code>
	 * may also activate <code>QPopupMenu</code>s, for example.
	 *
	 * @param toolBar
	 *        a pointer to the <code>QToolBar</code> to which this tool has to be added
	 * @return   a pointer to the <code>QToolButton</code> which was created and added
	 *           to the <code>toolBar</code>
	 */
	virtual QToolButton *addButtonToToolBar (QToolBar *toolBar) = 0;

	/**
	 * Returns the <code>QToolButton</code> of this <code>ToolInterface</code>, after it has been created
	 * with <code>addButtonToToolBar</code>. Useful for enabling/disabling a tool by enabling/disabling
	 * its tool button.
	 *
	 * @return a pointer to a <code>QToolButton</code> which has been created by <code>addButtonToToolBar</code>
	 * @see #addButtonToToolBar
	 */
	virtual QToolButton *getToolButton() = 0;
	
	/**
	 * Returns the tool cursor which is to be displayed as soon as the mouse is over
	 * the renderer area.
	 *
	 * @return the <code>QCursor</code> of this <code>ToolInterface</code>
	 */
	virtual QCursor *getToolCursor() const = 0;
	
	/**
	 * Returns a <code>QWidget</code> with control elements which control the settins of this
	 * <code>ToolInterface</code>. This widget is then added to a toplevel tool
	 * configuration dialog. The <code>QWidget</code> is <code>delete</code> as soon as
	 * this <code>ToolInterface</code> is <code>delete</code>d, that is, there is no need
	 * to <code>delete</code> the <code>QWidget</code> after use.
	 *
	 * @return a <code>QWidget</code> with control elements; may be 0
	 */ 
	virtual QWidget *getToolConfigurationWidget() const = 0;

	/**
	 * Returns <code>true</code> if this <code>ToolInterface</code> is selectable, <code>false</code>
	 * if any conditions this <code>ToolInterface</code> requires are not met. Conditions for
	 * the availability of a <code>ToolInterface</code> might include:
	 * <ul>
	 *   <li>an <code>Object</code> in the <code>Scene</code> is available</li>
	 *   <li>the <em>main</em> <code>RendererInterface</code> is an <em>interactive</em> renderer</li>
	 *   <li>a <code>Brush</code> is available</li>
	 * </ul>
	 *
	 * @return <code>true</code> if this <code>ToolInterface</code> is available; <code>false</code> else
	 */
	virtual bool isAvailable() const = 0;

	/**
	 * Handles mouse press events which occured when the mouse was over the
	 * renderer area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	virtual void handleMousePressEvent (QMouseEvent *event);

	/**
	 * Handles mouse release events which occured when the mouse was over the
	 * render area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	virtual void handleMouseReleaseEvent (QMouseEvent *event) = 0;

	/**
	 * Handles mouse move events which occured when the mouse was over the
	 * render area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	virtual void handleMouseMoveEvent (QMouseEvent *event) = 0;
	
	/**
	 * Handles key press events which are not handled by the application
	 * itself.
	 *
	 * @param event
	 *        the specific <code>QKeyEvent</code>
	 */
	virtual void handleKeyPressEvent (QKeyEvent *event);

	/**
	 * Handles key release events which are not handled by the application
	 * itself.
	 *
	 * @param event
	 *        the specific <code>QKeyEvent</code>
	 */
	virtual void handleKeyReleaseEvent (QKeyEvent *event) = 0;

	/**
	 * This method is called whenever the tool is selected.
	 */
	virtual void selectTool() = 0;

	/**
	 * This method is called whenever the tool is deselected.
	 */
	virtual void deselectTool() = 0;

	/**
	 * Sets the global transformation mode. If the global transformation mode is active, the tool
	 * behaves similar to the navigation tool 
	 *
	 * @param isOn
	 *		true if the global transformation mode should be used.
	 * @see #isGlobalTransformationActive
	 */
	virtual void setGlobalTransformationActive(bool isOn);

	/**
	 * This method returns true if the object is globally transformed, false otherwise
	 * @see #setGlobalTransformationActive
	 */
	inline bool isGlobalTransformationActive() {
		return globalTransform;
	}

protected:
	bool	globalTransform;

private:
	QPoint	lastMouseDragPosition;

public slots:

	/**
	 * This method is called whenever the <code>RendererInterface</code> has repainted its
	 * renderer widget. The current tool may then redraw its contents with a <code>QPainter</code> into
	 * the <code>QWidget</code>.
	 *
	 * <b>Note:</b> This signal is <em>only</em> sent to this <code>ToolInterface</code> when
	 *              the tool is <em>selected</em>.
	 *
	 * @see RendererInterface#getRendererWidget
	 */
	virtual void handleRendererWidgetRepainted() = 0;

};

#endif  // __TOOLINTERFACE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
