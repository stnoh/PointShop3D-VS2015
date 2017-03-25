// Title:   Matrix.h
// Created: Thu Sep 25 15:01:42 2003
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

//-------------------------------------------------------------------------
// Matrix operations
// Note: matrices are represented as arrays of floats or doubles in column
// major order!
//
// Matrix.h
//-------------------------------------------------------------------------

#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <math.h>

#include "../../DataTypes/src/MyDataTypes.h"
#include "../../DataTypes/src/Vector3D.h"
#include "UtilitiesDLL.h"

// Function prototypes
UTILITIES_API void MtrUnity4x4f(float *m);
UTILITIES_API void MtrUnity4x4d(double *m);

UTILITIES_API void MtrCopy4x4f(const float *s, float *d);
UTILITIES_API void MtrCopy4x4d(const double *s, double *d);
UTILITIES_API void MtrCopy3x3f(const float *s, float *d);
UTILITIES_API void MtrCopy3x3d(const double *s, double *d);

UTILITIES_API void MtrTranspose4x4f(const float *s, float *d);
UTILITIES_API void MtrTranspose4x4d(const double *s, double *d);
UTILITIES_API void MtrTranspose3x3f(const float *s, float *d);
UTILITIES_API void MtrTranspose3x3d(const double *s, double *d);

UTILITIES_API void MtrMultScal3x3f(const float *s, const float c, float *r);
UTILITIES_API void MtrMultScal3x3d(const double *s, const double c, double *r);

UTILITIES_API void MtrMultVector3f (const float *m, const float *v, float *r);

/**
 * Multiplies the matrix <code>m</code> with the vector <code>v</code>: r = M * v
 *
 * @param M
 *        the <code>TransformationMatrix16f</code> which is to be multiplied with the vector <code>v</code>
 * @param v
 *        the <code>Vector3D</code>
 * @param R
 *        a pointer to the resulting <code>Vector3D</code>: r = M * v
 */
UTILITIES_API void MtrMultVector3D (const MyDataTypes::TransformationMatrix16f M, const Vector3D v, Vector3D *r);
UTILITIES_API void MtrMultVector3DHom (const MyDataTypes::TransformationMatrix16f M, const Vector3D v, Vector3D *r);

UTILITIES_API void MtrLocalToGlobalTransform (Vector3D position, Vector3D normal, float *matrix);
UTILITIES_API void MtrGlobalToLocalTransform (Vector3D position, Vector3D normal, float *matrix);



UTILITIES_API void MtrMultVector4f(const float *a, const float *v, float *r);

UTILITIES_API void MtrMult4x4f(const float *a, const float *b, float *r);
UTILITIES_API void MtrMult4x4d(const double *a, const double *b, double *r);
UTILITIES_API void MtrMult3x3f(const float *a, const float *b, float *r);
UTILITIES_API void MtrMult3x3d(const double *a, const double *b, double *r);

/**
 * Matrix inversion functions. In case the matrix is singular, i.e., it does not have an inverse, these
 * functions return <code>false</code>.
 */
UTILITIES_API bool MtrInverse4x4f(const float *m, float *r);
UTILITIES_API bool MtrInverse4x4d(const double *m, double *r);
UTILITIES_API bool MtrInverse3x3f(const float *m, float *r);
UTILITIES_API bool MtrInverse3x3d(const double *m, double *r);

/**
 * Calculates the inverse of a 2x2 float matrix <code>m</code>. The
 * result is stored in <code>r</code>. <code>m</code> and <code>r</code>
 * must point to different positions.
 */
UTILITIES_API bool MtrInverse2x2f (const float *m, float *r);
/**
 * Calculates the inverse of a 2x2 double matrix <code>m</code>. The
 * result is stored in <code>r</code>. <code>m</code> and <code>r</code>
 * must point to different positions.
 */
UTILITIES_API bool MtrInverse2x2d (const double *m, double *r);

UTILITIES_API void MtrAdjoint4x4f(const float *m, float *r);
UTILITIES_API void MtrAdjoint4x4d(const double *m, double *r);
UTILITIES_API void MtrAdjoint3x3f(const float *m, float *r);
UTILITIES_API void MtrAdjoint3x3d(const double *m, double *r);

