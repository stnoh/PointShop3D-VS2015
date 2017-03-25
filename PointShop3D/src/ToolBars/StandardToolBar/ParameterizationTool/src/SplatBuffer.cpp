// Title:   SplatBuffer.cpp
// Created: Thu Sep 25 14:58:03 2003
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

#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Core/Scene/src/Object.h"
#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Utilities/src/Common.h"
#include "../../../../Core/Painter/src/Painter.h"

#include "SplatBuffer.h"

#include <qlabel.h>
// debug!
//#include "ImageViewer.h" 


// pack R,G,B components
#define PACK_INT_RGB(_c,_r,_g,_b) \
	_c = (((_r) << 16) & 0x00ff0000) | \
		(((_g) << 8) & 0x0000ff00) | \
		((_b) & 0x000000ff)

// unpack R,G,B components
#define UNPACK_FLOAT_RGB(_r,_g,_b,_c) \
	_r = (float)((_c & 0x00ff0000) >> 16); \
	_g = (float)((_c & 0x0000ff00) >> 8); \
	_b = (float)(_c & 0x000000ff)


SplatBuffer::SplatBuffer (const QSize size, float initCoverageThreshold) {

	bufferSize = size;
	cutoffRadius = 3.f;
	coverageThreshold = initCoverageThreshold;

	buffer = new BufferEntry[size.width() * size.height()];

	// init weight lookup table
	weightTableSize = 1024;
	weightTable = new float[weightTableSize];
	float d = 1.f / ((float)weightTableSize - 1.f);
	for(int i=0; i<weightTableSize; i++) {
		// including pre-normalization factor 1/(2*M_PI)
		weightTable[i] = (float)( 1/(2*M_PI) * exp(-0.5 * (float)i * cutoffRadius*cutoffRadius * d) );
	}

}

SplatBuffer::~SplatBuffer() {

	delete[] buffer;
	delete[] weightTable;
}

void SplatBuffer::clearBuffer() {

	int i, j;
	BufferEntry* bufferEntry;
	Vector3D zero = Vector3D(0,0,0);

	for(i=0; i<bufferSize.height(); i++) {

		for(j=0; j<bufferSize.width(); j++) {
			// clear buffer entry
			bufferEntry = &(buffer[i*bufferSize.width() + j]);
			bufferEntry->weight = 0.f;
			bufferEntry->normal = zero;
			bufferEntry->position = zero;
			bufferEntry->radius = 0.f;
			bufferEntry->r = 0.f;
			bufferEntry->g = 0.f;
			bufferEntry->b = 0.f;
			bufferEntry->uv[0] = 0.f;
			bufferEntry->uv[1] = 0.f;
		}

	}

}

