//
// Title:   ZBuffer_SurfSplat.cpp
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
// ZBuffer.c 
//-------------------------------------------------------------------------

#include <malloc.h>
#include <string.h>
#include <math.h>

#include "../../../../DataTypes/src/MyDataTypes.h"
#include "../../../../DataTypes/src/Vector3D.h"
#include "../../../../Core/Interfaces/src/SurfelInterface.h"
#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../SurfelGPFrameBuffer.h"

#include "ZBuffer_SurfSplat.h"
#include "Shader_SurfSplat.h"


// Global variables
int zbf_rows, zbf_cols;
float zbf_x_cur, zbf_y_cur;
int zbf_i_cur;
int zbf_xsize, zbf_ysize;
int zbf_material;
SrfZBufItem *zbf_buf;

// Static variables
static float zbf_constThreshold;
static float zbf_distThreshold;
static float zbf_angleThreshold;
static float *zbf_filterLUT;
static int zbf_LUTsize;
static float zbf_cutoffRadius_2;
static float _zbf_cutoffRadius_2;


// Function declarations

//-------------------------------------------------------------------------
// Construct new z-buffer structure
//-------------------------------------------------------------------------
ZBuffer* ZBfNew (int xsize, int ysize, int LUTsize, FrameBufferInterface *frameBuffer)
{
	ZBuffer *zbf;
	int i;

	zbf = (ZBuffer *)malloc(sizeof(ZBuffer));
	zbf->xsize = xsize;
	zbf->ysize = ysize;
	zbf->bufsize = xsize * ysize * sizeof(SrfZBufItem);
	zbf->buf = (SrfZBufItem *)malloc(zbf->bufsize);
	zbf->cleardata = (SrfZBufItem *)calloc(zbf->bufsize, 1);
	zbf->frameBuffer = frameBuffer;
	zbf->options = 0;

	// default values for surface splatting
	zbf->cutoffRadius = 1.f;
	zbf->constThreshold = 0.4f;
	zbf->distThreshold = 2.0f;
	zbf->angleTrheshold = 8.f;

	zbf->LUTsize = LUTsize;
	zbf->filterLUT = (float *)malloc(sizeof(float) * LUTsize);
	ZBfComputeFilterLUT(zbf);

	// init clear data
	for(i=0; i<xsize*ysize; i++)
	{
		// initialize with a 'large' value. to improve...
		zbf->cleardata[i].z = 100000.f;
		zbf->cleardata[i].zMin = 1000000.0f;
		zbf->cleardata[i].zMax = 1000000.0f;
	}

	zbf->splatFunction = ZBfSurfaceSplat;
	ZbfEnable(zbf, ZBF_PERSURFELSHADING);

	return zbf;
}


//-------------------------------------------------------------------------
// Free a z-buffer
//-------------------------------------------------------------------------
void ZBfFree(ZBuffer *zbf)
{
	free(zbf->buf);
	free(zbf->cleardata);
	free(zbf->filterLUT);
	free(zbf);
}


//-------------------------------------------------------------------------
// Compute the filter lookup table
//-------------------------------------------------------------------------
void ZBfComputeFilterLUT(ZBuffer *zbf)
{
	float d;
	int i;

	d = 1.f / ((float)zbf->LUTsize - 1.f);
	// table will be indexed by squared distance
	for(i=0; i<zbf->LUTsize; i++)
	{
		zbf->filterLUT[i] = (float)exp(-(float)i * d * zbf->cutoffRadius*zbf->cutoffRadius);
	}
}

void ZBfScaleAllWeights(ZBuffer *zbf, float scaleWeight) {

	int i, j;

	for(i = 0; i < zbf->xsize; i++) {
		for(j = 0; j < zbf->ysize; j++) {
			zbf->buf[j*zbf->xsize + i].w *= scaleWeight;
			zbf->buf[j*zbf->xsize + i].c[0] *= scaleWeight;
			zbf->buf[j*zbf->xsize + i].c[1] *= scaleWeight;
			zbf->buf[j*zbf->xsize + i].c[2] *= scaleWeight;

		}
	}

}


//-------------------------------------------------------------------------
// Set the size
// note: this is used for progressive rendering. the new size must be
// smaller than the on given to ZBfNew()!
//-------------------------------------------------------------------------
void ZBfSetSize(ZBuffer *zbf, int xsize, int ysize)
{
	zbf->xsize = xsize;
	zbf->ysize = ysize;

	// adjust memory size to make clearing of z-buffer more efficient
	zbf->bufsize = xsize*ysize*sizeof(SrfZBufItem);
}


//-------------------------------------------------------------------------
// Prepare for writing
//-------------------------------------------------------------------------
void ZBfPrepareForWriting(ZBuffer *zbf)
{
	zbf_buf = zbf->buf;
	zbf_xsize = zbf->xsize;
	zbf_ysize = zbf->ysize;
	zbf_filterLUT = zbf->filterLUT;
	zbf_LUTsize = zbf->LUTsize;
	zbf_cutoffRadius_2 = zbf->cutoffRadius*zbf->cutoffRadius;
	_zbf_cutoffRadius_2 = 1/(zbf_cutoffRadius_2);
	zbf_constThreshold = zbf->constThreshold;
	zbf_distThreshold = zbf->distThreshold;
	zbf_angleThreshold = zbf->angleTrheshold;
}


//-------------------------------------------------------------------------
// Prepare for reading
//-------------------------------------------------------------------------
void ZBfPrepareForReading(ZBuffer *zbf)
{
	zbf_buf = zbf->buf;
	zbf_xsize = zbf->xsize;
	zbf_ysize = zbf->ysize;
}


//-------------------------------------------------------------------------
// Clear a z-buffer
//-------------------------------------------------------------------------
void ZBfClear(ZBuffer *zbuf)
{
	memmove(zbuf->buf, zbuf->cleardata, zbuf->bufsize);
}


