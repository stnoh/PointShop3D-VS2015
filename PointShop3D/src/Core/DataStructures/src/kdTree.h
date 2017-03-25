// Title:   kdTree.h
// Created: Thu Sep 25 14:29:38 2003
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

#ifndef __KDTREE_H_
#define __KDTREE_H_

#include "../../../DataTypes/src/Vector3D.h"
#include "PriorityQueue.h"
#include "../../src/CoreDLL.h"
#include <vector>

#define SQR(x) ((x)*(x))

typedef MaxPriorityQueue<int, float> PQueue;
typedef PQueue::Element Neighbour;

typedef struct kdTreePoint {
	Vector3D	pos;
	int			index;
} KdTreePoint;

/**
 * Node of a kd tree. The node stands for a box in space.
 *
 * @author Richard Keiser
 * @version 2.0
 */
class KdNode {
public:
  bool leaf;
  union {
	struct {
	  KdNode *m_children[2];
	  float m_cutval;
	unsigned char	m_dim;			
	} nodedata;
	struct {
	  KdTreePoint *m_points;
	  unsigned int m_nOfElements;
	} leafdata;
};

/**
	 * creates a node
	 * @param leaf
	 *		  is this node a leaf or not
	 */
  KdNode(bool _leaf)
  {
	leaf = _leaf;
	if (!leaf)
	  {
		nodedata.m_children[0] = NULL;
		nodedata.m_children[1] = NULL;
	  }
  }

	/**
   * deletes the children of the node
	 */
  ~KdNode() {
	if (!leaf)
	  {
		delete nodedata.m_children[0];
		delete nodedata.m_children[1];       
	  }
  }

	/**
	 * look for the nearest neighbours
	 * @param rd 
	 *		  the distance of the query position to the node box
	 * @param queryPriorityQueue
	 *		  a priority queue
	 */
	void queryNode(float rd, PQueue* queryPriorityQueue, Vector3D const& queryPosition, Vector3D& queryOffsets);
};


/**
 * An efficient k-d tree for 3 dimensions
 * It is very similar to the k-d tree 
 * described by Arya and Mount in ``Algorithms
 *	for fast vector quantization,'' Proc.  of DCC '93: Data Compression
 *	Conference, eds. J. A. Storer and M. Cohn, IEEE Press, 1993, 381-390
 *  and their ANN software library
 *
 * @author Richard Keiser
 * @version 2.0
 */
class KdTree {

public:
	/**
	 * Creates a k-d tree from the positions
	 *
	 * @param positions
	 *			point positions
	 * @param nOfPositions
	 *			number of points
	 * @param maxBucketSize
	 *			number of points per bucket
	 */
	CORE_API KdTree(const Vector3D *positions, const unsigned int nOfPositions, const unsigned int maxBucketSize);
	/**
	 * Destructor
	 */
	CORE_API ~KdTree();

	/**
	 * look for the nearest neighbours at <code>position</code>
	 *
	 * @param position
	 *			the position of the point to query with
	 */
	CORE_API void queryPosition(const Vector3D &position);
	/**
	 * look for the nearest neighbours with a maximal squared distance <code>maxSqrDistance</code>. 
	 * If the set number of neighbours is smaller than the number of neighbours at this maximum distance, 
	 * this call is equal to queryPosition but may be faster if the maximum distance is close to the distance 
	 * of the nearest neighbour with the largest distance.
	 *
	 * @param position
	 *			the position of the point to query with
	 * @param maxSqrDistance
	 *			the maximal squared distance of a nearest neighbour
	 *			
	 */
	CORE_API void queryRange(const Vector3D &position, const float maxSqrDistance);
	/**
	 * set the number of nearest neighbours which have to be looked at for a query
	 *
	 * @params newNOfNeighbours
	 *			the number of nearest neighbours
	 */
	CORE_API void setNOfNeighbours (const unsigned int newNOfNeighbours);
	/**
	 * get the index of the i-th nearest neighbour to the query point
	 * i must be smaller than the number of found neighbours
	 *
	 * @param i
	 *			index of the nearest neighbour
	 * @return the index of the i-th nearest neighbour
	 */
	CORE_API inline unsigned int getNeighbourPositionIndex (const unsigned int i) const;
	/** 
	 * get the position of the i-th nearest neighbour
	 * i must be smaller than the number of found neighbours
	 *
	 * @param i
	 *			index of the nearest neighbour
	 * @return the position of the i-th nearest neighbour
	 */
	CORE_API inline Vector3D const& getNeighbourPosition(const unsigned int i) const;
	/**
	 * get the squared distance of the query point and its i-th nearest neighbour
	 * i must be smaller than the number of found neighbours
	 *
	 * @param i
	 *			index of the nearest neighbour
	 * @return the squared distance to the i-th nearest neighbour
	 */
	CORE_API inline float const& getSquaredDistance (const unsigned int i) const;
	/**
	 * get the number of found neighbours
	 * Generally, this is equal to the number of query neighbours
	 * except for range queries, where this number may be smaller than the number of query neigbhbours
	 *
	 * @return the number of found neighbours
	 */
	CORE_API inline unsigned int const& getNOfFoundNeighbours() const;

