// Title:   LSrList.cpp
// Created: Thu Sep 25 14:50:37 2003
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
// List of light sources
//
// LSrList.c
//-------------------------------------------------------------------------

// Standard Includes
#include <malloc.h>
#include <stdio.h>


// Project Includes
#include "LSrList.h"

// Global variables
LSLItem* llt_cur;


static LSrHandle lslGetHandle(LSrList *lsl, LSLItem **prev);
static LSLItem *lslGetItem(const LSrList *cml, const LSrHandle h);


//-------------------------------------------------------------------------
// Construct a list of light sources
//-------------------------------------------------------------------------
LSrList *LSLNew()
{
	LSrList *lsl;

	lsl = (LSrList *)malloc(sizeof(LSrList));
	lsl->all = NULL;
	lsl->disabled = NULL;
	lsl->enabled = NULL;

	return lsl;
}


//-------------------------------------------------------------------------
// Construct a list of light sources
//-------------------------------------------------------------------------
void LSLFree(LSrList *lsl)
{
	LSLItem *cur, *del;

	cur = lsl->all;
	while(cur)
	{
		del = cur;
		cur = cur->next;

		// free the light source here, this is the primary reference
		LSrFree(del->lsr);
		free(del);
	}

	cur = lsl->disabled;
	while(cur)
	{
		del = cur;
		cur = cur->next;

		free(del);
	}

	cur = lsl->enabled;
	while(cur)
	{
		del = cur;
		cur = cur->next;

		free(del);
	}

	free(lsl);
}


//-------------------------------------------------------------------------
// Add a new light to the list
//-------------------------------------------------------------------------
LSrHandle LSLNewLight(LSrList *lsl)
{
	LSrHandle h;
	LightSource *lsr;
	LSLItem *newitem, *prev;

	// get an unused handle and the insertion position for the new item
	h = lslGetHandle(lsl, &prev);

	// allocate a new light source (with default properties)
	lsr = LSrNew();

	// insert a new item into 'all' list
	// note that this list is ordered according to the handles
	newitem = (LSLItem *)malloc(sizeof(LSLItem));
	newitem->lsr = lsr;
	newitem->h = h;

	if(prev == NULL)
	{
		// new item is first in list
		newitem->next = lsl->all;
		newitem->prev = NULL;

		if(lsl->all)
		{
			lsl->all->prev = newitem;
		}
		lsl->all = newitem;
	} else
	{
		// add new item after prev
		newitem->next = prev->next;
		newitem->prev = prev;

		if(prev->next)
		{
			prev->next->prev = newitem;
		}
		prev->next = newitem;
	}

	// insert new item into 'disabled' list
	newitem = (LSLItem *)malloc(sizeof(LSLItem));
	newitem->lsr = lsr;
	newitem->h = h;

	if(lsl->disabled)
	{
		lsl->disabled->prev = newitem;
	}
	newitem->next = lsl->disabled;
	newitem->prev = NULL;
	lsl->disabled = newitem;
	
	return h;
}


//-------------------------------------------------------------------------
// Remove a light from the list
//-------------------------------------------------------------------------
void LSLFreeLight(LSrList *lsl, LSrHandle h)
{
	LSLItem *del, *cur;

	if( (del = lslGetItem(lsl, h)) == NULL )
		return;

	// search 'del' in 'enabled' and 'disabled' lists
	cur = lsl->enabled;
	while(cur && cur->h!=del->h)
	{
		cur = cur->next;
	}
	if(!cur)
	{
		cur = lsl->disabled;
		while(cur && cur->h!=del->h)
		{
			cur = cur->next;
		}
	}

	// remove item from 'enabled' or 'disabled' list
	// note: if cur is NULL here, then there is a bug in this code...
	if(cur->next)
	{
		cur->next->prev = cur->prev;
	}
	if(cur->prev)
	{
		cur->prev->next = cur->next;
	}

	if(cur == lsl->enabled)
	{
		lsl->enabled = cur->next;
	} else if(cur == lsl->disabled)
	{
		lsl->disabled = cur->next;
	}
	free(cur);

	// remove 'del' from 'all' list and free light source
	if(del->prev)
	{
		del->prev->next = del->next;
	}
	if(del->next)
	{
		del->next->prev = del->prev;
	}
	if(del == lsl->all)
	{
		lsl->all = del->next;
	}
	LSrFree(del->lsr);
	free(del);
}