//-------------------------------------------------------------------------
// Splat a surfel into the z-buffer
//-------------------------------------------------------------------------
int ZBfSurfaceSplat(ZBuffer *zbf, float x0, float y0, float z, float n[3], SurfelInterface *surfel, int l,
				   float scale_stoo, float scale_otoc, float vp_sx, float vp_sy, float vp_tx, float vp_ty, int bbox[4], Object *object)
{
	float V_x, V_y, V_z;		// viewing direction
	float S_x, S_y, S_z;		// S parameter direction on ST plane
	float T_x, T_y, T_z;		// T parameter direction on ST plane
	float Ix_x, Ix_y, Ix_z;		// direction of projection of screen x vector onto ST plane
	float Iy_x, Iy_y, Iy_z;		// direction of projection of screen y vector onto ST plane
	float r, r_, f;		
	float ndotv;				// N*V (normal dot viewing direction) dotproduct
	float sx, sy, tx, ty;		// derivatives of the screen to ST plane mapping
	float m11, m12, m22;
	float a, b, b_2, c;			// the EWA ellipse coefficients
	
	int Xmin, Xmax, Ymin, Ymax;	// bounding box of the ellipse to be rasterized
	float lx, ly;
	int X, Y;
	float x, y;
	float q, dq, ddq;

	float dzc_dxs, dzc_dys;			// dzc/dxs, dzc/dys derivatives
	float z_start, z_cur;			// z values to be rasterized

	int i;
	QRgb color, specularColor;
	float r_comp, g_comp, b_comp;

	float threshold_c0, threshold_c1, threshold_c3;
	float e;
	float w;

	float l_2, _l_2;
	float det_;

	Vector3D normal;
	float _radius;

	int attributes;
	int adjustedBufferHeight;  // = buffer height - 1            

	bool clip;

	//scale z buffer according to surfel radius
	float scale_otoc_radius;

	// init bounding box return value
	bbox[0] = zbf_xsize;
	bbox[1] = zbf_ysize;
	bbox[2] = bbox[3] = 0;

	l_2 = (float)(1 << l);
	_l_2 = 1/l_2;

	// thresholds for the 'merge/separate' decision
	// note: 
	// - these thresholds are dependent on the level of the warped samples, since the
	// distance on the surface between samples increases with the warped level.
	// - these offsets should be constant in units 'dz per unit in object space'. but since 
	// z coordinates in the z-buffer are in camera coordinates, we have to transform the offsets
	// to camera space by applying the 'scale_otoc' (object to camera space) scaling
	threshold_c0 = zbf_constThreshold * l_2 * scale_otoc;		// the basic constant offset
	threshold_c1 = zbf_distThreshold * l_2 * scale_otoc;		// offset dependent on the square distance to the ellipse center q=r^2
	threshold_c3 = zbf_angleThreshold;							// cut off for 'ndotv' used for calculating the derivatives dzc_dxs,dzc_dys

	l_2 *= l_2;

	// step 1: calculate the EWA ellipse coefficients

	// compute normalized viewing vector V
	// restore camera coordinates of projected point (on z=1 plane)
	V_x = -(x0 / vp_sx - vp_tx);
	V_y = -(y0 / vp_sy - vp_ty);
	V_z = -1.f;
	r_ = 1.f / (float)sqrt(V_x*V_x + V_y*V_y + 1.f);
	V_x *= r_; V_y *= r_; V_z *= r_;
	ndotv = n[0]*V_x + n[1]*V_y + n[2]*V_z;

	if(ndotv < 0) {
		if(zbf->options & ZBF_BIDINORMALS) {
			// two-sided lighting, flip normals
			n[0] = - n[0];
			n[1] = - n[1];
			n[2] = - n[2];
			ndotv = -ndotv;
		} else {
			// backface culling
			return 0;
		}
	}

	// project screen x,y unit vectors along V onto ST plane
	// determine direction Ix,Iy of projection of x,y
	Ix_x = -V_z*n[2]-V_y*n[1];		// (X x V) x N
	Ix_y = V_y*n[0];
	Ix_z = V_z*n[0];
	Iy_x = V_x*n[1];				// (Y x V) x N
	Iy_y = -V_x*n[0]-V_z*n[2];
	Iy_z = V_z*n[1];

	// f given by the vector equation Y+g*V=f*Iy resp X+g*V=f*Ix
	// Iy*f resp. Ix*f is the intersection point of Iy,Ix with Y+g*V resp. X+g*V
	// (see SurfaceSplatting.mws)
	f = -1.f/ndotv;

	// dzdx, dzdy derivatives for rasterizing z values on the ellipse
	if(f < -threshold_c3) e = -threshold_c3; else e = f;
	// note: since z coordinates in the z buffer are camera coordinate z-values, we have
	// to transform from screen to camera coordinates (by concatenating the screen-to-object
	// and object-to-camera scalings).
	dzc_dxs = Ix_z*e * scale_stoo*scale_otoc * z;
	dzc_dys = Iy_z*e * scale_stoo*scale_otoc * z;
	e = -e;

	// normalize Ix
	r = (float)sqrt(Ix_x*Ix_x + Ix_y*Ix_y + Ix_z*Ix_z);
	r_ = 1/r;
	S_x = Ix_x * r_;
	S_y = Ix_y * r_;
	S_z = Ix_z * r_;

	// determine T parameter direction on ST plane
	// note: S parameter direction is chosen as Ix
	// T is automatically normalized,  since n and S have unit length
	// and are orthogonal by construction of S
	T_x = n[1]*S_z - n[2]*S_y;		// N x S
	T_y = n[2]*S_x - n[0]*S_z;
	T_z = n[0]*S_y - n[1]*S_x;

	// compute sx, sy, tx, ty derivatives
	// these derivatives build the inverse jacobian inv(J)=[sx,sy | tx,ty]
	// of the mapping J from object surface to screen
	
	_radius = surfel->getRadius();
	scale_otoc_radius = scale_otoc * _radius;
	_radius = 1.0f / _radius;

	Iy_x *= f; Iy_y *= f; Iy_z *= f;		// f*Iy is the intersection point with Y+g*V
	sx = r * f * scale_stoo * z * _radius *_l_2;		// note f given by the vector equation X+g*V=f*Ix takes the same value as above
	tx = 0;
	sy = (Iy_x*S_x + Iy_y*S_y + Iy_z*S_z) * scale_stoo * z * _radius * _l_2;		// Iy*S projects Iy onto S
	ty = (Iy_x*T_x + Iy_y*T_y + Iy_z*T_z) * scale_stoo * z * _radius * _l_2;		// Iy*T projects Iy onto T

	// compute z-range of the reconstruction kernel
	// see ellipseboundingbox_general.mws, idea: compute the point on the ellipse, where the ellipse tangent
	// is perpendicular to the depth gradient, i.e. [dzc_dxs, dzc_dys]. this is the point with maximum depth
	// on the ellipse
	// NOTE: to avoid overshoots in z, we use the z-range of the reconstruction kernel for blending!
	// NOTE: the variable "d" in the maple sheet corresponds to "-zbf_cutoffRadius_2"!
	// first, compute the conic matrix of the reconstruction kernel, which is [a b/2 | b/2 c] = inv(J)^T*inv(J)
	a = sx*sx;
	b_2 = sx*sy;
	b = 2.f * b_2;
	c = sy*sy + ty*ty;
	float discr;
	discr = -4*a*dzc_dxs*b*dzc_dys*c - a*dzc_dys*dzc_dys*b*b + 4*c*dzc_dys*dzc_dys*a*a + 
		4*a*dzc_dxs*dzc_dxs*c*c + b*b*b*dzc_dxs*dzc_dys - b*b*dzc_dxs*dzc_dxs*c;

	float zExtremum_x, zExtremum_y, tmp;
	tmp = (float)sqrt(discr * zbf_cutoffRadius_2);
	zExtremum_x = tmp * (-dzc_dys*b+2*dzc_dxs*c) / discr;
	zExtremum_y = tmp * (-2*dzc_dys*a+dzc_dxs*b) / discr;
 
	float zRange_x, zRange_y;
	tmp = zExtremum_x * dzc_dxs;
	zRange_x = (tmp < 0) ? -tmp : tmp;
	tmp = zExtremum_y * dzc_dys;
	zRange_y = (tmp < 0) ? -tmp : tmp;

	float zMax, zMin;
	zMax = z + zRange_x + zRange_y;
	zMin = z - zRange_x - zRange_y;

	// guarantee a certain minimum z-range, otherwise blending fails for splats exactly parallel to the
	// image plane (the minimum z-range is 1 in object space, and then scaled to camera space).
	if(zMax-zMin < l_2 * scale_otoc_radius) {
		zMax += 0.5f*l_2*scale_otoc_radius;
		zMin -= 0.5f*l_2*scale_otoc_radius;
	}


	// calculate the matrix e[]=inv(J*transpose(J)+I), which describes the
	// EWA ellipse on the screen
	// note: column vectors are used, therefore the calculations are slightly
	// different to Heckbert's notation (cf. Maple file SurfaceSplatting2.mws)

	// the filtered variance matrix m[]
	// m[] = J*transpose(J)+I (details see SurfaceSplatting2.mws)
	r_ = 1.f / (sx*ty);
	r_ *= r_;							// r_ = 1/(sx*ty)^2, note that always tx=0, therefore some terms are missing
	m11 = (ty*ty + sy*sy) * r_ + 1.0f;	// in these formulas compared to the maple sheet
	m12 = -sy*sx * r_;
	m22 = sx*sx * r_ + 1.0f;

	// the filtered conic matric m[]^(-1)
	// matrix(A,B/2,B/2,C) = inv(m[])
	r_ = 1.f / (m11*m22 - m12*m12);
	a = m22 * r_;
	b_2 = -m12 * r_;
	b = b_2 * 2.f;
	c = m11 * r_;

	// calculate the normalization factor for the gaussian filter, which corresponds to the 
	// area of the reconstruction filter in source space, thus it is 
	// 1 / sqrt( det(inv(J)*transpose(inv(J))+I) )
	// note: the factor l_2 compensate for the level of detail of the LDC tree
    det_ = l_2 / (float)sqrt(sx*sx*ty*ty + sx*sx + sy*sy + ty*ty  + 1.f);

	// bounding box of the ellipse
	// see ellipseboundingbox.mws, an exact axis aligned bounding box is computed by finding the points on
	// the ellipse where the tangent of the ellipse is parallel to x- and y-axis respectively.
	// NOTE: the variable "d" in the maple sheet corresponds to "-zbf_cutoffRadius_2"!
	discr = (float)sqrt((-b*b+4*c*a)*zbf_cutoffRadius_2*a);
	ly = 2.f / (-b*b+4*c*a) * discr; 

	discr = (float)sqrt(c*(-b*b+4*c*a)*zbf_cutoffRadius_2);
	lx = 2.f / (-b*b+4*c*a) * discr;

	lx = (lx < 0) ? -lx : lx;
	ly = (ly < 0) ? -ly : ly;
	Xmax = (int)(x0 + lx) + 1;
	Xmin = (int)(x0 - lx);
	Ymax = (int)(y0 + ly) + 1;
	Ymin = (int)(y0 - ly);

	// get surfel color
	color = surfel->getDiffuseColor();
	UNPACK_FLOAT_RGB(r_comp,g_comp,b_comp,color);

	// modify color if surfel is selected and visualization of the selection is enabled
	if(surfel->isFlagOn (SurfelInterface::EMPHASISE) == true && 
	   (zbf->options & ZBF_VISUALIZESELECTION)) {
		if (surfel->isFlagOn (SurfelInterface::SELECTED1) == true) {
			r_comp *= 0.5f;
			g_comp *= 0.5f;
			b_comp = b_comp * 0.5f + 122.0f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED2) == true) {
			r_comp = r_comp * 0.5f + 122.0f;
			g_comp *= 0.5f;
			b_comp *= 0.5f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED3) == true) {
			r_comp *= 0.5f;
			g_comp = g_comp * 0.5f + 122.0f;
			b_comp *= 0.5f;
		}
	}

	// if in shadow adjust color values
	if ((zbf->options & ZBF_SHADOWS) && (surfel->isFlagOn (SurfelInterface::COVERED) == true)) {
		r_comp = 0.0f;
		g_comp = 0.0f;
		b_comp = 0.0f;
	}

	// get material attributes. in case we do per pixel shading, these attributes are written to 
	// the zBuffer, too.
	_shd_kA = surfel->getAmbientCoefficient();
	_shd_kD = surfel->getDiffuseCoefficient();
	_shd_kS = surfel->getSpecularCoefficient();
	_shd_shininess = surfel->getShininess();
	specularColor = surfel->getSpecularColor();
	UNPACK_FLOAT_RGB(_shd_specularColor.r,_shd_specularColor.g,_shd_specularColor.b,specularColor);

	if(zbf->options & ZBF_PERSURFELSHADING) {
		// perform per surfel shading
		// setup variables for shading
		_shd_nx_c = n[0];
		_shd_ny_c = n[1];
		_shd_nz_c = n[2];

		_shd_vx = V_x;
		_shd_vy = V_y;
		_shd_vz = V_z;
		_shd_ndotv = ndotv;

		_shd_Id.r = _shd_Ir.r = r_comp;
		_shd_Id.g = _shd_Ir.g = g_comp;
		_shd_Id.b = _shd_Ir.b = b_comp;

		// perform shading
		if(_shd_options & SHD_LIGHTING)
		{
			_shd_Ir.r = 0.f;
			_shd_Ir.g = 0.f;
			_shd_Ir.b = 0.f;
			_shd_lightsample();
		}

		// re-assign colors for further processing
		r_comp = _shd_Ir.r;
		g_comp = _shd_Ir.g;
		b_comp = _shd_Ir.b;
	}

	// step 2: rasterize the EWA ellipse

	// padding
	clip = false;
	if(Xmin < 0) {
		Xmin = 0;
		clip = true;
		if(Xmax < 0)
			return 0;
	}
	if(Xmax >= zbf_xsize) {
		Xmax = zbf_xsize-1;
		clip = true;
		if(Xmin >= zbf_xsize)
			return 0;
	}
	if(Ymin < 0) {
		Ymin = 0;
		clip = true;
		if(Ymax < 0)
			return 0;
	}
	if(Ymax >= zbf_ysize) {
		Ymax = zbf_ysize-1;
		clip = true;
		if(Ymin >= zbf_ysize)
			return 0;
	}

	// set bounding box (the bounding box is a return value)
	bbox[0] = Xmin; 
	bbox[1] = Ymin;
	bbox[2] = Xmax;
	bbox[3] = Ymax;

	// z value in the lower left corner of the rasterized area
	z_start = z - dzc_dxs*(x0-((float)Xmin+0.5f)) - dzc_dys*(y0-((float)Ymin+0.5f));

	x = ((float)Xmin+0.5f) - x0;
	ddq = 2*a;

	// write splat data to framebuffer if required
	attributes = zbf->frameBuffer->getAttributes();

	if(attributes & FrameBufferInterface::ALL_SPLATS && clip == false) {
		zbf->frameBuffer->setSplatInfo(surfel, x0, y0, z, a, b_2, c, bbox);
	}

	// *********************
	// ellipse rasterization
	// *********************
	if( (attributes & FrameBufferInterface::PERPIXEL_C_Z_N_W) || (attributes & FrameBufferInterface::PERPIXEL_SURFELLISTS) ) 
	{

		adjustedBufferHeight = zbf->ysize - 1;

		for(Y=Ymin; Y<=Ymax; Y++)
		{
			// finite differences for ellipse rasterization
			y = ((float)Y+0.5f) - y0;
			dq = a*(2*x+1)+b*y;
			q = (c*y + b*x)*y + a*x*x;

			// init z value
			z_cur = z_start;

			for(X=Xmin; X<=Xmax; X++)
			{
				i = X + zbf_xsize*Y;

				if(q<zbf_cutoffRadius_2)
				{

					// compare z-ranges
					if( !(zbf->buf[i].zMax < zMin || zMax < zbf->buf[i].zMin) )
					{
						// z-ranges overlap
						// increase z-range if necessary
						zbf->buf[i].zMin = (zMin < zbf->buf[i].zMin) ? zMin : zbf->buf[i].zMin;
						zbf->buf[i].zMax = (zMax > zbf->buf[i].zMax) ? zMax : zbf->buf[i].zMax;

						// merge contributions
						w = zbf_filterLUT[(int)(q*_zbf_cutoffRadius_2 * zbf_LUTsize)] * det_;

						zbf->buf[i].w += w;

						// add color contribution
						zbf->buf[i].c[0] += r_comp*w;
						zbf->buf[i].c[1] += g_comp*w;
						zbf->buf[i].c[2] += b_comp*w;

						// normals
						zbf->buf[i].n[0] += n[0] * w;
						zbf->buf[i].n[1] += n[1] * w;
						zbf->buf[i].n[2] += n[2] * w;

						// z 
						zbf->buf[i].z += z_cur * w;

						// per pixel shading
						if(!(zbf->options & ZBF_PERSURFELSHADING)) {
							zbf->buf[i].kA = _shd_kA;
							zbf->buf[i].kD = _shd_kD;
							zbf->buf[i].kS = _shd_kS;
							zbf->buf[i].shininess = _shd_shininess;
							zbf->buf[i].specularColor = specularColor;
						}

						// *******************************
						// write additional per pixel data
						// *******************************

						if (attributes & FrameBufferInterface::PERPIXEL_SURFELLISTS) {
							zbf->frameBuffer->addVisibleSurfel (X, adjustedBufferHeight - Y, surfel);
						}
						if (attributes & FrameBufferInterface::PERPIXEL_SURFEL) {
							zbf->buf[i].userData = surfel;
						}
						if (attributes & FrameBufferInterface::PERPIXEL_OBJECTPOINTER) {
							zbf->frameBuffer->setObjectAtPixel (X, adjustedBufferHeight - Y, object);
						}

					}
					else if (zMin < zbf->buf[i].zMin) {
						// new z-range does not overlap previous one, but is closer to viewer
						// update z-range
						zbf->buf[i].zMin = zMin;
						zbf->buf[i].zMax = zMax;

						// new frontmost contribution
						w = zbf_filterLUT[(int)(q*_zbf_cutoffRadius_2 * zbf_LUTsize)] * det_;
						zbf->buf[i].w = w;
						
						// add color contribution
						zbf->buf[i].c[0] = r_comp*w;
						zbf->buf[i].c[1] = g_comp*w;
						zbf->buf[i].c[2] = b_comp*w;

						zbf->buf[i].n[0] = n[0] * w;
						zbf->buf[i].n[1] = n[1] * w;
						zbf->buf[i].n[2] = n[2] * w;

						// update z value
						zbf->buf[i].z = z_cur * w;

						// per pixel shading
						if(!(zbf->options & ZBF_PERSURFELSHADING)) {
							zbf->buf[i].kA = _shd_kA;
							zbf->buf[i].kD = _shd_kD;
							zbf->buf[i].kS = _shd_kS;
							zbf->buf[i].shininess = _shd_shininess;
							zbf->buf[i].specularColor = specularColor;
						}

						// **************************************
						// reset per pixel data, write new values
						// **************************************

						if (attributes & FrameBufferInterface::PERPIXEL_SURFELLISTS) {
							zbf->frameBuffer->resetPosition (X, adjustedBufferHeight - Y);
							zbf->frameBuffer->addVisibleSurfel (X, adjustedBufferHeight - Y, surfel);
						}
						if (attributes & FrameBufferInterface::PERPIXEL_SURFEL) {
							zbf->buf[i].userData = surfel;
						}
						if (attributes & FrameBufferInterface::PERPIXEL_OBJECTPOINTER) {
							zbf->frameBuffer->setObjectAtPixel (X, adjustedBufferHeight - Y, object);
						}

					}
				} 

				q += dq;
				dq += ddq;

				z_cur += dzc_dxs;
			}
			z_start += dzc_dys;
		}
	}

	return 0;
}


