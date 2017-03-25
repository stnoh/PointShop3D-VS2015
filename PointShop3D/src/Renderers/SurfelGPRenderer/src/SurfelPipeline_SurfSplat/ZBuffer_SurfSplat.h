//
// Title:   ZBuffer_SurfSplat.h
// Created: Thu Sep 25 14:49:54 2003
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
// Z-buffer
// Supports surface splatting, but does not support multiple layers for 
// order independent transparency
//
// ZBuffer_SurfaceSplat.h 
//-------------------------------------------------------------------------

#ifndef ZBUFFER_SURFSPLAT_H
#define ZBUFFER_SURFSPLAT_H

#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../../Core/Interfaces/src/SurfelInterface.h"
#include "../../../../Utilities/src/Common.h"
#include "Srf.h"


// unpack R,G,B components
#define UNPACK_FLOAT_RGB(_r,_g,_b,_c) \
	_r = (float)((_c & 0x00ff0000) >> 16); \
	_g = (float)((_c & 0x0000ff00) >> 8); \
	_b = (float)(_c & 0x000000ff)

// pack R,G,B components
#define PACK_FLOAT_RGB(_c,_r,_g,_b) \
	_c = (((int)_r << 16) & 0x00ff0000) | \
		(((int)_g << 8) & 0x0000ff00) | \
		((int)_b & 0x000000ff)


// Variable and type declarations

// z-buffer properties that can be OR-ed 
typedef enum zbfProperties
{
	ZBF_BIDINORMALS = 1,
	ZBF_PERSURFELSHADING = 2,
	ZBF_VISUALIZESELECTION = 4,
	ZBF_SHADOWS = 8
} ZbfProperties;


typedef struct _ZBuffer ZBuffer;

// surface splatting function type
typedef int (*ZBfSplatSurfel)(ZBuffer *zbf, float x, float y, float z, float n[3], SurfelInterface *surfel, int level,
				   float stoo_scaling, float otoc_scaling, float sx, float sy, float tx, float ty, int bbox[4], Object* object);

struct _ZBuffer
{
	int xsize;
	int ysize;
	SrfZBufItem *buf;
	SrfZBufItem *cleardata;
	long bufsize;

	int options;

	ZBfSplatSurfel splatFunction;

	// surface splatting parameters
	float cutoffRadius;
	float covthreshold;			// hack for compatibility with srf.c/.h, this variable is not used here.
	int viscontrib;				// dito
	float recscaling;			// dito

	// blending thresholds used for surface splatting
	float constThreshold;			
	float distThreshold;
	float angleTrheshold;

	// reconstruction filter table used for surface splatting
	int LUTsize;
	float *filterLUT;

	// the extended frame buffer which holds data for
	// interactive manipulation
	FrameBufferInterface *frameBuffer;

};


// Global variable declaraitons
extern int zbf_rows, zbf_cols;
extern float zbf_x_cur, zbf_y_cur;
extern int zbf_i_cur;
extern int zbf_xsize, zbf_ysize;
extern int zbf_material;
extern SrfZBufItem *zbf_buf;


// Function prototypes

// functions called by the main program (OctreeRenderer)
ZBuffer* ZBfNew(int xsize, int ysize, int LUTsize, FrameBufferInterface *frameBuffer);
void ZBfFree(ZBuffer *zbf);
void ZBfSetSize(ZBuffer *zbuf, int xsize, int ysize);
void ZBfComputeFilterLUT(ZBuffer *zbf);
void ZBfScaleAllWeights(ZBuffer *zbf, float scaleWeight);

// functions called by the warper for initialisation and to write data
void ZBfPrepareForWriting(ZBuffer *zbf);
void ZBfClear(ZBuffer *zbuf);
void ZBfClearPixel(ZBuffer *zbuf, int x, int y);
int ZBfSurfaceSplat(ZBuffer *zbf, float x, float y, float z, float n[3], SurfelInterface *surfel, int level,
				   float stoo_scaling, float otoc_scaling, float sx, float sy, float tx, float ty, int bbox[4], Object* object);