//-------------------------------------------------------------------------
// Set a light source by copying data from a given light source
//-------------------------------------------------------------------------
void LSLSetLight(LSrList *lsl, LSrHandle h, LightSource *l)
{
	LSLItem *cur;

	cur = lslGetItem(lsl, h);

	if(cur)
	{
		LSrCopy(cur->lsr, l);
	}
}


//-------------------------------------------------------------------------
// Enable a light by moving it from the 'disabled' to the 'enabled' list
//-------------------------------------------------------------------------
void LSLEnable(LSrList *lsl, LSrHandle h)
{
	LSLItem *cur, *dis;

	cur = lslGetItem(lsl, h);

	if(cur)
	{
		// search item for handle 'h' in disabled list
		dis = lsl->disabled;
		while(dis && dis->h!=h)
		{
			dis = dis->next;
		}

		// move item with handle 'h' from 'disabled' to  'endabled' list
		if(dis)
		{
			// remove from disabled list
			if(dis->prev)
			{
				dis->prev->next = dis->next;
			}
			if(dis->next)
			{
				dis->next->prev = dis->prev;
			}
			if(dis == lsl->disabled)
			{
				lsl->disabled = dis->next;
			}

			// insert at start of 'enabled' list
			dis->next = lsl->enabled;
			dis->prev = NULL;
			if(lsl->enabled)
			{
				lsl->enabled->prev = dis;
			}
			lsl->enabled = dis;
		}
	}
}


//-------------------------------------------------------------------------
// Remove a light from the list
//-------------------------------------------------------------------------
void LSLDisable(LSrList *lsl, LSrHandle h)
{
	LSLItem *cur, *en;

	cur = lslGetItem(lsl, h);

	if(cur)
	{
		// search item for handle 'h' in 'enabled' list
		en = lsl->enabled;
		while(en && en->h!=h)
		{
			en = en->next;
		}

		// move item with handle 'h' from 'enabled' to  'disabled' list
		if(en)
		{
			// remove from enabled list
			if(en->prev)
			{
				en->prev->next = en->next;
			}
			if(en->next)
			{
				en->next->prev = en->prev;
			}
			if(en == lsl->enabled)
			{
				lsl->enabled = en->next;
			}

			// insert at start of 'disabled' list
			en->next = lsl->disabled;
			en->prev = NULL;
			if(lsl->disabled)
			{
				lsl->disabled->prev = en;
			}
			lsl->disabled = en;
		}
	}
}

void LSLSetLightDirection (const LSrList *lightList, const LSrHandle handle, const Vector3D newDirection) {

	LSLItem     *current;
	LightSource *lightSource;

	current = lslGetItem (lightList, handle);

	if (current != 0) {

		lightSource = current->lsr;

		lightSource->x = newDirection[0];
		lightSource->y = newDirection[1];
		lightSource->z = newDirection[2];

	}

}

const Vector3D LSLGetLightDirection (const LSrList *lightList, const LSrHandle handle) {

	LSLItem     *current;
	LightSource *lightSource;
	Vector3D    direction;

	current = lslGetItem (lightList, handle);

	if (current != 0) {

		lightSource = current->lsr;

		direction[0] = lightSource->x;
		direction[1] = lightSource->y;
		direction[2] = lightSource->z;

	}

	return direction;

}


// Static functions
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Get a valid handle
//-------------------------------------------------------------------------
static LSrHandle lslGetHandle(LSrList *lsl, LSLItem **prev)
{
	LSrHandle h;
	LSLItem *cur;

	// find the first unused handle
	// note that items are stored ordered according to their handle
	h = 0;
	cur = lsl->all;
	while(cur && cur->h==h)
	{
		cur = cur->next;
		h++;
	}

	// return the insertion location
	if(cur)
	{
		*prev = cur->prev;
	} else
	{
		*prev = NULL;
	}

	// return the handle
	return h;
}


//-------------------------------------------------------------------------
// Get a list item given a handle
//-------------------------------------------------------------------------
static LSLItem *lslGetItem(const LSrList *lsl, const LSrHandle h)
{
	LSLItem *cur;

	cur = lsl->all;
	while(cur && cur->h!=h)
	{
		cur = cur->next;
	}

	// note: 'cur' might be NULL indicating that handle 'h' does not exist
	return cur;
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
