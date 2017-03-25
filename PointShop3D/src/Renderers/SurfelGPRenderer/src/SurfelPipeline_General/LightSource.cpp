// Title:   LightSource.cpp
// Created: Thu Sep 25 14:50:28 2003
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

//-------------------------------------------------------------------------
// Specification of different light sources
//
// LightSource.c
//-------------------------------------------------------------------------


#include <malloc.h>
#include <math.h>

#include "LightSource.h"


//-------------------------------------------------------------------------
// Allocate a new light source
//-------------------------------------------------------------------------
LightSource *LSrNew()
{
	LightSource *lsr;

	lsr = (LightSource *)malloc(sizeof(LightSource));

	lsr->type = LSR_DIRECTIONAL;
	lsr->x = 0.0f;
	lsr->y = 0.0f;
	lsr->z = -1.0f;
	lsr->Ir = 1.0f;
	lsr->Ig = 1.0f;
	lsr->Ib = 1.0f;
	lsr->Ar = 0.5f;
	lsr->Ag = 0.5f;
	lsr->Ab = 0.5f;
	
	return lsr;
}


//-------------------------------------------------------------------------
// Free a ambient light source
//-------------------------------------------------------------------------
void LSrFree(LightSource *lsr)
{
	free(lsr);
}


//-------------------------------------------------------------------------
// Copy a light source
//-------------------------------------------------------------------------
void LSrCopy(LightSource *dst, LightSource *src)
{
	dst->type = src->type;
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
	dst->Ir = src->Ir;
	dst->Ig = src->Ig;
	dst->Ib = src->Ib;
	dst->Ar = src->Ar;
	dst->Ag = src->Ag;
	dst->Ab = src->Ab;
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
