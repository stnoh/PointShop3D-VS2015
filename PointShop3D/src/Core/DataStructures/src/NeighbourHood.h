// Title:   NeighbourHood.h
// Created: Thu Sep 25 14:30:01 2003
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

#ifndef __NeighbourHood_H_
#define __NeighbourHood_H_

//#include <qobject.h>	// simply for definition of unsigned int

#include "../../../DataTypes/src/Vector3D.h"
//#include "../../src/CoreDLL.h"
#include "../../../Utilities/MagicSoft/MgcEigen.h"
#include "../../../Utilities/MagicSoft/MgcEigen.h"

#include "kdTree.h"

/**
 * This data structure allows to query the n-th nearest neighbours to a given
 * point.
 *
 * @author Mark Pauly
 * @version 1.2
 */
class NeighbourHood {

public:
	/**
	 * Creates this <code>NeighbourHood</code> structure. The k-d tree is created not only before
	 * the <code>setPositions</code> method is called.
	 *
	 * @param bucketSize
	 *		  maximum number of elements in a bucket
	 * @see #setPositions
	 *
	 */
	NeighbourHood (const unsigned int bucketSize = 10);
	/**
	 * Creates this <code>NeighbourHood</code> structure, based on the <code>positions</code> array
	 * which contains <code>nofPositions</code> positions. The <code>positions</code> must be
	 * preallocated.
	 *
	 * @param positions
	 *        a pointer to a <code>Vector3D</code> array
	 * @param nofPositions
	 *        the number of elements in the <code>Vector3D</code> array
	 * @param bucketSize
	 *		  maximum number of elements in a bucket
	 */
	NeighbourHood (const Vector3D *positions, const unsigned int nofPositions, const unsigned int bucketSize = 10);

	/**
	 * Destroys this <code>NeighbourHood</code>. The <code>positions</code> are <em>not</em> <code>delete</code>d. 
	 */
	~NeighbourHood();

	/**
	 * sets the maximum query squared distance for range queries
	 * After a query distance is set, all queries are range queries.
	 * A query distance can be 'removed' with <code>clearMaxQueryDistance</code>
	 *
	 * @param maxSqrDist
	 *			the maximum squared distance
	 * @see #clearMaxQuerySqrDistance
	 * @see #getNofFoundNeighbours
	 */
	void setMaxQuerySqrDistance(float maxSqrDist);
	/**
	 * sets the maximum query distance back to 'no distance'.
	 * If no distance is specified, a 'normal query will performed
	 *
	 * @see #setMaxQuerySqrDistance
	 */
	void clearMaxQueryDistance();

	/**
	 * Sets the <code>newPositions</code> for which the neighbourhood has to be calculated.
	 *
	 * @param newPositions
	 *        a pointer to an array of <code>Vector3D</code> points
	 * @param nofPositions
	 *        the number of points in <code>newPositions</code>
	 */
	void setPositions (const Vector3D *newPositions, const unsigned int nofPositions);

	/**
	 * Returns the points for which the neighbourhood is to be calculated.
	 *
	 * @return a pointer to an array of <code>Vector3D</code> points
	 * @see #getNofPositions
	 */
	const Vector3D *getPositions() const;

	/**
	 * Returns the number of points.
	 *
	 * @return the number of points
	 * @see #getPositions
	 */
	unsigned int getNofPositions() const;	

	/**
	 * Sets the number of neighbours which are to be calculated.
	 *
	 * @param newNofNeighbours
	 *        the number of neighbours which are to be calculated
	 */
	void setNofNeighbours (const unsigned int newNofNeighbours);

	/**
	 * Returns the number of neighbours which are to be calculated.
	 *
	 * @return the number of neighbours which are to be calculated
	 */
	unsigned int getNofNeighbours() const;
	/**
	 * returns the number of found neighbours
	 * Generally, this is equal to the number of query neighbours
	 * except for range queries (if a maximum distance is specified), 
	 * where this number may be smaller than the number of query neigbhbours
	 */
	unsigned int getNofFoundNeighbours();