	/**
	 * get the number of query neighbors
	 * Generally, this is equal to the number of found neighbours
	 * except for range queries, where this number may be larger than the number of found neigbhbours
	 *
	 * @return the number of querye neighbours
	 */
	CORE_API inline unsigned int const& getNOfQueryNeighbours() const;

protected:
	/**
	 * compute distance from point to box
	 *
	 * @param q 
	 *		the point position
	 * @param lo
	 *		low point of box
	 * @param hi
	 *		high point of box
     * @param offset
     *      distance to the box in each dimension
	 * @return the squared distance to the box
	 */
	inline float computeBoxSqrDistance(const Vector3D &q, const Vector3D &lo, const Vector3D &hi, Vector3D &offset);
	/**
	 * computes the enclosing box of the points
	 *
	 * @param lowCorner
	 *			the low corner of the enclosing box is returned
	 * @param hiCorner
	 *			the high corner of the enclosing box is returned
	 */
	void computeEnclosingBoundingBox(Vector3D &lowCorner, Vector3D &hiCorner);
	/** 
	 * creates the tree using the sliding midpoint splitting rule
	 * 
	 * @param node
	 *		  the node to split
	 * @param start
	 *		  first index in the data array
	 * @param end
	 *		  last index in the data array
	 * @param maximum
	 *		  maximum coordinates of the data points
	 * @param minimum
	 *		  minimum coordinates of the data points
	 */
	void createTree(KdNode &node, const int start, const int end, Vector3D &maximum, Vector3D &minimum);

	
private:
	
	KdTreePoint*				m_points;
	const Vector3D*				m_positions;
    std::vector<Neighbour>  	m_neighbours;
	int							m_bucketSize;
	KdNode*						m_root;
	unsigned int				m_nOfFoundNeighbours,
								m_nOfNeighbours,
								m_nOfPositions;
	PQueue*						m_queryPriorityQueue;
	Vector3D					m_boundingBoxLowCorner;
	Vector3D					m_boundingBoxHighCorner;

	// gets the minimum and maximum value of all points at dimension dim
	void getMinMax(KdTreePoint *points, int nOfPoints, int dim, float &min, float &max);
	// splits the points such that on return for all points:
	//		points[0..br1-1] < cutVal
	//		points[br1-1..br2-1] == cutVal
	//		points[br2..nOfPoints-1] > cutVal
	void splitAtMid(KdTreePoint *points, int nOfPoints, int dim, float cutVal, int &br1, int &br2);
	// get the axis aligned bounding box of points
	void getSpread(KdTreePoint* points, int nOfPoints, Vector3D &maximum, Vector3D &minimum);
};

inline unsigned int const& KdTree::getNOfFoundNeighbours() const {
	return m_nOfFoundNeighbours;
}

inline unsigned int const&  KdTree::getNOfQueryNeighbours() const {
	return m_nOfNeighbours;
}

inline unsigned int KdTree::getNeighbourPositionIndex(const unsigned int neighbourIndex) const {
	return m_neighbours[neighbourIndex].index;
}

inline Vector3D const& KdTree::getNeighbourPosition(const unsigned int neighbourIndex) const {
	return m_positions[m_neighbours[neighbourIndex].index];
}

inline float const& KdTree::getSquaredDistance (const unsigned int neighbourIndex) const {
	return m_neighbours[neighbourIndex].weight;
}

#endif

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
