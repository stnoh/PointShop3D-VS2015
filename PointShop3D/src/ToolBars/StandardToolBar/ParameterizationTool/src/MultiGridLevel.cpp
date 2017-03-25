// Title:   MultiGridLevel.cpp
// Created: Thu Sep 25 14:58:28 2003
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

#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Utilities/src/Common.h"

#include "MultiGridLevel.h"

#include <assert.h>

// **************
// public methods
// **************

MultiGridLevel::MultiGridLevel (NeighbourHood *neighbourHood, const Vector3D *normals, float *uvCoordinates) {

	this->neighbourHood = neighbourHood;
	this->normals       = normals;
	this->uvCoordinates = uvCoordinates;

	positions    = neighbourHood->getPositions();
	nofPositions = neighbourHood->getNofPositions();
	leastSquares = new SparseLeastSquares (nofPositions * 2);

	positionFlags = new PositionFlags[nofPositions];
	this->resetFlags();

	// add directional derivatives constraints. we also detect and mark points on the boundary of 
	// the selection. using these boundary flags, we can avoid unbalanced regularization constraints
	// in the next step.
	this->addDirectionalDerivativesConstraints();
}

MultiGridLevel::~MultiGridLevel() {
	delete[] positionFlags;
	delete leastSquares;
}

void MultiGridLevel::setNeighbourHood (NeighbourHood *newNeighbourHood) {
	neighbourHood = newNeighbourHood;
}

NeighbourHood *MultiGridLevel::getNeighbourHood() const {
	return neighbourHood;
}

void MultiGridLevel::setNormals (const Vector3D *newNormals) {
	normals = newNormals;
}

const Vector3D *MultiGridLevel::getNormals() const {
	return normals;
}

void MultiGridLevel::setUVCoordinates (float *newUVCoordinates) {
	uvCoordinates = newUVCoordinates;
}

float *MultiGridLevel::getUVCoordinates() const {
	return uvCoordinates;
}

void MultiGridLevel::setPrecision (const float newPrecision) {
	precision = newPrecision;
}

float MultiGridLevel::getPrecision() const {
	return precision;
}

void MultiGridLevel::addFittingConstraints (const float *fittingConstraintsU, const float *fittingConstraintsV,
										    const uint *fittingConstraintIndices, const uint nofConstraints, float weight) {

	std::vector<float> a;	 // the coefficients array of the constraint
	std::vector<int>   I;	 // the index array of the constraint
	float         b[2];	 // u and v components of the right hand side of the constraint
    uint          i;

	// fitting constraints have exactly one non-zero coefficient!
	I.resize(1);
	a.resize(1);

	// adjust size of internal tables, all tables have the same size
	this->fittingConstraintsU.resize (nofConstraints);
	this->fittingConstraintsV.resize (nofConstraints);
	this->fittingConstraintsIndex.resize (nofConstraints);

 	for (i = 0; i < nofConstraints; i++) {

		// copy fitting constraints to private table
		this->fittingConstraintsU.at(i)     = fittingConstraintsU[i];
		this->fittingConstraintsV.at(i)     = fittingConstraintsV[i];
		this->fittingConstraintsIndex.at(i) = fittingConstraintIndices[i];

		I.at(0) = fittingConstraintIndices[i];

		a.at(0) = 1.0f;
		
		// get the corresponding texture coordinates here...
		b[0] = fittingConstraintsU[i];
		b[1] = fittingConstraintsV[i];

		this->normalizeAndAddConstraint (I, a, b, weight);

	}

}

void MultiGridLevel::removeFittingConstraints() {

	std::vector<float> a;	 // the coefficients array of the constraint
	std::vector<int>   I;     // the index array of the constraint
	float         b[2];	 // u and v components of the right hand side of the constraint
	uint          size,
		          i;

	// fitting constraints have exactly one non-zero coefficient!
	I.resize(1);
	a.resize(1);

	size = fittingConstraintsU.size();
	// remove the constraints by multiplying the weights of the constraints by -1!

	for (i = 0; i < size; i++) {

		I.at(0) = fittingConstraintsIndex.at(i);

		a.at(0) = 1.0f;
		
		// get the corresponding texture coordinates here...
		b[0] = fittingConstraintsU.at(i);
		b[1] = fittingConstraintsV.at(i);

		this->normalizeAndAddConstraint(I, a, b, 1.0f, false);

	}

}

void MultiGridLevel::generateUVCoordinates () {
	
	this->precision = precision;
	this->solveLeastSquares();
}

void MultiGridLevel::solveLeastSquares() {
	
	// solve the least squares optimization problem
	leastSquares->solve (uvCoordinates, neighbourHood->getNofPositions(), precision);
}

// ***************
// private methods
// ***************

