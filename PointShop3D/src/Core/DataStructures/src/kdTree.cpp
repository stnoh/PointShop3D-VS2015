// Title:   kdTree.cpp
// Created: Thu Sep 25 14:19:18 2003
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

#include "kdTree.h"
#include <stdlib.h>

#define SWAP_POINTS(a,b) \
			KdTreePoint tmp = points[a];\
		    points[a] = points[b];\
		    points[b] = tmp;


// ******************
// global definitions
// ******************

KdTree::KdTree(const Vector3D *positions, const unsigned int nOfPositions, const unsigned int maxBucketSize) {
	m_bucketSize			= maxBucketSize;
	m_positions				= positions;
	m_nOfPositions			= nOfPositions;
	m_points				= new KdTreePoint[nOfPositions];
	m_nOfFoundNeighbours	= 0;
	m_nOfNeighbours			= 0;
	m_queryPriorityQueue	= new PQueue();
	for (unsigned int i=0; i<nOfPositions; i++) {
		m_points[i].pos = positions[i];
		m_points[i].index = i;
	}
	computeEnclosingBoundingBox(m_boundingBoxLowCorner, m_boundingBoxHighCorner);
	m_root = new KdNode(false);
	Vector3D maximum, minimum;
	getSpread(m_points, nOfPositions, maximum, minimum);
	createTree(*m_root, 0, nOfPositions, maximum, minimum);
	setNOfNeighbours(1);
}


KdTree::~KdTree() {
	delete m_root;
	delete[] m_points;
	delete m_queryPriorityQueue;
}

void KdTree::computeEnclosingBoundingBox(Vector3D &lowCorner, Vector3D &hiCorner) {
	hiCorner = lowCorner = m_points[0].pos;	
	for (unsigned int i=1; i<m_nOfPositions; i++) {
		Vector3D const& tmp = m_positions[i];
		if (hiCorner[0] < tmp[0]) {
			hiCorner[0] = tmp[0];
		}
		else if (lowCorner[0] > tmp[0]) {
			lowCorner[0] = tmp[0];
		}
		if (hiCorner[1] < tmp[1]) {
			hiCorner[1] = tmp[1];
		}
		else if (lowCorner[1] > tmp[1]) {
			lowCorner[1] = tmp[1];
		}
		if (hiCorner[2] < tmp[2]) {
			hiCorner[2] = tmp[2];
		}
		else if (lowCorner[2] > tmp[2]) {
			lowCorner[2] = tmp[2];
		}
	}		 
}

inline float KdTree::computeBoxSqrDistance(const Vector3D &q, const Vector3D &lo, const Vector3D &hi, Vector3D& offset) {
	float dist = 0.0;
	float t;

	if (q[0] < lo[0]) {
		t = lo[0] - q[0];
		offset[0] = t;
		dist = t*t;
	}
	else if (q[0] > hi[0]) {
		t = q[0] - hi[0];
		offset[0] = t;
		dist = t*t;
	}
	if (q[1] < lo[1]) {
		t = lo[1] - q[1];
		offset[1] = t;
		dist += t*t;
	}
	else if (q[1] > hi[1]) {
		t = q[1] - hi[1];
		offset[1] = t;
		dist += t*t;
	}
	if (q[2] < lo[2]) {
		t = lo[2] - q[2];
		offset[2] = t;
		dist += t*t;
	}
	else if (q[2] > hi[2]) {
		t = q[2] - hi[2];
		offset[2] = t;
		dist += t*t;
	}

	return dist;
}

void KdTree::queryPosition(const Vector3D &position) {
	if (m_neighbours.size() == 0) {
		return;
	}
	Vector3D queryOffsets(0,0,0);
	m_queryPriorityQueue->init();
	m_queryPriorityQueue->insert(-1, FLT_MAX);
	float sqrDist = computeBoxSqrDistance(position, m_boundingBoxLowCorner, m_boundingBoxHighCorner, queryOffsets);
	m_root->queryNode(sqrDist, m_queryPriorityQueue, position, queryOffsets);
	
	if (m_queryPriorityQueue->getMax().index == -1) {
		m_queryPriorityQueue->removeMax();
	}

	m_nOfFoundNeighbours = m_queryPriorityQueue->getNofElements();

	for(int i=m_nOfFoundNeighbours-1; i>=0; i--) {
		m_neighbours[i] = m_queryPriorityQueue->getMax();
		m_queryPriorityQueue->removeMax();
	}
}

