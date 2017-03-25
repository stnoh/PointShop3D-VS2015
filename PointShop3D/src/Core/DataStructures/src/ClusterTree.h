// Title:   ClusterTree.h
// Created: Thu Sep 25 14:29:47 2003
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

#ifndef __SPHERECLUSTERTREE_H_
#define __SPHERECLUSTERTREE_H_

#include "Clustering.h"
#include "ClusterNode.h"
#include "../../../Core/Interfaces/src/SurfelInterface.h"
#include "../../src/CoreDLL.h"

/**
 * Builds a tree of clusters. The root node contains all surfels, it is then splitted according to PCA.
 *
 * @author Richard Keiser
 * @version 2.0
 */
class ClusterTree {

public:
	/**
	 * Constructor, initializes the tree
	 */
	CORE_API ClusterTree();

	/**
	 * Destructor, deletes the tree
	 */
	CORE_API ~ClusterTree();

	/**
	 * Builds a tree of clusters. The root nodes contains all <code>surfels</code>. The nodes are splitted
	 * according to PCA until the maximum level <code>maxLevel</code> is reached, or both the size of the cluster 
	 * is equal or smaller than <code>maxClusterSize</code> and the variation of the cluster is smaller than
	 * <code>maxVariation</code>.
	 *
	 * <b>Note:</b> If the curvature should not be considered, set <code>maxVariation</code> to 1.0. This also
	 *				speeds up the computation.
	 *
	 * @param surfels
	 *		  a vector of <code>SurfelInterface</code>s
	 * @param nOfSurfels
	 *		  the size of the array
	 * @param maxClusterSize
	 *		  the maximum number of surfels in a cluster
	 * @param maxVariation
	 *		  the maximum variation in a cluster (0 < <code>maxVariation</code> <= 1.0)
	 * @param maxLevel
	 *		  the maximum level (height) of the tree
	 *
	 */
	CORE_API void buildTree(std::vector<SurfelInterface*> *surfels, unsigned int maxClusterSize, float maxVariation=1.0, unsigned int maxLevels = 30);

	/**
	 * returns the root node containing all surfels
	 */
	inline ClusterNode* getRoot();

	/**
	 * returns all clusters as a <code>std::vector</code>
	 */
	inline std::vector<ClusterNode*>* getClusters();

private:
	SurfelInterface				**m_surfelArray;
	unsigned int				m_nOfSurfels;
	unsigned int				m_maxLevels;
	unsigned int				m_maxSurfels;
	unsigned int				m_cntLevels;
	Clustering					*m_clustering;

	void clear();
};

inline ClusterNode* ClusterTree::getRoot() {
	if (m_clustering) {
		return m_clustering->getRoot();
	}
	else {
		return NULL;
	}
}

inline std::vector<ClusterNode*>* ClusterTree::getClusters() {
	if (m_clustering) {
		return m_clustering->getClusters();
	}
	else {
		return NULL;
	}
}



#endif


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
