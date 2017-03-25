//
// Title:   LightSource.cpp<2>
// Created: Thu Sep 25 14:50:26 2003
// Authors: Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003
// Computer Graphics Lab, ETH Zurich
// Mitsubishi Electric Research Laboratories (MERL), Cambridge MA
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
// IN NO EVENT SHALL MERL AND/OR ETH BE LIABLE TO ANY PARTY
// FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
// SOFTWARE AND ITS DOCUMENTATION, EVEN IF MERL OR ETH HAS BEEN
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
// 
// MERL AND ETH SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
// HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER MERL NOR ETH HAVE
// ANY OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// ENHANCEMENTS, OR MODIFICATIONS.
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
// LightSource.h
//-------------------------------------------------------------------------

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

// Variable and type declarations
typedef enum lsrType
{
	LSR_DIRECTIONAL = 0,
	LSR_POINT = 1,
	LSR_SPOT = 2
} LSrType;

typedef struct lightSource
{	
	LSrType type;		// type of lightsource

	float x;			// position or direction of light, depending on type
	float y;
	float z;

	float Ir;			// red intensity
	float Ig;			// green intensity
	float Ib;			// blue intensity

	float Ar;			// ambient red intensity
	float Ag;			// ambient green intensity
	float Ab;			// ambient blue intensity
} LightSource;


// Function prototypes
LightSource *LSrNew();
void LSrFree(LightSource *lsr);
void LSrCopy(LightSource *dst, LightSource *src);


// Macros
#define LSRGET_X(_lsr) (_lsr->x)
#define LSRGET_Y(_lsr) (_lsr->y)
#define LSRGET_Z(_lsr) (_lsr->z)
#define LSRGET_IR(_lsr) (_lsr->Ir)
#define LSRGET_IG(_lsr) (_lsr->Ig)
#define LSRGET_IB(_lsr) (_lsr->Ib)
#define LSRGET_AR(_lsr) (_lsr->Ar)
#define LSRGET_AG(_lsr) (_lsr->Ag)
#define LSRGET_AB(_lsr) (_lsr->Ab)


#endif // LIGHTSOURCE_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
