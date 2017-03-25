// Title:   Clustering.cpp
// Created: Thu Sep 25 14:25:33 2003
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

#include "Clustering.h"

Clustering::~Clustering() {
	deleteTree(m_root);
	delete m_root;
	m_root = NULL;
}

void Clustering::deleteTree(ClusterNode* node) {
	if (node) {
		ClusterNode* children = node->getChildren();
		if (children) {		
			deleteTree(children);
			deleteTree(children+1);
			delete[] children;
		}
	}
}


void Clustering::downsampleHierarchicalClustering (SurfelInterface **surfels, int nOfSurfels, 
												   int maxClusterSize, float maxVariation, unsigned int maxLevel) {
	
	hCmaxVariation = maxVariation;
	hCmaxClusterSize = maxClusterSize;
	hCmaxLevel = maxLevel;
	hCClusters.clear();
	
	// build tree of clusters
	if (m_root) {
		delete m_root;
	}
	m_root = new ClusterNode();
	m_root->init(NULL, surfels, nOfSurfels);

	if (maxVariation < 1.0) {
		createClusterNodeTreeWithVariation (m_root, maxLevel-1);
	}
	else {
		createClusterNodeTreeWithoutVariation (m_root, maxLevel-1);
	}
}


void Clustering::recomputeHierarchicalClustering(ClusterNode *startNode, int maxClusterSize, float maxVariation, unsigned int maxLevel) {

	hCmaxVariation = maxVariation;
	hCmaxClusterSize = maxClusterSize;
	hCmaxLevel = maxLevel;

	deleteTree(startNode);

	if (maxVariation < 1.0) {
		createClusterNodeTreeWithVariation (startNode, maxLevel-1);
	}
	else {
		createClusterNodeTreeWithoutVariation (startNode, maxLevel-1);
	}
	
}

//////////////////////////////////////////////////////////////////////
//

void Clustering::createClusterNodeTreeWithVariation (ClusterNode* node, unsigned int level) {

	// compute the variation, the center (average) and the split vector
	node->computeVariation();
	
	// use the average of the surfels position and the radius as an approximation of the miniball
	node->computeRadius();	
	// otherwise the correct miniball could be computed on the expense of more computation time
	//node->computeMiniball();
	
	if ((level > 0) &&
		(node->getNOfSurfels() > 3) && 
		((node->getNOfSurfels() > hCmaxClusterSize) ||
		(node->getVariation() > hCmaxVariation))) {
		
		// split node		
		int left = 0, right = node->getNOfSurfels() - 1;
		SurfelInterface **surfels = node->getArray();
		
		while (1) 
		{
			// March the "left" pointer to the right
			while ((Vector3D::dotProduct (node->getSplitVector(), surfels[left]->getPosition() - node->getCenter()) < 0) && (left < right)) {
				left++;
			}
			
			// March the "right" pointer to the left
			while ((Vector3D::dotProduct (node->getSplitVector(), surfels[right]->getPosition() - node->getCenter()) >= 0) && (right > left)) {
				right--;
			}

			// If the pointers have crossed, we're done
			if (right <= left) 
				break;
			
			// Else, swap and repeat
			SurfelInterface* tmp = surfels[left];
			surfels[left] = surfels[right];
			surfels[right] = tmp;
		}
		ClusterNode* children = new ClusterNode[2];
		children[0].init(node, surfels, left);
		children[1].init(node, surfels+left, node->getNOfSurfels() - left);
		node->setChildren(children);
		createClusterNodeTreeWithVariation (children, level-1);
		createClusterNodeTreeWithVariation (children+1, level-1);
	}	
	else {
		
		if (node->getNOfSurfels() > 0) {
			// put node into list of clusters
			hCClusters.push_back (node);		
		}
	}
}

//////////////////////////////////////////////////////////////////////
//

void Clustering::createClusterNodeTreeWithoutVariation (ClusterNode* node, unsigned int level) {

	// compute the center (averange) and the split vector
	node->computeSplitVector();
	
	// use the average of the surfels position and the radius as an approximation of the miniball
	node->computeRadius();	
	// otherwise the correct miniball could be computed on the expense of more computation time
	//node->computeMiniball();
	
	if ( (level > 0) &&
		 (node->getNOfSurfels() > 3) && 
		 (node->getNOfSurfels() > hCmaxClusterSize)
	   ) {
		
		// split node		
		int left = 0, right = node->getNOfSurfels() - 1;
		SurfelInterface **surfels = node->getArray();
		
		while (1) 
		{
			// March the "left" pointer to the right
			while ((Vector3D::dotProduct (node->getSplitVector(), surfels[left]->getPosition() - node->getCenter()) < 0) && (left < right)) {
				left++;
			}
			
			// March the "right" pointer to the left
			while ((Vector3D::dotProduct (node->getSplitVector(), surfels[right]->getPosition() - node->getCenter()) >= 0) && (right > left)) {
				right--;
			}

			// If the pointers have crossed, we're done
			if (right <= left) 
				break;
			
			// Else, swap and repeat
			SurfelInterface* tmp = surfels[left];
			surfels[left] = surfels[right];
			surfels[right] = tmp;
		}
		ClusterNode* children = new ClusterNode[2];
		children[0].init(node, surfels, left);
		children[1].init(node, surfels+left, node->getNOfSurfels() - left);
		node->setChildren(children);
		createClusterNodeTreeWithoutVariation (children, level-1);
		createClusterNodeTreeWithoutVariation (children+1, level-1);
	}
	else {
		
		if (node->getNOfSurfels() > 0) {
			// put node into list of clusters
			hCClusters.push_back (node);		
		}
	}
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
