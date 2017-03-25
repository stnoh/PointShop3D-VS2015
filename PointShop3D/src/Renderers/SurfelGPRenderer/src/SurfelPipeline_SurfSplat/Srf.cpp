//
// Title:   Srf.cpp<2>
// Created: Thu Sep 25 14:50:20 2003
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
// Srf.c
//-------------------------------------------------------------------------

#include <malloc.h>

#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../OctreeRenderer_SurfSplat/ConfigHeaders/Shader.h"
#include "../OctreeRenderer_SurfSplat/ConfigHeaders/ZBuffer.h"
#include "Warper.h"
#include "Srf.h"

// Surfel context structure
struct _SrfContext
{
	Warper *warper;						// the warper
	ZBuffer *zBuffer;					// the z-buffer
	Shader *shader;						// the shader
	LSrList *lightSourceList;			// list of light sources

	int superSampling;
	int fWidth, fHeight;
	int iWidth, iHeight;
	int pWidth, pHeight;
};


// Initialisation
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Instantiate surfel context
//-------------------------------------------------------------------------
SrfContext *SrfNew(FrameBufferInterface *frameBuffer, int supersampling)
{
	SrfContext *sct;

	sct = (SrfContext *)malloc(sizeof(SrfContext));

	sct->superSampling = supersampling;
	sct->fWidth  = frameBuffer->getSize().width();
	sct->fHeight = frameBuffer->getSize().height();

	sct->iWidth  = sct->fWidth  / supersampling;
	sct->iHeight = sct->fHeight / supersampling;

	sct->pWidth = sct->iWidth;
	sct->pHeight = sct->iHeight;

	// allocate and initialize the z-buffer for the display image
	sct->zBuffer = ZBfNew(sct->fWidth, sct->fHeight, 1024, frameBuffer);
	
	// allocate and initialize the warper
	sct->warper = WrpNew(sct->zBuffer);
	// set the default view frustum (similar to gluPerspective): angle of field of view, 
	// aspect ratio, near and far clipping planes
	WrpSetFrustum(sct->warper, 45.f, 1.f, 3.f, 14000.f);
	// WrpSetViewport(sct->warper, sct->fWidth, sct->fHeight);
	WrpSetViewport(sct->warper, frameBuffer->getSize().width(), frameBuffer->getSize().height());

	// allocate the light source list
	sct->lightSourceList = LSLNew();

	// allocate and initialize the shader
	sct->shader = ShdNew(sct->warper, sct->zBuffer->frameBuffer, sct->lightSourceList);

	return sct;
}


//-------------------------------------------------------------------------
// Free a surfel rendering context
//-------------------------------------------------------------------------
void SrfFree(SrfContext *sct)
{
	
	WrpFree(sct->warper);
	ZBfFree(sct->zBuffer);
	ShdFree(sct->shader);
	
	LSLFree(sct->lightSourceList);

	free(sct);
}


//-------------------------------------------------------------------------
// Set the viewing frustum
//-------------------------------------------------------------------------
void SrfSetFrustum(SrfContext *sct, float fofv, float aspect, float nearP, float farP)
{
	WrpSetFrustum(sct->warper, fofv, aspect, nearP, farP);
}


//-------------------------------------------------------------------------
// Set an asymmetric viewing frustum
//-------------------------------------------------------------------------
void SrfSetFrustumAsymmetric(SrfContext *sct, float r, float l, float t, float b, float n, float f)
{
	WrpSetFrustumAsymmetric(sct->warper, r, l, t, b, n, f);
}


//-------------------------------------------------------------------------
// Set the viewport
//-------------------------------------------------------------------------
void SrfSetViewport(SrfContext *sct, int width, int height, int supersampling)
{
  (void)sct; (void)width; (void)height; (void)supersampling;
}


//-------------------------------------------------------------------------
// Get the viewport
//-------------------------------------------------------------------------
void SrfGetViewPort(SrfContext *sct, int *width, int *height, int *supersampling)
{
	*width  = sct->zBuffer->frameBuffer->getSize().width();
	*height = sct->zBuffer->frameBuffer->getSize().height();
	*supersampling = sct->superSampling;
}



// Rendering
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Begin a frame
//-------------------------------------------------------------------------
void SrfBeginFrame(SrfContext *sct)
{
	WrpClearZBuffer(sct->warper);
	ShdPrepareForShading(sct->shader);
}


