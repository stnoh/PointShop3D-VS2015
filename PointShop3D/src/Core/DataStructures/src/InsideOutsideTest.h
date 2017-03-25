// Title:   InsideOutsideTest.h
// Created: Tue Oct 21 16:42:05 2003
// Author : Richard Keiser
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

#ifndef __INSIDE_OUTSIDE_TEST_H_
#define __INSIDE_OUTSIDE_TEST_H_

#include "../../src/CoreDLL.h"
#include "SurfelCollection.h"
#include "KdTree.h"
#include "../../../DataTypes/src/Vector3D.h"

/**
 * Tests if a point in space is inside or outside a model defined by a surfel collection
 *
 * @author Richard Keiser
 * @version 2.0
 */
class InsideOutsideTest {

public:

	CORE_API InsideOutsideTest() {
		kdTree = NULL;
	}

	CORE_API ~InsideOutsideTest() {
		if (kdTree) {
			delete kdTree;
		}
	}

		
	/**
	 * builds a kd-Tree search data structure from the surfel collection
	 *
	 * <b>Note</b>: The <code>surfelCollection</code> is expected not to change
	 *				during the use of this class
	 *
	 * @param surfelCollection
	 *		a collection of surfels which are used to build the kd-Tree
	 * @param nofNeighbors
	 *		number of query neighbors. 1 (= closest neighbor) as default
	 */
	CORE_API void buildSearchDataStructure(SurfelCollection *surfelCollection, const int nofNeighbors = 1);

	/**
	 * resets the number of query neighbors
	 *
	 * @param nofNeighbors
	 *		number of query neighbors
	 */
	inline void setNOfNeighbors(const int nofNeighbors);

	/**
	 * returns true if a point at <code>position</code> is inside, false otherwise
	 *
	 * @param position
	 *		  the point to test for inside/outside
	 * @param sqrDistance
	 *		  the squared distance to the next point is returned
	 * @param closestSurfel
	 *		  pointer to the nearest surfel is returned
	 *
	 * @return true if inside, false otherwise
	 */
	inline bool isInside(const Vector3D &position, float &sqrDistance, SurfelInterface **closestSurfel);

		

private:

	SurfelInterface		**surfels;
	Vector3D			*positions;
	KdTree				*kdTree;
	int					nofNeighbors;
	

};


inline bool InsideOutsideTest::isInside(const Vector3D &position, float &sqrDistance, SurfelInterface **closestSurfel) {

	Vector3D	toPoint;
	
	kdTree->queryPosition(position);
	sqrDistance = kdTree->getSquaredDistance(0);
	*closestSurfel = surfels[kdTree->getNeighbourPositionIndex(0)];

	bool inside = true;
	int i=0;
	while(i < nofNeighbors  && inside) {
		toPoint = kdTree->getNeighbourPosition(i) - position;
		inside &= (Vector3D::dotProduct(toPoint, surfels[kdTree->getNeighbourPositionIndex(i)]->getNormal()) > 0);
		i++;
	}
	
	return inside;
}

inline void InsideOutsideTest::setNOfNeighbors(const int nofNeighbors) {

	this->nofNeighbors = nofNeighbors;
	kdTree->setNOfNeighbours(nofNeighbors);
}

#endif
// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
