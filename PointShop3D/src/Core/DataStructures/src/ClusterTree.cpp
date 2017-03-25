// Title:   ClusterTree.cpp
// Created: Thu Sep 25 14:19:43 2003
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

#include "ClusterTree.h"

ClusterTree::ClusterTree() {
	m_surfelArray = NULL;
	m_clustering = NULL;
}

ClusterTree::~ClusterTree() {
	clear();
}


void ClusterTree::buildTree(std::vector<SurfelInterface*> *surfels, unsigned int maxClusterSize, float maxVariation, unsigned int maxLevels) {
	unsigned int i;

	clear();

	m_nOfSurfels = surfels->size();
	ASSERT(m_nOfSurfels > 0);
	if (m_nOfSurfels <= 0) {
		return;
	}

	m_surfelArray = new SurfelInterface*[m_nOfSurfels];
	for (i = 0; i < m_nOfSurfels; i++) {
		m_surfelArray[i] = (*surfels)[i];
	}

	m_clustering = new Clustering();
	// order of surfels in m_surfelArray might change
	m_clustering->downsampleHierarchicalClustering(m_surfelArray, m_nOfSurfels, maxClusterSize, maxVariation, maxLevels);
}



void ClusterTree::clear() {
	if (m_surfelArray) {
		delete[] m_surfelArray;
		m_surfelArray = NULL;
	}
	if (m_clustering) {
		delete m_clustering;
		m_clustering = NULL;
	}	
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
