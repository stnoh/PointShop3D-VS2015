// Title:   TrackBall.cpp
// Created: Thu Sep 25 15:01:29 2003
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

#include <math.h>

#ifdef WIN32
#include <float.h>
#endif

#include "TrackBall.h"

static const float ONE_OVER_SMALL_NUMBER = 1000000000.0f;

bool TrackBall::calculateRotationMatrix (const float pos[4], float rot[4]) {

	float dx, dy;
	float px, py;
	float r, z;
	float x0, y0, z0;
	float x1, y1, z1;
	float n, ax, ay, az;
	float length;
	
	x0 = pos[0]; y0 = pos[1];
	x1 = pos[2]; y1 = pos[3];

	dx = x0 - x1;
	dy = y0 - y1;


	
	// calculate rotation angle (always proportional to mouse movement)
	length = (float)sqrt (dx * dx + dy * dy);
	if (length <= -1.0) {
		n = -90.0f;
	}
	else if  (length >= 1.0) {
		n = 90.0f;
	}
	else {
		n = 90.0f * (float)asin (length);
	}

	// normalize direction of mouse movement (let's call it 'mouse ray')
	r   = 1.0f / (float)sqrt (dx * dx + dy * dy);
	dx *= r;
	dy *= r;

	// project [0,0] onto mouse ray and
	// calculate distance of mouse ray to [0,0]
	r  = -(x1 * dx + y1 * dy);
	px = x1 + dx * r;
	py = y1 + dy * r;
	r  = (float)sqrt (px * px + py * py);
	
	// clamp distance to 1
	r = (r > 1.0f ? 1.0f : r);

	// z coordinate for the two [x,y] points given by the mouse coordinates
	z = (float)sqrt(1 - r * r);

	// normalize vectors
	r = 1.0f / (float)sqrt(x0 * x0 + y0 * y0 + z * z);
	x0 = x0 * r;
	y0 = y0 * r;
	z0 =  z * r;

	r = 1.0f / (float)sqrt (x1 * x1 + y1 * y1 + z * z);
	x1 = x1 * r;
	y1 = y1 * r;
	z1 =  z * r;

	// calculate rotation axis
	ax = y0 * z1 - z0 * y1;
	ay = z0 * x1 - x0 * z1;
	az = x0 * y1 - y0 * x1;

	// check rotation angle and axis
	r = 1.0f / (float)sqrt (ax * ax + ay * ay + az * az);
	if (r < ONE_OVER_SMALL_NUMBER) {
		
		rot[0] = n;
		rot[1] = -r*ax;
		rot[2] = r*ay;
		rot[3] = -r*az;  // convert to a left-handed (OpenGL) system -> minus sign
		return true;
	}
	else {
		// indicate no rotation
		return false;
	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
