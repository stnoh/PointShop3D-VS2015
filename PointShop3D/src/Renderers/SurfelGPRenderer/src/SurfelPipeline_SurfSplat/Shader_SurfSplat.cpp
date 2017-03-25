// Title:   Shader_SurfSplat.cpp
// Created: Thu Sep 25 14:50:25 2003
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
// Shader, shades samples in a z-buffer associated with a warper
// and writes result into an image buffer
//
// Note: 
// - this version does not support semi-transparent surfaces
// - shading is performed in camera space, the phong illumination model
// is evaluated using cube reflectance maps
//
// Shader_C_CubeReflMap.c 
//-------------------------------------------------------------------------

// Standard Includes
#include <malloc.h>

// Project Includes
#include "Shader_SurfSplat.h"
#include "../../../../Utilities/src/Matrix.h"
#include "../../../../DataTypes/src/MyDataTypes.h"
#include "../../../../DataTypes/src/Vector3D.h"


#ifdef STATISTIC_TME_SHADER
#include <time.h>
static clock_t start, end;
static float sec;
#endif

// Static variables for efficiency reasons
// dereferenced member variables
static const Vector3D *shd_normalmap_table;
static int shd_diffcolor;
static int *shd_dbgcolors;
static LSrList *shd_lsl;
static ShdTextureFilteringFnct shd_gettexture;

// additional variables used over function scope
static float _shd_objScaling;
static unsigned char _shd_nroflevels;
static int _shd_nrofuserbytes;

// global variables (potentially accessed by external shading functions)
float _shd_nx_c, _shd_ny_c, _shd_nz_c;
float _shd_x_c, _shd_y_c, _shd_z_c;
float _shd_vx, _shd_vy, _shd_vz;
float _shd_ndotv;
float _shd_kA, _shd_kD, _shd_kS;
unsigned char _shd_shininess;
MyDataTypes::RGBTriple _shd_specularColor;
MyDataTypes::RGBTriple _shd_Id;
MyDataTypes::RGBTriple _shd_Ir;
void *_shd_userdata;
int _shd_options;
ShdLightSampleFnct _shd_lightsample;

static float sqrt_3, sqrt_2;

// Static functions
//	static void shdEnvMapSample();					// NOT SUPPORTED IN THIS VERSION!
static void shdLightSamplePhong_H();
static void shdLightSamplePhong_R();
//	static void shdLightSampleTorranceSparrow();	// NOT SUPPORTED IN THIS VERSION!



//-------------------------------------------------------------------------
// Allocate a new shd and connect a warper and image buffer to it
//-------------------------------------------------------------------------
Shader* ShdNew(Warper *wrp, FrameBufferInterface *frameBuffer, LSrList *lsl)
{
	Shader *shd;

	shd = (Shader *)malloc(sizeof(Shader));
	shd->wrp = wrp;
	shd->frameBuffer = frameBuffer;
	shd->lsl = lsl;
	shd->options = 0;
	shd->diffColor = SHD_DIFFCOL_DEFAULT;
	shd->shdLightSample = shdLightSamplePhong_R;

	shd->shdUserShadingData = NULL;
	shd->shdUserShadingFunction = NULL;

	// color sets for debugging
	shd->dbgcolors[0] = 0x000000ff;
	shd->dbgcolors[1] = 0x0000ff00;
	shd->dbgcolors[2] = 0x00ff0000;

	sqrt_3 = (float)sqrt(3.0f);
	sqrt_2 = (float)sqrt(2.0f);

	ShdEnable(shd, SHD_PRESHADESPLAT);
	ShdDisable(shd, SHD_PERPIXELSHADING);

	return shd;
}


//-------------------------------------------------------------------------
// Free a shd
//-------------------------------------------------------------------------
void ShdFree(Shader *shd)
{
	free(shd);
}


//-------------------------------------------------------------------------
// Prepare for shading, set global variables
//-------------------------------------------------------------------------
void ShdPrepareForShading(Shader *shd)
{
	shd_lsl = shd->lsl;

	_shd_options = shd->options;
	_shd_lightsample = shd->shdLightSample;
}


