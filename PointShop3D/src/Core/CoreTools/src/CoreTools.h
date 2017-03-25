// Title:   CoreTools.h
// Created: Thu Sep 25 14:30:19 2003
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

#ifndef __CORETOOLS_H_
#define __CORETOOLS_H_

#include "../../src/CoreDLL.h"
#include "../ColorPickerTool/src/ColorPickerTool.h"
#include "../SelectionTool/src/SelectionTool.h"

/**
 * This class gives access to the core tools such as the <code>SelectionTool</code>
 * which may be used from within other <code>ToolInterface</code>s or <code>PluginInterface</code>s.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class CoreTools {

public:

	/**
	 * Creates an instance of this <code>CoreTools</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>CoreTools</code>
	 */
	CORE_API static CoreTools *getInstance();

	/**
	 * Destroys the instance of this <code>CoreTools</code>, if there is any. Does
	 * <em>not</em> <code>delete</code> the registered core tools.
	 */
	CORE_API static void destroyInstance();

	// **********
	// core tools
	// **********

	/**
	 * Registers the <code>SelectionTool</code>.
	 *
	 * @param selectionTool
	 *        a pointer to the <code>SelectionTool</code> to be registered
	 * @see SelectionTool
	 */
	CORE_API void setSelectionTool (SelectionTool *selectionTool);
	
	/**
	 * Gives access to the <code>SelectionTool</code>.
	 *
	 * @return a pointer to the <code>SelectionTool</code>
	 * @see SelectionTool
	 */
	CORE_API SelectionTool *getSelectionTool();

	/**
	 * Registers the <code>ColorPickerTool</code>.
	 *
	 * @param colorPickerTool
	 *        a pointer to the <code>ColorPickerTool</code> to be registered
	 * @see ColorPickerTool
	 */
	CORE_API void setColorPickerTool (ColorPickerTool *colorPickerTool);

	/**
	 * Gives access to the <code>ColorPickerTool</code>.
	 *
	 * @return a pointer to the <code>ColorPickerTool</code>
	 * @see ColorPickerTool
	 */
	CORE_API ColorPickerTool *getColorPickerTool();

private:
	
	ColorPickerTool *colorPickerTool;
	SelectionTool   *selectionTool;


	// singleton pattern
	CoreTools();
	virtual ~CoreTools();

};

#endif  // __CORETOOLS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
