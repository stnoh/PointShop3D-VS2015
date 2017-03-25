// Title:   Painter.h
// Created: Thu Sep 25 14:28:45 2003
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

#ifndef __PAINTER_H_
#define __PAINTER_H_




#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../../Utilities/src/Matrix.h"
#include "../../Brush/src/Brush.h"
#include "../../OrthoMapper/src/OrthoMapper.h"
#include "../../DataStructures/src/SurfelCollection.h"
#include "../../Scene/src/Scene.h"
#include "../../Interfaces/src/FrameBufferInterface.h"


/**
 * Implements a painter. This class is used to perform surface editing operations using the brush tool.
 *
 * @author Mark Pauly
 * @version 1.2
 */

class Painter {


public:

	/**
	 * Builds a new painter using the <code>brush</code>
	 *
	 * @param brush
	 *		  the <code>Brush</code> used by the painter
	 */
	CORE_API Painter (const Brush *brush);
	CORE_API virtual ~Painter();

	
	/**
	 * Perform a paint operation.
	 *
	 * @param surfels
	 *        a list of all surfels that should be painted
	 * @param numSurfels
	 *        number of Surfels
	 * @param textureCoordinates
	 *        stores the texture coordinates for each surfel. If textureCoordinates = 0
	 *        the surfels are assumed to lie on a regular grid according to the brush resolution
	 * @param scale
	 *        scale for carving
	 * @param carveDirection
	 *        direction of carving
	 * @param maxDeviation
	 *        maximmum deviation of positions before and after displacement
	 * @param gaps
	 *        if the surfels lie on a regular grid and the grid contains gaps, i.e., positions, where
	 *        no surfel is stored (marked by a 0 pointer), this flag has to be set to true
	 *
     * @return returns true if painting was succesful
	*/
	CORE_API bool paint (std::vector<SurfelInterface *>* surfels, int numSurfels,
						 const std::vector<MyDataTypes::TextureCoordinate>* textureCoords,
						 const float scale = 1.0f, const Vector3D *carveDirection = 0,
						 const float maxDeviation = 2.0f, const bool gaps = false,
						 float* jacobians = 0, const Vector3D* localX = 0, const Vector3D* localY = 0,
						 const bool useEllipticalSurfels = false, float lowPassFilter = 0.f);


	/**
	 * select/deselect the surfels according their texture color.
	 * If the blue fraction is larger than 0.66 the surfel is selected (<code>SurfelInterface::SELECTED1</code>)
	 * if the blue fraction is smaller than the surfel is deselected. 
	 * Analog for red <-> <code>SurfelInterface::SELECTED2</code> and
	 * green <-> <code>SurfelInterface::SELECTED1</code>.
	 * This function might be used to set the selection by means of a bitmap.
	 *
	 * @param surfels
	 *		  the surfels to test
	 * @param numSurfels
	 *		  the number of surfels
	 * @param textureCoordinates
	 *		  the texture coordinates of the surfels
	 * @param selectionCounters
	 *		  counts the number of surfels which are selected for each possible selection
	 */
	CORE_API void select (std::vector<SurfelInterface *>* surfels, int numSurfels,
						  const std::vector<MyDataTypes::TextureCoordinate>* textureCoords,
						  int *selectionCounters);
	/**
	 * changes the attributes of all <code>surfels</code> according the brush
	 *
	 * @param surfels
	 *		  the surfels to paint
	 */
	CORE_API void paintWholeObject (SurfelCollection *surfels);


	/**
	 * Updates all relelant data structures when the brush has been changed.
	 *
	 * @param newBrush
	 *        pointer to the new brush
    */
	CORE_API void setBrush (const Brush *newBrush);


private:


	int	             sizeX,                 //  size of brush
		             sizeY; 
	
	BrushChannel     *displacementChannel,
                     *diffColorChannel,
					 *specColorChannel,
					 *ambCoeffChannel,
					 *diffCoeffChannel,
					 *specCoeffChannel,
					 *shininessChannel;

	ChannelComponent *displacementNormal,
		             *displacementCarving,
		             *displacementAlpha,		
	                 *diffRedComponent,
		             *diffGreenComponent,
		             *diffBlueComponent,
		             *diffColorAlpha,	
	                 *specRedComponent,
		             *specGreenComponent,
		             *specBlueComponent,
		             *specColorAlpha,	
					 *ambientCoefficient,
					 *ambientCoefficientAlpha,
	                 *diffuseCoefficient,
		             *diffuseCoefficientAlpha,	
	                 *specularCoefficient,
		             *specularCoefficientAlpha,
					 *shininess,
					 *shininessAlpha;

	Scene            *scene;		
	bool             surfelRadiusTooBig;
	float	         expectedRadius;    // expected average surfel radius

	float			 *filterBlendingGrid;

	struct BrushSample {
	    float redDiffColor;
	    float greenDiffColor;
	    float blueDiffColor;
	    float diffColorAlpha;
	    float redSpecColor;
	    float greenSpecColor;
	    float blueSpecColor;
	    float specColorAlpha;
	    float ambientCoeff;
	    float ambientCoeffAlpha;
	    float diffCoeff;
	    float diffCoeffAlpha;
	    float specCoeff;
	    float specCoeffAlpha;
	    float shininess;
	    float shininessAlpha;
	};

	float lowPassFilter;
	float cutoffRadius;

	bool computeRadiiAndNormals (std::vector<SurfelInterface *>* surfels);
	bool computeRadiiAndNormalsWithGaps (std::vector<SurfelInterface *>* surfels);

	inline void setRadiusAndNormal (SurfelInterface *surfel, Vector3D dx, Vector3D dy);

	inline bool computeDX(std::vector<SurfelInterface *>* surfels, const int x, const int y, Vector3D& dX);
	inline bool computeDY(std::vector<SurfelInterface *>* surfels, const int x, const int y, Vector3D& dY);

	BrushSample rasterizeSurfel (SurfelInterface *surfels, MyDataTypes::TextureCoordinate texCoords, float jacobian[4], float localKernel[4], Vector3D localX, Vector3D localY, float lowPassFilter);
};


#endif  // __PAINTER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