void KdTree::queryRange(const Vector3D &position, const float maxSqrDistance) {
	if (m_neighbours.size() == 0) {
		return;
	}
	Vector3D queryOffsets(0,0,0);

    m_nOfFoundNeighbours = 0;
    float sqrDist = computeBoxSqrDistance(position, m_boundingBoxLowCorner, m_boundingBoxHighCorner, queryOffsets);	

    if (sqrDist > maxSqrDistance) {
        return;
    }

	m_queryPriorityQueue->init();
	m_queryPriorityQueue->insert(-1, maxSqrDistance);

	m_root->queryNode(sqrDist, m_queryPriorityQueue, position, queryOffsets);

	if (m_queryPriorityQueue->getMax().index == -1) {
		m_queryPriorityQueue->removeMax();
	}

	m_nOfFoundNeighbours = m_queryPriorityQueue->getNofElements();

	for(int i=m_nOfFoundNeighbours-1; i>=0; i--) {
		m_neighbours[i] = m_queryPriorityQueue->getMax();
		m_queryPriorityQueue->removeMax();
	}
}

void KdTree::setNOfNeighbours (const unsigned int newNOfNeighbours) {
	if (newNOfNeighbours != m_nOfNeighbours) {
		m_nOfNeighbours = newNOfNeighbours;
		m_queryPriorityQueue->setSize(m_nOfNeighbours);
		m_nOfNeighbours = newNOfNeighbours;
		m_neighbours.resize(m_nOfNeighbours);
		m_nOfFoundNeighbours = 0;
	}
}


void KdTree::createTree(KdNode &node, int start, int end, Vector3D &maximum, Vector3D &minimum) {
	int	mid;

	int n = end-start;
	Vector3D diff = maximum - minimum;
	unsigned char dim;
	// get longest axis
	if (diff[0] > diff[1]) {
		if (diff[0] > diff[2]) {
			dim = 0;	//x-axe is longest axe
		}
		else {
			dim = 2;	// z-axe is longest axe
		}
	}
	else {
		if (diff[1] > diff[2]) {
			dim = 1;	// y-axe is longest axe
		}
		else {
			dim = 2;	// z-axe is longest axe
		}
	}
	
  node.nodedata.m_dim = (unsigned char)dim;
  float bestCut = (maximum[dim]+minimum[dim])/2.0f;
	float min, max;
	getMinMax(m_points+start, n, dim, min, max);	// find min/max coordinates
	if (bestCut < min)		// slide to min or max as needed
	node.nodedata.m_cutval = min;
    else if (bestCut > max)
	node.nodedata.m_cutval = max;
    else
	node.nodedata.m_cutval = bestCut;

    int br1, br2;
	// permute points accordingly
	splitAtMid(m_points+start, n, dim, node.nodedata.m_cutval, br1, br2);	

	if (bestCut < min) mid = start+1;
    else if (bestCut > max) mid = end-1;
    else if (br1 > n/2.0) mid = start+br1;
    else if (br2 < n/2.0) mid = start+br2;
    else mid = start + (n>>1);

	if (mid-start <= m_bucketSize) {
		// new leaf
		KdNode* leaf = new KdNode(true);
		node.nodedata.m_children[0] = leaf;
		leaf->leafdata.m_points = (m_points+start);
		leaf->leafdata.m_nOfElements = mid-start;
	}
	else {
		// new node
		KdNode* child = new KdNode(false);
		node.nodedata.m_children[0] = child;
		float oldMax = maximum[dim];
		maximum[dim] = node.nodedata.m_cutval;
		createTree(*child, start, mid, maximum, minimum);
		maximum[dim] = oldMax;
	}
	
	if (end-mid <= m_bucketSize) {
		// new leaf
		KdNode* _leaf = new KdNode(true);
		node.nodedata.m_children[1] = _leaf;
		_leaf->leafdata.m_points = (m_points+mid);
		_leaf->leafdata.m_nOfElements = end-mid;
	}
	else {
		// new node
      minimum[dim] = node.nodedata.m_cutval;
	  KdNode* child = new KdNode(false);
	  node.nodedata.m_children[1] = child;
	  createTree(*child, mid, end, maximum, minimum);
	}
}

