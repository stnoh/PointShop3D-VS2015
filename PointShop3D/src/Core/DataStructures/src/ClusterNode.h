// Title:   ClusterNode.h
// Created: Thu Sep 25 14:30:10 2003
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

#ifndef __CLUSTERNODE_H_
#define __CLUSTERNODE_H_

#include <list>
#include <qpainter.h>
#include "../../../DataTypes/src/Vector3D.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"

#include "../../../Utilities/MagicSoft/MgcAppr3DPlaneFit.h"
#include "../../../Utilities/MagicSoft/MgcEigen.h"
#include "../../../Utilities/MagicSoft/MgcLinearSystem.h"
#include "../../../Core/Scene/src/Scene.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "Miniball/Miniball.h"
using namespace Mgc;

/**
 * A node of the cluster tree
 *
 * @author Richard Keiser
 * @version 2.0
 */
class ClusterNode {

public:
	
	~ClusterNode();

	/**
	 * initialize the node
	 * 
	 * @param _parent
	 *		  the parent node of this node
	 * @param surfelArray
	 *		  this node contains all surfels of the <code>surfelArray</code>
	 * @param nOfSurfels
	 *		  number of surfels in <code>surfelArray</code>
	 */
	void init(ClusterNode* _parent, SurfelInterface **surfelArray, unsigned int nOfSurfels);
	
	/**
	 * computes the variation of the surfels in this node.
	 * Additionally, also the split vector and the center of the node are computed
	 *
	 * @see #getVariation
	 * @see #getSplitVector
	 * @see #getCenter
	 * @see #computeSplitVector
	 */
	void computeVariation ();

	/**
	 * computes the split vector and the center (average) of the surfels in the node
	 *
	 * @see #getSplitVector
	 * @see #getCenter
	 * @see #computeVariation
	 */
	void computeSplitVector();

	/** 
	 * computes the minimal ball containing the surfels. Sets the radius and the center
	 *
	 * @see #getCenter
	 * @see #getRadius
	 */
	void computeMiniball();

	/**
	 * computes the center (average) of the surfels in this node
	 *
	 * @see #getCenter
	 */
	inline void computeCenter();

	/**
	 * computes the radius of the node such that it contains all surfels
	 *
	 * @see #getCenter
	 * @see #getRadius
	 */
	inline void computeRadius();

	/**
	 * returns the center
	 *
	 * @see #computeCenter
	 */
	inline Vector3D getCenter()  {
		return m_center;
	}

	/**
	 * returns the split vector
	 *
	 * @see #computeSplitVector
	 * @see #computeVariation
	 */
	inline Vector3D getSplitVector()  {
		return m_splitVector;
	}

	/**
	 * returns the variation
	 * @see #computeVariation
     */
	inline float getVariation()  {
		return m_variation;
	}

	/**
	 * sets the parent
	 */
	inline void setParent(ClusterNode* parent) {
		m_parent = parent;
	}

	/**
	 * returns the parent
	 */
	inline ClusterNode* getParent()  {
		return m_parent;
	}

	/**
	 * sets an array of child nodes
	 */
	inline void setChildren(ClusterNode* children) {
		m_children = children;
	}

	/**
	 * returns an array child nodes
	 */
	inline ClusterNode* getChildren()  {
		return m_children;
	}

	/**
	 * returns true if the node has no children
	 */
	inline bool isLeaf()  {
		return m_children == NULL;
	}

	/**
	 * returns the number of surfels which belong to the node
	 */
	inline unsigned int getNOfSurfels()  {
		return m_nOfSurfels;
	}

	/**
	 * returns the radius
	 *
	 * @see computeRadius
	 */
	inline float getRadius()  {
		return m_radius;
	}

	/**
	 * returns the data array of surfels which belong to this node
	 */
	inline SurfelInterface** getArray() {
		return m_surfelArray;
	}

private:

	Vector3D			m_center;
	float				m_variation;
	Vector3D			m_splitVector;
	ClusterNode			* m_parent;					// pointer to parent node
	ClusterNode			* m_children;				// pointer to left child
	float				m_radius;
	SurfelInterface		**m_surfelArray;
	unsigned int		m_nOfSurfels;

};

inline void ClusterNode::computeCenter () {

	unsigned int i;
	m_center.makeZero();
	for (i = 0; i < m_nOfSurfels; i++) {
		m_center += m_surfelArray[i]->getPosition();
	}
	m_center /= (float) (m_nOfSurfels);

}


inline void ClusterNode::computeRadius() {
	unsigned int i;
	m_radius = 0.0f;
	for (i = 0; i < m_nOfSurfels; i++) {
		m_radius = __max(m_radius, (m_center - m_surfelArray[i]->getPosition()).getSquaredLength()+m_surfelArray[i]->getRadius());
	}
	m_radius = sqrt(m_radius);
}




#endif  // __CLUSTERNODE_H_


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