//-------------------------------------------------------------------------
// Shade the samples in the z-buffer of a warper
//-------------------------------------------------------------------------
void ShdShadeZBuffer(Shader *shd, int magfactor, int bbox[4], QRgb backgroundColor)
{
	Frustum *frst;
	Viewport *vprt;

	int xsize, ysize;				// x and y screen coordinates size			
	
	SrfZBufItem *zbufitem;				// current zbuffer item
	
	float vp_sx, vp_sy;				// variables for inverse viewport mapping
	float vp_tx, vp_ty;

	float w_, vec_len_;
	int c, specularColor;

	int start_dim_x, start_dim_y;	// writing the color values to the display image
	int end_dim_x, end_dim_y;		// (supports progressive rendering)
	int dim_y, dim_x;
	uint adjustedBufferHeight;      // buffer height - 1

	int attributes;

#ifdef STATISTIC_LOD_SHADER
	float dbg_z;
#endif

#ifdef STATISTIC_TME_SHADER
	start = clock();
#endif

	ZBfPrepareForReading(shd->wrp->zbf);
	//DImPrepareForAccess(shd->dim);

	// initialise local variables for more efficient access
	xsize = shd->wrp->viewport.xS;
	ysize = shd->wrp->viewport.yS; 
	frst = &(shd->wrp->frustum);
	vprt = &(shd->wrp->viewport);

	// initialise static variables for efficient access over function scope
	shd_diffcolor = shd->diffColor;
	shd_dbgcolors = shd->dbgcolors;

	// set variables for inverse viewport mapping 
	vp_sx = 2*frst->xP / vprt->xS;
	vp_sy = 2*frst->yP / vprt->yS;
	vp_tx = frst->xC - frst->xP;
	vp_ty = frst->yC - frst->yP;

	// shade z-buffer
	// bbox[] specifies a bounding box
	ZBFSETPOS(bbox[0],bbox[1]);
	attributes = shd->frameBuffer->getAttributes();
	adjustedBufferHeight = shd->frameBuffer->getSize().height() - 1;
	while(ZBFROWS <= bbox[3])
	{
		while(ZBFCOLS <= bbox[2])

		{
			ZBFGETITEM(zbufitem);

			// avoid division by zero!
			if(zbufitem->w != 0.0f)
			{
				// NOTE: we do per surfel shading, hence all there is left to do for the shader is
				// to normalize the depth, colors and normals, and write them to the display image

				w_ = 1.f / zbufitem->w;

				// normalize colors
				_shd_Id.r = _shd_Ir.r = zbufitem->c[0] * w_;
				_shd_Id.g = _shd_Ir.g = zbufitem->c[1] * w_;
				_shd_Id.b = _shd_Ir.b = zbufitem->c[2] * w_;

				if ( (_shd_options & SHD_PERPIXELSHADING) && (_shd_options & SHD_LIGHTING) ) {

					// re-normalize normal
					_shd_nx_c = zbufitem->n[0];
					_shd_ny_c = zbufitem->n[1];
					_shd_nz_c = zbufitem->n[2];
					w_ = 1.f / (float)sqrt(_shd_nx_c*_shd_nx_c+_shd_ny_c*_shd_ny_c+_shd_nz_c*_shd_nz_c);
					_shd_nx_c *= w_;
					_shd_ny_c *= w_;
					_shd_nz_c *= w_;

					// compute viewing vector
					_shd_vx = -(zbf_x_cur * vp_sx + vp_tx);
					_shd_vy = -(zbf_y_cur * vp_sy + vp_ty);
					_shd_vz = -1.f;
					vec_len_ = 1.f / (float)sqrt(_shd_vx*_shd_vx + _shd_vy*_shd_vy + 1.f);
					_shd_vx *= vec_len_; _shd_vy *= vec_len_; _shd_vz *= vec_len_;
					_shd_ndotv = _shd_nx_c*_shd_vx + _shd_ny_c*_shd_vy + _shd_nz_c*_shd_vz;

					_shd_kA = zbufitem->kA;
					_shd_kD = zbufitem->kD;
					_shd_kS = zbufitem->kS;
					_shd_shininess = zbufitem->shininess;
					specularColor = zbufitem->specularColor;
					UNPACK_FLOAT_RGB(_shd_specularColor.r,_shd_specularColor.g,_shd_specularColor.b,specularColor);

					_shd_Ir.r = 0.f;
					_shd_Ir.g = 0.f;
					_shd_Ir.b = 0.f;
					_shd_lightsample();
				}
				
				// clamp color intensities
				if(_shd_Ir.r>255.0) _shd_Ir.r = 255.0;
				if(_shd_Ir.g>255.0) _shd_Ir.g = 255.0;
				if(_shd_Ir.b>255.0) _shd_Ir.b = 255.0;

				// pack color to format for packed RGB arithmetic
				PACK_DIM_RGB(c,(int)_shd_Ir.r,(int)_shd_Ir.g,(int)_shd_Ir.b);

				// write to display image, blow up pixels for progressive rendering
				start_dim_x = zbf_cols * magfactor;
				end_dim_x = start_dim_x + magfactor;
				start_dim_y = zbf_rows * magfactor;
				end_dim_y = start_dim_y + magfactor;

				// write output color to frame buffer
				for(dim_y=start_dim_y; dim_y<end_dim_y; dim_y++)
				{
					for(dim_x=start_dim_x; dim_x<end_dim_x; dim_x++)
					{
						shd->frameBuffer->setColor (dim_x, adjustedBufferHeight - dim_y, c);
					}
				}
			} 
			else {
				// write to display image, blow up pixels for progressive rendering
				start_dim_x = zbf_cols * magfactor;
				end_dim_x = start_dim_x + magfactor;
				start_dim_y = zbf_rows * magfactor;
				end_dim_y = start_dim_y + magfactor;
				for(dim_y=start_dim_y; dim_y<end_dim_y; dim_y++)
				{
					for(dim_x=start_dim_x; dim_x<end_dim_x; dim_x++)
					{
						shd->frameBuffer->setColor (dim_x, adjustedBufferHeight - dim_y, backgroundColor);
					}
				}
			}

			ZBFNEXTCOL();
		}
		bbox[1]++;
		ZBFSETPOS(bbox[0],bbox[1]);
	}

#ifdef STATISTIC_TME_SHADER
	end = clock();
	sec = ((float)end - (float)start) / CLOCKS_PER_SEC;
	printf("Shader::Timing: shaded z-buffer in %f sec.\n", sec);
#endif
}


