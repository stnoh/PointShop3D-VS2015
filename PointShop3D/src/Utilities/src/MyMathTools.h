// Title:   MyMathTools.h
// Created: Thu Sep 25 15:01:35 2003
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

#ifndef __MYMATHTOOLS_H_
#define __MYMATHTOOLS_H_

#include "../../DataTypes/src/MyDataTypes.h"
#include "../../DataTypes/src/Vector3D.h"
#include "../../DataTypes/src/Vector2D.h"
#include "UtilitiesDLL.h"

/**
 * This namespace provides some advanced mathematical functions.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace MyMathTools {

	/**
	 * Calculates the inner product of two gaussians, given by their
	 * <code>inverseVarianceMatrix</code> V<sup>-1</sup> and their translation vectors
	 * (<code>xTranslation</code>, <code>yTranslation</code>).
	 * 
	 * @param inverseVarianceMatrix1
	 *        inverse variance matrix of the first splat
	 * @param xTranslation1
	 *        translation in x-direction of the first splat (object coordinate)
	 * @param yTranslation1
	 *        translation in y-direction of the first splat (object coordinate)
	 * @param inverseVarianceMatrix2
	 *        inverse variance matrix of the second splat
	 * @param xTranslation2
	 *        translation in x-direction of the second splat (object coordinate)
	 * @param yTranslation2
	 *        translation in y-direction of the second splat (object coordinate)
	 * @return the inner product
	 */
	UTILITIES_API float innerProduct (const MyDataTypes::SymmetricMatrix3f inverseVarianceMatrix1, const float xTranslation1, const float yTranslation1,
		                              const MyDataTypes::SymmetricMatrix3f inverseVarianceMatrix2, const float xTranslation2, const float yTranslation2);

	/**
	 * Calculates the euklidean distance between the two 3D-points indicated by <code>vector1</code> and
	 * <code>vector2</code>:
	 * <pre>
	 *   dist = sqrt ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2))
	 * </pre>
	 *
	 * @param vector1
	 *        a <code>Vector3D</code> indicating the position of point 1
	 * @param vector2
	 *        a <code>Vector3D</code> indicating the position of point 2
	 * @return the euklidean distance
	 * @see #squaredEuklideanDistance3D
	 */
	UTILITIES_API float euklideanDistance3D (const Vector3D vector1, const Vector3D);

	/**
	 * Calculates the squared euklidean distance between the two 3D-points indicated by <code>vector1</code> and
	 * <code>vector2</code>:
	 * <pre>
	 *   dist = (x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2)
	 * </pre>
	 *
	 * @param vector1
	 *        a <code>Vector3D</code> indicating the position of point 1
	 * @param vector2
	 *        a <code>Vector3D</code> indicating the position of point 2
	 * @return the squared euklidean distance
	 * @see #euklideanDistance3D
	 */
	UTILITIES_API float squaredEuklideanDistance3D (const Vector3D vector1, const Vector3D);

	/**
	 * Calculates the euklidean distance between the two 2D-points indicated by <code>vector1</code> and
	 * <code>vector2</code>:
	 * <pre>
	 *   dist = sqrt ((x1 - x2)^2 + (y1 - y2)^2)
	 * </pre>
	 *
	 * @param vector1
	 *        a <code>Vector2D</code> indicating the position of point 1
	 * @param vector2
	 *        a <code>Vector2D</code> indicating the position of point 2
	 * @return the euklidean distance
	 * @see #squaredEuklideanDistance2D
	 */
	UTILITIES_API float euklideanDistance2D (const Vector2D vector1, const Vector2D);

	/**
	 * Calculates the squared euklidean distance between the two 2D-points indicated by <code>vector1</code> and
	 * <code>vector2</code>:
	 * <pre>
	 *   dist = (x1 - x2)^2 + (y1 - y2)^2)
	 * </pre>
	 *
	 * @param vector1
	 *        a <code>Vector2D</code> indicating the position of point 1
	 * @param vector2
	 *        a <code>Vector2D</code> indicating the position of point 2
	 * @return the squared euklidean distance
	 * @see #euklideanDistance2D
	 */
	UTILITIES_API float squaredEuklideanDistance2D (const Vector2D vector1, const Vector2D);

	/**
	 * Returns the maximum of the <em>integers</em> <code>a</code> and <code>b</code>.
	 * 
	 * @param a
	 *        the first <em>integer</em> value to be compared
	 * @param b
	 *        the second <em>integer</em> value to be compared
	 * @return the maximum of <code>a</code> and <code>b</code>
	 */
	UTILITIES_API inline int mini (const int a, const int b) {
	  return (a < b) ? a : b;
	}

	/**
	 * Returns the minimum of the <em>integers</em> <code>a</code> and <code>b</code>.
	 * 
	 * @param a
	 *        the first <em>integer</em> value to be compared
	 * @param b
	 *        the second <em>integer</em> value to be compared
	 * @return the minimum of <code>a</code> and <code>b</code>
	 */
	UTILITIES_API inline int maxi (const int a, const int b) {
	  return (a > b) ? a : b;
	}

	/**
	 * Returns the minimum of the <em>floats</em> <code>a</code> and <code>b</code>.
	 * 
	 * @param a
	 *        the first <em>float</em> value to be compared
	 * @param b
	 *        the second <em>float</em> value to be compared
	 * @return the maximum of <code>a</code> and <code>b</code>
	 */
	UTILITIES_API inline float minf (const float a, const float b) {
	  return (a < b) ? a : b;
	}

	/**
	 * Returns the maximum of the <em>floats</em> <code>a</code> and <code>b</code>.
	 * 
	 * @param a
	 *        the first <em>float</em> value to be compared
	 * @param b
	 *        the second <em>float</em> value to be compared
	 * @return the minimum of <code>a</code> and <code>b</code>
	 */
	UTILITIES_API inline float maxf (const float a, const float b) {
	  return (a > b) ? a : b;
	}	

};


#endif  // __MYMATHTOOLS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
