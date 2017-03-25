// Title:   Parameterization.h
// Created: Thu Sep 25 14:58:17 2003
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

#ifndef __PARAMETERIZATION_H_
#define __PARAMETERIZATION_H_

//#include <qintdict.h>

//#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Core/DataStructures/src/NeighbourHood.h"
#include "../../../../Core/DataStructures/src/Cluster.h"
//#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "MultiGridLevel.h"
#include "SparseLeastSquares.h"
#include <vector>

#include <glm/glm.hpp>

// TODO: just use the "textureAlpha" images for the ToolPatch, textures not needed (they only define the size
//       of the ToolPatch, which is not efficient, and very obscure for the user)

/**
 * This class implements a tool with which the user can generate a UV parameterization
 * (i.e., texture coordinates) on the surface
 *
 * @author Matthias Zwicker
 * @version 1.2
 */
class Parameterization /*: public QObject */ {

//	Q_OBJECT

public:

	Parameterization();
	virtual ~Parameterization();

	void setNofLevels (const uint newNumberOfLevels);
	uint getNofLevels() const;

	void setClusterSize (const uint newClusterSize);
	uint getClusterSize() const;

	void setPrecision (const float newPrecision);
	float getPrecision() const;	

	void setFittingConstrWeights(const float newWeigths);
	float getFittingConstrWeights() const;
	
	void setCoverageThreshold (const float newCoverageThreshold);
	float getCoverageThreshold() const;

	void setLowPassFilter (const float newLowPassFilter);
	float getLowPassFilter() const;

	void setDisplacementScaling (const float newDisplacementScaling);
	float getDisplacementScaling() const;

	void generateUVCoordinates (bool resampleAtTextureResolutionFlag);

	bool isFilterBrushEnabled();
	void setFilterBrushEnabled(const bool enable);

	////////////////////////////////////////////////////////////
	// additional data structure for parameterization-only
	////////////////////////////////////////////////////////////
	std::vector<glm::vec3> targetCloudPoints;
	std::vector<glm::vec3> targetCloudNormals;
	std::vector<glm::vec2> finalUV;
	std::vector<glm::vec2> markers2D;
	std::vector<int> markers3Dindex;

private:
	
	uint			   nofLevels,						// number of multigrid levels
				       clusterSize;						// size of clusters for generating multigrid levels
	float			   precision,
	         		   coverageThreshold,				// used when resampling at texture resolution: threshold indicating full surface coverage
	                   displacementScaling,				// scaling factor for normal displacements stored in displacement maps
			   lowPassFilter;				// low-pass filter used during resampling
	float		   fittingConstrWeights;			// weights for the fitting constraints (vs. the minimum distortion constraints)
	uint               *levelSizes,                     // the number of entries at each level
	                   nofFittingConstraints;
	MultiGridLevel     **multiGridLevels;
	Vector3D           **positions,
		               **normals;
//	SurfelInterface    **selectedSurfels;               // we store the pointers to the selected surfels,
	                                                    // so the generated uv coordinates can be easily
	                                                    // assigned to them once the solution has been calculated
	float              **uvCoordinates;                 // the resulting uv coordinates
	NeighbourHood      **neighbourHoods;	            
	Cluster            **clusters;

	float              **fittingConstraintsU;			// U coordinates of the currently active fitting constraints
	float              **fittingConstraintsV;			// V coordinates of the currently active fitting constraints
	uint               **fittingConstraintIndices;		// indices of the surfels in the currently active fitting constraints
	
	bool               isMultiGridValid;

	bool               applyTexture,					// indicates whether the color texture should be applied to the parameterized surfels
	                   applyTextureAlpha,				// indicates whether the texture alpha channel should be applied to the parameterized surfels
	                   applyDisplacement,				// indicates whether the normal displacement should be applied to the parameterized surfels
	                   applyDisplacementAlpha;			// indicates whether the normal displacement alpha channel should be applied to the parameterized surfels
	
	bool		   useEllipticalSurfels;
	bool               filterBrush;

	void initializeFittingConstraints();			    // get the fitting constraints from the markers and store constraints in above arrays
//	void assignTexture ();			                  	// assign texture colors to original surfels (keeps original surfels, just assigns colors)

//	void resampleAtTextureResolution();					// resample the surface at the resolution of the texture (removes original surfels)
	bool computeJacobian (int positionIndex, float J[4], Vector3D& X, Vector3D& Y);
//	bool computeJacobianUnstable (SurfelInterface* surfel, float J[4], Vector3D& X, Vector3D& Y);
	
	//void clearMultiGridLevels();						// clears all data structures associated with multigrid levels
	void initSolutionFromLowerLevel (const uint levelIndex);			// init uv solution vector from a lower multigrid level

	// initializes the multigrid data structure, filling the nofLevels-1 (bottom) with the coordinates
	// and normals of the surfels in the currrent selection - the other 0..nofLevels-2 levels are initialized to 0
	void initializeMultiGrid();

	// releases all resources used by the multigrid data structure
	void clearMultiGrid();
	/*
private slots:

	// connected to SelectionTool: selectionChanged - release the data structure
	void handleSelectionChanged (SurfelInterface::Flag selectionFlag);
	//*/
};


#endif  // __PARAMETERIZATION_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
