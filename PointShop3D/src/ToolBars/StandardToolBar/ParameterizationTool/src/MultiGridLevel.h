// Title:   MultiGridLevel.h
// Created: Thu Sep 25 14:58:26 2003
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

#ifndef __MULTIGRIDLEVEL_H_
#define __MULTIGRIDLEVEL_H_

//#include <qintdict.h>

#include <vector>
//#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Core/DataStructures/src/NeighbourHood.h"
#include "../../../../DataTypes/src/Vector3D.h"
//#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "SparseLeastSquares.h"

/**
 * Multigrid level.
 *
 * @author Matthias Zwicker, Oliver Knoll
 * @version 1.2
 */
class MultiGridLevel {

public:

	/**
	 * Creates this <code>MultiGridLevel</code> with a given <code>neighbourHood</code> search structure,
	 * which also contains the points where this <code>MultiGridLevel</code> operates on, and the initial
	 * <code>uvCoordinates</code> array, which will contain the more accurate solution UV coordinates
	 * later on.
	 *
	 * <b>Note:</b> The <code>uvCoordinates</code> array must contain the same amount of UV coordinate entries
	 *              as the point array in the <code>neighbourHood</code>, where a UV coordinate consists of
	 *              <em>two</em> <code>float</code> values, U and V.
	 *
	 * @param neighbourHood
	 *        a pointer to a <code>NeighbourHood</code> which also contains the points for which this
	 *        <code>MultiGridLevel</code> calculates the UV coordinates.
	 * @param normals
	 *		  an array of normals of the surfels in the <code>neighbourHood</code>
	 * @param uvCoordinates
	 *        a pointer to a <code>float</code> array which contains the initial UV coordinates for each point in the <code>neighbourHood</code>
	 *        structure; the first <em>n</em> contain the U <code>float</code> values, the second <em>n</em> entries
	 *        the V <code>float</code> value, where <em>n</em> is the number of points in the <code>neighbourHood</code>;
	 *        will contain the updated UV coordinates later on, the old values being overwritten
	 * @see NeighbourHood#getNofPositions
	 * @see NeighbourHood#getPositions
	 */
	MultiGridLevel (NeighbourHood *neighbourHood, const Vector3D *normals, float *uvCoordinates);
	virtual ~MultiGridLevel();

	/**
	 * sets the search structure
	 *
	 * @param newNeighbourHood
	 *        a pointer to a <code>NeighbourHood</code> which also contains the points for which this
	 *        <code>MultiGridLevel</code> calculates the UV coordinates.
	 */
	void setNeighbourHood (NeighbourHood *newNeighbourHood);

	/**
	 * returns the search structure
	 */
	NeighbourHood *getNeighbourHood() const;
	
	/**
	 * sets the normal vectors
	 *
	 * @param newNormals
	 *		  an array of normals of the surfels in the <code>neighbourHood</code>
	 */
	void setNormals (const Vector3D *newNormals);

	/**
	 * gets the array of normal vectors
	 */
	const Vector3D *getNormals() const;

	/**
	 * sets the UV coordinates
	 *
	 * @param newUVCoordinates
	 *        a pointer to a <code>float</code> array which contains the initial UV coordinates for each point in the <code>neighbourHood</code>
	 *        structure; the first <em>n</em> contain the U <code>float</code> values, the second <em>n</em> entries
	 *        the V <code>float</code> value, where <em>n</em> is the number of points in the <code>neighbourHood</code>;
	 *        will contain the updated UV coordinates later on, the old values being overwritten
	 */
	void setUVCoordinates (float *newUVCoordinates);

	/**
	 * gets the UV coordinates
	 */
	float *getUVCoordinates() const;

	/**
	 * A convenience method which returns the number of UV coordinates (each UV coordinate consists of
	 * <em>two</em> <code>float</code> values, U and V). This is the same value as returned by
	 * <code>getNeighbourHood()->getNofPositions()</code>.
	 * 
	 * @return the number of UV coordinates
	 * @see NeighbourHood#getNofPositions
	 * @see #getNeighbourHood
	 */
	uint getNofUVCoordinates() const;

	/**
	 * set the precision used for solving the least squares problem
	 *
	 * @see SparseLeastSquares#solve
	 */
	void setPrecision (const float newPrecision);

	/**
	 * get the precision
	 */
	float getPrecision() const;

	/**
	 * Add fitting constraints to the least squares system.
	 */
	void addFittingConstraints (const float *fittingConstraintsU, const float *fittingConstraintsV,
								const uint *fittingConstraintsIndex, const uint nofConstraints, float weight);

	/**
	 * Remove the fitting constraints from the least squares system.
	 */
	void removeFittingConstraints();					// remove the fitting constraints currently stored in the fittingConstraints* arrays

	
	/**
	 * Generate UV coordinates by solving the least squares system.
	 */
	void generateUVCoordinates();

private:

	typedef enum positionFlag {

		NO_FLAGS          = 0x00000000,
		UVBOUNDARY        = 0x00000001,
		GEOMETRICBOUNDARY = 0x00000002,
		ALL_FLAGS         = 0xffffffff

	} PositionFlag;

	typedef int PositionFlags;
	
	float              precision;
	NeighbourHood      *neighbourHood;	                // a search structure to do nearest neighbor queries
													    // NOTE: this member indicates the state of the parameterization process:
													    // 0: the selection process is still going on
													    // !=0: the selection is finished. if the user attempts to modify the selection,
													    // the old selection is first discarded
	const Vector3D     *positions;                      // the positions, as stored in the 'neighbourHood'
	const Vector3D     *normals;                        // the corresponding normals
	uint               nofPositions;
	float              *uvCoordinates;
	PositionFlags      *positionFlags;                  // an array which holds flags for each position

	SparseLeastSquares *leastSquares;

	std::vector<float>      fittingConstraintsU;				// U coordinates of the currently active fitting constraints
	std::vector<float>      fittingConstraintsV;				// V coordinates of the currently active fitting constraints
	std::vector<int>        fittingConstraintsIndex;			// indices of the surfels in the currently active fitting constraints			

	

	// the sparse least squares optimization problem	
	void solveLeastSquares();

	void addRegularizationConstraints();
	void addDirectionalDerivativesConstraints();
	void normalizeAndAddConstraint (std::vector<int> &I, std::vector<float> &a, float b[2], const float w, bool addFlag = true);

	// resets all the position flags to NO_FLAGS
	void resetFlags();

};


#endif  // __MULTIGRIDLEVEL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