void MultiGridLevel::addRegularizationConstraints() {

	unsigned int  j;
	unsigned int  nNeighbours;
	unsigned int  nIndices;

	std::vector<float> a;	// the coefficients array of the constraint
	std::vector<int>   I;		// the index array of the constraint
	float         b[2];			// u and v components of the right hand side of the constraint

	Vector3D      diff;
	float         w,
		          sumW;

	uint          index;

	uint          currentNeighbourIndex;

	nNeighbours = 9;

	neighbourHood->setNofNeighbours (nNeighbours);

	// there is one regularization constraint per non-boundary vertex
	//for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {
	for (index = 0; index < nofPositions; index++) {

		if ((positionFlags[index] & GEOMETRICBOUNDARY) != 0) {
			// do not add a constraint for this point, just reset the boundary flag
			//surfel->setFlag(Surfel::GEOMETRICBOUNDARY, false);
			positionFlags[index] &= ~GEOMETRICBOUNDARY;
		}
		else {
			// point is NOT on boundary, hence regularization constraint will be balanced

			// get nearest neighbors of current surfel
			// NOTE: the current surfel is returned as the neighbor 0! nNeighbours includes the 
			// current surfel, too.
			//neighbourHood->computeNearestPoints (surfel->getPosition(), nNeighbours);
			neighbourHood->setSourcePoint (positions[index]);

			// setup the indices and the coefficients array for the current constraint. 
			// they contain all neighbors plus the current vertex itself, given that these
			// are FREE texture vertices.
			I.resize (nNeighbours);
			a.resize (nNeighbours);
			b[0]     = 0.0f;
			b[1]     = 0.0f;
			sumW     = 0.0f;
			nIndices = 0;

			// NOTE: neighbour 0 is the current surfel itself, hence start at j=1!
			for (j = 1; j < nNeighbours; j++) {

				// compute distance between current surfel and current neighbor
				//currentNeighbor = neighborhoodSearchStructure->getNeighbor(j);
				currentNeighbourIndex = neighbourHood->getNeighbourPositionIndex(j);

				//diff = surfel->getPosition() - currentNeighbor->getPosition();
				diff = positions[index] - positions[currentNeighbourIndex];

				w = 1.0f / diff.getLength();
				sumW += w;

				if ((positionFlags[currentNeighbourIndex] & UVBOUNDARY) == 0) {

					// free texture surfel
					I.at (nIndices) = index;
					a.at(nIndices)  = w;
					nIndices++;

				} else {

					// texture boundary surfel, hence move contribution from left to right
					// hand side. note that there is a u and a v texture coordinate component
					//currentNeighbor->getTextureCoords(uv);
					b[0] -= w * uvCoordinates[currentNeighbourIndex];
					b[1] -= w * uvCoordinates[currentNeighbourIndex + nofPositions];

				}

			}

			// add the current vertex to the constraint
			//if (surfel->getFlag(Surfel::UVBOUNDARY) == false) {
			if ((positionFlags[index] & UVBOUNDARY) == 0) {

				//  current vertex is a free texture vertex, i.e. add contribution to the index 
				// and the coefficient array
				//I.at(nIndices) = surfel->getIndex();
				I.at (nIndices) = index;
				a.at(nIndices)  = -sumW;
				nIndices++;
				
			} else {

				//  current vertex is a texture boundary condition, hence move contribution
				// from left to right hand side.
				b[0] += w * uvCoordinates[currentNeighbourIndex];
				b[1] += w * uvCoordinates[currentNeighbourIndex + nofPositions];
			
			}

			I.resize (nIndices);	
			a.resize (nIndices);

			// normalize constraint and add to least squares system
			this->normalizeAndAddConstraint(I, a, b, 1.0);

		}

	}

}

