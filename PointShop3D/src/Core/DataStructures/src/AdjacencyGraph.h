// Title:   AdjacencyGraph.h
// Created: Thu Sep 25 14:30:17 2003
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

#ifndef __ADJACENCY_GRAPH_H_
#define __ADJACENCY_GRAPH_H_

#include <qobject.h>	// simply for definition of uint

#include "../../../DataTypes/src/Vector3D.h"
#include "../../src/CoreDLL.h"
#include "NeighbourHood.h"
#include "SurfelCollection.h"
#include <vector>

#include "../../../Utilities/MagicSoft/MgcEigen.h"


/**
 * Computes and stores for each surfel its nearest neighbors such that constant access time to the neighbors is granted
 *
 * @author Mark Pauly
 * @version 1.2
 */
class AdjacencyGraph {

public:

	/**
	 * computes the <code>numNeighbors</code>-nearest neighbors for each surfel in <code>surfelCollection</code>
	 *
	 * @param surfelCollection
	 *		  collection of surfels
	 * @param numNeighbors
	 *		  number of nearest neighbors
	 */
	CORE_API AdjacencyGraph (SurfelCollection* surfelCollection, unsigned int numNeighbors);

	/**
	 * computes the <code>numNeighbors</code>-nearest neighbors for each surfel in <code>surfels</code>
	 *
	 * <b>Note</b>: <code>surfels</code> is deleted when AdjacencyGraph is deleted
	 *
	 * @param surfels
	 *		  surfel vector
	 * @param numNeighbors
	 *		  number of nearest neighbors
	 */
	CORE_API AdjacencyGraph (std::vector<SurfelInterface*> *surfels, unsigned int numNeighbors);
	
	/**
	 * computes the <code>numNeighbors</code>-nearest neighbors for each surfel in <code>surfels</code>
	 *
	 * <b>Note</b>: <code>surfels</code> is deleted when AdjacencyGraph is deleted
	 *
	 * @params surfelSearch
	 *		  uses <code>surfelSearch</code> as search structure	
	 * @param surfels
	 *		  list of surfels
	 * @param numNeighbors
	 *		  number of nearest neighbors
	 */	
	CORE_API AdjacencyGraph (NeighbourHood *surfelSearch, std::vector<SurfelInterface*> *surfels, unsigned int numNeighbors);

	/**
	 * Destroys this <code>AdjacencyGraph</code>. 
	 */
	CORE_API ~AdjacencyGraph();

	/**
	 * returns a surfel from the surfel list
	 *
	 * @param surfelIndex
	 *		  index of the surfel in the list to return
	 * @return the surfel at <code>surfelIndex</code>
	 *
	 * #see getNeighbor
	 */
	CORE_API SurfelInterface* getSurfel (unsigned int surfelIndex);
	/** 
	 * returns the index of the <code>neighborIndex</code>-nearest surfel to the surfel at <code>surfelIndex</code>
	 *
	 * <b>Note</b>: For neighborIndex = 0, the surfel itself is returned.
	 *
	 * @param surfelIndex
	 *		  index of the surfel in the surfel list
	 * @param neighborIndex
	 *		  <code>neighborIndex</code>-nearest surfel
	 * @return the index of the neighbor
	 *
	 * @see #getNeighbor
	 */
	CORE_API unsigned int getNeighborIndex (unsigned int surfelIndex, unsigned int neighborIndex);
	/** 
	 * returns the index of the <code>neighborIndex</code>-nearest surfel to the surfel at <code>surfelIndex</code>
	 *
	 * <b>Note</b>: Generally, getSurfel(getNeighborIndex(...)) gets the same result as getNeighbor(...)
	 *
	 * @param surfelIndex
	 *		  index of the surfel in the surfel list
	 * @param neighborIndex
	 *		  <code>neighborIndex</code>-nearest surfel
	 * @return the neighbor surfel
	 *
	 * @see #getNeighborIndex
	 */
	CORE_API SurfelInterface* getNeighbor (unsigned int surfelIndex, unsigned int neighborIndex);
	/**
	 * computes a normal for a surfel from its nearest neighbors using PCA
	 *
	 * @param surfelIndex
	 *		  index of the surfel in the surfel list
	 * @param numNeighbors
	 *		  number of neighbors to consider for the PCA.
	 *		  This number must be equal or lower than the number in the constructor (@see #AdjacencyGraph)
	 */
	CORE_API Vector3D computeNormal (unsigned int surfelIndex, unsigned int numNeighbors);

private:
	
	std::vector<SurfelInterface*>::iterator surfelList;
	unsigned int             *neighborList;
	unsigned int             numNeighbors;	
	Mgc::Eigen*				 eigenSolver;

};


#endif  // __ADJACENCY_GRAPH_H_
// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