void SplatBuffer::rasterizeSurfel (SurfelInterface* surfel, MyDataTypes::TextureCoordinate uv, float J[4], 
				   float localKernel[4], Vector3D localX, Vector3D localY, float lowPassFilter) {

	// bounding box of the ellipse to be rasterized
	int uMin, uMax, vMin, vMax;
	float lx, ly;
	MyDataTypes::TextureCoordinate centerTexture;
	
	int U, V;
	float u, v;
	float q, dq, ddq;
	
	float w;
	float conic[4], invJ[4];
	float detJ, detConic;
	float splat[4];
	float normalization;
	float newRadius;

	Vector3D surfacePoint;
	Vector3D dSurface_du, dSurface_dv;

	int width = bufferSize.width();
	int height = bufferSize.height();
	BufferEntry* bufferEntry;

	Vector3D centerSurface = surfel->getPosition();
	Vector3D surfaceNormal = surfel->getNormal();
	QRgb color = surfel->getDiffuseColor();
	float r, g, b;
	UNPACK_FLOAT_RGB(r,g,b,color);

	// NOTE: 
	// - the Jacobian J[] stores the partial derivatives J[] = [[dudx,dvdx]' [dudy,dvdy]']
	// - since surfel texture coordinates are in the range [0..1]x[0..1], but the texels have
	// coordinates in the range [0.5..width-0.5]x[0.5..height-0.5] ("texel coordinates"), we 
	// have to scale the texture coordinates to texel coordinates. note that this scaling
	// implements the matrix operation 
	// [dTexelU dTexelV] = [[width-1 0]' [0 height-1]] * [[dudx,dvdx]' [dudy,dvdy]'] * [dx dy] 
	J[0] *= (width-1.f);
	J[1] *= (width-1.f);
	J[2] *= (height-1.f);
	J[3] *= (height-1.f);

        // concatenate the Jacobian with the matrix 'localKernel' describing the ellipse
	// in the local tangent space, i.e., we compute splat = J * localKernel. this results in the 
	// "mapping matrix" of the splat in texture space.
	splat[0] = J[0]*localKernel[0] + J[1]*localKernel[2];
	splat[1] = J[0]*localKernel[1] + J[1]*localKernel[3];
	splat[2] = J[2]*localKernel[0] + J[3]*localKernel[2];
	splat[3] = J[2]*localKernel[1] + J[3]*localKernel[3];

	// scale the surfel texture coordinates to texel coordinates
	// centerTexture = surfel->getTextureCoordinate();
	centerTexture = uv;
	centerTexture.u = centerTexture.u * (width  - 1.0f) + 0.5f;
	centerTexture.v = centerTexture.v * (height - 1.0f) + 0.5f;

	// step 1: calculate the EWA ellipse coefficients
	// compute bounding box
	// NOTE: also consider the low pass filter!!
	splat[0] < 0.0f ? (lx = -splat[0]) : (lx = splat[0]);
	splat[2] < 0.0f ? (ly = -splat[2]) : (ly = splat[2]);
	splat[1] < 0.0f ? (lx -= splat[1]) : (lx += splat[1]);
	splat[3] < 0.0f ? (ly -= splat[3]) : (ly += splat[3]);
	lx += (float)sqrt(lowPassFilter);
	ly += (float)sqrt(lowPassFilter);
	lx *= cutoffRadius;
	ly *= cutoffRadius;
	uMax = (int)(centerTexture.u + lx) + 1;
	uMin = (int)(centerTexture.u - lx) - 1;
	vMax = (int)(centerTexture.v + ly) + 1;
	vMin = (int)(centerTexture.v - ly) - 1;

	// compute the conic matrix of the splat, where conic = ( splat*transpose(splat) + diag(lowPassfilter) )^(-1)
	// NOTE: the *conic* is used for rasterization (not the variance!)	
	// NOTE: we also add the low pass filter here!
	conic[0] = splat[2]*splat[2] + splat[3]*splat[3] + lowPassFilter;
	conic[1] = conic[2] = -(splat[0]*splat[2] + splat[1]*splat[3]);
	conic[3] = splat[0]*splat[0] + splat[1]*splat[1] + lowPassFilter;

	detConic = 1 / (splat[0]*splat[0]*splat[3]*splat[3] + 
			splat[1]*splat[1]*splat[2]*splat[2] - 
			2.0*splat[0]*splat[2]*splat[1]*splat[3] +
			splat[0]*splat[0]*lowPassFilter + 
			splat[1]*splat[1]*lowPassFilter + 
			splat[2]*splat[2]*lowPassFilter + 
			splat[3]*splat[3]*lowPassFilter + 
			lowPassFilter*lowPassFilter );

	conic[0] *= detConic;
	conic[1] *= detConic;
	conic[2] *= detConic;
	conic[3] *= detConic;

	// hack to determine whether the splat is valid:
	// if its condition number (the quotient of the biggest and smallest eigenvalue)
	// is too big (i.e. if quotient of the major and the minor radius of the ellipse
	// is too big), discard it
	float ev0 = 0.5f*conic[0]+0.5f*conic[3]+0.5f*sqrt(conic[0]*conic[0]-2.f*conic[0]*conic[3]+conic[3]*conic[3]+4.f*conic[1]*conic[2]);
	float ev1 = 0.5f*conic[0]+0.5f*conic[3]-0.5f*sqrt(conic[0]*conic[0]-2.f*conic[0]*conic[3]+conic[3]*conic[3]+4.f*conic[1]*conic[2]);
	if(fabs(ev0/ev1) > 200.f || fabs(ev1/ev0) > 200.f)
		return;

	// normalization of the gaussian according to the surface splatting paper. note that if the low pass
	// filter is zero, the normalization factor is exactly one.
	// normalization is det(splat)*sqrt(det(conic)) or 1/det(splat^(-1))*1/sqrt(det(variance)) (Equation 6 
	// in the paper). this is the correct thing (probably the renderer still uses a buggy version, check...)
	normalization = fabs((splat[0]*splat[3]-splat[1]*splat[2])) * fabs(sqrt(detConic));

	// step 2: rasterize the EWA ellipse

	// clipping
	if (uMin < 0) {
		uMin = 0;
		if(uMax < 0)
		    return;
	}
	if (uMax >= bufferSize.width()) {
		uMax = bufferSize.width() - 1;
		if(uMin >=bufferSize.width())
		    return;
	}
	if (vMin < 0) {
		vMin = 0;
		if(vMax < 0)
		    return;
	}
	if (vMax >= bufferSize.height()) {
		vMax = bufferSize.height() - 1;
		if(vMin >= bufferSize.height())
		    return;
	}

	u = ((float)uMin + 0.5f) - centerTexture.u;
	ddq = 2.0f * conic[0];

	// compute a step delta(surface)/delta(U,V) = [localX, localY] * delta(X,Y)/delta(U,V)
	// by multiplying [localX, localY] * inv(J), since inv(J) = delta(X,Y)/delta(U,V)
	detJ = J[0]*J[3] - J[1]*J[2];
	invJ[0] = J[3] / detJ;
	invJ[1] = -J[1] / detJ;
	invJ[2] = -J[2] / detJ;
	invJ[3] = J[0] / detJ;
	dSurface_du[0] = localX[0]*invJ[0] + localY[0]*invJ[2];
	dSurface_du[1] = localX[1]*invJ[0] + localY[1]*invJ[2];
	dSurface_du[2] = localX[2]*invJ[0] + localY[2]*invJ[2];
	dSurface_dv[0] = localX[0]*invJ[1] + localY[0]*invJ[3];
	dSurface_dv[1] = localX[1]*invJ[1] + localY[1]*invJ[3];
	dSurface_dv[2] = localX[2]*invJ[1] + localY[2]*invJ[3];

	// compute the radius of the new splat in the X,Y local frame
	// NOTE: this is a conservative estimate!
	invJ[0] < 0.0f ? (lx = -invJ[0]) : (lx = invJ[0]);
	invJ[2] < 0.0f ? (ly = -invJ[2]) : (ly = invJ[2]);
	invJ[1] < 0.0f ? (lx -= invJ[1]) : (lx += invJ[1]);
	invJ[3] < 0.0f ? (ly -= invJ[3]) : (ly += invJ[3]);
	if(lx > ly) {
		newRadius = lx;
	} else {
		newRadius = ly;
	}

	// ellipse rasterization
	for (V = vMin; V <= vMax; V++) {

		// finite differences for ellipse rasterization
		v  = ((float)V + 0.5f) - centerTexture.v;
		dq = conic[0] * (2 * u + 1) + conic[1] * 2.0f * v;
		q  = (conic[3] * v + conic[1] * 2.0f * u) * v + conic[0] * u * u;

		surfacePoint = centerSurface + dSurface_du * u + dSurface_dv * v;

		for (U = uMin; U <= uMax; U++) {

			if (q < cutoffRadius*cutoffRadius) {

				w = normalization/(2.f*3.1415926f)*exp(-0.5*q);
				bufferEntry = &(buffer[V * width + U]);
				bufferEntry->position = bufferEntry->position + w * surfacePoint;
				bufferEntry->normal += w * surfaceNormal;
				bufferEntry->radius += w * newRadius;
				bufferEntry->r += w * r;
				bufferEntry->g += w * g;
				bufferEntry->b += w * b;
				bufferEntry->weight += w;

				bufferEntry->surfel = surfel;

				bufferEntry->uv[0] += w * centerTexture.u;
				bufferEntry->uv[1] += w * centerTexture.v;
			}

			surfacePoint += dSurface_du;

			q  += dq;
			dq += ddq;
		}
	} 

}