//-------------------------------------------------------------------------
// Continue rendering a frame. Hence, do not clear the z-buffer, but
// make sure the shader sets its global variables.
//-------------------------------------------------------------------------
void SrfContinueFrame(SrfContext *sct) 
{
	ShdPrepareForShading(sct->shader);
}


//-------------------------------------------------------------------------
// End a frame
//-------------------------------------------------------------------------
void SrfShadeFrame(SrfContext *sct, int bbox[4], QRgb backgroundColor)
{
	int magfactor;

	magfactor = (sct->iWidth > sct->pWidth ? sct->iWidth / sct->pWidth : 1);

	// shade and copy to display image
	ShdShadeZBuffer(sct->shader, magfactor, bbox, backgroundColor);
}


//-------------------------------------------------------------------------
// Set the transformation matrix for warping
//-------------------------------------------------------------------------
void SrfSetTrafo(SrfContext *sct, const float trafo[16])
{
	WrpSetTrafo(sct->warper, trafo);
}


//-------------------------------------------------------------------------
// Warp surfels
//-------------------------------------------------------------------------
void SrfWarp(SrfContext *sct, Object* object, SurfelCollection *surfelCollection, int bbox[4])
{
	WrpWarpToZBufferSOT(sct->warper, object, surfelCollection, bbox);
}

void SrfWarp(SrfContext *sct, Object* object, const std::vector<SurfelInterface*>* surfels, int nSurfels, int bbox[4])
{
	WrpWarpToZBufferSOT(sct->warper, object, surfels, nSurfels, bbox);
}


void SrfScaleAllWeights (SrfContext *sct, float scaleWeight) {


	ZBfScaleAllWeights(sct->zBuffer, scaleWeight);
}


//-------------------------------------------------------------------------
// Set the buffer size for progressive rendering
//-------------------------------------------------------------------------
void SrfSetProgressiveSize(SrfContext *sct, int width, int height)
{
	int h, w;

	// potential supersampling complicates the situation a little bit...
	sct->pWidth = width;
	w = (sct->pWidth <= sct->iWidth) ? sct->pWidth : sct->iWidth;
	sct->pHeight = height;
	h = (sct->pHeight <= sct->iHeight) ? sct->pHeight : sct->iHeight;

	WrpSetViewport(sct->warper, sct->pWidth, sct->pHeight);
	ZBfSetSize(sct->zBuffer, sct->pWidth, sct->pHeight);
}


//-------------------------------------------------------------------------
// Get internal frame buffer
//-------------------------------------------------------------------------
SrfZBufItem* SrfGetInternalFrameBuffer(SrfContext *sct) 
{
	return sct->zBuffer->buf;
}




// State variables
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Enable surfel context property
//-------------------------------------------------------------------------
void SrfEnable(SrfContext *sct, SrfProperty p)
{
	// warper
	if(p & SRF_WRP_LODCOLORS)
	{
		WrpEnable(sct->warper, WRP_LODCOLORS);
	}
	if(p & SRF_WRP_SKIPSAMPLES)
	{
		WrpEnable(sct->warper,WRP_SKIPSAMPLES);
	}

	// shader
	if(p & SRF_SHD_BIDINORMALS)
	{
		ShdEnable(sct->shader, SHD_BIDINORMALS);
		ZbfEnable(sct->zBuffer, ZBF_BIDINORMALS);
	}
	if(p & SRF_SHD_DBGCOLORS)
	{
		ShdEnable(sct->shader, SHD_DBGCOLORS);
	}
	if(p & SRF_SHD_ENVMAPPING)
	{
		ShdEnable(sct->shader, SHD_ENVMAPPING);
	}
	if(p & SRF_SHD_LIGHTING)
	{
		ShdEnable(sct->shader, SHD_LIGHTING);
	}
	if(p & SRF_SHD_PRESHADESPLAT)
	{
		ShdEnable(sct->shader, SHD_PRESHADESPLAT);
	}
	if(p & SRF_SHD_PERPIXELSHADING)
	{
		ShdEnable(sct->shader, SHD_PERPIXELSHADING);
		ZbfDisable(sct->zBuffer, ZBF_PERSURFELSHADING);
	}

}


