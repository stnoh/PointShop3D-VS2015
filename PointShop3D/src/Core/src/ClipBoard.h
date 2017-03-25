//
// Title:   ClipBoard.h
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


#ifndef __CLIPBOARD_H_
#define __CLIPBOARD_H_

#include <qevent.h>

#include "../Scene/src/Object.h"
#include "CoreDLL.h"

/**
 * This <code>ClipBoard</code> provides methods for cutting, copying and pasting (selected parts of)
 * <code>Object</code>s. The <em>content</em> of this <code>ClipBoard</code> is also accessible.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ClipBoard {
 
public:

	/**
	 * Creates an instance of this <code>ClipBoard</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>ClipBoard</code>
	 */
	CORE_API static ClipBoard *getInstance();

	/**
	 * Destroys the instance of this <code>ClipBoard</code>, if there is any.
	 */
	CORE_API static void destroyInstance();

	/**
	 * Returns the current <em>content</em> of this <code>ClipBoard</code>, if
	 * there is any.
	 *
	 * @return a pointer to an <code>Object<code> (the <em>content</em>), if there is any; may be 0
	 */
	CORE_API Object *getContent() const;

	/**
	 * Clears the <em>content</em> of this <code>ClipBoard</code>.
	 */
	CORE_API void clear();

	/**
	 * Cuts the selected <code>SurfelCollection</code> and makes it the current <em>content</em>.
	 *
	 * @see SelectionTool
	 */
	CORE_API void cut();

	/**
	 * Cuts the <em>active</em> <code>Object</code> and makes it the current <em>content</em>.
	 *
	 * @see Scene#getActiveObject
	 */
	CORE_API void cutObject();


	/**
	 * Copies the selected <code>SurfelCollection</code> and makes it the current <em>content</em>.
	 *
	 * @see SelectionTool
	 */
	CORE_API void copy();

	/**
	 * Copies the <em>active</em> <code>Object</code> and makes it the current <em>content</em>.
	 *
	 * @see Scene#getActiveObject
	 */
	CORE_API void copyObject();

	/**
	 * Pastes the <em>content</em>, if there is any, into the <code>Scene</code>.
	 */
	CORE_API void paste();

private:

	Object *content;

	ClipBoard();
	virtual ~ClipBoard();

};

#endif  // __CLIPBOARD_H_