void SplatBuffer::rasterizeSurfelNearestNeighbor (SurfelInterface *surfel, MyDataTypes::TextureCoordinate uv, float J[4], Vector3D localX, Vector3D localY) {

	MyDataTypes::TextureCoordinate centerTexture;
	
	int width = bufferSize.width();
	int height = bufferSize.height();
	BufferEntry* bufferEntry;

	float detJ, invJ[4];
	float lx, ly, radius;

	Vector3D centerSurface = surfel->getPosition();
	Vector3D surfaceNormal = surfel->getNormal();
	QRgb color = surfel->getDiffuseColor();
	(void)color;

	centerTexture.u = uv.u * (width  -1.0f) + 0.5f;
	centerTexture.v = uv.v * (height -1.0f) + 0.5f;

	J[0] *= (width-1.f);
	J[1] *= (height-1.f);
	J[2] *= (width-1.f);
	J[3] *= (height-1.f);

	detJ = J[0]*J[3] - J[1]*J[2];
	invJ[0] = J[3] / detJ;
	invJ[1] = -J[1] / detJ;
	invJ[2] = -J[2] / detJ;
	invJ[3] = J[0] / detJ;

	invJ[0] < 0.0f ? (lx = -invJ[0]) : (lx = invJ[0]);
	invJ[1] < 0.0f ? (ly = -invJ[1]) : (ly = invJ[1]);
	invJ[2] < 0.0f ? (lx -= invJ[2]) : (lx += invJ[2]);
	invJ[3] < 0.0f ? (ly -= invJ[3]) : (ly += invJ[3]);
	if(lx > ly) {
		radius = lx;
	} else {
		radius = ly;
	}


	if( centerTexture.u > 0.0f && centerTexture.u < (width  - 1) && 
		centerTexture.v > 0.0f && centerTexture.v < (height - 1) ) {
		bufferEntry = &(buffer[(int)centerTexture.v * width + (int)centerTexture.u]);
		bufferEntry->position += centerSurface;
		bufferEntry->normal += surfaceNormal;
		bufferEntry->weight += 1;
		bufferEntry->radius = radius;
		bufferEntry->surfel = surfel;
	}

}

