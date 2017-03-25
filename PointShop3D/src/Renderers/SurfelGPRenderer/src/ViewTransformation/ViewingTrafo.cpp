// Title:   ViewingTrafo.cpp
// Created: Thu Sep 25 14:49:42 2003
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
// Viewing transformatino
//
// ViewingTrafo.c 
//-------------------------------------------------------------------------
#include <malloc.h>

#include "ViewingTrafo.h"
// #include "../OctreeRenderer_SurfSplat/Debug.h"
#include "../../../../Utilities/src/Matrix.h"
#include "../../../../Utilities/src/Common.h"



//-------------------------------------------------------------------------
// Allocate a new viewing trafo struct
//-------------------------------------------------------------------------
ViewingTrafo* VTRNew()
{
	ViewingTrafo *vtr;
	int i;
	
	vtr = (ViewingTrafo *)malloc(sizeof(ViewingTrafo));
	vtr->translation[0] = 0.0f;
	vtr->translation[1] = 0.0f;
	vtr->translation[2] = 0.0f;

	for(i=0; i<16; i++)
	{
		vtr->rotation[i] = 0.0f;
	}
	vtr->rotation[0] = 1.0f;
	vtr->rotation[5] = 1.0f;
	vtr->rotation[10] = 1.0f;
	vtr->rotation[15] = 1.0f;

	return vtr;
}


//-------------------------------------------------------------------------
// Free a viewing trafo
//-------------------------------------------------------------------------
void VTRFree(ViewingTrafo *vtr)
{
	free(vtr);
}


//-------------------------------------------------------------------------
// Reset the viewing trafo
//-------------------------------------------------------------------------
void VTRReset(ViewingTrafo *vtr)
{
	int i;

	vtr->translation[0] = 0.0f;
	vtr->translation[1] = 0.0f;
	vtr->translation[2] = 0.0f;

	for(i=0; i<16; i++)
	{
		vtr->rotation[i] = 0.0f;
	}
	vtr->rotation[0] = 1.0f;
	vtr->rotation[5] = 1.0f;
	vtr->rotation[10] = 1.0f;
	vtr->rotation[15] = 1.0f;
}


//-------------------------------------------------------------------------
// Apply translation
//-------------------------------------------------------------------------
void VTRTranslate(ViewingTrafo *vtr, float x, float y, float z)
{
	vtr->translation[0] += x;
	vtr->translation[1] += y;
	vtr->translation[2] += z;
}

//-------------------------------------------------------------------------
// Set position
//-------------------------------------------------------------------------
void VTRSetPosition (ViewingTrafo *vtr, float x, float y, float z)
{
	vtr->translation[0] = x;
	vtr->translation[1] = y;
	vtr->translation[2] = z;
}


//-------------------------------------------------------------------------
// Apply rotation
//-------------------------------------------------------------------------
// angle is in radians
void VTRRotate (float angle, float x, float y, float z, ViewingTrafo *vtr) {

	float userRotation[16];
	float finalRotation[16];
	
    MtrCreateRotation4x4fr (angle, x, y, z, userRotation);
	MtrMult4x4fr (userRotation, vtr->rotation, finalRotation);
	MtrCopy4x4f (finalRotation, vtr->rotation);

}

//-------------------------------------------------------------------------
// Get z translation
//-------------------------------------------------------------------------
float VTRGetZTranslation(ViewingTrafo *vtr)
{
	return vtr->translation[2];
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
