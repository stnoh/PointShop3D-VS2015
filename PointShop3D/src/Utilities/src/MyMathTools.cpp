// Title:   MyMathTools.cpp
// Created: Thu Sep 25 15:01:37 2003
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

#include <math.h>

#include "../../DataTypes/src/MyDataTypes.h"
#include "../../Utilities/src/Matrix.h"
#include "../../DataTypes/src/Vector3D.h"
#include "../../DataTypes/src/Vector2D.h"

#include "MyMathTools.h"

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

float MyMathTools::innerProduct (const MyDataTypes::SymmetricMatrix3f inverseVarianceMatrix1, const float tx1, const float ty1,
		                         const MyDataTypes::SymmetricMatrix3f inverseVarianceMatrix2, const float tx2, const float ty2) {

	// the 2x2 row-major inverste variance matrices
	double invV1[4],
		   invV2[4];

	// matrix elements
	double v1,
		   v2,
		   v3,
		   v4;

	// intermediate row-major 3x3 matrices
	double tmpV1[9],
		   tmpV2[9],
		   S[9];

	// intermediate results
	double a,
	       b,
	       factor,
	       determinant;

	invV1[0] = inverseVarianceMatrix1.a;
	invV1[1] = inverseVarianceMatrix1.b;
	invV1[2] = inverseVarianceMatrix1.b;
	invV1[3] = inverseVarianceMatrix1.c;

	invV2[0] = inverseVarianceMatrix2.a;
	invV2[1] = inverseVarianceMatrix2.b;
	invV2[2] = inverseVarianceMatrix2.b;
	invV2[3] = inverseVarianceMatrix2.c;

	// *************************
	// first intermediate result
	// *************************

	// elements of the first inverse variance matrix
	v1 = invV1[0];
	v2 = invV1[1];
	v3 = invV1[2];
	v4 = invV1[3];

	// first row
	tmpV1[0] = v1;
	tmpV1[1] = v2;
	// tmpV1[2] = -tx1 * v1 - ty1 * v2;

	// second row
	tmpV1[3] = v3;
	tmpV1[4] = v4;
	// tmpV1[5] = -tx1 * v3 - ty1 * v4;

	// third row
	tmpV1[6] = -tx1 * v1 - ty1 * v3;
	tmpV1[7] = -tx1 * v2 - ty1 * v4;
	tmpV1[8] =  tx1 * tx1 * v1 + tx1 * ty1 * v2 + tx1 * ty1 * v3 + ty1 * ty1 * v4;

	// **************************
	// second intermediate result
	// **************************

	// elements of the second inverse variance matrix
	v1 = invV2[0];
	v2 = invV2[1];
	v3 = invV2[2];
	v4 = invV2[3];

	// first row
	tmpV2[0] = v1;
	tmpV2[1] = v2;
	// tmpV2[2] = -tx2 * v1 - ty1 * v2;

	// second row
	tmpV2[3] = v3;
	tmpV2[4] = v4;
	// tmpV2[5] = -tx2 * v3 - ty2 * v4;

	// third row
	tmpV2[6] = -tx2 * v1 - ty2 * v3;
	tmpV2[7] = -tx2 * v2 - ty2 * v4;
	tmpV2[8] =  tx2 * tx2 * v1 + tx2 * ty2 * v2 + tx2 * ty2 * v3 + ty2 * ty2 * v4;

	// *******
	// combine
	// *******

	// S = tmpV1 + tmpV2
	S[0] = tmpV1[0] + tmpV2[0];
	S[1] = tmpV1[1] + tmpV2[1];

	S[3] = tmpV1[3] + tmpV2[3];
	S[4] = tmpV1[4] + tmpV2[4];

	S[6] = tmpV1[6] + tmpV2[6];
	S[7] = tmpV1[7] + tmpV2[7];
	S[8] = tmpV1[8] + tmpV2[8];

	// f =(S(3,3)*S(1,2)^2 - 2*S(2,3)*S(1,3)*S(1,2) + S(1,1)*S(2,3)^2 - S(3,3)*S(1,1)*S(2,2) + S(1,3)^2*S(2,2)) / (-S(1,1)*S(2,2)+S(1,2)^2)

	a = S[8] * S[3] * S[3] 
		+ S[0] * S[7] * S[7] 
		+ S[6] * S[6] * S[4]
		- 2.0f * S[7] * S[6] * S[3]		
		- S[8] * S[0] * S[4];

	b = -S[0] * S[4] + S[3] * S[3];

	factor = a / b;

	// overwrite tmpV1 and tmpV2
	tmpV1[0] = S[0];
	tmpV1[1] = S[1];
	tmpV1[2] = S[3];
	tmpV1[3] = S[4];

	// intermediate result is now in tmpV2, the inverse of
	// submatrix of S
	MtrInverse2x2d (tmpV1, tmpV2);

	determinant = tmpV2[0] * tmpV2[3] - tmpV2[1] * tmpV2[2];

	a = sqrt (determinant) * exp (-0.5 * factor) * sqrt (fabs (invV1[0] * invV1[3] - invV1[1] * invV1[2]));
	b = 2.0 * M_PI;		

	return (float)(a / b);

}

float MyMathTools::euklideanDistance3D (const Vector3D vector1, const Vector3D vector2) {

	float x,
		  y,
		  z;
	
	x = vector1[0] - vector2[0];
	y = vector1[1] - vector2[1];
	z = vector1[2] - vector2[2];

	return (float)sqrt (x * x + y * y + z * z);

}

float MyMathTools::squaredEuklideanDistance3D (const Vector3D vector1, const Vector3D vector2) {

	float x,
		  y,
		  z;
	
	x = vector1[0] - vector2[0];
	y = vector1[1] - vector2[1];
	z = vector1[2] - vector2[2];

	return (x * x + y * y + z * z);

}

float MyMathTools::euklideanDistance2D (const Vector2D vector1, const Vector2D vector2) {

	float x,
		  y;
	
	x = vector1[0] - vector2[0];
	y = vector1[1] - vector2[1];

	return (float)sqrt (x * x + y * y);

}

float MyMathTools::squaredEuklideanDistance2D (const Vector2D vector1, const Vector2D vector2) {

	float x,
		  y;
	
	x = vector1[0] - vector2[0];
	y = vector1[1] - vector2[1];

	return (x * x + y * y);

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