	/**
	 * Sets the <code>newSourcePoint</code> for which the neighbours are to be calculated.
	 *
	 * @param newSourcePoint
	 *        a <code>Vector3D</code> point for which the neighbours are to be calculated
	 */
	void setSourcePoint (const Vector3D newSourcePoint);

	/**
	 * Returns the source point for which the neighbours are to be calculated.
	 *
	 * @return a <code>Vector3D</code> point for which the neighbours are to be calculated
	 */
	Vector3D getSourcePoint() const;

	/**
	 * Returns an index into the <code>positions</code> array of the <code>neighbourIndex</code>-th neighbour
	 * of the point specified by the method <code>setSourcePoint</code>.
	 *
	 * @param neighbourIndex
	 *        an index which specifies the neighbour of which the position index should be returned; must be
	 *        between 0 and the value returned by <code>getNofNeighbours</code> - 1
	 * @return an index into the <code>positions</code> array
	 * @see #setSourcePoint
	 * @see #getNofNeighbours
	 */
	unsigned int getNeighbourPositionIndex (const unsigned int neighbourIndex);

	/**
	 * Returns the position of the <code>neighbourIndex</code>-th neighbour
	 * of the point specified by the method <code>setSourcePoint</code>.
	 *
	 * @param neighbourIndex
	 *        an index which specifies the neighbour of which the position should be returned; must be
	 *        between 0 and the value returned by <code>getNofNeighbours</code> - 1
	 * @return the <code>Vector3D</code> position of the <code>neighbourIndex</code>-th neighbour
	 * @see #setSourcePoint
	 * @see #getNofNeighbours
	 */
	Vector3D getNeighbourPosition (const unsigned int neighbourIndex);
	
	/**
	 * Returns an array which contains the squared distances of all neighbours of the point
	 * specified by the method <code>setSourcePoint</code>.
	 *
	 * @return a pointer to a <code>float</code> array which contains <em>n</em> elements, where
	 *         <em>n</em> is the value returned by <code>getNofNeighbours</code>
	 * @see #setSourcePoint
	 * @see #getNofNeighbours
	 */
	const float *getSquaredDistances();

	/**
	 * Returns the squared distance between the <code>neighbourIndex</code>-th neighbour
	 * and the point specified by the method <code>setSourcePoint</code>.
	 *
	 * @param neighbourIndex
	 *        an index which specifies the neighbour of which the squared position should be returned; must be
	 *        between 0 and the value returned by <code>getNofNeighbours</code> - 1
	 * @return the squared distance between the <code>neighbourIndex</code>-th neighbour
	 *         and the point specified by the method <code>setSourcePoint</code>
	 * @see #setSourcePoint
	 * @see #getNofNeighbours
	 */
	float getSquaredDistance (const unsigned int neighbourIndex);
	
	/**
	 * computes an approximation of the normal vector at the source point using PCA.
	 *
	 * First <code>setSourcePoint</code> and <code>setNofNeighbours</code> need to be set
	 *
	 * @return an approximation of the normal vector
	 *
	 * @see #setSourcePoint
	 * @see #setNofNeighbours
	 */
	Vector3D computeNormal ();

	/**
	 * computes the variation at the source point using PCA.
	 *
	 * First <code>setSourcePoint</code> and <code>setNofNeighbours</code> need to be set
	 *
	 * @return the variation
	 *
	 * @see #setSourcePoint
	 * @see #setNofNeighbours
	 */
	float computeVariation ();

private:
	
	const Vector3D    *positions;
	Vector3D          sourcePoint;   // the point for which the neighbours are computed
	unsigned int	  nofPositions,
		              nofNeighbours,
					  bucketSize;
	float			  maxSqrDistance;					  
	
	bool              neighboursUpdated;
	Mgc::Eigen		  *eigenSolver;
	KdTree*			  kdTree;

	// rebuilds the KDTree - call this as soon as a new 'positions' array has been set
	void rebuildKDTree();
	
};


#endif  // __NEIGHBOURHOOD_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
