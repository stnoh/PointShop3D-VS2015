// Title:   sfldefs_duumyt.h
// Created: Thu Sep 25 15:46:15 2003
// Author:  Tim Weyrich
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

/*
 *    Title: sfldefs.h
 *  Created: Sat Apr 28 19:47:12 2001
 *   Author: Tim Weyrich <weyrich@ira.uka.de>
 *      $Id: sfldefs_duumyt.h,v 1.2 2003/09/25 14:09:16 rkeiser Exp $
 *
 * copyright (c) 2000 by quasi.modo software
 *
 * $Log: sfldefs_duumyt.h,v $
 * Revision 1.2  2003/09/25 14:09:16  rkeiser
 * inserted headers
 *
 * Revision 1.1.1.1  2003/07/08 16:08:42  mwaschbu
 * initial import
 *
 * Revision 1.1.1.1  2002/05/08 08:58:47  weyrich
 * surfel file format
 *
 * Revision 1.1.1.1  2002/03/26 17:32:13  weyrich
 * first check-in
 *
 */

#ifndef __SFLDEFS__
#define __SFLDEFS__

/*
 *  Win32 Stuff
 */

#ifdef _WIN32
/* The following ifdef block is the standard way of creating macros which make exporting
 * from a DLL simpler. All files within this DLL are compiled with the SFL_EXPORTS
 * symbol defined on the command line. this symbol should not be defined on any project
 * that uses this DLL. This way any other project whose source files include this file see
 * SFL_API functions as being imported from a DLL, wheras this DLL sees symbols
 * defined with this macro as being exported.
 */
#  ifdef SFL_EXPORTS
#    define SFL_API __declspec(dllexport)
#    include "../stdafx.h"
#  else
#    define SFL_API __declspec(dllimport)
#  endif
#else
#  define SFL_API
#endif

/*
 *  Supported Surfel Properties
 */

#define  SFLPROP_POSITION             0x00000001
#define  SFLPROP_NORMAL               0x00000002

#define  SFLPROP_NORMAL_COMPR_MASK    0x0000000c /*!< \brief These two bits encode the normal compression. */
#define  SFLPROP_NORMAL_COMPR_SHIFT            2 /*!< \brief Right-shift, that is needed to right-align the compression mask. */

#define  SFLPROP_RADIUS               0x00000010
#define  SFLPROP_WEIGHT               0x00000020
#define  SFLPROP_SAMPLING_DENSITY     0x00000040
#define  SFLPROP_TEXTURE_UV			  0x00000080

#define  SFLPROP_AMBIENT_COLOR        0x00000100
#define  SFLPROP_AMBIENT_COEFF        0x00000200
#define  SFLPROP_DIFFUSE_COLOR        0x00000400
#define  SFLPROP_DIFFUSE_COEFF        0x00000800
#define  SFLPROP_SPECULAR_COLOR       0x00001000
#define  SFLPROP_SPECULAR_COEFF       0x00002000
#define  SFLPROP_SHININESS            0x00008000

#define  SFLPROP_COLOR_MODEL_L        0x00010000
#define  SFLPROP_COLOR_MODEL_LA       0x00090000
#define  SFLPROP_COLOR_MODEL_RGB      0x00070000
#define  SFLPROP_COLOR_MODEL_RGBA     0x000f0000
#define  SFLPROP_COLOR_MODEL_YUV      0x00100000
#define  SFLPROP_COLOR_MODEL_YUVA     0x00180000

#define  SFLPROP_COLOR_MODEL_MASK     0x001f0000 /*!< \brief masks all color model information */

#define  SFLPROP_USER_FLAGS           0x08000000

/* Not supported at the moment: */
#define  SFLPROP_USER_FIELD0          0x10000000
#define  SFLPROP_USER_FIELD1          0x20000000
#define  SFLPROP_USER_FIELD2          0x40000000
#define  SFLPROP_USER_FIELD3          0x80000000


/*
 *  File Format
 */

#define  GFF_API  SFL_API
#include "gff.h"

#define  SFL_MAGIC    "SFLF"
#define  SFL_VERSION  0x0000

#define  SFL_SURFEL_FORMAT_VERSION  0x00000000UL

/* Tags for the main header
 */
#define  SFLTAG_TITLE               0x0100 /* optional */
#define  SFLTAG_AUTHOR              0x0101 /* optional */
#define  SFLTAG_APPLICATION         0x0102 /* optional */
#define  SFLTAG_DESCRIPTION         0x0103 /* optional */
#define  SFLTAG_VERSION             0x0104 /* optional */
#define  SFLTAG_DATE                0x0105 /* optional */

#define  SFLTAG_RIGHT_VEC           0x0110 /* default: 1 0 0 */
#define  SFLTAG_UP_VEC              0x0111 /* default: 0 1 0 */
#define  SFLTAG_BACK_VEC            0x0112 /* default: 0 0 1 */

#define  SFLTAG_DEFAULT_UNITS       0x0113 /* default: 1.0 */

#define  SFLTAG_BOUNDING_BOX        0x0118 /* optional */
/* The overall bounding box is given as (x1, y1, z1, x2, y2, z2) in
   world coordinates, using DEFAULT_UNITS. */

#define  SFLTAG_SURFEL_SET_LIST_IDX 0x0120 /* Child index of the surfel-set directory. Mandatory. */

/* Tags for surfel set list header
 */
#define  SFLTAG_NUM_SURFEL_SETS     0x0130 /* mandatory */

/* Tags for surfel set header
 *
 *   Besides the tags below, the following tags may be repeated:
 *
 *     SFLTAG_BOUNDING_BOX
 *
 *   The bounding box is given in object coordinates (before the
 *   transform)
 *
 */
#define  SFLTAG_SURFEL_SET_NUM_RESOLUTIONS  0x0140 /* default: 1 */
#define  SFLTAG_SURFEL_SET_RES0_IDX         0x0141 /* default: numResolutions-1 */
#define  SFLTAG_SURFEL_SET_FORMAT_VERSION   0x0150 /* mandatory (at the moment: (uint32)0x0) */
#define  SFLTAG_SURFEL_SET_SURFEL_SIZE      0x0151 /* mandatory (uint32) */
#define  SFLTAG_SURFEL_SET_PROPERTIES       0x0152 /* bit-vector(!); mandatory (uint32s) */
#define  SFLTAG_SURFEL_SET_PROP_INDICES     0x0153 /* mandatory: field indices corresponding to offsets (uint16s) */
#define  SFLTAG_SURFEL_SET_PROP_OFFSETS     0x0154 /* mandatory: field offsets inside the surfel (uint32s) */
#define  SFLTAG_SURFEL_SET_PROP_TYPES       0x0155 /* not used at the moment (uint32s) */
#define  SFLTAG_SURFEL_SET_UNITS            0x0160 /* not written, at the moment (float) */
#define  SFLTAG_SURFEL_SET_TRANSFORM        0x0161 /* default: id (16 floats)*/
#define  SFLTAG_SURFEL_SET_IDENTIFIER	    0x0162 /* default: "surfelSet%02d" */

/* Tags for resolution header
 */
#define  SFLTAG_SURFEL_RSL_NUM_SURFELS      0x0170 /* mandatory */
#define  SFLTAG_SURFEL_RSL_DATA             0x0171 /* LDB follows */

#endif

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
