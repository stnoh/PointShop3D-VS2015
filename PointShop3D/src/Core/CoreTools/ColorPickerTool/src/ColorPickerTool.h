// Title:   ColorPickerTool.h
// Created: Thu Sep 25 14:30:45 2003
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

#ifndef __COLORPICKERTOOL_H_
#define __COLORPICKERTOOL_H_

#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qcursor.h>
#include <qevent.h>
#include <qwidget.h>
#include <qobject.h>
#include <qpixmap.h>

// hack around MSVC 6.0 compiler bug (release mode only)
#if defined (_WIN32) && defined (NDEBUG)
// the include path as to be added in _every_ project which depends on this header file
#include <ToolInterface.h>
#else
#include "../../../Interfaces/src/ToolInterface.h"
#endif

#include "../../../CoreResourceTools/ColorChooserTool/src/ColorChooserTool.h"

/**
 * This class implements a tool with which allows the user to pick the
 * foreground and background color in the rendered image. The <code>ColorChooserTool</code>
 * is updated accordingly.
 *
 * @see ColorChooserTool
 * @author Oliver Knoll
 * @version 1.2
 */
class ColorPickerTool : public ToolInterface {

	Q_OBJECT

public:
	
	TOOL_API ColorPickerTool();
	TOOL_API virtual ~ColorPickerTool();

	/**
	 * Makes the <code>ColorChooserTool</code> known to this <code>ColorPickerTool</code>
	 * so it can be updated according to the picked color.
	 *
	 * @param colorChooserTool
	 *        a pointer to the <code>ColorChooserTool</code> which gets updated each time
	 *        a new color has been picked; may be 0, in which case the <code>ColorChooserTool</code>
	 *        is not updated
	 */ 
	TOOL_API void setupColorChooserTool (ColorChooserTool *colorChooserTool);

	/**
	 * Picks a foreground (FG) color at mouse (screen) coordinates <code>mouseX</code>
	 * and <code>mouseY</code>.
	 *
	 * @param mouseX
	 *        the x-coordinate of the mouse at which the color is to be picked
	 * @param mouseY
	 *        the y-coordinate of the mouse at which the color is to be picked
	 * @return the picked QRgb color
	 */
	TOOL_API QRgb pickFGColor (const int mouseX, const int mouseY);

	/**
	 * Picks a background (bg) color at mouse (screen) coordinates <code>mouseX</code>
	 * and <code>mouseY</code>.
	 *
	 * @param mouseX
	 *        the x-coordinate of the mouse at which the color is to be picked
	 * @param mouseY
	 *        the y-coordinate of the mouse at which the color is to be picked
	 * @return the picked QRgb color
	 */
	TOOL_API QRgb pickBGColor (const int mouseX, const int mouseY);
	
	// ***********************
	// interface ToolInterface
	// ***********************

	/**
	 * Creates a tool button and adds it to the <code>toolBar</code>. The <code>QToolButton</code>
	 * may also activate <code>QPopupMenu</code>s, for example.
	 *
	 * @param toolBar
	 *        a pointer to the <code>QToolBar</code> to which this tool has to be added
	 * @return   a pointer to the <code>QToolButton</code> which was created and added
	 *           to the <code>toolBar</code>
	 */
	TOOL_API QToolButton *addButtonToToolBar (QToolBar *toolBar);

	/**
	 * Returns the <code>QToolButton</code> of this <code>ToolInterface</code>, after it has been created
	 * with <code>addButtonToToolBar</code>. Useful for enabling/disabling a tool by enabling/disabling
	 * its tool button.
	 *
	 * @return a pointer to a <code>QToolButton</code> which has been created by <code>addButtonToToolBar</code>
	 * @see #addButtonToToolBar
	 */
	TOOL_API QToolButton *getToolButton();
	
	/**
	 * Returns the tool cursor which is to be displayed as soon as the mouse is over
	 * the renderer area.
	 *
	 * @return the <code>QCursor</code> of this <code>ToolInterface</code>
	 */
	TOOL_API QCursor *getToolCursor() const;
	
	/**
	 * Returns a <code>QWidget</code> with control elements which control the settins of this
	 * <code>ToolInterface</code>. This widget is then added to a toplevel tool
	 * configuration dialog. The <code>QWidget</code> is <code>delete</code> as soon as
	 * this <code>ToolInterface</code> is <code>delete</code>d, that is, there is no need
	 * to <code>delete</code> the <code>QWidget</code> after use.
	 *
	 * @return a <code>QWidget</code> with control elements; may be 0
	 */ 
	TOOL_API QWidget *getToolConfigurationWidget() const;

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
	TOOL_API bool isAvailable() const;

	/**
	 * Handles mouse press events which occured when the mouse was over the
	 * renderer area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	TOOL_API void handleMousePressEvent (QMouseEvent *event);

	/**
	 * Handles mouse release events which occured when the mouse was over the
	 * render area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	TOOL_API void handleMouseReleaseEvent (QMouseEvent *event);

	/**
	 * Handles mouse move events which occured when the mouse was over the
	 * render area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	TOOL_API void handleMouseMoveEvent (QMouseEvent *event);
	
	/**
	 * Handles key press events which are not handled by the application
	 * itself.
	 *
	 * @param event
	 *        the specific <code>QKeyEvent</code>
	 */
	TOOL_API void handleKeyPressEvent (QKeyEvent *event);

	/**
	 * Handles key release events which are not handled by the application
	 * itself.
	 *
	 * @param event
	 *        the specific <code>QKeyEvent</code>
	 */
	TOOL_API void handleKeyReleaseEvent (QKeyEvent *event);

	/**
	 * This method is called whenever the tool is selected.
	 */
	TOOL_API void selectTool();

	/**
	 * This method is called whenever the tool is deselected.
	 */
	TOOL_API void deselectTool();

public slots:

	void handleRendererWidgetRepainted();

private:

	QCursor          *toolCursor;
	QToolButton      *toolButton;
	ColorChooserTool *colorChooserTool;   // a reference to the color chooser tool
	                                      // so we can update the current chosen color
	                                      // according to the picked color
	
	
	// picks a color at mouse (screen) coordinates mouseX and mouseY
	QRgb pickColor (const int mouseX, const int mouseY);

	bool checkToolAvailability() const;
	
};

#endif  // __COLORPICKERTOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
