// Title:   TrackBall.h
// Created: Thu Sep 25 15:01:26 2003
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

#ifndef __TRACKBALL_H_
#define __TRACKBALL_H_

#include "UtilitiesDLL.h"

/**
 * This namespace provides a trackball function.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace TrackBall {

	/**
	 * Calculates the rotation angle and vector given the normalized last and current
	 * mouse positions in <code>pos</code> and stores the result in the
	 * array <code>rot</code> for a <em>left</em>-handed coordinate system (like in OpenGL).
	 *
	 * @param pos
	 *        stores the normalized mouse positions [in [-1.0, 1.0]; the 
	 *        first two elements store the previous mouse position, the last
	 *        two elements the current mouse position
	 * @param rot
	 *        the first element stores the angle, the last 3 elements
	 *        the vector around which the rotation is to be made in a <em>left</em>-handed system
	 */
	UTILITIES_API bool calculateRotationMatrix (const float pos[4], float rot[4]);

}


#endif // __TRACKBALL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
