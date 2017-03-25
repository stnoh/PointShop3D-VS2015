// Title:   MyDataTypes.h
// Created: Thu Sep 25 14:35:16 2003
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

#ifndef __MYDATATYPES_H_
#define __MYDATATYPES_H_



/**
 * This namespace provides some useful custom data types, such as <code>Vector3f</code> and
 * <code>SymmetricMatrix3f</code>.
 *
 * @version 1.2
 * @author Oliver Knoll
 */
namespace MyDataTypes {


	/**
	 * Symmetric matrix of the form:
	 * <pre>
	 * /     \
	 * | a b |
	 * | b c |
	 * \     /
	 * </pre>
	 */
	typedef struct symmetricMatrix {

		float a;
		float b;
		float c;

	} SymmetricMatrix3f;

	/**
	 * A 4x4 transformation matrix which stores <code>float</code> values.
	 */
	typedef float TransformationMatrix16f[16];

	/**
	 * A <code>struct</code> containing two transformation matrices - translation (and scale) and rotation - which
	 * define the camera position.
	 */
	typedef struct cameraPosition {
		TransformationMatrix16f scaleTranslationMatrix,
			                    rotationMatrix;
	} CameraPosition;

	/**
	 * A struct containing U, V coordinates of a texture.
	 */
	typedef struct textureCoordinate {
		float u,
              v;
	} TextureCoordinate;

	/**
	 * A <code>struct</code> which defines the view frustum.
	 */
	typedef struct viewFrustum {

		float fieldOfView,
		      aspectRatio,
		      nearPlane,
			  farPlane;

	} ViewFrustum;

	/**
	 * A struct storing the red <code>r</code>, green <code>g</code> and blue <code>b</code>
	 * color components as <code>float</code> values.
	 */
	typedef struct rgbTriple {
		float r;
		float g;
		float b;
	} RGBTriple;

	/**
	 * A struct which stores the minimum and maximum coordinates of a bounding cube.
	 */
	typedef struct boundingCube {

		int minX,
			maxX,
			minY,
			maxY,
			minZ,
			maxZ;

	} BoundingCube;
	
	/**
	 * Splat data structure describing splat primitives that were rendered to the frame buffer.
	 * The renderer writes this data in an implementation specific fashion.
	 */	
	typedef struct splat {
		float z;
		float conic[4];				// conic matrix for rasterizing the ellipse
		int bbox[4];				// bounding box for rasterizing the ellipse (pixel coordinates)
	} Splat;

};



#endif  // __MYDATATYPES_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