//-------------------------------------------------------------------------
// Disable surfel context property
//-------------------------------------------------------------------------
void SrfDisable(SrfContext *sct, SrfProperty p)
{	
	// warper
	if(p & SRF_WRP_LODCOLORS)
	{
		WrpDisable(sct->warper, WRP_LODCOLORS);
	}
	if(p & SRF_WRP_SKIPSAMPLES)
	{
		WrpDisable(sct->warper,WRP_SKIPSAMPLES);
	}

	// shader
	if(p & SRF_SHD_BIDINORMALS)
	{
		ShdDisable(sct->shader, SHD_BIDINORMALS);
		ZbfDisable(sct->zBuffer, ZBF_BIDINORMALS);
	}
	if(p & SRF_SHD_DBGCOLORS)
	{
		ShdDisable(sct->shader, SHD_DBGCOLORS);
	}
	if(p & SRF_SHD_ENVMAPPING)
	{
		ShdDisable(sct->shader, SHD_ENVMAPPING);
	}
	if(p & SRF_SHD_LIGHTING)
	{
		ShdDisable(sct->shader, SHD_LIGHTING);
	}
	if(p & SRF_SHD_PRESHADESPLAT)
	{
		ShdDisable(sct->shader, SHD_PRESHADESPLAT);
	}
	if(p & SRF_SHD_PERPIXELSHADING)
	{
		ShdDisable(sct->shader, SHD_PERPIXELSHADING);
		ZbfEnable(sct->zBuffer, ZBF_PERSURFELSHADING);
	}

}



// Surface Splatting
//-------------------------------------------------------------------------
void SrfSetSplatFunction(SrfContext *sct, SrfSplatFunction f)
{
	WrpDisable (sct->warper, WRP_ELLIPTICALSURFELS);

	if(f == SRF_SURFACESPLAT){
		sct->zBuffer->splatFunction = ZBfSurfaceSplat;
	} 
	else if (f == SRF_SURFACEUNSPLAT) {
		sct->zBuffer->splatFunction = ZBfSurfaceUnSplat;
	}
	else if (f == SRF_NEARESTNEIGHBOR) {
		sct->zBuffer->splatFunction = ZBfNearestNeighbor;
	}
	else if (f == SRF_ELLIPTICALSURFEL) {
		WrpEnable (sct->warper, WRP_ELLIPTICALSURFELS);
	}

}


void SrfSetCoverageThreshold(SrfContext *sct, float f)
{
	sct->zBuffer->covthreshold = f;
}

float SrfGetCoverageThreshold(SrfContext *sct)
{
	return sct->zBuffer->covthreshold;
}

void SrfSetReconstructionScaling(SrfContext *sct, float f)
{
	sct->zBuffer->recscaling = f;
}

float SrfGetReconstructionScaling(SrfContext *sct)
{
	return sct->zBuffer->recscaling;
}

void SrfSetVisualizeContributionsFlag(SrfContext *sct, int i)
{
	sct->zBuffer->viscontrib = i;
}

int SrfGetVisualizeContributionsFlag(SrfContext *sct)
{
	return sct->zBuffer->viscontrib;
}

void SrfSetCutoffRadius(SrfContext *sct, float radius) {
	ZBfSetCutoffRadius(sct->zBuffer, radius);
	ZBfComputeFilterLUT(sct->zBuffer);
}

float SrfGetCutoffRadius(SrfContext *sct) {
	return sct->zBuffer->cutoffRadius;
}

void SrfSetBlendingThresholds(SrfContext *sct, float constThreshold, float distThreshold, float angleThreshold)
{
	ZBfSetBlendingThresholds(sct->zBuffer, constThreshold, distThreshold, angleThreshold);
}

void SrfGetBlendingThresholds(SrfContext *sct, float* constThreshold, float* distThreshold, float* angleThreshold)
{
	ZBfGetBlendingThresholds(sct->zBuffer, constThreshold, distThreshold, angleThreshold);
}



// Shading
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Set the shading model
//-------------------------------------------------------------------------
void SrfSetShadingModel(SrfContext *sct, SrfShadingModel m)
{
	ShdShadingModel s;

	s = (ShdShadingModel)m;
	ShdSetShadingModel(sct->shader, s);
}


//-------------------------------------------------------------------------
// Set a user shading function
//-------------------------------------------------------------------------
void SrfSetUserShadingFunction(SrfContext *sct, SrfShadingFunction f, void *u)
{
	ShdLightSampleFnct s;

	s = (ShdLightSampleFnct)f;
	ShdSetUserShadingFunction(sct->shader, s, u);
}


//-------------------------------------------------------------------------
// Set the texture filtering method
//-------------------------------------------------------------------------
void SrfSetTextureFiltering(SrfContext *sct, SrfTextureFiltering t)
{
	(void)sct;
	
	ShdTextureFiltering s;

	s = (ShdTextureFiltering)t;
//	ShdSetTextureFiltering(sct->shader, s);
}


