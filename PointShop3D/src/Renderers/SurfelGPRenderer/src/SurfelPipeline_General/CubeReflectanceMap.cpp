// Title:   CubeReflectanceMap.cpp
// Created: Thu Sep 25 14:50:38 2003
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
// Cube Reflectance Map
//
// CubeReflectanceMap.c 
//-------------------------------------------------------------------------

#include <stdlib.h>
#include <math.h>
#include "CubeReflectanceMap.h"
#include "../../../../DataTypes/src/MyDataTypes.h"



//-------------------------------------------------------------------------
// Construct a reflectance map
//-------------------------------------------------------------------------
CubeReflectanceMap *CRMNew(int size)
{
	int i;

	CubeReflectanceMap *crm;

	crm = (CubeReflectanceMap *)malloc(sizeof(CubeReflectanceMap));

	crm->size = size;
	crm->size_2 = 0.5f * (size-1);
	crm->faceSize = size * size;
	crm->v = (MyDataTypes::RGBTriple *)malloc(6 * crm->faceSize * sizeof(MyDataTypes::RGBTriple));

	crm->interp = CRM_INTERP_NEAREST;

	for(i=0; i<6*crm->faceSize; i++)
	{
		crm->v[i].r = 0.0f;
		crm->v[i].g = 0.0f;
		crm->v[i].b = 0.0f;
	}

	return crm;
}


//-------------------------------------------------------------------------
// Construct a reflectance map by copying
//-------------------------------------------------------------------------
CubeReflectanceMap *CRMNewCopy(CubeReflectanceMap *src)
{
	int i;

	CubeReflectanceMap *dst;

	dst = (CubeReflectanceMap *)malloc(sizeof(CubeReflectanceMap));

	dst->size = src->size;
	dst->size_2 = src->size_2;
	dst->faceSize = src->faceSize;
	dst->v = (MyDataTypes::RGBTriple *)malloc(6 * dst->faceSize * sizeof(MyDataTypes::RGBTriple));

	dst->interp = src->interp;

	for(i=0; i<6*dst->faceSize; i++)
	{
		dst->v[i].r = src->v[i].r;
		dst->v[i].g = src->v[i].g;
		dst->v[i].b = src->v[i].b;
	}

	return dst;
}


//-------------------------------------------------------------------------
// Free a reflectance map
//-------------------------------------------------------------------------
void CRMFree(CubeReflectanceMap *crm)
{
	free(crm->v);
	free(crm);
}


//-------------------------------------------------------------------------
// Set interpolation method
//-------------------------------------------------------------------------
void CRMSetInterp(CubeReflectanceMap *crm, CRMInterp i)
{
	crm->interp = i;
}


//-------------------------------------------------------------------------
// Clear a reflection map
//-------------------------------------------------------------------------
void CRMClear(CubeReflectanceMap *crm)
{
	int i;

	for(i=0; i<6*crm->faceSize; i++)
	{
		crm->v[i].r = 0.0f;
		crm->v[i].g = 0.0f;
		crm->v[i].b = 0.0f;
	}
}


//-------------------------------------------------------------------------
// Add a point light to a reflection map
//-------------------------------------------------------------------------
void CRMAddLightSource(CubeReflectanceMap *crm, CRMType t, LightSource *l, Material *mtl)
{
	switch(t)
	{
	case CRM_N_MAP:
		CRMAddAmbient(crm, l, mtl->getAmbientCoefficient());
		CRMAddDiffuse(crm, l, mtl->getDiffuseCoefficient());
		break;
	case CRM_R_MAP:
		// note: the specular reflection coefficent 'ks'
		// is premultiplied into the specular material
		// color, therefore it's not passed here
		CRMAddSpecular(crm, l, mtl->getShininess());
		break;
	default:
		break;
	}
}



//-------------------------------------------------------------------------
// Add an ambient component to a reflectance map
//-------------------------------------------------------------------------
void CRMAddAmbient(CubeReflectanceMap *crm, LightSource *l, float ka)
{
	int i;
	float Ar, Ag, Ab;

	Ar = l->Ar * ka;
	Ag = l->Ag * ka;
	Ab = l->Ab * ka;

	for(i=0; i<6*crm->faceSize; i++)
	{
		crm->v[i].r += Ar;
		crm->v[i].g += Ag;
		crm->v[i].b += Ab;
	}
}


