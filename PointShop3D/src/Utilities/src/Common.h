// Title:   Common.h
// Created: Thu Sep 25 15:02:00 2003
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

#ifndef __COMMON_H_
#define __COMMON_H_

//#include <qcolor.h>

#include <float.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

#ifndef PI_180
#define PI_180 (float)M_PI/180.0f 
#endif


namespace Constants {
	// number of levels in hierarchical z-buffer
	const int PULLPUSH_LEVELS = 10;

	// very small number
	const float EPSILON2 = 0.00001f;

//	const QColor TOOLBAR_BACKGROUND = QColor (220, 205, 180);
		
}

typedef unsigned int uint;


#endif	// __COMMON_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