void KdTree::getSpread(KdTreePoint* points, int nOfPoints, Vector3D &maximum, Vector3D &minimum) {
	const Vector3D& p = points->pos;
	maximum = Vector3D(p[0], p[1], p[2]);
	minimum = Vector3D(p[0], p[1], p[2]);
	points++;
	for (int i = 1; i < nOfPoints; i++) {
		const Vector3D& pos = points->pos;
		if (pos[0] < minimum[0]) {
			minimum[0] = pos[0];
		}
		if (pos[0] > maximum[0]) {
			maximum[0] = pos[0];
		}
		if (pos[1] < minimum[1]) {
			minimum[1] = pos[1];
		}
		if (pos[1] > maximum[1]) {
			maximum[1] = pos[1];
		}
		if (pos[2] < minimum[2]) {
			minimum[2] = pos[2];
		}
		if (pos[2] > maximum[2]) {
			maximum[2] = pos[2];
		}
		points++;
	}
}

void KdTree::getMinMax(KdTreePoint *points, int nOfPoints, int dim, float &mmin, float &mmax) {
	mmin = points->pos[dim];
	mmax = points->pos[dim];
	points++;
	for (int i=1; i<nOfPoints; i++) {
		if (points->pos[dim] < mmin) {
			mmin = points->pos[dim];
		}
		else if (points->pos[dim] > mmax) {
			mmax = points->pos[dim];
		}
		points++;
	}
}


void KdTree::splitAtMid(KdTreePoint *points, int nOfPoints, int dim, float cutVal, int &br1, int &br2) {
    int l = 0;
    int r = nOfPoints-1;
    for(;;) {				// partition points[0..n-1] about the cut value
		while (l < nOfPoints && points[l].pos[dim] < cutVal) {
			l++;
		}
		while (r >= 0 && points[r].pos[dim] >= cutVal) {
			r--;
		}
		if (l > r) 
			break;
		SWAP_POINTS(l,r);
		l++; 
		r--;
    }
    br1 = l;			// now: points[0..br1-1] < cutVal <= points[br1..n-1]
    r = nOfPoints-1;
    for(;;) {				// partition points[br1..n-1] about cutVal
		while (l < nOfPoints && points[l].pos[dim] <= cutVal) { 
			l++;
		}
		while (r >= br1 && points[r].pos[dim] > cutVal) {
			r--;
		}
		if (l > r) 
			break;
		SWAP_POINTS(l,r);
		l++; 
		r--;
    }
    br2 = l;			// now: points[br1..br2-1] == cutVal < points[br2..n-1]
}

void KdNode::queryNode(float rd, PQueue* queryPriorityQueue, Vector3D const& queryPosition, Vector3D& queryOffsets) {
	
	if (!leaf)
	{
		float old_off = queryOffsets[nodedata.m_dim];
		float new_off = queryPosition[nodedata.m_dim] - nodedata.m_cutval;
		if (new_off < 0) {
			nodedata.m_children[0]->queryNode(rd, queryPriorityQueue, 
											  queryPosition, queryOffsets);
			rd = rd - SQR(old_off) + SQR(new_off);
			if (rd < queryPriorityQueue->getMaxWeight()) {
		  		queryOffsets[nodedata.m_dim] = new_off;
		  		nodedata.m_children[1]->queryNode(rd, queryPriorityQueue, 
		  	   	 								queryPosition, queryOffsets);
		  		queryOffsets[nodedata.m_dim] = old_off;
			}
		}
		else {
			nodedata.m_children[1]->queryNode(rd, queryPriorityQueue, 
										  queryPosition, queryOffsets);
			rd = rd - SQR(old_off) + SQR(new_off);
			if (rd < queryPriorityQueue->getMaxWeight()) {
		  		queryOffsets[nodedata.m_dim] = new_off;
		  		nodedata.m_children[0]->queryNode(rd, queryPriorityQueue, 
												queryPosition, queryOffsets);
		  		queryOffsets[nodedata.m_dim] = old_off;
			}
		}
	}
  	else {
		
		float sqrDist;
		//use pointer arithmetic to speed up the linear traversing
		KdTreePoint* point = leafdata.m_points;
		for (unsigned int i=0; i<leafdata.m_nOfElements; i++) {
			sqrDist = (point->pos - queryPosition).getSquaredLength();
			if (sqrDist < queryPriorityQueue->getMaxWeight()) {
				queryPriorityQueue->insert(point->index, sqrDist);
			}
			point++;
		}		
	}
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