UTILITIES_API float MtrDeterminant4x4f(const float *m);
UTILITIES_API double MtrDeterminant4x4d(const double *m);
UTILITIES_API float MtrDeterminant3x3f(const float a1, const float b1, const float c1,
                                       const float a2, const float b2, const float c2,
                                       const float a3, const float b3, const float c3);
UTILITIES_API double MtrDeterminant3x3d(const double a1, const double b1, const double c1,
                                        const double a2, const double b2, const double c2,
                                        const double a3, const double b3, const double c3);
UTILITIES_API float MtrDeterminant2x2f(const float a, const float b, const float c, const float d);
UTILITIES_API double MtrDeterminant2x2d(const double a, const double b, const double c, const double d);


// operations on row major matrices
UTILITIES_API void MtrMult4x4fr(const float *a, const float *b, float *r);
UTILITIES_API void MtrMult4x4dr(const double *a, const double *b, double *r);
UTILITIES_API void MtrMult3x3fr(const float *a, const float *b, float *r);
UTILITIES_API void MtrMult3x3dr(const double *a, const double *b, double *r);

//------------------------------------------------------------------------
// operations which replace the corresponding OpenGL matrix operations
// NOTE: These functions expect ROW-MAJOR matrices!
// -----------------------------------------------------------------------

/**
 * Multiplies a 4x4 matrix 'm' with the 4x4 translation matrix t, which is
 * given by 'x', 'y' and 'z', and stores the result in the 4x4 matrix 'r'.
 * The matrices 'm' and 'r' may be the same matrix, in which case 'm' gets
 * overwritten by the result matrix 'r'.
 * 
 * @param *m  a pointer to the 4x4 matrix 'm'
 * @param x   translation in x direction
 * @param y   translation in y direction
 * @param z   translation in z direction
 * @param *r  result: a pointer to the 4x4 matrix 'r' which stores
 *            r = m * t, where t is the translation matrix as defined
 *            by 'x', 'y' and 'z'
 */
UTILITIES_API void MtrTranslate4x4fr (const float *m, const float x, const float y, const float z, float *r);

/**
 * Creates a 4x4 translation matrix 't', which is defined by the
 * translation parameters 'x', 'y' and 'z'. 't' has the following
 * form:
 * <pre>
 * 1 0 0 x
 * 0 1 0 y
 * 0 0 1 z
 * 0 0 0 1
 * </pre>
 * The translation matrix 't' is stored in row-major order.
 *
 * @param x   translation in x direction
 * @param y   translation in y direction
 * @param z   translation in z direction
 * @param *t  result: the translation matrix 't'
 */
UTILITIES_API void MtrCreateTranslation4x4fr (const float x, const float y, const float z, float *t);

/**
 * Creates a 4x4 rotation matrix 'r', which is given
 * by the 'angle' and the axis defined by the axis vector ['x', 'y', 'z'], and
 * stores the result in the 4x4 matrix 'r'. The 'angle' defines the counter-clockwise
 * rotation around the axis. The matrix is in <em>row</em> major order.
 *
 * @param angle defines the rotation counter-clockwise [in radians]
 * @param x   x component of the axis vector
 * @param y   y component of the axis vector
 * @param z   z component of the axis vector
 * @param *r  result: a pointer to the 4x4 matrix 'r' which contains the rotation
 *            matrix
 */
UTILITIES_API void MtrCreateRotation4x4fr (const float angle, const float x, const float y, const float z, float *r);

/**
 * Creates a 4x4 rotation matrix 'r', which is given
 * by the 'angle' and the axis defined by the axis vector ['x', 'y', 'z'], and
 * stores the result in the 4x4 matrix 'r'. The 'angle' defines the counter-clockwise
 * rotation around the axis. The matrix is in <em>column</em> major order.
 *
 * @param angle defines the rotation counter-clockwise [in radians]
 * @param x   x component of the axis vector
 * @param y   y component of the axis vector
 * @param z   z component of the axis vector
 * @param *r  result: a pointer to the 4x4 matrix 'r' which contains the rotation
 *            matrix
 */
UTILITIES_API void MtrCreateRotation4x4fc (const float angle, const float x, const float y, const float z, float *r);


UTILITIES_API float MtrGetUniformScalingFactorSquared4x4f (const float *a);
UTILITIES_API float MtrGetUniformScalingFactor4x4f (const float *a);

#endif  // __MATRIX_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
