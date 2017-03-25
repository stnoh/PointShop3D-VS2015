// Title:   Clustering.h
// Created: Thu Sep 25 14:30:07 2003
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

#ifndef __CLUSTERING_H_
#define __CLUSTERING_H_

#include <vector>
#include "../../../DataTypes/src/Vector3D.h"
#include "SurfelCollection.h"
#include "ClusterNode.h"

/**
 * Builds a tree of clusters. The root node contains all surfels, it is then splitted according to PCA.
 *
 * @author Richard Keiser
 * @version 2.0
 */
class Clustering {
	
public:

	/**
	 * Constructor, initializes the tree
	 */
	Clustering() {
		m_root = NULL;
	}

	/**
	 * Destructor, deletes the tree
	 */
	~Clustering();

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
	 *		  an array of <code>SurfelInterface</code>s
	 * @param nOfSurfels
	 *		  the size of the array
	 * @param maxClusterSize
	 *		  the maximum number of surfels in a cluster
	 * @param maxVariation
	 *		  the maximum variation in a cluster (0 < <code>maxVariation</code> <= 1.0)
	 * @param maxLevel
	 *		  the maximum level (height) of the tree
	 *
	 * @see #recomputeHierarchicalClustering
	 */
	void downsampleHierarchicalClustering (SurfelInterface **surfels, int nOfSurfels, int maxClusterSize, float maxVariation, unsigned int maxLevel);

	/**
	 * similar to <code>downsampleHierarchicalClustering</code>, but starting with startNode and the new parameters.
	 * This method might be useful to refine only certain nodes.
	 *
	 * <b>Note:</b> The children of startNode are deleted.
	 *
	 * @param startNode
	 *		  starts the recomputation with this node
	 * @param nOfSurfels
	 *		  the size of the array
	 * @param maxClusterSize
	 *		  the maximum number of surfels in a cluster
	 * @param maxVariation
	 *		  the maximum variation in a cluster (0 < <code>maxVariation</code> <= 1.0)
	 * @param maxLevel
	 *		  the maximum level (height) of the tree
	 *
	 * @see #downsampleHierarchicalClusterin
	 */
	void recomputeHierarchicalClustering(ClusterNode *startNode, int maxClusterSize, float maxVariation, unsigned int maxLevel);

	/**
	 * returns the root node containing all surfels
	 */
	inline ClusterNode* getRoot() const {
		return m_root;
	}

	/**
	 * returns all clusters as a <code>std::vector</code>
	 */
	inline std::vector<ClusterNode*>* getClusters() {
		return &hCClusters;
	}


protected:
	/**
	 * recursive method which computes the children of node, called by <code>downsampleHierarchicalClustering</code>
	 * and <code>recomputeHierarchicalClustering</code>.
	 * Uses the variation information to split the node. Slower than <code>createClusterNodeTree</code>
	 *
	 * @param node
	 *		  actual node to split
	 * @param level
	 *		  actual level of the tree, where the root node has the maximum level
	 * @see #recomputeHierarchicalClustering
	 * @see #downsampleHierarchicalClusterin
	 * @see #createClusterNodeTreeWithoutVariation
	 */
	void createClusterNodeTreeWithVariation (ClusterNode* node, unsigned int level);

	/**
	 * recursive method which computes the children of node, called by <code>downsampleHierarchicalClustering</code>
	 * and <code>recomputeHierarchicalClustering</code>.
	 * Does not compute the variation. This method is used if the variation is 1.0
	 *
	 * @param node
	 *		  actual node to split
	 * @param level
	 *		  actual level of the tree, where the root node has the maximum level
	 * @see #recomputeHierarchicalClustering
	 * @see #downsampleHierarchicalClusterin
	 * @see createClusterNodeTreeWithVariation
	 */
	void createClusterNodeTreeWithoutVariation (ClusterNode* node, unsigned int level);

	/**
	 * deletes all children of <code>node</code>
	 *
	 * <b>Note:</b><code>node</code> itself is not deleted
	 */
	void deleteTree(ClusterNode* node);
	
private:
	float						hCmaxVariation;	// used for hierarchical clustering
	unsigned int				hCmaxClusterSize;
	unsigned int				hCmaxLevel;
	ClusterNode					*m_root;
	std::vector<ClusterNode*>	hCClusters;
};






#endif  // __CLUSTERING_H_


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
