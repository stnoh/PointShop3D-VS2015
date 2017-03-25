// Title:   ViewingTrafo.h
// Created: Thu Sep 25 14:49:32 2003
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
// Viewing transformation
//
// ViewingTrafo.h 
//-------------------------------------------------------------------------

#ifndef VIEWINGTRAFO_H
#define VIEWINGTRAFO_H


// Variable and type declarations
// viewing transformation
typedef struct viewingTrafo
{
	float rotation[16];
	float translation[3];
} ViewingTrafo;


// Function prototypes
ViewingTrafo* VTRNew();
void VTRFree(ViewingTrafo *vtr);
void VTRReset(ViewingTrafo *vtr);
void VTRRotate(ViewingTrafo *vtr, float a, float x0, float x1, float x2);
void VTRTranslate(ViewingTrafo *vtr, float x, float y, float z);
void VTRSetPosition (ViewingTrafo *vtr, const float x, const float y, const float z);
float VTRGetZTranslation(ViewingTrafo *vtr);


#endif // VIEWINGTRAFO_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
