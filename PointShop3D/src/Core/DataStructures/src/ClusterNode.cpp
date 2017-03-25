// Title:   ClusterNode.cpp
// Created: Thu Sep 25 14:25:40 2003
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

#include "ClusterNode.h"

//////////////////////////////////////////////////////////////////////
//
// Constructor for ClusterNode
// Parent = NULL is the root of the tree
//

void ClusterNode::init(ClusterNode* _parent, SurfelInterface **surfelArray, unsigned int nOfSurfels) {
	m_parent = _parent;
	m_children = NULL;
	m_surfelArray = surfelArray;
	m_nOfSurfels = nOfSurfels;
}


//////////////////////////////////////////////////////////////////////
//
// Destructor for ClusterNode
//

ClusterNode::~ClusterNode() {
}




//////////////////////////////////////////////////////////////////////
//
// for covariance subsampling
//

void ClusterNode::computeVariation () {

	unsigned int i;		
	
	computeCenter();	

	if (m_nOfSurfels < 4) {
		m_variation = 0.0f;
		m_splitVector = Vector3D (1, 0, 0);
	}
	else {
			
		Vector3D diffVector;
		Vector3D maxDiffVector;
		float maxDiffSquaredLength = 0.0f;
		float diffSquaredLength;

		// compute sums of products
		// compute sums of products
		Real fSumXX = 0.0f, fSumXY = 0.0f, fSumXZ = 0.0f;
		Real fSumYY = 0.0f, fSumYZ = 0.0f, fSumZZ = 0.0f;
		
		
		for (i = 0; i < m_nOfSurfels; i++) {
			
			diffVector = m_center - m_surfelArray[i]->getPosition();
			diffSquaredLength = diffVector.getSquaredLength();
			if (diffSquaredLength > maxDiffSquaredLength) {
				maxDiffVector = diffVector;
				maxDiffSquaredLength = diffSquaredLength;
			}
			
			fSumXX += diffVector[0] * diffVector[0];
			fSumXY += diffVector[0] * diffVector[1];
			fSumXZ += diffVector[0] * diffVector[2];
			fSumYY += diffVector[1] * diffVector[1];
			fSumYZ += diffVector[1] * diffVector[2];
			fSumZZ += diffVector[2] * diffVector[2];
		}
		
		// setup the eigenSolver
		Eigen eigenSolver(3);
		eigenSolver.Matrix(0,0) = fSumXX;
		eigenSolver.Matrix(0,1) = fSumXY;
		eigenSolver.Matrix(0,2) = fSumXZ;
		eigenSolver.Matrix(1,0) = eigenSolver.Matrix(0,1);
		eigenSolver.Matrix(1,1) = fSumYY;
		eigenSolver.Matrix(1,2) = fSumYZ;
		eigenSolver.Matrix(2,0) = eigenSolver.Matrix(0,2);
		eigenSolver.Matrix(2,1) = eigenSolver.Matrix(1,2);
		eigenSolver.Matrix(2,2) = fSumZZ;
		
		// compute eigenstuff, smallest eigenvalue is in last position
		eigenSolver.DecrSortEigenStuff3();
		
		m_variation =  (eigenSolver.GetEigenvalue (2) / 
			(eigenSolver.GetEigenvalue (0) + eigenSolver.GetEigenvalue (1) + eigenSolver.GetEigenvalue (2)));
		
		if ((m_variation < 0) || (m_variation > 0.34f)) {
			m_variation = 0;
		}


		m_splitVector = maxDiffVector;
	}
}


//////////////////////////////////////////////////////////////////////
//
// for node splitting
//

void ClusterNode::computeSplitVector () {

	unsigned int i;		
	
	computeCenter();	

	if (m_nOfSurfels < 4) {
		m_splitVector = Vector3D (1, 0, 0);
	}
	else {
			
		Vector3D diffVector;
		Vector3D maxDiffVector;
		float maxDiffSquaredLength = 0.0f;
		float diffSquaredLength;

		// compute sums of products
		// compute sums of products
		Real fSumXX = 0.0f, fSumXY = 0.0f, fSumXZ = 0.0f;
		Real fSumYY = 0.0f, fSumYZ = 0.0f, fSumZZ = 0.0f;
		
		
		for (i = 0; i < m_nOfSurfels; i++) {
			
			diffVector = m_center - m_surfelArray[i]->getPosition();
			diffSquaredLength = diffVector.getSquaredLength();
			if (diffSquaredLength > maxDiffSquaredLength) {
				maxDiffVector = diffVector;
				maxDiffSquaredLength = diffSquaredLength;
			}
			
			fSumXX += diffVector[0] * diffVector[0];
			fSumXY += diffVector[0] * diffVector[1];
			fSumXZ += diffVector[0] * diffVector[2];
			fSumYY += diffVector[1] * diffVector[1];
			fSumYZ += diffVector[1] * diffVector[2];
			fSumZZ += diffVector[2] * diffVector[2];
		}
		
		m_splitVector = maxDiffVector;
	
	}
}


//////////////////////////////////////////////////////////////////////
//
// for computing the minimal ball
//

void ClusterNode::computeMiniball() {
	Miniball<3> mb;
	m_radius = 0;
	for (unsigned int i = 0; i < m_nOfSurfels; i++) {
		mb.check_in(Miniball<3>::Point(m_surfelArray[i]->getPosition().source()));
		m_radius = __max(m_surfelArray[i]->getRadius(), m_radius);
	}
	mb.build();
	m_center = Vector3D((float *) mb.center().begin());
	m_radius += sqrt(mb.squared_radius());
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
