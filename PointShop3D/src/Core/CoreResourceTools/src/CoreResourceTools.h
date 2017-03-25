// Title:   CoreResourceTools.h
// Created: Thu Sep 25 14:31:03 2003
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

#ifndef __CORERESOURCETOOLS_H_
#define __CORERESOURCETOOLS_H_

#include "../../CoreTools/ColorPickerTool/src/ColorPickerTool.h"
#include "../BrushChooserTool/src/BrushChooserTool.h"
#include "../ColorChooserTool/src/ColorChooserTool.h"
#include "../BitmapManagerTool/src/BitmapManagerTool.h"
#include "../../src/CoreDLL.h"

/**
 * This class gives access to the core resource tools such as the <code>BitmapResourceTool</code>
 * which may be used from within other tools or plugins.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class CoreResourceTools {

public:

	/**
	 * Creates an instance of this <code>CoreResourceTools</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>CoreResourceTools</code>
	 */
	CORE_API static CoreResourceTools *getInstance();

	/**
	 * Destroys the instance of this <code>CoreResourceTools</code>, if there is any. Does
	 * <em>not</em> <code>delete</code> the registered core tools.
	 */
	CORE_API static void destroyInstance();

	// **************
	// resource tools
	// **************

	/**
	 * Registers a <code>BrushChooserTool</code>.
	 *
	 * @param brushChooserTool
	 *        a pointer to a <code>BrushChooserTool</code> to be registered
	 */
	CORE_API void setBrushChooserTool (BrushChooserTool *brushChooserTool);

	/**
	 * Gives access to a <code>BrushChooserTool</code>.
	 *
	 * @return a pointer to a <code>BrushChooserTool</code>.
	 */
	CORE_API BrushChooserTool *getBrushChooserTool();

	/**
	 * Registers the <code>ColorChooserTool</code>.
	 *
	 * @param colorChooserTool
	 *        a pointer to the <code>ColorChooserTool</code> to be registered
	 * @see ColorChooserTool
	 */
	CORE_API void setColorChooserTool (ColorChooserTool *colorChooserTool);

	/**
	 * Gives access to the <code>ColorChooserTool</code>.
	 *
	 * @return a pointer to the <code>ColorChooserTool</code>
	 * @see ColorChooserTool
	 */
	CORE_API ColorChooserTool *getColorChooserTool();

	/**
	 * Registers the <code>BitmapManagerTool</code>.
	 *
	 * @param bitmapManagerTool
	 *        a pointer to the <code>BitmapManagerTool</code> to be registered
	 * @see BitmapManagerTool
	 */
	CORE_API void setBitmapManagerTool (BitmapManagerTool *bitmapManagerTool);

	/**
	 * Gives access to the <code>BitmapManagerTool</code>.
	 *
	 * @return a pointer to the <code>BitmapManagerTool</code>
	 * @see BitmapManagerTool
	 */
	CORE_API BitmapManagerTool *getBitmapManagerTool();



private:
	
	ColorChooserTool  *colorChooserTool;
	BrushChooserTool  *brushChooserTool;
	BitmapManagerTool *bitmapManagerTool;

	// singleton pattern
	CoreResourceTools();
	virtual ~CoreResourceTools();

};

#endif  // __CORERESOURCETOOLS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