//-------------------------------------------------------------------------
// Splat a surfel into the z-buffer
//-------------------------------------------------------------------------
int ZBfSurfaceSplatEllipse(ZBuffer *zbf,
						   float x0,
						   float y0,
						   float x_c,
						   float y_c,
						   float z_c,
						   float n[3], 
						   float tan1[3],
						   float tan2[3],
						   SurfelInterface *surfel,
						   int l,
				           float scale_stoo,
						   float scale_otoc,
						   float vp_sx,
						   float vp_sy,
						   float vp_tx,
						   float vp_ty,
						   int bbox[4],
						   Object *object,
						   float *clipAxes, 
						   int numClipAxes)
{
	float V_x, V_y, V_z;		// viewing direction
	float Ix_x, Ix_y, Ix_z;		// direction of projection of screen x vector onto ST plane
	float Iy_x, Iy_y, Iy_z;		// direction of projection of screen y vector onto ST plane
	float r, r_, f;		
	float ndotv;				// N*V (normal dot viewing direction) dotproduct
	float sx, sy, tx, ty;		// derivatives of the screen to ST plane mapping
	float m11, m12, m22;
	float a, b, b_2, c;			// the EWA ellipse coefficients
	
	int Xmin, Xmax, Ymin, Ymax;	// bounding box of the ellipse to be rasterized
	float lx, ly;
	int X, Y;
	float x, y;
	float q, dq, ddq;

	float dzc_dxs, dzc_dys;			// dzc/dxs, dzc/dys derivatives
	float z_start, z_cur;			// z values to be rasterized

	int i;
	QRgb color, specularColor;
	float r_comp, g_comp, b_comp;

	float threshold_c0, threshold_c1, threshold_c3;
	float e;
	float w;

	float l_2, _l_2;
	float det_;

	Vector3D normal;

	float tmp;

	int attributes;
	int adjustedBufferHeight;  // = buffer height - 1            

	bool clip;

	int cIt;      // clip iteration
	bool clipIt;  // flag if pixel should be clipped by clip axis
	
	zbf_cutoffRadius_2 = zbf->cutoffRadius*zbf->cutoffRadius;
	_zbf_cutoffRadius_2 = 1.0f/(zbf_cutoffRadius_2);


	// init bounding box return value
	bbox[0] = zbf_xsize;
	bbox[1] = zbf_ysize;
	bbox[2] = bbox[3] = 0;

	l_2 = (float)(1 << l);
	_l_2 = 1/l_2;

	// thresholds for the 'merge/separate' decision
	// note: 
	// - these thresholds are dependent on the level of the warped samples, since the
	// distance on the surface between samples increases with the warped level.
	// - these offsets should be constant in units 'dz per unit in object space'. but since 
	// z coordinates in the z-buffer are in camera coordinates, we have to transform the offsets
	// to camera space by applying the 'scale_otoc' (object to camera space) scaling
	threshold_c0 = zbf_constThreshold * l_2 * scale_otoc;		// the basic constant offset
	threshold_c1 = zbf_distThreshold * l_2 * scale_otoc;		// offset dependent on the square distance to the ellipse center q=r^2
	threshold_c3 = zbf_angleThreshold;							// cut off for 'ndotv' used for calculating the derivatives dzc_dxs,dzc_dys

	l_2 *= l_2;

	// step 1: calculate the EWA ellipse coefficients

	// compute normalized viewing vector V
	// restore camera coordinates of projected point (on z=1 plane)
	V_x = -(x0 / vp_sx - vp_tx);
	V_y = -(y0 / vp_sy - vp_ty);
	V_z = -1.f;
	r_ = 1.f / (float)sqrt(V_x*V_x + V_y*V_y + 1.f);
	V_x *= r_; V_y *= r_; V_z *= r_;
	ndotv = n[0]*V_x + n[1]*V_y + n[2]*V_z;


	if ((numClipAxes > 0) && (ndotv < 0.1)) {
		return 0;
	}

	if(ndotv < 0) {
		if(zbf->options & ZBF_BIDINORMALS) {
			// two-sided lighting, flip normals
			n[0] = - n[0];
			n[1] = - n[1];
			n[2] = - n[2];
			ndotv = -ndotv;
		} else {
			// backface culling
			return 0;
		}
	}

	// project screen x,y unit vectors along V onto ST plane
	// determine direction Ix,Iy of projection of x,y
	Ix_x = -V_z*n[2]-V_y*n[1];		// (X x V) x N
	Ix_y = V_y*n[0];
	Ix_z = V_z*n[0];
	Iy_x = V_x*n[1];				// (Y x V) x N
	Iy_y = -V_x*n[0]-V_z*n[2];
	Iy_z = V_z*n[1];

	// f given by the vector equation Y+g*V=f*Iy resp X+g*V=f*Ix
	// Iy*f resp. Ix*f is the intersection point of Iy,Ix with Y+g*V resp. X+g*V
	// (see SurfaceSplatting.mws)
	f = -1.f/ndotv;

	// dzdx, dzdy derivatives for rasterizing z values on the ellipse
	if(f < -threshold_c3) e = -threshold_c3; else e = f;
	// note: since z coordinates in the z buffer are camera coordinate z-values, we have
	// to transform from screen to camera coordinates (by concatenating the screen-to-object
	// and object-to-camera scalings).
	dzc_dxs = Ix_z*e * scale_stoo*scale_otoc * z_c;
	dzc_dys = Iy_z*e * scale_stoo*scale_otoc * z_c;
	e = -e;

	// normalize Ix
	r = (float)sqrt(Ix_x*Ix_x + Ix_y*Ix_y + Ix_z*Ix_z);
	r_ = 1/r;

	// compute the inverse of the jacobian invJ = [sx sy | tx ty]. this is done by perspectively projecting the 
	// tangent plane axes (which define the object space ellipse) to screen space. the jacobian is a linear 
	// approximation of this mapping, consisting of the partial derivatives of the mapping. 
	// NOTE: for more details on these computations see the maple sheet "ewa-estimate.mws"!
	tmp = 1.f / (-tan1[0]*z_c*tan2[1]+tan1[0]*tan2[2]*y_c+tan1[2]*x_c*tan2[1]+tan2[0]*z_c*tan1[1]-tan2[0]*tan1[2]*y_c-tan2[2]*x_c*tan1[1]);	
	sx = (tan2[1]*z_c-y_c*tan2[2])*z_c * tmp * scale_stoo;
	sy = (-tan2[0]*z_c+tan2[2]*x_c)*z_c * tmp * scale_stoo;
	tx = -(tan1[1]*z_c-tan1[2]*y_c)*z_c * tmp * scale_stoo;
	ty = -(-tan1[0]*z_c+tan1[2]*x_c)*z_c * tmp * scale_stoo;

	// compute z-range of the reconstruction kernel
	// see ellipseboundingbox_general.mws, idea: compute the point on the ellipse, where the ellipse tangent
	// is perpendicular to the depth gradient, i.e. [dzc_dxs, dzc_dys]. this is the point with maximum depth
	// on the ellipse
	// NOTE: to avoid overshoots in z, we use the z-range of the reconstruction kernel for blending!
	// NOTE: the variable "d" in the maple sheet corresponds to "-zbf_cutoffRadius_2"!
	// first, compute the conic matrix of the reconstruction kernel, which is [a b/2 | b/2 c] = inv(J)^T*inv(J)
	a = sx*sx + tx*tx;
	b_2 = sx*sy + tx*ty;
	b = 2.f * b_2;
	c = sy*sy + ty*ty;
	float discr;
	discr = -4*a*dzc_dxs*b*dzc_dys*c - a*dzc_dys*dzc_dys*b*b + 4*c*dzc_dys*dzc_dys*a*a + 
		4*a*dzc_dxs*dzc_dxs*c*c + b*b*b*dzc_dxs*dzc_dys - b*b*dzc_dxs*dzc_dxs*c;

	float zExtremum_x, zExtremum_y;
	tmp = (float)sqrt(discr * zbf_cutoffRadius_2);
	zExtremum_x = tmp * (-dzc_dys*b+2*dzc_dxs*c) / discr;
	zExtremum_y = tmp * (-2*dzc_dys*a+dzc_dxs*b) / discr;
 
	float zRange_x, zRange_y;
	tmp = zExtremum_x * dzc_dxs;
	zRange_x = (tmp < 0) ? -tmp : tmp;
	tmp = zExtremum_y * dzc_dys;
	zRange_y = (tmp < 0) ? -tmp : tmp;

	float zMax, zMin;
	zMax = z_c + zRange_x + zRange_y;
	zMin = z_c - zRange_x - zRange_y;



	// guarantee a certain minimum z-range, otherwise blending fails for splats exactly parallel to the
	// image plane (the minimum z-range is 1 in object space, and then scaled to camera space).
	float scale_otoc_radius = scale_otoc * surfel->getRadius();
	if(zMax-zMin < l_2 * scale_otoc_radius) {
		zMax += 0.5f*l_2*scale_otoc_radius;
		zMin -= 0.5f*l_2*scale_otoc_radius;
	}

	// calculate the matrix e[]=inv(J*transpose(J)+I), which describes the
	// EWA ellipse on the screen
	// note: column vectors are used, therefore the calculations are slightly
	// different to Heckbert's notation (cf. Maple file SurfaceSplatting2.mws)

	// the filtered variance matrix m[]
	// m[] = J*transpose(J)+I (details see SurfaceSplatting2.mws)
	r_ = 1.f / (sx*ty - sy*tx);
	r_ *= r_;							// r_ = 1/(sx*ty)^2, note that always tx=0, therefore some terms are missing
	m11 = (ty*ty + sy*sy) * r_ + 1.0f;	// in these formulas compared to the maple sheet
	m12 = (-ty*tx -sy*sx) * r_;
	m22 = (tx*tx + sx*sx) * r_ + 1.0f;

	// the filtered conic matric m[]^(-1)
	// matrix(A,B/2,B/2,C) = inv(m[])
	r_ = 1.f / (m11*m22 - m12*m12);
	a = m22 * r_;
	b_2 = -m12 * r_;
	b = b_2 * 2.f;
	c = m11 * r_;

	// calculate the normalization factor for the gaussian filter, which corresponds to the 
	// area of the reconstruction filter in source space, thus it is 
	// 1 / sqrt( det(inv(J)*transpose(inv(J))+I) )
	// note: the factor l_2 compensate for the level of detail of the LDC tree
    det_ = l_2 / (float)sqrt(sx*sx*ty*ty + sx*sx + sy*sy*tx*tx + sy*sy + tx*tx + ty*ty  + 1.f - 2*sx*tx*sy*ty);

	// bounding box of the ellipse
	// see ellipseboundingbox.mws, an exact axis aligned bounding box is computed by finding the points on
	// the ellipse where the tangent of the ellipse is parallel to x- and y-axis respectively.
	// NOTE: the variable "d" in the maple sheet corresponds to "-zbf_cutoffRadius_2"!
	discr = (float)sqrt((-b*b+4*c*a)*zbf_cutoffRadius_2*a);
	ly = 2.f / (-b*b+4*c*a) * discr; 

	discr = (float)sqrt(c*(-b*b+4*c*a)*zbf_cutoffRadius_2);
	lx = 2.f / (-b*b+4*c*a) * discr;

	lx = (lx < 0) ? -lx : lx;
	ly = (ly < 0) ? -ly : ly;
	Xmax = (int)(x0 + lx) + 1;
	Xmin = (int)(x0 - lx);
	Ymax = (int)(y0 + ly) + 1;
	Ymin = (int)(y0 - ly);

	// get surfel color
	color = surfel->getDiffuseColor();
	UNPACK_FLOAT_RGB(r_comp,g_comp,b_comp,color);

	// modify color if surfel is selected and visualization of the selection is enabled
	if(surfel->isFlagOn (SurfelInterface::EMPHASISE) == true && 
	   (zbf->options & ZBF_VISUALIZESELECTION)) {
		if (surfel->isFlagOn (SurfelInterface::SELECTED1) == true) {
			r_comp *= 0.5f;
			g_comp *= 0.5f;
			b_comp = b_comp * 0.5f + 122.0f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED2) == true) {
			r_comp = r_comp * 0.5f + 122.0f;
			g_comp *= 0.5f;
			b_comp *= 0.5f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED3) == true) {
			r_comp *= 0.5f;
			g_comp = g_comp * 0.5f + 122.0f;
			b_comp *= 0.5f;
		}
	}

	// if in shadow adjust color values
	if ((zbf->options & ZBF_SHADOWS) && (surfel->isFlagOn (SurfelInterface::COVERED) == true)) {
		r_comp = 0.0f;
		g_comp = 0.0f;
		b_comp = 0.0f;
	}

	// get material attributes. in case we do per pixel shading, these attributes are written to 
	// the zBuffer, too.
	_shd_kA = surfel->getAmbientCoefficient();
	_shd_kD = surfel->getDiffuseCoefficient();
	_shd_kS = surfel->getSpecularCoefficient();
	_shd_shininess = surfel->getShininess();
	specularColor = surfel->getSpecularColor();
	UNPACK_FLOAT_RGB(_shd_specularColor.r,_shd_specularColor.g,_shd_specularColor.b,specularColor);

	if(zbf->options & ZBF_PERSURFELSHADING) {
		// perform per surfel shading
		// setup variables for shading
		_shd_nx_c = n[0];
		_shd_ny_c = n[1];
		_shd_nz_c = n[2];

		_shd_vx = V_x;
		_shd_vy = V_y;
		_shd_vz = V_z;
		_shd_ndotv = ndotv;

		_shd_Id.r = _shd_Ir.r = r_comp;
		_shd_Id.g = _shd_Ir.g = g_comp;
		_shd_Id.b = _shd_Ir.b = b_comp;

		// perform shading
		if(_shd_options & SHD_LIGHTING)
		{
			_shd_Ir.r = 0.f;
			_shd_Ir.g = 0.f;
			_shd_Ir.b = 0.f;
			_shd_lightsample();
		}

		// re-assign colors for further processing
		r_comp = _shd_Ir.r;
		g_comp = _shd_Ir.g;
		b_comp = _shd_Ir.b;
	}

	// step 2: rasterize the EWA ellipse

	// padding
	clip = false;
	if(Xmin < 0) {
		Xmin = 0;
		clip = true;
		if(Xmax < 0)
			return 0;
	}
	if(Xmax >= zbf_xsize) {
		Xmax = zbf_xsize-1;
		clip = true;
		if(Xmin >= zbf_xsize)
			return 0;
	}
	if(Ymin < 0) {
		Ymin = 0;
		clip = true;
		if(Ymax < 0)
			return 0;
	}
	if(Ymax >= zbf_ysize) {
		Ymax = zbf_ysize-1;
		clip = true;
		if(Ymin >= zbf_ysize)
			return 0;
	}

	// set bounding box (the bounding box is a return value)
	bbox[0] = Xmin; 
	bbox[1] = Ymin;
	bbox[2] = Xmax;
	bbox[3] = Ymax;

	// z value in the lower left corner of the rasterized area
	z_start = z_c - dzc_dxs*(x0-((float)Xmin+0.5f)) - dzc_dys*(y0-((float)Ymin+0.5f));

	x = ((float)Xmin+0.5f) - x0;
	ddq = 2*a;

	// write splat data to framebuffer if required
	attributes = zbf->frameBuffer->getAttributes();

	if(attributes & FrameBufferInterface::ALL_SPLATS && clip == false) {
		zbf->frameBuffer->setSplatInfo(surfel, x0, y0, z_c, a, b_2, c, bbox);
	}

	// *********************
	// ellipse rasterization
	// *********************
	if( (attributes & FrameBufferInterface::PERPIXEL_C_Z_N_W) || (attributes & FrameBufferInterface::PERPIXEL_SURFELLISTS) ) 
	{

		adjustedBufferHeight = zbf->ysize - 1;

		for(Y=Ymin; Y<=Ymax; Y++)
		{
			// finite differences for ellipse rasterization
			y = ((float)Y+0.5f) - y0;
			dq = a*(2*x+1)+b*y;
			q = (c*y + b*x)*y + a*x*x;

			// init z value
			z_cur = z_start;

			for(X=Xmin; X<=Xmax; X++)
			{
				i = X + zbf_xsize*Y;

				if(q<zbf_cutoffRadius_2)
				{
					
					clipIt = false;
					// clip against clip axes
					for (cIt = 0; cIt < numClipAxes; cIt++) {
						
						if ((X - clipAxes[cIt*4]) * clipAxes[cIt*4+2] + (Y - clipAxes[cIt*4+1]) * clipAxes[cIt*4+3] > 0) {
							clipIt = true;
							break;
						}
					}
					if (!clipIt) {
																		
						// compare z-ranges
						if( !(zbf->buf[i].zMax < zMin || zMax < zbf->buf[i].zMin) )
						{
							// z-ranges overlap
							// increase z-range if necessary
							zbf->buf[i].zMin = (zMin < zbf->buf[i].zMin) ? zMin : zbf->buf[i].zMin;
							zbf->buf[i].zMax = (zMax > zbf->buf[i].zMax) ? zMax : zbf->buf[i].zMax;
							
							// merge contributions
							w = zbf_filterLUT[(int)(q*_zbf_cutoffRadius_2 * zbf_LUTsize)] * det_;

							zbf->buf[i].w += w;
							
							// add color contribution
							zbf->buf[i].c[0] += r_comp*w;
							zbf->buf[i].c[1] += g_comp*w;
							zbf->buf[i].c[2] += b_comp*w;
							
							// normals
							zbf->buf[i].n[0] += n[0] * w;
							zbf->buf[i].n[1] += n[1] * w;
							zbf->buf[i].n[2] += n[2] * w;
							
							// z 
							zbf->buf[i].z += z_cur * w;
							
							// per pixel shading
							if(!(zbf->options & ZBF_PERSURFELSHADING)) {
								zbf->buf[i].kA = _shd_kA;
								zbf->buf[i].kD = _shd_kD;
								zbf->buf[i].kS = _shd_kS;
								zbf->buf[i].shininess = _shd_shininess;
								zbf->buf[i].specularColor = specularColor;
							}
							
							// *******************************
							// write additional per pixel data
							// *******************************
							
							if (attributes & FrameBufferInterface::PERPIXEL_SURFELLISTS) {
								zbf->frameBuffer->addVisibleSurfel (X, adjustedBufferHeight - Y, surfel);
							}
							if (attributes & FrameBufferInterface::PERPIXEL_SURFEL) {
								zbf->buf[i].userData = surfel;
							}
							if (attributes & FrameBufferInterface::PERPIXEL_OBJECTPOINTER) {
								zbf->frameBuffer->setObjectAtPixel (X, adjustedBufferHeight - Y, object);
							}
							
						}
						//					else if (z_cur < zbf->buf[i].z) {
						else if (zMin < zbf->buf[i].zMin) {
							// new z-range does not overlap previous one, but is closer to viewer
							// update z-range
							zbf->buf[i].zMin = zMin;
							zbf->buf[i].zMax = zMax;
							
							// new frontmost contribution
								w = zbf_filterLUT[(int)(q*_zbf_cutoffRadius_2 * zbf_LUTsize)] * det_;

							zbf->buf[i].w = w;
							
							// add color contribution
							zbf->buf[i].c[0] = r_comp*w;
							zbf->buf[i].c[1] = g_comp*w;
							zbf->buf[i].c[2] = b_comp*w;
							
							zbf->buf[i].n[0] = n[0] * w;
							zbf->buf[i].n[1] = n[1] * w;
							zbf->buf[i].n[2] = n[2] * w;
							
							// update z value
							zbf->buf[i].z = z_cur * w;
							
							// per pixel shading
							if(!(zbf->options & ZBF_PERSURFELSHADING)) {
								zbf->buf[i].kA = _shd_kA;
								zbf->buf[i].kD = _shd_kD;
								zbf->buf[i].kS = _shd_kS;
								zbf->buf[i].shininess = _shd_shininess;
								zbf->buf[i].specularColor = specularColor;
							}
							
							// **************************************
							// reset per pixel data, write new values
							// **************************************
							
							if (attributes & FrameBufferInterface::PERPIXEL_SURFELLISTS) {
								zbf->frameBuffer->resetPosition (X, adjustedBufferHeight - Y);
								zbf->frameBuffer->addVisibleSurfel (X, adjustedBufferHeight - Y, surfel);
							}
							if (attributes & FrameBufferInterface::PERPIXEL_SURFEL) {
								zbf->buf[i].userData = surfel;
							}
							if (attributes & FrameBufferInterface::PERPIXEL_OBJECTPOINTER) {
								zbf->frameBuffer->setObjectAtPixel (X, adjustedBufferHeight - Y, object);
							}
							
						}
					} 
				}

				q += dq;
				dq += ddq;

				z_cur += dzc_dxs;
			}
			z_start += dzc_dys;
		}
	}

	return 0;
}