//-------------------------------------------------------------------------
// Add a diffuse component to a reflectance map
//-------------------------------------------------------------------------
void CRMAddDiffuse(CubeReflectanceMap *crm, LightSource *l, float kd)
{
	float lx, ly, lz;
	float ri, rj, rk, rx, ry, rz;
	float Ir, Ig, Ib;
	int i, j, n;
	int crm_size, crm_facesize;
	float v;
	MyDataTypes::RGBTriple *crm_v;
	float t;

	// only directional light source can be handled!
	if(l->type != LSR_DIRECTIONAL)
	{
		return;
	}

	crm_size = crm->size;
	crm_facesize = crm->faceSize;
	crm_v = crm->v;

	Ir = l->Ir * kd;
	Ig = l->Ig * kd;
	Ib = l->Ib * kd;

	// get light direction and normalize vector
	lx = l->x;
	ly = l->y;
	lz = l->z;
	t = 1.0f / (float)sqrt(lx*lx + ly*ly + lz*lz);
	lx = lx * t;
	ly = ly * t;
	lz = lz * t;

	// over all indices on a cube face
	for(j=0; j<crm_size; j++)
	{
		for(i=0; i<crm_size; i++)
		{
			// calculate and normalize direction corresponding to i,j-indices
			ri = 2.0f * i/(crm_size-1) - 1.0f;
			rj = 2.0f * j/(crm_size-1) - 1.0f;
			rk = 1.0f;
			t = 1.0f / (float)sqrt(ri*ri + rj*rj + rk*rk);
			ri = ri * t;
			rj = rj * t;
			rk = rk * t;

			// generate directions and indices for all 6 cube faces
			// then calculate dot products
			rx = ri;
			ry = rj;
			rz = rk;
			n = i + j * crm_size;
			if( (v = rx*lx + ry*ly + rz*lz) > 0 )
			{
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}

			ry = ri;
			rz = rj;
			rx = rk;
			n = crm_facesize + (i + j*crm_size);
			if( (v = rx*lx + ry*ly + rz*lz) > 0 )
			{
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}

			rz = ri;
			rx = rj;
			ry = rk;
			n = 2*crm_facesize + (i + j*crm_size);
			if( (v = rx*lx + ry*ly + rz*lz) > 0 )
			{
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}

			rx = ri;
			ry = rj;
			rz = -rk;
			n = 3*crm_facesize + (i + j*crm_size);
			if( (v = rx*lx + ry*ly + rz*lz) > 0 )
			{
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}
			
			ry = ri;
			rz = rj;
			rx = -rk;
			n = 4*crm_facesize + (i + j*crm_size);
			if( (v = rx*lx + ry*ly + rz*lz) > 0 )
			{
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}

			rz = ri;
			rx = rj;
			ry = -rk;
			n = 5*crm_facesize + (i + j*crm_size);
			if( (v = rx*lx + ry*ly + rz*lz) > 0 )
			{
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}
		}
	}
}



//-------------------------------------------------------------------------
// Add a specular component to a reflectance map
//-------------------------------------------------------------------------
void CRMAddSpecular(CubeReflectanceMap *crm, LightSource *l, int c)
{
	float lx, ly, lz;
	float ri, rj, rk, rx, ry, rz;
	float Ir, Ig, Ib;
	int i, j, n, m;
	int crm_size, crm_facesize;
	MyDataTypes::RGBTriple *crm_v;
	float v;
	float t;

	// only directional light source can be handled!
	if(l->type != LSR_DIRECTIONAL)
	{
		return;
	}

	crm_size = crm->size;
	crm_facesize = crm->faceSize;
	crm_v = crm->v;

	// get light direction and normalize vector
	lx = l->x;
	ly = l->y;
	lz = l->z;
	t = 1.0f / (float)sqrt(lx*lx + ly*ly + lz*lz);
	lx = lx * t;
	ly = ly * t;
	lz = lz * t;

	Ir = l->Ir;
	Ig = l->Ig;
	Ib = l->Ib;

	// over all indices on a cube face
	for(j=0; j<crm_size; j++)
	{
		for(i=0; i<crm_size; i++)
		{
			// calculate and normalize direction corresponding to i,j-indices
			ri = 2.0f * i/(crm_size-1) - 1.0f;
			rj = 2.0f * j/(crm_size-1) - 1.0f;
			rk = 1.0f;
			t = 1.0f / (float)sqrt(ri*ri + rj*rj + rk*rk);
			ri = ri * t;
			rj = rj * t;
			rk = rk * t;

			// generate directions and indices for all 6 cube faces
			// then calculate dot products
			rx = ri;
			ry = rj;
			rz = rk;
			n = i + j * crm_size;
			if( (t = rx*lx + ry*ly + rz*lz) > 0)
			{
				for(v=1, m=0; m<c; m++) v=v*t;
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;

			}

			ry = ri;
			rz = rj;
			rx = rk;
			n = crm_facesize + (i + j*crm_size);
			if( (t = rx*lx + ry*ly + rz*lz) > 0)
			{
				for(v=1, m=0; m<c; m++) v=v*t;
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}

			rz = ri;
			rx = rj;
			ry = rk;
			n = 2*crm_facesize + (i + j*crm_size);
			if( (t = rx*lx + ry*ly + rz*lz) > 0)
			{
				for(v=1, m=0; m<c; m++) v=v*t;
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}

			rx = ri;
			ry = rj;
			rz = -rk;
			n = 3*crm_facesize + (i + j*crm_size);
			if( (t = rx*lx + ry*ly + rz*lz) > 0)
			{
				for(v=1, m=0; m<c; m++) v=v*t;
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}
			
			ry = ri;
			rz = rj;
			rx = -rk;
			n = 4*crm_facesize + (i + j*crm_size);
			if( (t = rx*lx + ry*ly + rz*lz) > 0)
			{
				for(v=1, m=0; m<c; m++) v=v*t;
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}

			rz = ri;
			rx = rj;
			ry = -rk;
			n = 5*crm_facesize + (i + j*crm_size);
			if( (t = rx*lx + ry*ly + rz*lz) > 0)
			{
				for(v=1, m=0; m<c; m++) v=v*t;
				crm_v[n].r += v * Ir;
				crm_v[n].g += v * Ig;
				crm_v[n].b += v * Ib;
			}
		}
	}
}


