//
// Title:   Srf.h
// Created: Thu Sep 25 14:50:04 2003
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
// Wrapper for surfel rendering pipeline
//
// Srf.h
//-------------------------------------------------------------------------

#ifndef SRF_H
#define SRF_H

#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../../DataTypes/src/Vector3D.h"
#include "../SurfelPipeline_General/LightSource.h"

typedef struct _SrfContext SrfContext;
typedef void *p_SurfelOctree;
typedef void *p_SOTArray;


// Properties of the surfel rendering pipeline
typedef enum _SrfProperty
{
	SRF_WRP_LODCOLORS = 1,
	SRF_WRP_SKIPSAMPLES = 2,

	SRF_SHD_BIDINORMALS = 4,
	SRF_SHD_DBGCOLORS = 8,
	SRF_SHD_ENVMAPPING = 16,
	SRF_SHD_LIGHTING = 32,
	SRF_SHD_PRESHADESPLAT = 64,
	SRF_SHD_PERPIXELSHADING = 128,

	SRF_IMB_DETECTHOLES = 256,
	SRF_IMB_FILLHOLES = 512,
	SRF_IMB_VISWEIGHTS = 1024,
} SrfProperty;


// Enumerations
// note: all number match the ones in the corresponding component header files,
// thus parameters are passed to component functions by simply casting
typedef enum _SrfShadingModel
{
	SRF_PHONG_H = 1,
	SRF_PHONG_R = 2,
	SRF_TORRANCESPARROW = 3,
	SRF_USERFNCT = 4
} SrfShadingModel;

typedef enum _SrfTextureFiltering
{
	SRF_TEXTUREFILTERING_NOINTERP = 1,
	SRF_TEXTUREFILTERING_LINEAR = 2
} SrfTextureFiltering;

typedef enum _SrfDiffColor
{
	SRF_DIFFCOL_DEFAULT = 0,
	SRF_DIFFCOL_MIPMAPLEVEL = 1,
	SRF_DIFFCOL_LODLEVEL = 2
} SrfDiffColor;

typedef enum _SrfCubeMapType
{
	SRF_R_MAP = 0,
	SRF_N_MAP = 1
} SrfCubeMapType;

typedef enum _SrfSplatFunction
{
	SRF_SURFACESPLAT = 0,
	SRF_NEARESTNEIGHBOR = 1,
	SRF_SURFACEUNSPLAT = 2,
	SRF_ELLIPTICALSURFEL = 3
} SrfSplatFunction;

// these properties can be OR-ed
typedef enum srfZbfProperties {
	SRF_BIDINORMALS = 1,
	SRF_PERSURFELSHADING = 2,
	SRF_VISUALIZESELECTION = 4,
	SRF_SHADOWS = 8
} SrfZbfProperties;

// Other typedefs
typedef unsigned int SrfHandle;
typedef void (*SrfShadingFunction)();

// export the z-buffer item structure for fast external access to 
// data stored in the z-buffer
typedef struct _SrfZBufItem
{
	float z;				// the reconstructed camera space z-value of a surfel
	float zMin;				// z-range used for blending
	float zMax;
	float w;				// accumulated weights
	float n[3];				// accumulated normal
	float c[3];				// accumulated color values
	unsigned char alpha;	// composite alpha value

	float kA;
	float kD;
	float kS;
	unsigned char shininess;
	unsigned int specularColor;

	void* userData;			// pointer to user data
} SrfZBufItem;


// Surfel rendering pipeline

// Initialisation
SrfContext *SrfNew(FrameBufferInterface *frameBuffer, int supersampling);
void SrfFree(SrfContext *sct);

void SrfSetFrustum(SrfContext *sct, float fofv, float aspect, float nearP, float farP);
void SrfSetFrustumAsymmetric(SrfContext *sct, float r, float l, float t, float b, float n, float f);
void SrfSetViewport(SrfContext *sct, int width, int height, int supersampling);

void SrfGetViewPort(SrfContext *sct, int *width, int *height, int *supersampling);