int ZBfSurfaceSplatEllipse(ZBuffer *zbf, float x0, float y0, float x_c, float y_c, float z_c, float n[3], 
						   float tan1[3], float tan2[3], SurfelInterface *surfel, int l,
				           float scale_stoo, float scale_otoc, float vp_sx, float vp_sy,
						   float vp_tx, float vp_ty, int bbox[4], Object *object, float *clipAxes, int numClipAxes);
int ZBfSurfaceUnSplat(ZBuffer *zbf, float x0, float y0, float z, float n[3], SurfelInterface *surfel, int l,
				   float scale_stoo, float scale_otoc, float vp_sx, float vp_sy, float vp_tx, float vp_ty, int bbox[4], Object* object);
int ZBfNearestNeighbor(ZBuffer *zbf, float x, float y, float z, float n[3], SurfelInterface *surfel, int level,
				   float stoo_scaling, float otoc_scaling, float sx, float sy, float tx, float ty, int bbox[4], Object* object);

// functions called by the shader to read data
void ZBfPrepareForReading(ZBuffer *zbf);
void ZBfGetSample(int *i, float *x, float *y, SrfZBufItem **zbufitem);

// input/output
void ZBfWriteToFile(ZBuffer *zbf, char *name);

// access to FilterLUT
float *ZBfGetFilterLUT();
int ZBfGetFilterLUTSize();

// properties
void ZbfEnable(ZBuffer* zbf, ZbfProperties p);
void ZbfDisable(ZBuffer* zbf, ZbfProperties p);
void ZBfSetCutoffRadius(ZBuffer* zbf, float cutoffRadius);
void ZBfSetBlendingThresholds(ZBuffer* zbf, float constThreshold, float distThreshold, float angleThreshold);
void ZBfGetBlendingThresholds(ZBuffer* zbf, float* constThreshold, float* distThreshold, float* angleThreshold);



// macros instead of functions for better performance
// caution: these macros rely on global variables which
// have to be set by calling ZBfPrepareForWriting()!


//-------------------------------------------------------------------------
// Get all the samples from the z-buffer
//-------------------------------------------------------------------------
#define ZBFGETITEM(_zbufitem) \
{ \
	/* zbf_i_cur, zbf_cols, zbf_rows, zbf_x_cur, zbf_y_cur contain */ \
	/* the current position in the z-buffer */ \
	_zbufitem = &(zbf_buf[zbf_i_cur]); \
}

#define ZBFFIRSTROW() \
{ \
	/* note: x,y values are pixel centers! */ \
	zbf_rows = 0; \
	zbf_y_cur = 0.5f; \
	zbf_x_cur = 0.5f; \
	zbf_i_cur = 0; \
}

#define ZBFFIRSTCOL() \
{ \
	/* note: x,y values are pixel centers! */ \
	zbf_cols = 0; \
	zbf_x_cur = 0.5f; \
}

#define ZBFNEXTROW() \
{ \
	zbf_y_cur += 1.0f; \
	zbf_rows++; \
}


#define ZBFNEXTCOL() \
{ \
	zbf_x_cur += 1.0f; \
	zbf_i_cur++; \
	zbf_cols++; \
}

#define ZBFSETPOS(_x,_y) \
{ \
	zbf_x_cur = _x+0.5f; \
	zbf_y_cur = _y+0.5f; \
	zbf_rows = _y; \
	zbf_cols = _x; \
	zbf_i_cur = _y*zbf_xsize + _x; \
}

#define ZBFCOLS zbf_cols
#define ZBFROWS zbf_rows



//-------------------------------------------------------------------------
// Demo code
//-------------------------------------------------------------------------
#ifdef DEMO_CODE
#define ZBFSETWARPEDONLEVEL(_x_c,_y_c,_l) \
{ \
	int _i; \
\
	_i = (int)_x_c + (int)_y_c * zbf_xsize; \
	zbf_buf[_i].warpedOnLevel = _l; \
}

#endif



#endif // ZBUFFER_SURFSPLAT_H
// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