//-------------------------------------------------------------------------
// Splat surfel using nearest neighbor interpolation
//-------------------------------------------------------------------------
int ZBfNearestNeighbor(ZBuffer *zbf, float x, float y, float z, float n[3], SurfelInterface *surfel, int level,
				   float stoo_scaling, float otoc_scaling, float sx, float sy, float tx, float ty, int bbox[4], Object* object)
{
	// dummy function for compatibility with srf.c/.h
	return 0;
}


//-------------------------------------------------------------------------
// Clear z buffer pixel
//-------------------------------------------------------------------------
void ZBfClearPixel(ZBuffer *zbf, int x, int y)
{
	SrfZBufItem* zbfItem;

	zbfItem = &(zbf->buf[y*zbf->xsize + x]);

	zbfItem->z = 100000.f;
	zbfItem->w = -1.f;
	zbfItem->n[0] = 0.f;
	zbfItem->n[1] = 0.f;
	zbfItem->n[0] = 0.f;

	zbfItem->c[0] = 0.f;
	zbfItem->c[1] = 0.f;
	zbfItem->c[2] = 0.f;
	zbfItem->alpha = 0;
}


#if 0
//-------------------------------------------------------------------------
// Write the z buffer to a file
//-------------------------------------------------------------------------
void ZBfWriteToFile(ZBuffer *zbf, char *name)
{
	FILE * f;
	int x, y, i;

	f = fopen(name, "wb");

	// write dimensions
	// fwrite(&(zbf->xsize), 1, sizeof(int), f);
	// fwrite(&(zbf->ysize), 1, sizeof(int), f);
	fprintf(f, "%i %i \n", zbf->xsize, zbf->ysize);

	// write z values
	for (y=0; y<zbf->ysize; y++) 
	{
		for (x=0; x<zbf->xsize; x++) 
		{
			// flip around vertical axis!
			i = zbf->xsize-1 - x + y * zbf->xsize;
			fprintf(f, "%.12f ", zbf->buf[i].z);
		}
	}

	fclose(f);
}
#endif