//-------------------------------------------------------------------------
// Calculate phong shading for a sample
// note: this function uses the formulation with the halfway vector. given
// a shininess coefficient c, it therefore doesn't produce the same results 
// as cube maps, since they are use the reflection vector formulation.
//-------------------------------------------------------------------------
static void shdLightSamplePhong_H()
{
	LightSource *lsr;
	float Ir, Ig, Ib;
	float Ar, Ag, Ab;
	float Lx, Ly, Lz;
	float Hx, Hy, Hz;
	float t, r, ndotl, ndoth;
	int j;

	LLTNEW(shd_lsl);
	while(!LLTENDREACHED)
	{
		lsr = LLTGETCURRENT;
		Ir = LSRGET_IR(lsr);
		Ig = LSRGET_IG(lsr);
		Ib = LSRGET_IB(lsr);

		Ar = LSRGET_AR(lsr);
		Ag = LSRGET_AG(lsr);
		Ab = LSRGET_AB(lsr);

		// ambient contribution
		_shd_Ir.r += _shd_kA * Ar * _shd_Id.r;
		_shd_Ir.g += _shd_kA * Ag * _shd_Id.g;
		_shd_Ir.b += _shd_kA * Ab * _shd_Id.b;

		Lx = LSRGET_X(lsr);
		Ly = LSRGET_Y(lsr);
		Lz = LSRGET_Z(lsr);
		switch(lsr->type)
		{
		case LSR_POINT:
			Lx = Lx - _shd_x_c;
			Ly = Ly - _shd_y_c;
			Lz = Lz - _shd_z_c;
			r = 1 / (float)sqrt(Lx*Lx + Ly*Ly + Lz*Lz);
			Lx = Lx * r; Ly = Ly * r; Lz = Lz * r;
			break;
		default:
			break;
		}
		// calculate normalized halfway vector
		Hx = Lx + _shd_vx; Hy = Ly + _shd_vy; Hz = Lz + _shd_vz;
		r = 1 / (float)sqrt(Hx*Hx + Hy*Hy + Hz*Hz);
		Hx = Hx * r; Hy = Hy * r; Hz = Hz * r;

		// calculate the N*L and N*H dot products
		ndotl = _shd_nx_c*Lx + _shd_ny_c*Ly + _shd_nz_c*Lz;
		ndotl = (ndotl < 0 ? 0 : ndotl);

		ndoth = _shd_nx_c*Hx + _shd_ny_c*Hy + _shd_nz_c*Hz;
		ndoth = (ndoth < 0 ? 0 : ndoth);

		// calculate the phong shininess power
		r = ndoth;
		j = _shd_shininess;
		while(j>0)
		{
			r *= ndoth;
			j--;
		}

		// increment intensities
		t = _shd_kD * ndotl;
		r *= _shd_kS;
		_shd_Ir.r += Ir * (t * _shd_Id.r + _shd_specularColor.r*r);
		_shd_Ir.g += Ig * (t * _shd_Id.g + _shd_specularColor.g*r);
		_shd_Ir.b += Ib * (t * _shd_Id.b + _shd_specularColor.b*r);

		LLTNEXT;
	}
}


