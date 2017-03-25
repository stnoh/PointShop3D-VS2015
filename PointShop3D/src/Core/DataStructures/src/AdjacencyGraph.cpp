// Title:   AdjacencyGraph.cpp
// Created: Thu Sep 25 14:21:36 2003
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

#include "../../../DataTypes/src/Vector3D.h"

#include "AdjacencyGraph.h"



AdjacencyGraph::AdjacencyGraph (SurfelCollection* surfelCollection, unsigned int numNeighbors) {

	SurfelInterface *surfel;
	unsigned int    i, j,
		            numSurfels;

	this->numNeighbors = numNeighbors;
	numSurfels = surfelCollection->getNofSurfels();

	NeighbourHood *neighborHood = surfelCollection->buildNeighbourHood();
	// surfel itself is in neighborhood, so we need one more
	neighborHood->setNofNeighbours (numNeighbors + 1);

	surfelList = surfelCollection->getSurfelVector()->begin();

	neighborList = new unsigned int [numSurfels * numNeighbors];

	for (i = 0, surfel = surfelCollection->getFirstSurfel(); i < numSurfels; i++, surfel = surfelCollection->getNextSurfel()) {

		neighborHood->setSourcePoint (surfel->getPosition());

		for (j = 0; j < numNeighbors; j++) {			
			neighborList[i * numNeighbors + j] = neighborHood->getNeighbourPositionIndex (j + 1);
		}
	}
	
	eigenSolver = new Mgc::Eigen(3);


	delete neighborHood;
}

AdjacencyGraph::AdjacencyGraph (std::vector<SurfelInterface*>* surfels, unsigned int numNeighbors) {

	unsigned int    i, j,
					numSurfels;

	numSurfels = surfels->size();
	this->numNeighbors = numNeighbors;
	Vector3D* surfelPositions = new Vector3D[numSurfels];

	for (i = 0; i < numSurfels; i++) {
		surfelPositions[i] = surfels->at(i)->getPosition();
	}

	NeighbourHood *neighborHood = new NeighbourHood(surfelPositions, numSurfels);
	// surfel itself is in neighborhood, so we need one more
	neighborHood->setNofNeighbours (numNeighbors + 1);

	surfelList = surfels->begin();

	neighborList = new unsigned int [numSurfels * numNeighbors];

	for (i = 0; i < numSurfels; i++) {

		neighborHood->setSourcePoint ((*(surfelList + i))->getPosition());

		for (j = 0; j < numNeighbors; j++) {			
			neighborList[i * numNeighbors + j] = neighborHood->getNeighbourPositionIndex (j + 1);
		}
	}
	
	eigenSolver = new Mgc::Eigen(3);


	delete neighborHood;
}

AdjacencyGraph::AdjacencyGraph (NeighbourHood *surfelSearch, std::vector<SurfelInterface*> *surfels, unsigned int numNeighbors) {

	unsigned int    i, j,
					numSurfels;

	numSurfels = surfels->size();
	this->numNeighbors = numNeighbors;

		// surfel itself is in neighborhood, so we need one more
	surfelSearch->setNofNeighbours (numNeighbors + 1);

	surfelList = surfels->begin();

	neighborList = new unsigned int [numSurfels * numNeighbors];

	for (i = 0; i < numSurfels; i++) {

		surfelSearch->setSourcePoint ((*(surfelList + i))->getPosition());

		for (j = 0; j < numNeighbors; j++) {			
			neighborList[i * numNeighbors + j] = surfelSearch->getNeighbourPositionIndex (j + 1);
		}
	}
	
	eigenSolver = new Mgc::Eigen(3);

}


AdjacencyGraph::~AdjacencyGraph() {

	delete neighborList;
	delete eigenSolver;
}


unsigned int AdjacencyGraph::getNeighborIndex (unsigned int surfelIndex, unsigned int neighborIndex) {
	return neighborList [surfelIndex * numNeighbors + neighborIndex];
}


SurfelInterface* AdjacencyGraph::getNeighbor (unsigned int surfelIndex, unsigned int neighborIndex) {	
	return *(surfelList + (neighborList [surfelIndex * numNeighbors + neighborIndex]));
}


SurfelInterface* AdjacencyGraph::getSurfel (unsigned int surfelIndex) {
	return *(surfelList + surfelIndex);
}


Vector3D AdjacencyGraph::computeNormal (unsigned int surfelIndex, unsigned int numNeighbors) {

	unsigned int i;
	Vector3D     diffVector,
	             normal;

	// compute sums of products
	Mgc::Real fSumXX = 0.0f, fSumXY = 0.0f, fSumXZ = 0.0f;
	Mgc::Real fSumYY = 0.0f, fSumYZ = 0.0f, fSumZZ = 0.0f;
	

	Vector3D mean;
	mean.makeZero();
	for (i = 0; i < numNeighbors; i++) {		
		mean += getNeighbor (surfelIndex, i)->getPosition();
	}
	mean /= numNeighbors;

	for (i = 0; i < numNeighbors; i++) {

		diffVector = mean - getNeighbor (surfelIndex, i)->getPosition();
		
		fSumXX += diffVector[0] * diffVector[0];
		fSumXY += diffVector[0] * diffVector[1];
		fSumXZ += diffVector[0] * diffVector[2];
		fSumYY += diffVector[1] * diffVector[1];
		fSumYZ += diffVector[1] * diffVector[2];
		fSumZZ += diffVector[2] * diffVector[2];
	}
	
	// setup the eigensolver
	eigenSolver->Matrix(0,0) = fSumXX;
	eigenSolver->Matrix(0,1) = fSumXY;
	eigenSolver->Matrix(0,2) = fSumXZ;
	eigenSolver->Matrix(1,0) = eigenSolver->Matrix(0,1);
	eigenSolver->Matrix(1,1) = fSumYY;
	eigenSolver->Matrix(1,2) = fSumYZ;
	eigenSolver->Matrix(2,0) = eigenSolver->Matrix(0,2);
	eigenSolver->Matrix(2,1) = eigenSolver->Matrix(1,2);
	eigenSolver->Matrix(2,2) = fSumZZ;
	
	// compute eigenstuff, smallest eigenvalue is in last position
	eigenSolver->DecrSortEigenStuff3();
	
	
	
	// unit-length direction for best-fit line
	normal = Vector3D (eigenSolver->GetEigenvector(0,2), eigenSolver->GetEigenvector(1,2), eigenSolver->GetEigenvector(2,2));

	if (Vector3D::dotProduct (normal, getSurfel (surfelIndex)->getNormal()) < 0.0f) {
		normal = -normal;
	}

	return normal;

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