//-------------------------------------------------------------------------
// Get the filterLUT
//-------------------------------------------------------------------------
float *ZBfGetFilterLUT() {
	return zbf_filterLUT;
}

//-------------------------------------------------------------------------
// Get the filterLUT size
//-------------------------------------------------------------------------
int ZBfGetFilterLUTSize() {
	return zbf_LUTsize;
}


//-------------------------------------------------------------------------
// Splat a surfel into the z-buffer
//-------------------------------------------------------------------------
int ZBfSurfaceUnSplat(ZBuffer *zbf, float x0, float y0, float z, float n[3], SurfelInterface *surfel, int l,
				      float scale_stoo, float scale_otoc, float vp_sx, float vp_sy, float vp_tx, float vp_ty, int bbox[4], Object* object)
{
	float V_x, V_y, V_z;		// viewing direction
	float S_x, S_y, S_z;		// S parameter direction on ST plane
	float T_x, T_y, T_z;		// T parameter direction on ST plane
	float Ix_x, Ix_y, Ix_z;		// direction of projection of screen x vector onto ST plane
	float Iy_x, Iy_y, Iy_z;		// direction of projection of screen y vector onto ST plane
	float r, r_, f;		
	float ndotv;				// N*V (normal dot viewing direction) dotproduct
	float sx, sy, tx, ty;		// derivatives of the screen to ST plane mapping
	float m11, m12, m22;
	float a, b, b_2, c;			// the EWA ellipse coefficients
	
	int Xmin, Xmax, Ymin, Ymax;	// bounding box of the ellipse to be rasterized
	float lx, ly;
	int X, Y;
	float x, y;
	float q, dq, ddq;

	float dzc_dxs, dzc_dys;		// dzc/dxs, dzc/dys derivatives
	float z_start, z_cur;	    // z values to be rasterized

	int i;
	QRgb  color, specularColor;
	float r_comp, g_comp, b_comp;

	float threshold_c0, threshold_c1, threshold_c3;
	float e;
	float w;

	float l_2, _l_2;
	float det_;

	Vector3D						splatCoordinate;
	Vector3D						normal;
	float							_radius;

	int adjustedBufferHeight;	// = buffer height - 1 
	bool clip;

	// init bounding box return value
	bbox[0] = zbf_xsize;
	bbox[1] = zbf_ysize;
	bbox[2] = bbox[3] = 0;

	l_2 = (float)(1 << l);
	_l_2 = 1/l_2;

	// thresholds for the 'merge/separate' decision
	// note: 
	// - these thresholds are dependent on the level of the warped samples, since the
	// distance on the surface between samples increases with the warped level.
	// - these offsets should be constant in units 'dz per unit in object space'. but since 
	// z coordinates in the z-buffer are in camera coordinates, we have to transform the offsets
	// to camera space by applying the 'scale_otoc' (object to camera space) scaling
	threshold_c0 = 0.4f * l_2 * scale_otoc;		// the basic constant offset
	threshold_c1 = 2.0f * l_2 * scale_otoc;		// offset dependent on the square distance to the ellipse center q=r^2
	threshold_c3 = 8.f;							// cut off for 'ndotv' used for calculating the derivatives dzc_dxs,dzc_dys

	l_2 *= l_2;


	// step 1: calculate the EWA ellipse coefficients

	// compute normalized viewing vector V
	// restore camera coordinates of projected point (on z=1 plane)
	V_x = -(x0 / vp_sx - vp_tx);
	V_y = -(y0 / vp_sy - vp_ty);
	V_z = -1.f;
	r_ = 1.f / (float)sqrt(V_x*V_x + V_y*V_y + 1.f);
	V_x *= r_; V_y *= r_; V_z *= r_;
	ndotv = n[0]*V_x + n[1]*V_y + n[2]*V_z;

	if(ndotv < 0) {
		if(zbf->options & ZBF_BIDINORMALS) {
			// two-sided lighting, flip normals
			n[0] = - n[0];
			n[1] = - n[1];
			n[2] = - n[2];
			ndotv = -ndotv;
		} else {
			// backface culling
			return 0;
		}
	}

	// project screen x,y unit vectors along V onto ST plane
	// determine direction Ix,Iy of projection of x,y
	Ix_x = -V_z*n[2]-V_y*n[1];		// (X x V) x N
	Ix_y = V_y*n[0];
	Ix_z = V_z*n[0];
	Iy_x = V_x*n[1];				// (Y x V) x N
	Iy_y = -V_x*n[0]-V_z*n[2];
	Iy_z = V_z*n[1];

	// f given by the vector equation Y+g*V=f*Iy resp X+g*V=f*Ix
	// Iy*f resp. Ix*f is the intersection point of Iy,Ix with Y+g*V resp. X+g*V
	// (see SurfaceSplatting.mws)
	f = -1.f/ndotv;

	// dzdx, dzdy derivatives for rasterizing z values on the ellipse
	if(f < -threshold_c3) e = -threshold_c3; else e = f;
	// note: since z coordinates in the z buffer are camera coordinate z-values, we have
	// to transform from screen to camera coordinates (by concatenating the screen-to-object
	// and object-to-camera scalings).
	dzc_dxs = Ix_z*e * scale_stoo*scale_otoc * z;
	dzc_dys = Iy_z*e * scale_stoo*scale_otoc * z;
	e = -e;

	// normalize Ix
	r = (float)sqrt(Ix_x*Ix_x + Ix_y*Ix_y + Ix_z*Ix_z);
	r_ = 1/r;
	S_x = Ix_x * r_;
	S_y = Ix_y * r_;
	S_z = Ix_z * r_;

	// determine T parameter direction on ST plane
	// note: S parameter direction is chosen as Ix
	// T is automatically normalized,  since n and S have unit length
	// and are orthogonal by construction of S
	T_x = n[1]*S_z - n[2]*S_y;		// N x S
	T_y = n[2]*S_x - n[0]*S_z;
	T_z = n[0]*S_y - n[1]*S_x;

	// compute sx, sy, tx, ty derivatives
	// these derivatives build the inverse jacobian inv(J)=[sx,sy | tx,ty]
	// of the mapping J from object surface to screen
	_radius = 1 / surfel->getRadius();
	Iy_x *= f; Iy_y *= f; Iy_z *= f;		// f*Iy is the intersection point with Y+g*V
	sx = r * f * scale_stoo * z * _radius *_l_2;		// note f given by the vector equation X+g*V=f*Ix takes the same value as above
	tx = 0;
	sy = (Iy_x*S_x + Iy_y*S_y + Iy_z*S_z) * scale_stoo * z * _radius * _l_2;		// Iy*S projects Iy onto S
	ty = (Iy_x*T_x + Iy_y*T_y + Iy_z*T_z) * scale_stoo * z * _radius * _l_2;		// Iy*T projects Iy onto T

	// see ellipseboundingbox_general.mws, idea: compute the point on the ellipse, where the ellipse tangent
	// is perpendicular to the depth gradient, i.e. [dzc_dxs, dzc_dys]. this is the point with maximum depth
	// on the ellipse
	// NOTE: to avoid overshoots in z, we use the z-range of the reconstruction kernel for blending!
	// NOTE: the variable "d" in the maple sheet corresponds to "-zbf_cutoffRadius_2"!
	// first, compute the conic matrix of the reconstruction kernel, which is [a b/2 | b/2 c] = inv(J)*inv(J)^T 
	a = sx*sx + sy*sy;
	b_2 = sy*ty;
	b = 2.f * b_2;
	c = ty*ty;
	float discr;
	discr = -4*a*dzc_dxs*b*dzc_dys*c - a*dzc_dys*dzc_dys*b*b + 4*c*dzc_dys*dzc_dys*a*a + 
		4*a*dzc_dxs*dzc_dxs*c*c + b*b*b*dzc_dxs*dzc_dys - b*b*dzc_dxs*dzc_dxs*c;

	float zExtremum_x, zExtremum_y, tmp;
	tmp = (float)sqrt(discr * zbf_cutoffRadius_2);
	zExtremum_x = tmp * (-dzc_dys*b+2*dzc_dxs*c) / discr;
	zExtremum_y = tmp * (-2*dzc_dys*a+dzc_dxs*b) / discr;
 
	float zRange_x, zRange_y;
	tmp = zExtremum_x * dzc_dxs;
	zRange_x = (tmp < 0) ? -tmp : tmp;
	tmp = zExtremum_y * dzc_dys;
	zRange_y = (tmp < 0) ? -tmp : tmp;

	float zMax, zMin;
	zMax = z + zRange_x + zRange_y;
	zMin = z - zRange_x - zRange_y;

	// guarantee a certain minimum z-range, otherwise blending fails for splats exactly parallel to the
	// image plane (the minimum z-range is 1 in object space, and then scaled to camera space).
	float scale_otoc_radius = scale_otoc * surfel->getRadius();
	if(zMax-zMin < l_2 * scale_otoc_radius) {
		zMax += 0.5f*l_2*scale_otoc_radius;
		zMin -= 0.5f*l_2*scale_otoc_radius;
	}

	// calculate the matrix e[]=inv(J*transpose(J)+I), which describes the
	// EWA ellipse on the screen
	// note: column vectors are used, therefore the calculations are slightly
	// different to Heckbert's notation (cf. Maple file SurfaceSplatting2.mws)

	// the filtered variance matrix m[]
	// m[] = J*transpose(J)+I (details see SurfaceSplatting2.mws)
	r_ = 1.f / (sx*ty);
	r_ *= r_;							// r_ = 1/(sx*ty)^2, note that always tx=0, therefore some terms are missing
	m11 = (ty*ty + sy*sy) * r_ + 1.0f;	// in these formulas compared to the maple sheet
	m12 = -sy*sx * r_;
	m22 = sx*sx * r_ + 1.0f;

	// the filtered conic matric m[]^(-1)
	// matrix(A,B/2,B/2,C) = inv(m[])
	r_ = 1.f / (m11*m22 - m12*m12);
	a = m22 * r_;
	b_2 = -m12 * r_;
	b = b_2 * 2.f;
	c = m11 * r_;

	// calculate the normalization factor for the gaussian filter, which corresponds to the 
	// area of the reconstruction filter in source space, thus it is 
	// 1 / sqrt( det(inv(J)*transpose(inv(J))+I) )
	// note: the factor l_2 compensate for the level of detail of the LDC tree
    det_ = l_2 / (float)sqrt(sx*sx*ty*ty + sx*sx + sy*sy + ty*ty  + 1.f);

	// bounding box of the ellipse
	// see ellipseboundingbox.mws, an exact axis aligned bounding box is computed by finding the points on
	// the ellipse where the tangent of the ellipse is parallel to x- and y-axis respectively.
	// NOTE: the variable "d" in the maple sheet corresponds to "-zbf_cutoffRadius_2"!
	discr = (float)sqrt((-b*b+4*c*a)*zbf_cutoffRadius_2*a);
	ly = 2.f / (-b*b+4*c*a) * discr; 

	discr = (float)sqrt(c*(-b*b+4*c*a)*zbf_cutoffRadius_2);
	lx = 2.f / (-b*b+4*c*a) * discr;

	lx = (lx < 0) ? -lx : lx;
	ly = (ly < 0) ? -ly : ly;
	Xmax = (int)(x0 + lx) + 1;
	Xmin = (int)(x0 - lx);
	Ymax = (int)(y0 + ly) + 1;
	Ymin = (int)(y0 - ly);

	// get surfel color
	color = surfel->getDiffuseColor();
	UNPACK_FLOAT_RGB(r_comp,g_comp,b_comp,color);

	// modify color if surfel is selected and visualization of selection is enabled
	if (surfel->isFlagOn (SurfelInterface::EMPHASISE) == true &&
		(zbf->options & ZBF_VISUALIZESELECTION)) {

		if (surfel->isFlagOn (SurfelInterface::SELECTED1) == true) {
			r_comp *= 0.5f;
			g_comp *= 0.5f;
			b_comp = b_comp * 0.5f + 122.0f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED2) == true) {
			r_comp = r_comp * 0.5f + 122.0f;
			g_comp *= 0.5f;
			b_comp *= 0.5f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED3) == true) {
			r_comp *= 0.5f;
			g_comp = g_comp * 0.5f + 122.0f;
			b_comp *= 0.5f;
		}
	}

	// if in shadow adjust color values
	if ((zbf->options & ZBF_SHADOWS) && (surfel->isFlagOn (SurfelInterface::COVERED) == true)) {
		r_comp = 0.0f;
		g_comp = 0.0f;
		b_comp = 0.0f;
	}



	// perform per surfel shading
	// setup variables for shading
	_shd_nx_c = n[0];
	_shd_ny_c = n[1];
	_shd_nz_c = n[2];

	_shd_vx = V_x;
	_shd_vy = V_y;
	_shd_vz = V_z;
	_shd_ndotv = ndotv;

	_shd_kA = surfel->getAmbientCoefficient(); 
	_shd_kD = surfel->getDiffuseCoefficient();; 
	_shd_kS = surfel->getSpecularCoefficient();
	_shd_shininess = surfel->getShininess();
	specularColor = surfel->getSpecularColor();
	UNPACK_FLOAT_RGB(_shd_specularColor.r,_shd_specularColor.g,_shd_specularColor.b,specularColor);

	_shd_Id.r = _shd_Ir.r = r_comp;
	_shd_Id.g = _shd_Ir.g = g_comp;
	_shd_Id.b = _shd_Ir.b = b_comp;

	// perform shading
	if(_shd_options & SHD_LIGHTING)
	{
		_shd_Ir.r = 0.f;
		_shd_Ir.g = 0.f;
		_shd_Ir.b = 0.f;
		_shd_lightsample();
	}

	// re-assign colors for further processing
	r_comp = _shd_Ir.r;
	g_comp = _shd_Ir.g;
	b_comp = _shd_Ir.b;

	// step 2: rasterize the EWA ellipse

	// padding
	clip = false;
	if(Xmin < 0) {
		Xmin = 0;
		clip = true;
		if(Xmax < 0)
			return 0;
	}
	if(Xmax >= zbf_xsize) {
		Xmax = zbf_xsize-1;
		clip = true;
		if(Xmin >= zbf_xsize)
			return 0;
	}
	if(Ymin < 0) {
		Ymin = 0;
		clip = true;
		if(Ymax < 0)
			return 0;
	}
	if(Ymax >= zbf_ysize) {
		Ymax = zbf_ysize-1;
		clip = true;
		if(Ymin >= zbf_ysize)
			return 0;
	}

	if(Xmin < 0) {
		Xmin = 0;
		clip = true;
		if(Xmax < 0)
			return 0;
	}
	if(Xmax >= zbf_xsize) {
		Xmax = zbf_xsize-1;
		clip = true;
		if(Xmin >= zbf_xsize)
			return 0;
	}
	if(Ymin < 0) {
		Ymin = 0;
		clip = true;
		if(Ymax < 0)
			return 0;
	}
	if(Ymax >= zbf_ysize) {
		Ymax = zbf_ysize-1;
		clip = true;
		if(Ymin >= zbf_ysize)
			return 0;
	}

	// set bounding box (the bounding box is a return value)
	bbox[0] = Xmin; 
	bbox[1] = Ymin;
	bbox[2] = Xmax;
	bbox[3] = Ymax;

	// z value in the lower left corner of the rasterized area
	z_start = z - dzc_dxs*(x0-((float)Xmin+0.5f)) - dzc_dys*(y0-((float)Ymin+0.5f));

	x = ((float)Xmin+0.5f) - x0;
	ddq = 2*a;

	// *********************
	// ellipse rasterization
	// *********************

	adjustedBufferHeight = zbf->ysize - 1;

	for(Y=Ymin; Y<=Ymax; Y++)
	{
		// finite differences for ellipse rasterization
		y = ((float)Y+0.5f) - y0;
		dq = a*(2*x+1)+b*y;
		q = (c*y + b*x)*y + a*x*x;

		// init z value
		z_cur = z_start;

		for(X=Xmin; X<=Xmax; X++)
		{
			i = X + zbf_xsize*Y;

			if(q<zbf_cutoffRadius_2)
			{

				// compare z-ranges
				if( !(zbf->buf[i].zMax < zMin || zMax < zbf->buf[i].zMin) )
				{
					// subtract contribution
					w = zbf_filterLUT[(int)(q*_zbf_cutoffRadius_2 * zbf_LUTsize)] * det_;
					zbf->buf[i].w -= w;

					if(zbf->buf[i].w > 0.001f)
					{
						// pixel is not empty yet
						zbf->buf[i].c[0] -= r_comp*w;
						zbf->buf[i].c[1] -= g_comp*w;
						zbf->buf[i].c[2] -= b_comp*w;

						// normals
						zbf->buf[i].n[0] -= n[0] * w;
						zbf->buf[i].n[1] -= n[1] * w;
						zbf->buf[i].n[2] -= n[2] * w;

						// z
						zbf->buf[i].z -= z_cur * w;
					} else
					{
						// pixel is empty, reset z buffer values
						zbf->buf[i].w = 0.f;
						zbf->buf[i].z = 100000.f;
						zbf->buf[i].zMin = 1000000.0f;
						zbf->buf[i].zMax = 1000000.0f;
						zbf->buf[i].c[0] = 0.f;
						zbf->buf[i].c[1] = 0.f;
						zbf->buf[i].c[2] = 0.f;
						zbf->buf[i].n[0] = 0.f;
						zbf->buf[i].n[1] = 0.f;
						zbf->buf[i].n[2] = 0.f;

						// if flag to write per pixel lists into frame buffer is set, update those!
						if (zbf->frameBuffer->getAttributes() & FrameBufferInterface::PERPIXEL_SURFELLISTS) {
							zbf->frameBuffer->resetPosition (X, adjustedBufferHeight - Y);
						}
					}
				}
			} 

			q += dq;
			dq += ddq;

			z_cur += dzc_dxs;
		}
		z_start += dzc_dys;
	}

	return 0;
}