void MultiGridLevel::addDirectionalDerivativesConstraints() {

	int           nNeighbours;

	std::vector<float> a;	// the coefficients array of the constraint
	std::vector<int>   I;	// the index array of the constraint
	float         b[2];			// u and v components of the right hand side of the constraint

	Vector3D      P, Pi, Pj, Pk, Pl;
	Vector3D      P_Pj, P_Pk;					// un-normalized vectors
	Vector3D      nPi_P, nPl_P, nP_Pj, nP_Pk;	// normalized vectors
	Vector3D      X, Y;
	Vector3D      n, n_jk;
	float         len_Pi_P;

	float         Aj, Ak, Al;
	float         maxAngleFromPlane     = (float)M_PI / 3.0f;
	float         minPjPkAngleThreshold = (float)-0.99f;
	int           i, j, k, l;

	float         x[3], y[3];
	float         DX[3];
	float         d;

	int           nIndices;
	uint          index,
		          indexI,
		          indexJ,
				  indexK = 0,
				  indexL;

	nNeighbours = 9;

	// set the size of the indices and the coefficients array for the constraints. each constraint
	// involves the center point plus three neighbors
	I.resize (nNeighbours);
	a.resize (nNeighbours);

	neighbourHood->setNofNeighbours (nNeighbours);

	for (index = 0; index < nofPositions; index++) {

		// get nearest neighbors of current surfel
		// NOTE: the current surfel is returned as the neighbor 0! nNeighbours includes the 
		// current surfel, too.
		P = positions[index];
		
		//neighborhoodSearchStructure->computeNearestPoints(P, nNeighbours);
		neighbourHood->setSourcePoint(P);

		b[0] = 0.f;
		b[1] = 0.f;

		// NOTE: neighbor 0 is the current surfel itself, hence start at j=1!
		for (i = 1; i < nNeighbours; i++) {

			// compute one directional derivative constraint for each neighbor
			indexI = neighbourHood->getNeighbourPositionIndex(i);
			Pi = positions[indexI];
			nPi_P = P - Pi;
			len_Pi_P = nPi_P.normalize();

			// the normal of the plane through P, which defines the direction of the directional
			// derivative
			n = Vector3D::crossProduct (nPi_P, normals[index]);
			n.normalize();

			// for each neighbor Pl, compute the angle between Pl_P and the plane given by P and n.
			// select the two neighbors with the smallest angle, under the condition that the angle
			// between Pi_P and Pl_P is bigger than 90 degrees
			j = k = -1;
			Aj = Ak = (float)M_PI;
			Pj = Vector3D(0,0,0);	// init to avoid compiler warnings

			indexJ = UINT_MAX;
			nP_Pj = Vector3D(0,0,0);
			
			for (l = 1; l < nNeighbours; l++) {
				
				if (l != i) {

					// compute the angle between Pl_P and the plane
					indexL = neighbourHood->getNeighbourPositionIndex(l);
					Pl = positions[indexL];
					nPl_P = P - Pl;
					nPl_P.normalize();
					Al = fabs (asin (Vector3D::dotProduct (nPl_P, n)));

					// check that Pl lies on the other side of the plane given by P and the normal Pi_P 
					// than Pi itself
					// NOTE: just check that the angle between Pi_P and Pl_P is bigger than 90 degrees
					if (acos(Vector3D::dotProduct(nPl_P, nPi_P)) > M_PI/2) {
						
						if (Al < Aj) {
							// guarantee that Aj < Ak by setting *k = *j
							k = j;
							Ak = Aj;
							Pk = Pj;
							//Sk = Sj;
							indexK = indexJ;
							nP_Pk = nP_Pj;

							// set new *j
							j = l;
							Aj = Al;
							Pj = Pl;
							//Sj = Sl;
							indexJ = indexL;
							nP_Pj = Pj - P;
							nP_Pj.normalize();
						} else if (Al < Ak && Vector3D::dotProduct(nPl_P, nP_Pj) > minPjPkAngleThreshold) {
							// reject Pk if the angle between Pk and Pj is too small (to guarantee that P,Pk,Pj define a plane properly)!
							// set new *k, we still guarantee Aj < Ak
							k = l;
							Ak = Al;
							Pk = Pl;
							//Sk = Sl;
							indexK = indexL;
							nP_Pk = Pk - P;
							nP_Pk.normalize();
						}
					}
				}
			}

			// vectors from P to Pj and Pk respectively
			P_Pj = Pj - P;
			P_Pk = Pk - P;

			// check if we found two neighbors that are sufficiently close to the plane
			// also check that P, Pj, and Pk are not colinear!
			if(j!=-1 && k!=-1 && Aj<maxAngleFromPlane && Ak<maxAngleFromPlane && 
			   fabs(Vector3D::dotProduct(P_Pj, P_Pk)/(P_Pj.getLength()*P_Pk.getLength())) < 0.99) {

				// find the intersection line of the plane given by P, Pj, and Pk, and the
				// plane given by P and the surface normal at P
				n_jk = Vector3D::crossProduct(P_Pj, P_Pk);
				n_jk.normalize();

				// compute the local X,Y coordinate system
				// NOTE: X must be oriented such that the angle between X and Pi_P is smaller
				// than 90 degrees (X must point away from P)
				X = Vector3D::crossProduct(n, n_jk);
				X.normalize();
				if(Vector3D::dotProduct(X, nPi_P) < 0.f) {
					// flip direction of X
					X = -X;
				}
				// the direction of Y does not matter
				Y = Vector3D::crossProduct(n_jk, X);

				// compute the coordinates of Pj and Pk in local coordinates
				x[0] = 0;
				x[1] = Vector3D::dotProduct(P_Pj, X);
				x[2] = Vector3D::dotProduct(P_Pk, X);
				y[0] = 0;
				y[1] = Vector3D::dotProduct(P_Pj, Y);
				y[2] = Vector3D::dotProduct(P_Pk, Y);
				
				// compute the coefficients for the directional derivative along X
				d = (x[1]-x[0])*(y[2]-y[0]) - (x[2]-x[0])*(y[1]-y[0]);
				DX[0] = (y[1]-y[2])/d;
				DX[1] = (y[2]-y[0])/d;
				DX[2] = (y[0]-y[1])/d;
				
				// add constraint
				nIndices = 0;

				if ((positionFlags[indexJ] & UVBOUNDARY) == 0) {

					// free texture surfel
					I.at (nIndices) = indexJ;
					a.at(nIndices) = DX[1];
					nIndices++;

				} else {

					// texture boundary surfel, hence move contribution from left to right
					// hand side. note that there is a u and a v texture coordinate component
					b[0] -= DX[1] * uvCoordinates[indexJ];
					b[1] -= DX[1] * uvCoordinates[indexJ + nofPositions];

				}

				// SK
				if ((positionFlags[indexK] & UVBOUNDARY) == 0) {
					// free texture surfel
					//I.at(nIndices) = Sk->getIndex();
					I.at (nIndices) = indexK;
					a.at(nIndices) = DX[2];
					nIndices++;

				} else {

					// texture boundary surfel
					b[0] -= DX[1] * uvCoordinates[indexK];
					b[1] -= DX[1] * uvCoordinates[indexK + nofPositions];

				}

				// S
				if ((positionFlags[index] & UVBOUNDARY) == 0) {
					// free texture surfel
					I.at (nIndices) = index;
					a.at(nIndices) = DX[0] - 1 / len_Pi_P;
					nIndices++;

				} else {

					// texture boundary surfel
					b[0] -= DX[1] * uvCoordinates[index];
					b[1] -= DX[1] * uvCoordinates[index + nofPositions];

				}	

				// Si
				if ((positionFlags[indexI] & UVBOUNDARY) == 0) {
					// free texture surfel
					I.at (nIndices) = indexI;
					a.at(nIndices) = 1 / len_Pi_P;
					nIndices++;

				} else {
					// texture boundary surfel
					b[0] -= DX[1] * uvCoordinates[indexI];
					b[1] -= DX[1] * uvCoordinates[indexI + nofPositions];
				}

				// normalize constraint and add to least squares system
				I.resize(nIndices);	
				a.resize(nIndices);
				normalizeAndAddConstraint(I, a, b, 1.0);

			} else {
				// we did not find any two neighbors complying with our criteria,
				// hence the current center point is a point on the boundary of
				// the selection. this flag is used to reject unbalanced regularization
				// constraints later.
				positionFlags[index] |= GEOMETRICBOUNDARY;
			}
		}
	}
}