//-------------------------------------------------------------------------
// Set the diffuse color used for shading
//-------------------------------------------------------------------------
void SrfSetDiffColor(SrfContext *sct, SrfDiffColor c)
{
	ShdDiffColor s;

	s = (ShdDiffColor)c;
	ShdSetDiffColor(sct->shader, s);
}


// Light sources
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Get a handle to a new light source
//-------------------------------------------------------------------------
SrfHandle SrfNewLight(SrfContext *sct)
{
	return (SrfHandle)LSLNewLight(sct->lightSourceList);
}


//-------------------------------------------------------------------------
// Free a light source
//-------------------------------------------------------------------------
void SrfFreeLight(SrfContext *sct, SrfHandle h)
{
	LSLFreeLight(sct->lightSourceList, (LSrHandle)h);
}


//-------------------------------------------------------------------------
// Set a light source
//-------------------------------------------------------------------------
void SrfSetLight(SrfContext *sct, SrfHandle h, LightSource *l)
{
	LSLSetLight(sct->lightSourceList, (LSrHandle)h, l);
}


//-------------------------------------------------------------------------
// Enable a light source
//-------------------------------------------------------------------------
void SrfEnableLight(SrfContext *sct, SrfHandle h)
{
	LSLEnable(sct->lightSourceList, (LSrHandle)h);
}

//-------------------------------------------------------------------------
// Disable a light source
//-------------------------------------------------------------------------
void SrfDisableLight(SrfContext *sct, SrfHandle h)
{
	LSLDisable(sct->lightSourceList, (LSrHandle)h);
}

void SrfSetLightDirection (const SrfContext *context, const SrfHandle handle, const Vector3D newDirection) {
	LSLSetLightDirection (context->lightSourceList, handle, newDirection);
}

Vector3D SrfSetLightDirection (const SrfContext *context, const SrfHandle handle) {
	return LSLGetLightDirection (context->lightSourceList, handle);
}

//-------------------------------------------------------------------------
// Enable a property
//-------------------------------------------------------------------------
void SrfEnableZBufferOption (SrfContext *sct, SrfZbfProperties p)
{
	sct->zBuffer->options |= p;
}


//-------------------------------------------------------------------------
// Disable a shader property
//-------------------------------------------------------------------------
void SrfDisableZBufferOption (SrfContext *sct, SrfZbfProperties p)
{
	int mask = 0xffffffff;

	p = (SrfZbfProperties)((int)p ^mask);
	sct->zBuffer->options &= p;
}


// Image buffer
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Load a background image
//-------------------------------------------------------------------------
void SrfLoadBackground(SrfContext *sct, unsigned int *buf)
{
	(void)sct; (void)buf;
}



// Surfel octrees
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Free surfel octree array
//-------------------------------------------------------------------------
#if 0
void SrfFreeSAr(p_SOTArray p_sar)
{
	int i;
	SLtArray *sar;

	sar = (SLtArray *)p_sar;

	for(i=0; i<SArGetNOfSLt(sar); i++)
	{
		SLtFree(SArGetSLt(sar, i));
	}
	SArFree(sar);
}



//-------------------------------------------------------------------------
// Get number of octrees
//-------------------------------------------------------------------------
int SrfGetNOfSOT(p_SOTArray sar)
{
	return SArGetNOfSLt((SLtArray *)sar);
}


//-------------------------------------------------------------------------
// Get a surfel octree
//-------------------------------------------------------------------------
p_SurfelOctree SrfGetSOT(p_SOTArray sar, int i)
{
	return (p_SurfelOctree)SrfGetSOT((SLtArray *)sar, i);
}



// Data access (a hack!)
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Get the material table
//-------------------------------------------------------------------------
void *SrfGetMaterialTable(SrfContext *sct)
{
	return (void *)sct->materialTable;
}
#endif

//-------------------------------------------------------------------------
// Set a reference to the frame buffer 
//-------------------------------------------------------------------------
void SrfSetFrameBuffer (SrfContext *sct, FrameBufferInterface *frameBuffer)
{
	sct->zBuffer->frameBuffer = frameBuffer;
}

//-------------------------------------------------------------------------
// Get a reference to the frame buffer
//-------------------------------------------------------------------------
FrameBufferInterface *SrfGetFrameBuffer (SrfContext *sct)
{
	return sct->zBuffer->frameBuffer;
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