//-------------------------------------------------------------------------
// Enable a property
//-------------------------------------------------------------------------
void ZbfEnable(ZBuffer* zbf, ZbfProperties p)
{
	zbf->options |= p;
}


//-------------------------------------------------------------------------
// Disable a shader property
//-------------------------------------------------------------------------
void ZbfDisable(ZBuffer *zbf, ZbfProperties p)
{
	int mask = 0xffffffff;

	p = (ZbfProperties)((int)p ^mask);
	zbf->options &= p;
}


//-------------------------------------------------------------------------
// Set the cutoff radius for surface splatting
//-------------------------------------------------------------------------
void ZBfSetCutoffRadius(ZBuffer *zbf, float radius) 
{
	zbf->cutoffRadius = radius;
}


//-------------------------------------------------------------------------
// Set the blending thresholds used for surface splatting
//-------------------------------------------------------------------------
void ZBfSetBlendingThresholds(ZBuffer *zbf, float constTrheshold, float distThreshold, float angleThreshold) 
{
	zbf->constThreshold = constTrheshold;
	zbf->distThreshold = distThreshold;
	zbf->angleTrheshold = angleThreshold;
}


//-------------------------------------------------------------------------
// Get the blending thresholds used for surface splatting
//-------------------------------------------------------------------------
void ZBfGetBlendingThresholds(ZBuffer *zbf, float* constTrheshold, float* distThreshold, float* angleThreshold) 
{
	(*constTrheshold) = zbf->constThreshold;
	(*distThreshold) = zbf->distThreshold;
	(*angleThreshold) = zbf->angleTrheshold;
}



// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
