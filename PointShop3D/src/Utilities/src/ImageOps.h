// Title:   ImageOps.h
// Created: Thu Sep 25 15:01:46 2003
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

#ifndef __IMAGEOPS_H_
#define __IMAGEOPS_H_

#include <qimage.h>

#include "UtilitiesDLL.h"

/**
 * This namespace provides some image operation functions.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace ImageOps {

	/**
	 * Copies the values of the 8 bit per pixel <code>grayscale8</code>, which must be a grayscale image,
	 * into the alpha channel of the 32 bit per pixel <code>image32</code>. The newly combined image is
	 * returned and must be <code>delete</code>d after use.
	 *
	 * @param image32
	 *        a pointer to a 32 bit per pixel <code>QImage</code>
	 * @param grayscale8
	 *        a pointer to a grayscale 8 bit per pixel <code>QImage</code>
	 * @return a pointer to a 32 bit per pixel <code>QImage</code>; must be <code>delete</code>d after use
	 */
	UTILITIES_API QImage *combineImages (const QImage *image32, const QImage *grayscale8);

}

#endif  // __IMAGEOPS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