void MultiGridLevel::normalizeAndAddConstraint(std::vector<int>& I, std::vector<float>& a, float b[2], const float w, bool addFlag) {

	int j;
	int nIndices = I.size();
	float sumC[2];

	// NOTE: the texture coordinates have two components, hence each constraint results
	// in two 'constraint components' one for each texture component (u and v)
	// u component
	
	// compute normalization factors for normalizing the constraint
	// NOTE: the normalization factor for the u and v constraint components may differ,
	// since the right hand side may differ.
	sumC[0] = sumC[1] = 0.f;
	for(j = 0; j<nIndices; j++) {
		sumC[0] += (float)fabs(a.at(j));
		sumC[1] += (float)fabs(a.at(j));
	}
	sumC[0] += (float)fabs(b[0]);
	sumC[1] += (float)fabs(b[1]);

	// normalize u component
	// NOTE: w is an additional factor that is used to weight the constraint
	for(j = 0; j<nIndices; j++) {
		a.at(j) = a.at(j) / sumC[0] * w;
	}
	b[0] = b[0] / sumC[0] * w;

	// add u component
	leastSquares->addConstraint(b[0], I, a, addFlag);

	// v component
	// normalize v component (first, undo u normalization, then apply v normalization)
	for(j = 0; j<nIndices; j++) {
		a.at(j) *= sumC[0] / sumC[1];
	}
	b[1] = b[1] / sumC[1] * w;

	// NOTE: the index array is modified, such that the u and v constraint components are
	// independent. however, the constraint coefficients are the same.
	for (j = 0; j < nIndices; j++) {
		I.at(j) += nofPositions;
	}

	// add v component
	leastSquares->addConstraint(b[1], I, a, addFlag);

}

void MultiGridLevel::resetFlags() {

	uint i;

	for (i = 0; i < nofPositions; i++) {
		positionFlags[i] = NO_FLAGS;
	}

}






// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