// Rendering
void SrfBeginFrame(SrfContext *sct);
void SrfContinueFrame(SrfContext *sct);
void SrfShadeFrame(SrfContext *sct, int bbox[4], QRgb backgroundColor);
void SrfSetTrafo(SrfContext *sct, const float trafo[16]);
void SrfWarp(SrfContext *sct, Object* object, SurfelCollection *surfelCollection, int bbox[4]);
void SrfWarp(SrfContext *sct, Object* object, const std::vector<SurfelInterface*> *surfels, int nSurfels, int bbox[4]);
void SrfScaleAllWeights (SrfContext *sct, float scaleWeight);
SrfZBufItem *SrfGetInternalFrameBuffer(SrfContext *sct);


// Progressive rendering
void SrfSetProgressiveSize(SrfContext *sct, int width, int height);


// State variables
void SrfEnable(SrfContext *sct, SrfProperty p);
void SrfDisable(SrfContext *sct, SrfProperty p);

// Surface Splatting
void SrfSetSplatFunction(SrfContext *sct, SrfSplatFunction f);
void SrfSetCoverageThreshold(SrfContext *sct, float f);
float SrfGetCoverageThreshold(SrfContext *sct);
void SrfSetReconstructionScaling(SrfContext *sct, float f);
float SrfGetReconstructionScaling(SrfContext *sct);
void SrfSetVisualizeContributionsFlag(SrfContext *sct, int i);
int SrfGetVisualizeContributionsFlag(SrfContext *sct);
void SrfSetCutoffRadius(SrfContext *sct, float radius);
float SrfGetCutoffRadius(SrfContext *sct);
void SrfSetBlendingThresholds(SrfContext *sct, float constThreshold, float distThreshold, float angleThreshold);
void SrfGetBlendingThresholds(SrfContext *sct, float* constThreshold, float* distThreshold, float* angleThreshold);

// Shading
void SrfSetShadingModel(SrfContext *sct, SrfShadingModel m);
void SrfSetUserShadingFunction(SrfContext *sct, SrfShadingFunction f, void *u);
void SrfSetTextureFiltering(SrfContext *sct, SrfTextureFiltering t);
void SrfSetDiffColor(SrfContext *sct, SrfDiffColor c);

// Light sources
SrfHandle SrfNewLight(SrfContext *sct);
void SrfFreeLight(SrfContext *sct, SrfHandle h);
void SrfSetLight(SrfContext *srf, SrfHandle h, LightSource *l);
void SrfEnableLight(SrfContext *srf, SrfHandle h);
void SrfDisableLight(SrfContext *srf, SrfHandle h);

// Z Buffer options
void SrfEnableZBufferOption (SrfContext *sct, SrfZbfProperties p);
void SrfDisableZBufferOption (SrfContext *sct, SrfZbfProperties p);

/**
 * Sets the direction for the light source given by the <code>handle</code>. The direction is heading
 * <em>towards</em> the light source and must be normalized.
 *
 * @param context
 *        a pointer to a <code>SrfContext</code> managing the light source list
 * @param handle
 *        a <code>SrfHandle</code> identifying the light source
 * @param newDirection
 *        a <code>Vector3D</code> specifying the new normalized direction for the light source
 */
void SrfSetLightDirection (const SrfContext *context, const SrfHandle handle, const Vector3D newDirection);

/**
 * Returns the direction for the light source given by the <code>handle</code>. The direction is heading
 * <em>towards</em> the light source and must be normalized.
 *
 * @param context
 *        a pointer to a <code>SrfContext</code> managing the light source list
 * @param handle
 *        a <code>SrfHandle</code> identifying the light source
 * @return a <code>Vector3D</code> specifying the normalized direction of the light source
 */
Vector3D SrfSetLightDirection (const SrfContext *context, const SrfHandle handle);

// Image buffer
void SrfLoadBackground(SrfContext *sct, unsigned int *buf);

// frame buffer access
void SrfSetFrameBuffer (SrfContext *sct, FrameBufferInterface *frameBuffer);
FrameBufferInterface *SrfGetFrameBuffer (SrfContext *sct);


#endif // SRF.H


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
