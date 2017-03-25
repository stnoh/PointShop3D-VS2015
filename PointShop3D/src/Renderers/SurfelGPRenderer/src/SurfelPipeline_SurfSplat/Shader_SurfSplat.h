//
// Title:   Shader_SurfSplat.cpp
// Created: Thu Sep 25 14:50:23 2003
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
// Shader, shades samples in a z-buffer associated with a warper
// and writes result into an image buffer
//
// Note: 
// - this version does not support semi-transparent surfaces
// - performs shading in camera space, the phong illumination model is
// evaluated using cube reflectance maps 
//
// Shader_C_CubeReflMap.h 
//-------------------------------------------------------------------------

#ifndef SHADER_C_CUBEREFLMAP_H
#define SHADER_C_CUBEREFLMAP_H

#include "../SurfelPipeline_General/LSrList.h"
#include "../../../../DataTypes/src/MyDataTypes.h"
#include "Warper.h"

#define NOF_DBGCOLORS 8

// pack R,G,B components to format for display
#define PACK_DIM_RGB(_c,_r,_g,_b) \
	_c = ((_r << 16) & 0x00ff0000) | \
		((_g << 8) & 0x0000ff00) | \
		(_b & 0x000000ff)


// Global variables (for use by external shading functions)
extern float _shd_nx_c, _shd_ny_c, _shd_nz_c;
extern float _shd_x_c, _shd_y_c, _shd_z_c;
extern float _shd_vx, _shd_vy, _shd_vz;
extern float _shd_ndotv;
extern MyDataTypes::RGBTriple _shd_specularColor;
extern MyDataTypes::RGBTriple _shd_Id;
extern MyDataTypes::RGBTriple _shd_Ir;
extern float _shd_kA, _shd_kD, _shd_kS;
extern unsigned char _shd_shininess;
extern void *_shd_userdata;
extern int _shd_options;


// Variable and type declarations
typedef enum shdProperties
{
	SHD_BIDINORMALS = 1,
	SHD_DBGCOLORS = 2,
	SHD_ENVMAPPING = 4,
	SHD_LIGHTING = 8,
	SHD_PRESHADESPLAT = 16,
	SHD_PERPIXELSHADING = 32
} ShdProperties;

typedef enum shdShadingModel
{
	SHD_PHONG_H = 1,
	SHD_PHONG_R = 2,
	SHD_TORRANCESPARROW = 3,
	SHD_USERFNCT = 4
} ShdShadingModel;

typedef enum shdTextureFiltering
{
	SHD_TEXTUREFILTERING_NOINTERP = 1,
	SHD_TEXTUREFILTERING_LINEAR = 2
} ShdTextureFiltering;

typedef enum shdDiffColor
{
	SHD_DIFFCOL_DEFAULT = 0,
	SHD_DIFFCOL_MIPMAPLEVEL = 1,
	SHD_DIFFCOL_LODLEVEL = 2
} ShdDiffColor;

typedef void (*ShdLightSampleFnct)();
typedef void (*ShdTextureFilteringFnct)();

extern ShdLightSampleFnct _shd_lightsample;

typedef struct shader
{
	Warper *wrp;
	FrameBufferInterface *frameBuffer;

	LSrList *lsl;				

	ShdLightSampleFnct shdLightSample;

	ShdLightSampleFnct shdUserShadingFunction;
	void *shdUserShadingData;

	int options;
	ShdDiffColor diffColor;
	int dbgcolors[NOF_DBGCOLORS];
} Shader;


// Function prototypes
Shader* ShdNew(Warper *wrp, FrameBufferInterface *frameBuffer, LSrList *lsl);
void ShdFree(Shader *shd);

void ShdPrepareForShading(Shader *shd);
void ShdShadeZBuffer(Shader *shader, int magfactor, int bbox[4], QRgb backgroundColor);

void ShdEnable(Shader *shd, ShdProperties p);
void ShdDisable(Shader *shd, ShdProperties p);

void ShdSetShadingModel(Shader *shd, ShdShadingModel s);
void ShdSetUserShadingFunction(Shader *shd, ShdLightSampleFnct f, void *u);
void ShdSetDiffColor(Shader *shd, ShdDiffColor c);


#endif // SHADER_C_CUBEREFLMAP_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
