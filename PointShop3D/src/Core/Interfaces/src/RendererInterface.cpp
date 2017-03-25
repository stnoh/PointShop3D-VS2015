// Title:   RendererInterface.cpp
// Created: Thu Sep 25 14:16:34 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker, Tim Weyrich
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

#include "RendererInterface.h"

std::set<void (*)(void)>  *RendererInterface::postRenderCBMap = 0;

RendererInterface::RendererInterface()
{
	if (!postRenderCBMap) {
		postRenderCBMap = new std::set<void (*)(void)>;
	}
}

RendererInterface::~RendererInterface()
{
}

void  RendererInterface::registerPostRenderCB(void (*extCallBack)(void))
{
	postRenderCBMap->insert(extCallBack);
}

void  RendererInterface::unregisterPostRenderCB(void (*extCallBack)(void))
{
	postRenderCBMap->erase(extCallBack);
}

void  RendererInterface::callPostRenderCBs(void)
{
	std::set<void (*)(void)>::iterator  it;
	for (it=(*postRenderCBMap).begin(); it!=(*postRenderCBMap).end(); ++it)
	{
		(*it)();
	}
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