//-------------------------------------------------------------------------
// Load an environment map into the reflectance map
//-------------------------------------------------------------------------
void CRMAddEnvironment(CubeReflectanceMap *crm, unsigned int *buf)
{
	int facesize, size, size2, size3;
	int i, j, i_off, j_off;
	MyDataTypes::RGBTriple *v;
	int k, l;

	size = crm->size;
	size2 = size * 2;
	size3 = size * 3;
	facesize = crm->faceSize;

	v = crm->v;

	// face 0
	i_off = size;
	j_off = 0;
	for(j=0; j<size; j++)
	{
		for(i=0; i<size; i++)
		{
			k = i + (size-j-1)*size;
			l = (j + i_off) + (i + j_off)*size3;

			v[k].r += (buf[l] & 0x000000ff) / 255.0f;
			v[k].g += ((buf[l] & 0x0000ff00) >> 8) / 255.0f;
			v[k].b += ((buf[l] & 0x00ff0000) >> 16) / 255.0f; 
		}
	}

	// face 1
	i_off = 0;
	j_off = 0;
	for(j=0; j<size; j++)
	{
		for(i=0; i<size; i++)
		{
			k = (size-i-1) + (size-j-1)*size + facesize;
			l = (j + i_off) + (i + j_off)*size3;

			v[k].r += (buf[l] & 0x000000ff) / 255.0f;
			v[k].g += ((buf[l] & 0x0000ff00) >> 8) / 255.0f;
			v[k].b += ((buf[l] & 0x00ff0000) >> 16) / 255.0f; 
		}
	}

	// face 2
	i_off = size2;
	j_off = 0;
	for(j=0; j<size; j++)
	{
		for(i=0; i<size; i++)
		{
			k = (size-i-1) + j*size + facesize*2;
			l = (j + i_off) + (i + j_off)*size3;

			v[k].r += (buf[l] & 0x000000ff) / 255.0f;
			v[k].g += ((buf[l] & 0x0000ff00) >> 8) / 255.0f;
			v[k].b += ((buf[l] & 0x00ff0000) >> 16) / 255.0f; 
		}
	}

	// face 3
	i_off = size;
	j_off = size;
	for(j=0; j<size; j++)
	{
		for(i=0; i<size; i++)
		{
			k = i + (size-j-1)*size + facesize*3;
			l = (j + i_off) + (i + j_off)*size3;

			v[k].r += (buf[l] & 0x000000ff) / 255.0f;
			v[k].g += ((buf[l] & 0x0000ff00) >> 8) / 255.0f;
			v[k].b += ((buf[l] & 0x00ff0000) >> 16) / 255.0f; 
		}
	}

	// face 4
	i_off = 0;
	j_off = size;
	for(j=0; j<size; j++)
	{
		for(i=0; i<size; i++)
		{
			k = (size-i-1) + (size-j-1)*size + facesize*4;
			l = (j + i_off) + (i + j_off)*size3;

			v[k].r += (buf[l] & 0x000000ff) / 255.0f;
			v[k].g += ((buf[l] & 0x0000ff00) >> 8) / 255.0f;
			v[k].b += ((buf[l] & 0x00ff0000) >> 16) / 255.0f; 
		}
	}

	// face 5
	i_off = size2;
	j_off = size;
	for(j=0; j<size; j++)
	{
		for(i=0; i<size; i++)
		{
			k = (size-i-1) + j*size + facesize*5;
			l = (j + i_off) + (i + j_off)*size3;

			v[k].r += (buf[l] & 0x000000ff) / 255.0f;
			v[k].g += ((buf[l] & 0x0000ff00) >> 8) / 255.0f;
			v[k].b += ((buf[l] & 0x00ff0000) >> 16) / 255.0f; 
		}
	}
	
/*	SetDIBits(dcC, crmH, 0, 256, buf, ], DIB_RGB_COLORS);
	SelectObject(dcC, crmH);
		
	dcW = GetDC(ghWnd);
	BitBlt(dcW, 0, 0, 384, 256, dcC, 0, 0, SRCCOPY);
	ReleaseDC(ghWnd, dcW);	*/
}


