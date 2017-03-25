//
// Title:   DefaultMouseHandler.h
// Created: Mon 22 Sep 2003
// Authors: Oliver Knoll
//
// Copyright (c) 2001, 2002, 2003, Computer Graphics Lab, ETH Zurich
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


#ifndef __DEFAULTMOUSEHANDLER_H_
#define __DEFAULTMOUSEHANDLER_H_

#include <qevent.h>

#include "CoreDLL.h"

/**
 * This namespace provides a default mouse handler for renderer <code>QWidget</code>s which simply
 * routes the <code>QMouseEvent</code>s to the currently selected <code>ToolInterface</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace DefaultMouseHandler {

	/**
	 * Sends the <code>event</code> to the current <code>ToolInterface</code>, if it is
	 * available.
	 *
	 * @param event
	 *        the <code>QMouseEvent</code> which was received by the <code>QWidget</code> of the current
	 *        <code>RendererInterface</code>
	 * @see ToolManager#getCurrentTool
	 * @see ToolInterface#isAvailable
	 */
	CORE_API void handleMousePressEvent (QMouseEvent *event);

	/**
	 * Sends the <code>event</code> to the current <code>ToolInterface</code>, if it is
	 * available.
	 *
	 * @param event
	 *        the <code>QMouseEvent</code> which was received by the <code>QWidget</code> of the current
	 *        <code>RendererInterface</code>
	 * @see ToolManager#getCurrentTool
	 * @see ToolInterface#isAvailable
	 */
	CORE_API void handleMouseMoveEvent (QMouseEvent *event);

	/**
	 * Sends the <code>event</code> to the current <code>ToolInterface</code>, if it is
	 * available.
	 *
	 * @param event
	 *        the <code>QMouseEvent</code> which was received by the <code>QWidget</code> of the current
	 *        <code>RendererInterface</code>
	 * @see ToolManager#getCurrentTool
	 * @see ToolInterface#isAvailable
	 */
	CORE_API void handleMouseReleaseEvent (QMouseEvent *event);

	/**
	 * Translates the <em>active</em> <code>Object</code> in z-Direction, if the <em>Control</em> key was pressed,
	 * else the whole <code>Scene</code> is translated in z-Direction.
	 *
	 * @param event
	 *        the <code>QWheelEvent</code> which was received by the <code>QWidget</code> of the current
	 *        <code>RendererInterface</code>
	 * @see Scene#getActiveObject
	 */
	CORE_API void handleMouseWheelEvent (QWheelEvent *event);

};

#endif  // __DEFAULTMOUSEHANDLER_H_