//-------------------------------------------------------------------------
// Calculate phong shading for a sample
// note: this function uses the formulation with the reflection vector. for
// directional light sources, it produces exactly the same results as cube
// maps.
//-------------------------------------------------------------------------
static void shdLightSamplePhong_R()
{
	LightSource *lsr;
	float Ir, Ig, Ib;
	float Ar, Ag, Ab;
	float Lx, Ly, Lz;
	float Rx, Ry, Rz;
	float t, r, ndotl, rdotv;
	int j;

	LLTNEW(shd_lsl);
	while(!LLTENDREACHED)
	{
		lsr = LLTGETCURRENT;
		Ir = LSRGET_IR(lsr);
		Ig = LSRGET_IG(lsr);
		Ib = LSRGET_IB(lsr);

		Ar = LSRGET_AR(lsr);
		Ag = LSRGET_AG(lsr);

		Ab = LSRGET_AB(lsr);

		// ambient contribution
		t = _shd_kA;
		_shd_Ir.r += t * Ar * _shd_Id.r;
		_shd_Ir.g += t * Ag * _shd_Id.g;
		_shd_Ir.b += t * Ab * _shd_Id.b;

		Lx = LSRGET_X(lsr);
		Ly = LSRGET_Y(lsr);
		Lz = LSRGET_Z(lsr);
		switch(lsr->type)
		{
		case LSR_POINT:
			Lx = Lx - _shd_x_c;
			Ly = Ly - _shd_y_c;
			Lz = Lz - _shd_z_c;
			r = 1 / (float)sqrt(Lx*Lx + Ly*Ly + Lz*Lz);
			Lx = Lx * r; Ly = Ly * r; Lz = Lz * r;
			break;
		default:
			break;
		}
		// calculate the N*L dot product
		ndotl = _shd_nx_c*Lx + _shd_ny_c*Ly + _shd_nz_c*Lz;
		ndotl = (ndotl < 0 ? 0 : ndotl);

		// calculate normalized reflection vector
		Rx = 2*_shd_nx_c*ndotl - Lx; 
		Ry = 2*_shd_ny_c*ndotl - Ly;
		Rz = 2*_shd_nz_c*ndotl - Lz;

		// calculate R*V dot product
		rdotv = _shd_vx*Rx + _shd_vy*Ry + _shd_vz*Rz;
		rdotv = (rdotv < 0 ? 0 : rdotv);

		// calculate the phong shininess power
		r = rdotv;
		j = _shd_shininess;
		while(j>0)
		{
			r *= rdotv;
			j--;
		}

		// increment intensities
		t = _shd_kD * ndotl;
		r *= _shd_kS;
		_shd_Ir.r += Ir * (t * _shd_Id.r + _shd_specularColor.r*r);
		_shd_Ir.g += Ig * (t * _shd_Id.g + _shd_specularColor.g*r);
		_shd_Ir.b += Ib * (t * _shd_Id.b + _shd_specularColor.b*r);

		LLTNEXT;
	}
}


//-------------------------------------------------------------------------
// Enable a shader property
//-------------------------------------------------------------------------
void ShdEnable(Shader *shd, ShdProperties p)
{
	shd->options |= p;
}


//-------------------------------------------------------------------------
// Disable a shader property
//-------------------------------------------------------------------------
void ShdDisable(Shader *shd, ShdProperties p)
{
	int mask = 0xffffffff;

	p = (ShdProperties)((int)p ^mask);
	shd->options &= p;
}


//-------------------------------------------------------------------------
// Set the shading model
//-------------------------------------------------------------------------
void ShdSetShadingModel(Shader *shd, ShdShadingModel s)
{
	switch(s)
	{
	case SHD_PHONG_H:
		shd->shdLightSample = shdLightSamplePhong_H;
		break;
	case SHD_PHONG_R:
		shd->shdLightSample = shdLightSamplePhong_R;
		break;
	case SHD_USERFNCT:
		if(shd->shdUserShadingFunction)
		{
			shd->shdLightSample = shd->shdUserShadingFunction;
		}
	default:
		break;
	}
}


//-------------------------------------------------------------------------
// Set a custom shading function
//-------------------------------------------------------------------------
void ShdSetUserShadingFunction(Shader *shd, ShdLightSampleFnct f, void *u)
{
	shd->shdUserShadingFunction = f;

	shd->shdUserShadingData = _shd_userdata = u;
}


//-------------------------------------------------------------------------
// Set the diffuse color used for shading
// note: 
// - shd->diffColor is ignored except if the DEMO_CODE macro is defined
// - per default, the diffuse color stored in each sample is used
//-------------------------------------------------------------------------
void ShdSetDiffColor(Shader *shd, ShdDiffColor c)
{
	// 'c' can be either 
	// - SHD_DIFFCOL_MIPMAPLEVEL, the diffuse color is chosen according to 
	// the mipmap level of a surfel
	// - SHD_DIFFCOL_LODLEVEL, the diffuse color is chosen according to 
	// the block level of a surfel
	shd->diffColor = c;
}

			


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
