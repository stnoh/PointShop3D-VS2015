//
// Title:   LSrList.cpp
// Created: Thu Sep 25 14:50:34 2003
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
// List of light sources
//
// LSrList.h
//-------------------------------------------------------------------------

#ifndef LSRLIST_H
#define LSRLIST_H

#include "../../../../DataTypes/src/Vector3D.h"
#include "LightSource.h"

// Variable and type declarations
typedef unsigned int LSrHandle;

typedef struct lslItem
{
	LightSource *lsr;
	LSrHandle h;
	struct lslItem *next;
	struct lslItem *prev;
} LSLItem;

typedef struct LSrList
{	
	LSLItem *enabled;			// list of enabled light sources
	LSLItem *disabled;			// list of disabled light sources
	LSLItem *all;				// primary list of light sources
} LSrList;


// Function prototypes
LSrList *LSLNew();
void LSLFree(LSrList *lsl);

LSrHandle LSLNewLight(LSrList *lsl);
void LSLFreeLight(LSrList *lsl, LSrHandle h);

void LSLSetLight(LSrList *lsl, LSrHandle h, LightSource *l);
void LSLEnable(LSrList *lsl, LSrHandle h);
void LSLDisable(LSrList *lsl, LSrHandle h);

/**
 * Sets the direction for the light source given by the <code>handle</code>. The direction is heading
 * <em>towards</em> the light source and must be normalized.
 *
 * @param lightSourceList
 *        a pointer to a <code>LSrList</code> containing the light sources
 * @param handle
 *        a <code>LSrHandle</code> identifying the light source
 * @param newDirection
 *        a <code>Vector3D</code> specifying the new normalized direction for the light source
 */
void LSLSetLightDirection (const LSrList *lightSourceList, const LSrHandle handle, const Vector3D newDirection);

/**
 * Returns the direction for the light source given by the <code>handle</code>. The direction is heading
 * <em>towards</em> the light source.
 *
 * @param lightSourceList
 *        a pointer to a <code>LSrList</code> containing the light sources
 * @param handle
 *        a <code>LSrHandle</code> identifying the light source
 * @return a <code>Vector3D</code> specifying the normalized direction of the light source
 */
const Vector3D LSLGetLightDirection (const LSrList *lightSourceList, const LSrHandle handle);

// Global variables
extern LSLItem* llt_cur;


// Macros
#define LLTNEW(_lsl) (llt_cur=_lsl->enabled)
#define LLTNEXT (llt_cur=llt_cur->next)
#define LLTENDREACHED (llt_cur==NULL)
#define LLTGETCURRENT (llt_cur->lsr)


#endif // LSRLIST_H

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
