// Title:   Cluster.cpp
// Created: Thu Sep 25 14:21:27 2003
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

#include <list>

#include "../../../DataTypes/src/Vector3D.h"
#include "../../src/CoreDLL.h"
#include "NeighbourHood.h"
#include "Cluster.h"

Cluster::Cluster (NeighbourHood *neighbourHood, const Vector3D *normals, const uint minimumClusterSize) {

	this->neighbourHood      = neighbourHood;
	this->normals            = normals;
	this->minimumClusterSize = minimumClusterSize;
	nofPositions             = neighbourHood->getNofPositions();
	isClusterUpdated         = false;

	clusterPoints            = 0;
	clusterNormals           = 0;
	parentIndices            = 0;

}

Cluster::~Cluster() {
	this->cleanUp();
}

void Cluster::setNeighbourHood (NeighbourHood *newNeighbourHood) {
	neighbourHood    = newNeighbourHood;
	nofPositions     = neighbourHood->getNofPositions();
	isClusterUpdated = false;
}

NeighbourHood *Cluster::getNeighbourHood() const {
	return neighbourHood;
}

void Cluster::setNormals (const Vector3D *newNormals) {
	normals = newNormals;
	isClusterUpdated = false;
}

const Vector3D *Cluster::getNormals() const {
	return normals;
}

void Cluster::setMinimumClusterSize (const uint newMinimumClusterSize) {
	minimumClusterSize = newMinimumClusterSize;
	isClusterUpdated = false;
}

uint Cluster::getMinimumClusterSize() const {
	return minimumClusterSize;
}

void Cluster::calculate (Vector3D **clusterPoints, Vector3D **clusterNormals, uint *nofClusters) {

	uint             i,
		             j,
			         neighbourIndex;
	Vector3D         *cCenters;
	Vector3D         *cNormals;
	uint             *neighbors;
	PositionFlags    *flags;
	const Vector3D   *positions;
	uint             *clusterSizes;
	uint             index;
	
	// remove the old solutions first
	this->cleanUp();

	parentIndices = new uint[nofPositions];

	// conservative guess on size
	cCenters = new Vector3D[nofPositions / minimumClusterSize];
	cNormals = new Vector3D[nofPositions / minimumClusterSize];
	neighbors = new uint [nofPositions * minimumClusterSize];
	

	flags = new PositionFlags[nofPositions];
	for (i = 0; i < nofPositions; i++) {
		flags[i] = NO_FLAGS;
	}
		
	this->nofClusters = 0;
	
	////////////////////////////////////////////////////
	// cluster points

	positions = neighbourHood->getPositions();	

	neighbourHood->setNofNeighbours (minimumClusterSize);

	for (i = 0; i < nofPositions; i++) {
		
		if (flags[i] != CLUSTER_COVERED) {

			// find closest points and check if no surfel has already been included
			// in a cluster
			neighbourHood->setSourcePoint (positions[i]);			

			for (j = 0; j < minimumClusterSize; j++) {	
				
				neighbourIndex = neighbourHood->getNeighbourPositionIndex(j);
				if (flags[neighbourIndex] & CLUSTER_COVERED) {
					flags[i] = CLUSTER_STRAY;
					break;
				}				
			}

			// mark all neighbors as already included in cluster
			if (flags[i] != CLUSTER_STRAY) {
				cCenters[this->nofClusters].makeZero();
				for (j = 0; j < minimumClusterSize; j++) {
					index = neighbourHood->getNeighbourPositionIndex(j);
					flags [index] = CLUSTER_COVERED;
					parentIndices [index] = this->nofClusters;
					cCenters[this->nofClusters] += positions[index];
					cNormals[this->nofClusters] += normals[index];
				}

				this->nofClusters++;
			}
			
			// and store neighborhood
			for (j = 0; j < minimumClusterSize; j++) {
				neighbors[i * minimumClusterSize + j] = neighbourHood->getNeighbourPositionIndex(j);
			}
		}	
	}
	
	// allocate final array of clusters
	this->clusterPoints  = new Vector3D [this->nofClusters];
	this->clusterNormals = new Vector3D [this->nofClusters];
	
	clusterSizes = new uint[this->nofClusters];	
	for(i = 0; i < this->nofClusters; i++) {
		clusterSizes[i] = minimumClusterSize;
	}

	// assign stray samples to cluster of closest point
	for (i = 0; i < nofPositions; i++) {
		if (flags[i] == CLUSTER_STRAY) {
			for (j = 0; j < minimumClusterSize; j++) {
				index = neighbors[i * minimumClusterSize + j];
				if (flags[index] == CLUSTER_COVERED) {
					parentIndices [i] = parentIndices [index];
					cCenters[parentIndices[i]] += positions[i];
					cNormals[parentIndices[i]] += normals[i];
					clusterSizes[parentIndices[i]]++;
					break;
				}
			}
		}
	}

	// normalize cluster centers and assign

	for(i = 0; i < this->nofClusters; i++) {
		this->clusterPoints[i] = cCenters[i] / clusterSizes[i];
		cNormals[i].normalize();
		this->clusterNormals[i] = cNormals[i];
	}
	
	// assign solution arrays to parameters (return values!)
	*clusterPoints  = this->clusterPoints;
	*clusterNormals = this->clusterNormals;
	*nofClusters = this->nofClusters;


	delete[] cCenters;
	delete[] cNormals;
	delete[] neighbors;
	delete[] clusterSizes;
	delete[] flags;

}


uint Cluster::getParentPositionIndex (const uint positionIndex) {
	return parentIndices[positionIndex];
}

// ***************
// private methods
// ***************

void Cluster::cleanUp() {

	if (clusterPoints != 0) {
		delete[] clusterPoints;
		clusterPoints = 0;
	}

	if (clusterNormals != 0) {
		delete[] clusterNormals;
		clusterNormals = 0;
	}

	if (parentIndices != 0) {
		delete[] parentIndices;
		parentIndices = 0;
	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