bool SplatBuffer::addTextureToActiveObject (Brush* brush, float displacementScaling, bool ellipticalSurfels) {

	int                            i,
		                           j;
	int                            width,
		                           height;

	BufferEntry                    *bufferEntry;
	Scene                          *scene;

	Vector3D dPos_du, dPos_dv;
	
	scene                  = Scene::getInstance();

	width  = bufferSize.width();
	height = bufferSize.height();

	std::vector<SurfelInterface*> surfelGrid (width * height);
	SurfelCollection *objectSurfels = scene->getActiveObject()->getSurfelCollection();

	Painter *painter = new Painter (brush);

	// normalize buffer entries
	float inverseWeight;

	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {

			bufferEntry = &(buffer[i*width + j]);

			// NOTE: the threshold is to check whether the texel lies on the boundary of the surface and
			// possibly should be discarded. this is a heuristic to generate a good boundary of the resampled
			// surface.
			if (bufferEntry->weight > coverageThreshold) {

				// normalize buffer attributes
				inverseWeight = 1.0f / bufferEntry->weight;
				bufferEntry->position = bufferEntry->position * inverseWeight;
				bufferEntry->normal.normalize();
				bufferEntry->r = bufferEntry->r * inverseWeight;
				bufferEntry->g = bufferEntry->g * inverseWeight;
				bufferEntry->b = bufferEntry->b * inverseWeight;
				
				bufferEntry->uv[0] *= inverseWeight;
				bufferEntry->uv[1] *= inverseWeight;

				bufferEntry->radius *= inverseWeight;

				// create new surfel with the same attributes as in the buffer, and add the 
				// new surfel to the objectSurfels collection
				surfelGrid[i * width + j] = objectSurfels->addSurfel (bufferEntry->surfel, false);
	
				// surfel Position					
				scene->setSurfelPosition (surfelGrid[i * width + j], bufferEntry->position, false);
				
				// surfel normal
				scene->setSurfelNormal (surfelGrid[i * width + j], bufferEntry->normal, false);

				// diffuse color
				scene->setSurfelDiffuseColor (surfelGrid[i * width + j], 
					qRgb ((int)bufferEntry->r, (int)bufferEntry->g, (int)bufferEntry->b), false);
			}
			else {
				surfelGrid[i * width + j] = 0;
			}

		}
	}
	
	// modify new surfels
	// the painter also computes surface normals, surfel radii, and surfel tangent axes
	std::vector<MyDataTypes::TextureCoordinate>* empty = NULL;
	if (painter->paint (&surfelGrid, width * height, empty, displacementScaling, 0, 2.f, true)) {

		delete painter;
		return true;								
	}
	else {
		// painting failed, remove surfels			
		for(i=0; i<height; i++) {
			for(j=0; j<width; j++) {
				if(surfelGrid[i * width + j]) {
					objectSurfels->removeSurfel (surfelGrid[i * width + j], false);
				}
			}
		}
		delete painter;
		return false;
	}
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
