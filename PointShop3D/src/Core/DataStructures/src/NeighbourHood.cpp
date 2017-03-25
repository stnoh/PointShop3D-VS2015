// Title:   NeighbourHood.cpp
// Created: Thu Sep 25 14:20:49 2003
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

#include "NeighbourHood.h"

NeighbourHood::NeighbourHood (const unsigned int bucketSize) {
	this->bucketSize	= bucketSize;
	nofNeighbours		= 1;
	maxSqrDistance		= FLT_MAX;
	eigenSolver			= new Mgc::Eigen(3);
	
	kdTree       = 0;
}


NeighbourHood::NeighbourHood (const Vector3D *positions, const unsigned int nofPositions, const unsigned int bucketSize) {
	this->positions		= positions;
	this->nofPositions	= nofPositions;
	this->bucketSize	= bucketSize;
	nofNeighbours		= 1;
	maxSqrDistance		= FLT_MAX;
	eigenSolver			= new Mgc::Eigen(3);
	
	kdTree       = 0;
	this->rebuildKDTree();
}

NeighbourHood::~NeighbourHood() {

	if (kdTree != 0) {
		delete kdTree;
	}
	delete eigenSolver;
	
}

void NeighbourHood::setMaxQuerySqrDistance(float maxSqrDist) {
	maxSqrDistance = maxSqrDist;
	neighboursUpdated = false;
}

void NeighbourHood::clearMaxQueryDistance() {
	maxSqrDistance = FLT_MAX;
	neighboursUpdated = false;
}

unsigned int NeighbourHood::getNofFoundNeighbours() {
	if (neighboursUpdated == false) {
		// we have to compute the neighbours first
		kdTree->queryRange(sourcePoint, maxSqrDistance);
		neighboursUpdated = true;
	}

	return kdTree->getNOfFoundNeighbours();
}

void NeighbourHood::setPositions (const Vector3D *newPositions, const unsigned int newNofPositions) {

	positions       = newPositions;
	nofPositions    = newNofPositions;
	this->rebuildKDTree();

}

const Vector3D *NeighbourHood::getPositions() const {
	return positions;
}

unsigned int NeighbourHood::getNofPositions() const {
	return nofPositions;
}

void NeighbourHood::setNofNeighbours (const unsigned int newNofNeighbours) {

	// allocate new data
	nofNeighbours    = newNofNeighbours;
	kdTree->setNOfNeighbours(newNofNeighbours);

	neighboursUpdated = false;

}

void NeighbourHood::setSourcePoint (const Vector3D newSourcePoint) {
	sourcePoint       = newSourcePoint;
	neighboursUpdated = false;
}

Vector3D NeighbourHood::getSourcePoint() const {
	return sourcePoint;
}

unsigned int NeighbourHood::getNofNeighbours() const {
	return nofNeighbours;
}

unsigned int NeighbourHood::getNeighbourPositionIndex (const unsigned int neighbourIndex) {

	if (neighboursUpdated == false) {
		// we have to compute the neighbours first
		kdTree->queryRange(sourcePoint, maxSqrDistance);
		neighboursUpdated = true;
	}

	return kdTree->getNeighbourPositionIndex(neighbourIndex);
	
}


Vector3D NeighbourHood::getNeighbourPosition (const unsigned int neighbourIndex) {

	if (neighboursUpdated == false) {
		// we have to compute the neighbours first
		kdTree->queryRange(sourcePoint, maxSqrDistance);
		neighboursUpdated = true;
	}

	return kdTree->getNeighbourPosition(neighbourIndex);
}


float NeighbourHood::getSquaredDistance (const unsigned int neighbourIndex) {

	if (neighboursUpdated == false) {
		// we have to compute the neighbours first
		// we have to compute the neighbours first
		kdTree->queryRange(sourcePoint, maxSqrDistance);
		neighboursUpdated = true;
	}

	return kdTree->getSquaredDistance(neighbourIndex);
}

// normal is not consistent!!!
Vector3D NeighbourHood::computeNormal () {

	unsigned int i;
	Vector3D     diffVector,
	             normal;

	// compute sums of products
	Mgc::Real fSumXX = 0.0f, fSumXY = 0.0f, fSumXZ = 0.0f;
	Mgc::Real fSumYY = 0.0f, fSumYZ = 0.0f, fSumZZ = 0.0f;
	

	Vector3D mean;
	mean.makeZero();
	for (i = 0; i < nofNeighbours; i++) {		
		mean += getNeighbourPosition(i);
	}
	mean /= nofNeighbours;

	for (i = 0; i < nofNeighbours; i++) {

		diffVector = mean - getNeighbourPosition(i);
		
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


	return normal;

}



float NeighbourHood::computeVariation () {

	if (neighboursUpdated == false) {
		// we have to compute the neighbours first
		kdTree->queryRange(sourcePoint, maxSqrDistance);
		neighboursUpdated = true;
	}

	
	unsigned int i;
	Vector3D     diffVector;
	float        variation;


	// compute sums of products
	Mgc::Real fSumXX = 0.0f, fSumXY = 0.0f, fSumXZ = 0.0f;
	Mgc::Real fSumYY = 0.0f, fSumYZ = 0.0f, fSumZZ = 0.0f;
	

	Vector3D mean;
	mean.makeZero();
	for (i = 0; i < nofNeighbours; i++) {		
		mean += getNeighbourPosition(i);
	}
	mean /= nofNeighbours;

	for (i = 0; i < nofNeighbours; i++) {

		diffVector = mean - getNeighbourPosition(i);
		
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
	
	
//	variation = sqrt (eigenSolver->GetEigenvalue(2)) / (sqrt(eigenSolver->GetEigenvalue(0)) + 
//		sqrt(eigenSolver->GetEigenvalue(1)) + sqrt(eigenSolver->GetEigenvalue(2)));

	variation = eigenSolver->GetEigenvalue(2) / (eigenSolver->GetEigenvalue(0) + 
		eigenSolver->GetEigenvalue(1) + eigenSolver->GetEigenvalue(2));

	return variation;
}





// ***************
// private methods
// ***************

void NeighbourHood::rebuildKDTree() {

	// delete the old KDTree and its point array, if necessary
	if (kdTree != 0) {
		delete kdTree;
	}

	// build search structure

	kdTree = new KdTree(positions, nofPositions, bucketSize);
	neighboursUpdated = false;	

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