//-------------------------------------------------------------------------
// Get a value from a reflectance map
//-------------------------------------------------------------------------
void CRMGetValue(CubeReflectanceMap *crm, float x, float y, float z, MyDataTypes::RGBTriple *r)
{
	int fx, fy, fz;
	float ax, ay, az;
	float crm_size_2;
	int crm_size, crm_faceSize;
	int n, f;
	int ti1, ti2, ti3;
	float i_f, j_f;

	int n1, n2, n3, n4;
	int i_l, i_h, j_l, j_h;
	float i_w, j_w, _i_w, _j_w;
	MyDataTypes::RGBTriple t1, t2, *v;

	// calculate absolute values of coordinates
	x<0 ? (fx = 1, ax = -x) : (fx = 0, ax = x);
	y<0 ? (fy = 1, ay = -y) : (fy = 0, ay = y);
	z<0 ? (fz = 1, az = -z) : (fz = 0, az = z);

	// find biggest coordinate and determine indices into reflectance map
	if(ax>ay)
	{
		if(ax>az)
		{
			// x is the biggest coordinate
			if(!fx)
				f = 1;
			else
				f = 4;
			ax = 1/ax;
			i_f = y * ax;
			j_f = z * ax;
		} else
		{
			// z is the biggest coordinate
			if(!fz)
				f = 0;
			else
				f = 3;
			az = 1/az;
			i_f = x * az;
			j_f = y * az;
		}
	} else if(ay>az)
	{
		// y is the biggest coordinate
		if(!fy)
			f = 2;
		else 
			f = 5;
		ay = 1/ay;
		i_f = z * ay;
		j_f = x * ay;
	} else
	{
		// z is the biggest coordinate
		if(!fz)
			f = 0;
		else
			f = 3;
		az = 1/az;
		i_f = x * az;
		j_f = y * az;
	}
	// i_f and j_f are now in [-1..1]
	// f is the face number in the cube map

	v = crm->v;
	crm_size = crm->size;
	crm_size_2 = crm->size_2;
	crm_faceSize = crm->faceSize;

	switch(crm->interp)
	{
		case CRM_INTERP_NEAREST:
		{
			// nearest neighbor intepolation
			// calculate indices into table
			i_l = (int)( (i_f+1) * crm_size_2 );
			j_l = (int)( (j_f+1) * crm_size_2 );
			n = f * crm_faceSize + (i_l + j_l*crm_size);

			r->r = v[n].r;
			r->g = v[n].g;
			r->b = v[n].b;
			return;
		}
		case CRM_INTERP_BILINEAR:
		{
			// bilinear interpolation
			
			// setup
			// calculate indices into table
			// floating point cube map coordinates
			i_f = (i_f+1) * crm_size_2;
			j_f = (j_f+1) * crm_size_2;
			// interpolation weights and 4 closest neighbours
			i_l = (int)i_f;
			j_l = (int)j_f;
			i_h = i_l+1 >= crm_size ? i_l : i_l+1;
			j_h = j_l+1 >= crm_size ? j_l : j_l+1;
			i_w = i_f - (int)i_f;
			j_w = j_f - (int)j_f;
			
			ti1 = f * crm_faceSize;
			ti2 = j_l * crm_size;
			ti3 = j_h * crm_size;
			n1 = ti1 + (i_l + ti2);
			n2 = ti1 + (i_l + ti3);
			n3 = ti1 + (i_h + ti2);
			n4 = ti1 + (i_h + ti3);

			// calc the bilinear interpolation
			_j_w = 1 - j_w;
			t1.r = v[n1].r*(_j_w) + v[n2].r*j_w;
			t1.g = v[n1].g*(_j_w) + v[n2].g*j_w;
			t1.b = v[n1].b*(_j_w) + v[n2].b*j_w;

			t2.r = v[n3].r*(_j_w) + v[n4].r*j_w;
			t2.g = v[n3].g*(_j_w) + v[n4].g*j_w;
			t2.b = v[n3].b*(_j_w) + v[n4].b*j_w;

			_i_w = 1 - i_w;
			r->r = t1.r*(_i_w) + t2.r*i_w;
			r->g = t1.g*(_i_w) + t2.g*i_w;
			r->b = t1.b*(_i_w) + t2.b*i_w;
			return;
		}
		default:
			break;
	}
}








// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
