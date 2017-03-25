//
// Title:   Warper.h<2>
// Created: Thu Sep 25 14:50:01 2003
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
// Warper 
//
// Warper.h 
//-------------------------------------------------------------------------

#ifndef WARPER_H
#define WARPER_H


#include "../OctreeRenderer_SurfSplat/ConfigHeaders/ZBuffer.h"

#define CULL_UNKNOWN 0
#define CULL_INSIDE 1
#define CULL_OUTSIDE 2


// Variable and type declarations
// viewing transformation
typedef enum wrpOptions
{
	WRP_LODCOLORS = 1,
	WRP_SKIPSAMPLES = 2,
	WRP_ELLIPTICALSURFELS = 4
} WrpOptions;

typedef struct transformation
{
	float rotation[9];			// model-view rotation
	float translation[3];		// model-view translation
	float invRotation[9];		// inverse model-view rotation (used for block clipping)
	float invTranslation[3];	// inverse model-view rotation (used for block clipping)
	float normalsRotation[9];	// rotation matrix to transform normals
	float scaling;				// uniform model-view scaling
} Transformation;

// view frustum
typedef struct frustum
{
	float xP, yP;				// x,y-extent of the view frustum at z=1
	float xC, yC;				// x,y-center point of the view frustum at z=1
	float nearplane, farplane;	// camera space z-values of near and far clipping planes
	float v[24];				// vertices of the view frustum in camera space
	float M[9];					// view frustum normalization transformation (used for clipping)
} Frustum;

// viewport
typedef struct viewport
{
	// note: the viewport has the origin at its lower left corner
	int xS, yS;					// size of the viewport in screen pixels
} Viewport;

// warper
typedef struct warper
{
	Transformation transformation;
	Frustum frustum;
	Viewport viewport;
	ZBuffer *zbf;

	int options;
} Warper;


// Function prototypes
Warper* WrpNew(ZBuffer *zbf);
void WrpFree(Warper *wrp);

void WrpSetFrustumAsymmetric(Warper *wrp, float r, float l, float t, float b, float n, float f);
void WrpSetFrustum(Warper *wrp, float fofv, float aspect, float nearP, float farP);
void WrpSetFrustum_obsolete(Warper *wrp, float f, float x, float y);
void WrpSetViewport(Warper *wrp, int xsize,int ysize);

void WrpClearZBuffer(Warper *wrp);
void WrpResetObjectCount(Warper *wrp);

void WrpWarpToZBufferSOT(Warper *r, Object* object, SurfelCollection *slt, int bbox[4]);
void WrpWarpToZBufferSOT(Warper *r, Object* object, const std::vector<SurfelInterface*> *slt, int nSurfels, int bbox[4]);
void WrpTestWarpToZBufferSOT(Warper *r, SurfelCollection *slt);
void WrpSetTrafo(Warper *r, const float trafo[16]);

void WrpEnable(Warper *wrp, WrpOptions o);
void WrpDisable(Warper *wrp, WrpOptions o);


#endif // WARPER_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
