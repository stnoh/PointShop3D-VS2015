// Title:   Cluster.h
// Created: Thu Sep 25 14:30:12 2003
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

#ifndef __CLUSTER_H_
#define __CLUSTER_H_

#include <qobject.h>	// simply for defintion of uint

#include "../../../DataTypes/src/Vector3D.h"
#include "../../src/CoreDLL.h"
#include "NeighbourHood.h"

/**
 * This data structure clusters the point array used by the <code>NeighbourHood</code> data structure by
 * putting approximately <code>minimumClusterSize</code> points into a new cluster and representing
 * this cluster by a new point stored in <code>clusterPoints</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see NeighbourHood
 */
class Cluster {

public:

	/**
	 * Creates this <code>Cluster</code>, initializing it with the <code>neighbourHood</code> to be
	 * clustered and its <code>normals</code>. <code>minimumClusterSize</code> points should be grouped
	 * into this <code>Cluster</code>.
	 *
	 * @param neighbourHood
	 *        a pointer to the <code>NeighbourHood</code> to be clustered
	 * @param normals
	 *        a pointer to an array of <code>Vector3D</code> containing the normals of the points
	 *        in the <code>neighbourHood</code>; must contain the same number of entries as there
	 *        are points in the <code>neighbourHood</code>
	 * @param minimumClusterSize
	 *        the minimum number of points which should be put together into a new cluster
	 */
	CORE_API Cluster (NeighbourHood *neighbourHood, const Vector3D *normals, const uint minimumClusterSize);
	CORE_API ~Cluster();

	/**
	 * Sets a <code>newNeighbourHood</code> for which the clustering is to be calculated.
	 *
	 * <b>Attention:</b> Make sure that the number of normals is the same
	 *                   as the number of points in the <code>newNeighbourHood</code>.
	 *                   Call <code>setNormals</code> accordingly.
	 *
	 * @param newNeighbourHood
	 *        a pointer to the new <code>NeighbourHood</code> to be clustered
	 * @see #setNormals
	 */
	CORE_API void setNeighbourHood (NeighbourHood *newNeighbourHood);

	/**
	 * Returns the <code>NeighbourHood</code> which is clustered.
	 *
	 * @return a pointer to the <code>NeighbourHood</code> to be clustered
	 */
	CORE_API NeighbourHood *getNeighbourHood() const;

	/**
	 * Sets the <code>newNormals</code> which belong to the <code>NeighbourHood</code> being clustered.
	 *
	 * <b>Attention:</b> Make sure that the number of points in the <code>NeighbourHood</code> is the same
	 *                   as the number of <code>newNormals</code>.
	 *                   Call <code>setNeighbourHood</code> accordingly.
	 *
	 * @param newNeighbourHood
	 *        a pointer to the new <code>NeighbourHood</code> to be clustered
	 * @see #setNormals
	 */
	CORE_API void setNormals (const Vector3D *newNormals);

	/**
	 * Returns the normals of the <code>NeighbourHood</code> which is clustered.
	 *
	 * @return a pointer to a <code>Vector3D</code> array which contains the normals
	 */
	CORE_API const Vector3D *getNormals() const;

	/**
	 * Sets the minimum number of points in a cluster.
	 *
	 * @param newMinimumClusterSize
	 *        the new mimimum cluster size
	 */
	CORE_API void setMinimumClusterSize (const uint newMinimumClusterSize);

	/**
	 * Returns the minimum number of points in a cluster.
	 *
	 * @return the mimimum cluster size
	 */
	CORE_API uint getMinimumClusterSize() const;

	/**
	 * Clusters the point array used by the <code>neighbourHood</code> by putting about
	 * <code>minimumClusterSize</code> points into a cluster and represents this
	 * cluster by a new point which is stored in <code>clusterPoints</code>.
	 *
	 * <b>Note:</b> The resulting <code>clusterPoints</code> and <code>normals</code> arrays are internally
	 * allocated, but must <em>not</em> be <code>delete[]</code>d after use!
	 *
	 * @param clusterPoints
	 *        a pointer to a pointer to the resulting point array which contains the <code>Vector3D</code>
	 *        points which represent the clusters; must <em>not</em> be <code>delete[]</code>d after use
	 * @param clusterNormals
	 *        a pointer to a pointer to the resulting normal array which contains the <code>Vector3D</code>
	 *        normals which correspond to the <code>clusterPoints</code>; must <em>not</em> be <code>delete[]</code>d after use
	 * @param nofClusters
	 *        the resulting number of clusters (that is the size of the <code>clusterPoints</code> and
	 *        <code>normals</code> array)
	 */
	CORE_API void calculate (Vector3D **clusterPoints, Vector3D **clusterNormals, uint *nofClusters);

	/**
	 * Returns the position index of the parent of the point specified by the <code>positionIndex</code>.
	 *
	 * @param positionIndex
	 *        an index into the position array which is used by the <code>NeighbourHood</code>; specifies the
	 *        point for which the parent position index is to be returned; must be
	 *        in [0, <em>n</em> - 1], where <em>n</em> is the value returned by
	 *        <code>NeighbourHood::getNofPositions</code>
	 * @return the position index of the parent of the point specified by <code>positionIndex</code>
	 */
	CORE_API uint getParentPositionIndex (const uint positionIndex);	

private:

	typedef enum positionFlag {

		NO_FLAGS        = 0x00000000,
		CLUSTER_COVERED = 0x00000001,
		CLUSTER_STRAY   = 0x00000002,
		ALL_FLAGS       = 0xffffffff

	} PositionFlag;

	typedef int PositionFlags;
	
	NeighbourHood       *neighbourHood;            // contains the points which we cluster
	const Vector3D      *normals;                  // and their normals
	Vector3D            *clusterPoints,            // the resulting cluster points
		                *clusterNormals;           // and their normals

	uint                minimumClusterSize,  // the minimum number of points in a cluster
		                nofPositions,              // the number of points in the 'neighbourHood'
	                    nofClusters;
	bool                isClusterUpdated;

	uint                *parentIndices;
	
	// removes the allocated resources:
	// - clusterPoints, clusterNormals
	// - clusterIndex, clusterIndexBase, parentIndex
	// - flags
	void cleanUp();
	
};


#endif  // __CLUSTER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
