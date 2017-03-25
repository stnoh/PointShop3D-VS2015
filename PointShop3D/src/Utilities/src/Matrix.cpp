// Title:   Matrix.cpp
// Created: Thu Sep 25 15:01:44 2003
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
// Matrix.c
//-------------------------------------------------------------------------

// Standard Includes
#include <assert.h>
#include <math.h>

// Project Includes
#include "../../DataTypes/src/MyDataTypes.h"
#include "../../DataTypes/src/Vector3D.h"
#include "Matrix.h"

#define SMALL_NUMBER 1.e-8

void MtrUnity4x4f(float *m)
{
	int i;

	for(i=0; i<16; i++)
	{
		m[i] = 0.0f;
	}
	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10] = 1.0f;
	m[15] = 1.0f;
}

void MtrUnity4x4d(double *m)
{
	int i;

	for(i=0; i<16; i++)
	{
		m[i] = 0.0;
	}
	m[0] = 1.0;
	m[5] = 1.0;
	m[10] = 1.0;
	m[15] = 1.0;
}


void MtrCopy4x4f(const float *s, float *d)
{
	memcpy (d, s, sizeof(float)*16);
}

void MtrCopy4x4d(const double *s, double *d)
{
	memcpy (d, s, sizeof(double)*16);
}

void MtrCopy3x3f(const float *s, float *d)
{
	memcpy (d, s, sizeof(float)*9);
}

void MtrCopy3x3d(const double *s, double *d)
{
	memcpy (d, s, sizeof(double)*9);
}

void MtrTranspose4x4f(const float *s, float *d)
{
	int i;

	for(i=0; i<16; i++)
		d[(i%4)*4+(i/4)] = s[i];
}

void MtrTranspose4x4d(const double *s, double *d)
{
	int i;

	for(i=0; i<16; i++)
		d[(i%4)*4+(i/4)] = s[i];
}

void MtrTranspose3x3f(const float *s, float *d)
{
	int i;

	for(i=0; i<9; i++)
		d[(i%3)*3+(i/3)] = s[i];
}

void MtrTranspose3x3d(const double *s, double *d)
{
	int i;

	for(i=0; i<9; i++)
		d[(i%3)*3+(i/3)] = s[i];
}

void MtrMultScal3x3f(const float *s, const float c, float *r)
{
	int i;

	for(i=0; i<9; i++)
		r[i] = s[i]*c;
}

void MtrMultScal3x3d(const double *s, const double c, double *r)
{
	int i;

	for(i=0; i<9; i++)
		r[i] = s[i]*c;
}

void MtrMultVector3f(const float *a, const float *v, float *r)
{
	int i, j;
	float t;

	for(i=0; i<3; i++)
	{
		t = 0;

		for(j=0; j<3; j++)
		{
			t += a[i + 3 * j] * v[j];
		}

		r[i] = t;
	}			
}

void MtrMultVector3D (const MyDataTypes::TransformationMatrix16f M, const Vector3D v, Vector3D *r) {

	int   i,
		  j;
	float t;

	for (i = 0; i < 3; i++) {
		
		t = 0;
		for (j = 0; j < 3; j++) {
			t += M[i + 4 * j] * v[j];
		}

		(*r)[i] = t;

	}		

}


void MtrMultVector3DHom (const MyDataTypes::TransformationMatrix16f M, const Vector3D v, Vector3D *r) {

	int   i,
		  j;
	float t;

	for (i = 0; i < 3; i++) {
		
		t = 0;
		for (j = 0; j < 3; j++) {
			t += M[i + 4 * j] * v[j];
		}

		(*r)[i] = t + M[i + 12];
	}		

}




void MtrLocalToGlobalTransform (Vector3D position, Vector3D normal, float *matrix) {

	Vector3D u, v;
	normal.getTwoOrthogonals (u, v);
	u.normalize();
	v.normalize();

	// column 1
	matrix[0]  = u[0];
	matrix[1]  = u[1];
	matrix[2]  = u[2];
	matrix[3]  = 0.0f;

	// column 2
	matrix[4]  = v[0];
	matrix[5]  = v[1];
	matrix[6]  = v[2];
	matrix[7]  = 0.0f;

	// column 3
	matrix[8]  = normal[0];
	matrix[9]  = normal[1];
	matrix[10] = normal[2];
	matrix[11] = 0.0f;

	// column 4
	matrix[12] = position[0];
	matrix[13] = position[1];
	matrix[14] = position[2];
	matrix[15] = 1.0f;
}


void MtrGlobalToLocalTransform (Vector3D position, Vector3D normal, float *matrix) {

	float temp[16];

	MtrLocalToGlobalTransform (position, normal, temp);
	MtrInverse4x4f (temp, matrix);

}






void MtrMultVector4f(const float *a, const float *v, float *r)
{
	int i, j;
	float t;

	for(i=0; i<4; i++)
	{
		t = 0;

		for(j=0; j<4; j++)
		{
			t += a[i + 4 * j] * v[j];
		}

		r[i] = t;
	}			
}

void MtrMult4x4f(const float *a, const float *b, float *r)
{
	int i, j, k;
	float t;

	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			t = 0;
			for(k=0; k<4; k++)
			{
				t += a[k*4 + i] * b[k + j*4];
			}
			r[j*4 + i] = t;
		}
	}
}

void MtrMult4x4d(const double *a, const double *b, double *r)
{
	int i, j, k;
	double t;

	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			t = 0;
			for(k=0; k<4; k++)
			{
				t += a[k*4 + i] * b[k + j*4];
			}
			r[j*4 + i] = t;
		}
	}
}

void MtrMult3x3f(const float *a, const float *b, float *r)
{
	int i, j, k;
	float t;

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			t = 0;
			for(k=0; k<3; k++)
			{
				t += a[k*3 + i] * b[k + j*3];
			}
			r[j*3 + i] = t;
		}
	}
}

void MtrMult3x3d(const double *a, const double *b, double *r)
{
	int i, j, k;
	double t;

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			t = 0;
			for(k=0; k<3; k++)
			{
				t += a[k*3 + i] * b[k + j*3];
			}
			r[j*3 + i] = t;
		}
	}
}

bool MtrInverse4x4f(const float *m, float *r)
{
	int i;
    float det;

    MtrAdjoint4x4f(m, r);

    det = MtrDeterminant4x4f(m);
    if(fabs(det) < SMALL_NUMBER) return false; 

    for (i=0; i<16; i++)
		r[i] = r[i] / det;

	return true;
}


bool MtrInverse4x4d(const double *m, double *r)
{
	int i;
    double det;

    MtrAdjoint4x4d(m, r);

    det = MtrDeterminant4x4d(m);

    if(fabs(det) < SMALL_NUMBER) return false; 

    for (i=0; i<16; i++)
		r[i] = r[i] / det;

	return true;
}

bool MtrInverse3x3f(const float *m, float *r)
{
	int i;
    float det;

    MtrAdjoint3x3f(m, r);

    det = MtrDeterminant3x3f(m[0], m[1], m[2],
							m[3], m[4], m[5],
							m[6], m[7], m[8]);

	if(fabs(det) < SMALL_NUMBER) return false; 

    for (i=0; i<9; i++)
		r[i] = r[i] / det;

	return true;
}

bool MtrInverse3x3d(const double *m, double *r)
{
	int i;
    double det;

    MtrAdjoint3x3d(m, r);

    det = MtrDeterminant3x3d(m[0], m[1], m[2],
							m[3], m[4], m[5],
							m[6], m[7], m[8]);

	if(fabs(det) < SMALL_NUMBER) return false; 

    for (i=0; i<9; i++)
		r[i] = r[i] / det;

	return true;
}

bool MtrInverse2x2f (const float *m, float *r)
{
	float determinant;

	determinant = m[0] * m[3] - m[1] * m[2];

	if(fabs(determinant) < SMALL_NUMBER) return false; 

	r[0] =  m[3] / determinant;
	r[1] = -m[1] / determinant;
	r[2] = -m[2] / determinant;
	r[3] =  m[0] / determinant;

	return true;
}

bool MtrInverse2x2d (const double *m, double *r)
{
	double determinant;

	determinant = m[0] * m[3] - m[1] * m[2];

	if(fabs(determinant) < SMALL_NUMBER) return false; 

	r[0] =  m[3] / determinant;
	r[1] = -m[1] / determinant;
	r[2] = -m[2] / determinant;
	r[3] =  m[0] / determinant;

	return true;
}

void MtrAdjoint4x4f(const float *m, float *r)
{
	float a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = m[ 0]; b1 = m[ 1]; c1 = m[ 2]; d1 = m[ 3];
	a2 = m[ 4]; b2 = m[ 5]; c2 = m[ 6]; d2 = m[ 7];
	a3 = m[ 8]; b3 = m[ 9]; c3 = m[10]; d3 = m[11];
	a4 = m[12]; b4 = m[13]; c4 = m[14]; d4 = m[15];

	// column 1
    r[0]  =   MtrDeterminant3x3f( b2, c2, d2, b3, c3, d3, b4, c4, d4);
    r[4]  = - MtrDeterminant3x3f( a2, c2, d2, a3, c3, d3, a4, c4, d4);
    r[8]  =   MtrDeterminant3x3f( a2, b2, d2, a3, b3, d3, a4, b4, d4);
    r[12]  = - MtrDeterminant3x3f( a2, b2, c2, a3, b3, c3, a4, b4, c4);
        
	// column 2
    r[1]  = - MtrDeterminant3x3f( b1, c1, d1, b3, c3, d3, b4, c4, d4);
    r[5]  =   MtrDeterminant3x3f( a1, c1, d1, a3, c3, d3, a4, c4, d4);
    r[9]  = - MtrDeterminant3x3f( a1, b1, d1, a3, b3, d3, a4, b4, d4);
    r[13]  =   MtrDeterminant3x3f( a1, b1, c1, a3, b3, c3, a4, b4, c4);
        
	// column 3
    r[2]  =   MtrDeterminant3x3f( b1, c1, d1, b2, c2, d2, b4, c4, d4);
    r[6]  = - MtrDeterminant3x3f( a1, c1, d1, a2, c2, d2, a4, c4, d4);
    r[10]  =   MtrDeterminant3x3f( a1, b1, d1, a2, b2, d2, a4, b4, d4);
    r[14]  = - MtrDeterminant3x3f( a1, b1, c1, a2, b2, c2, a4, b4, c4);
        
	// column 4
    r[3]  = - MtrDeterminant3x3f( b1, c1, d1, b2, c2, d2, b3, c3, d3);
    r[7]  =   MtrDeterminant3x3f( a1, c1, d1, a2, c2, d2, a3, c3, d3);
    r[11]  = - MtrDeterminant3x3f( a1, b1, d1, a2, b2, d2, a3, b3, d3);
    r[15]  =   MtrDeterminant3x3f( a1, b1, c1, a2, b2, c2, a3, b3, c3);
}

void MtrAdjoint4x4d(const double *m, double *r)
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = m[ 0]; b1 = m[ 1]; c1 = m[ 2]; d1 = m[ 3];
	a2 = m[ 4]; b2 = m[ 5]; c2 = m[ 6]; d2 = m[ 7];
	a3 = m[ 8]; b3 = m[ 9]; c3 = m[10]; d3 = m[11];
	a4 = m[12]; b4 = m[13]; c4 = m[14]; d4 = m[15];

	// column 1
    r[0]  =   MtrDeterminant3x3d( b2, c2, d2, b3, c3, d3, b4, c4, d4);
    r[4]  = - MtrDeterminant3x3d( a2, c2, d2, a3, c3, d3, a4, c4, d4);
    r[8]  =   MtrDeterminant3x3d( a2, b2, d2, a3, b3, d3, a4, b4, d4);
    r[12]  = - MtrDeterminant3x3d( a2, b2, c2, a3, b3, c3, a4, b4, c4);
        
	// column 2
    r[1]  = - MtrDeterminant3x3d( b1, c1, d1, b3, c3, d3, b4, c4, d4);
    r[5]  =   MtrDeterminant3x3d( a1, c1, d1, a3, c3, d3, a4, c4, d4);
    r[9]  = - MtrDeterminant3x3d( a1, b1, d1, a3, b3, d3, a4, b4, d4);
    r[13]  =   MtrDeterminant3x3d( a1, b1, c1, a3, b3, c3, a4, b4, c4);
        
	// column 3
    r[2]  =   MtrDeterminant3x3d( b1, c1, d1, b2, c2, d2, b4, c4, d4);
    r[6]  = - MtrDeterminant3x3d( a1, c1, d1, a2, c2, d2, a4, c4, d4);
    r[10]  =   MtrDeterminant3x3d( a1, b1, d1, a2, b2, d2, a4, b4, d4);
    r[14]  = - MtrDeterminant3x3d( a1, b1, c1, a2, b2, c2, a4, b4, c4);
        
	// column 4
    r[3]  = - MtrDeterminant3x3d( b1, c1, d1, b2, c2, d2, b3, c3, d3);
    r[7]  =   MtrDeterminant3x3d( a1, c1, d1, a2, c2, d2, a3, c3, d3);
    r[11]  = - MtrDeterminant3x3d( a1, b1, d1, a2, b2, d2, a3, b3, d3);
    r[15]  =   MtrDeterminant3x3d( a1, b1, c1, a2, b2, c2, a3, b3, c3);
}

void MtrAdjoint3x3f(const float *m, float *r)
{
	float a1, a2, a3, b1, b2, b3, c1, c2, c3;

	a1 = m[0]; b1 = m[1]; c1 = m[2];
	a2 = m[3]; b2 = m[4]; c2 = m[5];
	a3 = m[6]; b3 = m[7]; c3 = m[8];

	// column 1
    r[0]  =   MtrDeterminant2x2f( b2, c2, b3, c3);
    r[3]  = - MtrDeterminant2x2f( a2, c2, a3, c3);
    r[6]  =   MtrDeterminant2x2f( a2, b2, a3, b3);
        
	// column 2
    r[1]  = - MtrDeterminant2x2f( b1, c1, b3, c3);
    r[4]  =   MtrDeterminant2x2f( a1, c1, a3, c3);
    r[7]  = - MtrDeterminant2x2f( a1, b1, a3, b3);
        
	// column 3
    r[2]  =   MtrDeterminant2x2f( b1, c1, b2, c2);
    r[5]  = - MtrDeterminant2x2f( a1, c1, a2, c2);
    r[8] =   MtrDeterminant2x2f( a1, b1, a2, b2);
}

void MtrAdjoint3x3d(const double *m, double *r)
{
	double a1, a2, a3, b1, b2, b3, c1, c2, c3;

	a1 = m[0]; b1 = m[1]; c1 = m[2];
	a2 = m[3]; b2 = m[4]; c2 = m[5];
	a3 = m[6]; b3 = m[7]; c3 = m[8];

	// column 1
    r[0]  =   MtrDeterminant2x2d( b2, c2, b3, c3);
    r[3]  = - MtrDeterminant2x2d( a2, c2, a3, c3);
    r[6]  =   MtrDeterminant2x2d( a2, b2, a3, b3);
        
	// column 2
    r[1]  = - MtrDeterminant2x2d( b1, c1, b3, c3);
    r[4]  =   MtrDeterminant2x2d( a1, c1, a3, c3);
    r[7]  = - MtrDeterminant2x2d( a1, b1, a3, b3);
        
	// column 3
    r[2]  =   MtrDeterminant2x2d( b1, c1, b2, c2);
    r[5]  = - MtrDeterminant2x2d( a1, c1, a2, c2);
    r[8] =   MtrDeterminant2x2d( a1, b1, a2, b2);
}

float MtrDeterminant4x4f(const float *m)
{
	float ans;
    float a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = m[ 0]; b1 = m[ 1]; c1 = m[ 2]; d1 = m[ 3];
	a2 = m[ 4]; b2 = m[ 5]; c2 = m[ 6]; d2 = m[ 7];
	a3 = m[ 8]; b3 = m[ 9]; c3 = m[10]; d3 = m[11];
	a4 = m[12]; b4 = m[13]; c4 = m[14]; d4 = m[15];

    ans = a1 * MtrDeterminant3x3f( b2, c2, d2, b3, c3, d3, b4, c4, d4)
        - b1 * MtrDeterminant3x3f( a2, c2, d2, a3, c3, d3, a4, c4, d4)
        + c1 * MtrDeterminant3x3f( a2, b2, d2, a3, b3, d3, a4, b4, d4)
        - d1 * MtrDeterminant3x3f( a2, b2, c2, a3, b3, c3, a4, b4, c4);

    return ans;
}

double MtrDeterminant4x4d(const double *m)
{
	double ans;
    double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	a1 = m[ 0]; b1 = m[ 1]; c1 = m[ 2]; d1 = m[ 3];
	a2 = m[ 4]; b2 = m[ 5]; c2 = m[ 6]; d2 = m[ 7];
	a3 = m[ 8]; b3 = m[ 9]; c3 = m[10]; d3 = m[11];
	a4 = m[12]; b4 = m[13]; c4 = m[14]; d4 = m[15];

    ans = a1 * MtrDeterminant3x3d( b2, c2, d2, b3, c3, d3, b4, c4, d4)
        - b1 * MtrDeterminant3x3d( a2, c2, d2, a3, c3, d3, a4, c4, d4)
        + c1 * MtrDeterminant3x3d( a2, b2, d2, a3, b3, d3, a4, b4, d4)
        - d1 * MtrDeterminant3x3d( a2, b2, c2, a3, b3, c3, a4, b4, c4);

    return ans;
}

float MtrDeterminant3x3f(const float a1, const float b1, const float c1,
						 const float a2, const float b2, const float c2,
						 const float a3, const float b3, const float c3)
{
	float ans;
    
	ans = a1 * MtrDeterminant2x2f( b2, c2, b3, c3 )
        - b1 * MtrDeterminant2x2f( a2, c2, a3, c3 )
        + c1 * MtrDeterminant2x2f( a2, b2, a3, b3 );

    return ans;
}

double MtrDeterminant3x3d(const double a1, const double b1, const double c1,
						  const double a2, const double b2, const double c2,
						  const double a3, const double b3, const double c3)
{
	double ans;
    
	ans = a1 * MtrDeterminant2x2d( b2, c2, b3, c3 )
        - b1 * MtrDeterminant2x2d( a2, c2, a3, c3 )
        + c1 * MtrDeterminant2x2d( a2, b2, a3, b3 );

    return ans;
}

float MtrDeterminant2x2f(const float a, const float b, const float c, const float d)
{
	float ans;

    ans = a * d - b * c;

    return ans;
}


double MtrDeterminant2x2d(const double a, const double b, const double c, const double d)
{
	double ans;

    ans = a * d - b * c;

    return ans;
}


//------------------------------------------------------------------------
// operations on row major matrices
//------------------------------------------------------------------------

void MtrMult4x4fr(const float *a, const float *b, float *r)
{
	MtrMult4x4f(b, a, r);
}

void MtrMult4x4dr(const double *a, const double *b, double *r)
{
	MtrMult4x4d(b, a, r);
}

void MtrMult3x3fr(const float *a, const float *b, float *r)
{
	MtrMult3x3f(b, a, r);
}

void MtrMult3x3dr(const double *a, const double *b, double *r)
{
	MtrMult3x3d(b, a, r);
}

//------------------------------------------------------------------------
// operations which replace the corresponding OpenGL matrix operations
// NOTE: These functions expect ROW-MAJOR matrices!
// -----------------------------------------------------------------------

void MtrTranslate4x4fr (const float *m, const float x, const float y, const float z, float *r)
{
	float a, b, c; // helper variables
	int   i;       // and index

	for (i = 0; i < 3; i++) {

		// copy unmodified values from 'm' into a, b, c
		a = *m;
		m++;

		b = *m;
		m++;

		c = *m;
		m++;

		// write results into result matrix 'r'
		*r = a;
		r++;

		*r = b;
		r++;

		*r = c;
		r++;

		*r = a * x + b * y + c * z + *m;
		m++;
		r++;

	}

	// just copy the last row from 'm' into 'r'
	for (i = 0; i < 3; i++) {
		*r = *m;
		r++;
		m++;
	}

}

void MtrCreateTranslation4x4fr (const float x, const float y, const float z, float *t)
{
	float *tp;   // a pointer to the elements in t

	tp = t;

	*tp = 1.0f;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = x;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = 1.0f;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = y;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = 1.0f;
	tp++;

	*tp = z;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = 0.0f;
	tp++;

	*tp = 1.0f;
	tp++;

}

// implements the rotation matrix, as shown in OpenGL Programming Guide,
// Third Edition, p.672f
// ROW major order
// param:
//  angle: angle in radians
//  x, y, z: rotation axis
//  r: result 4x4 matrix, stored in row major order
void MtrCreateRotation4x4fr (const float angle, const float x, const float y, const float z, float *r)
{
	double norm;              // norm of the axis vector
	double xn, yn, zn;        // components from normalized axis vector
	double xnyn, xnzn, ynzn;  // xn * yn, xn * zn, yn * zn
	double cosa, sina;        // cos(angle), sin(angle)
	
	norm = sqrt (x * x + y * y + z * z);
	xn = x / norm;
	yn = y / norm;
	zn = z / norm;

	xnyn = xn * yn;
	xnzn = xn * zn;
	ynzn = yn * zn;

	cosa = cos (angle);
	sina = sin (angle);

	// m00
	*r++ = (float)(xn * xn + cosa * (1.0 - xn * xn));
	// m01
	*r++ = (float)(xnyn    - cosa * xnyn            + sina * (-zn));
	// m02
	*r++ = (float)(xnzn    - cosa * xnzn            + sina * yn);
	*r++ = 0.0f;

	// m10
	*r++ = (float)(xnyn    - cosa * xnyn            + sina * zn);
	// m11
	*r++ = (float)(yn * yn + cosa * (1.0 - yn * yn));
	// m12
	*r++ = (float)(ynzn    - cosa * ynzn            + sina * (-xn));
	*r++ = 0.0f;

	// m20
	*r++ = (float)(xnzn    - cosa * xnzn            + sina * (-yn));
	// m21
	*r++ = (float)(ynzn    - cosa * ynzn            + sina * xn);
	// m22
	*r++ = (float)(zn * zn + cosa * (1.0 - zn * zn));
	*r++ = 0.0f;

	// last row
	*r++ = 0.0;
	*r++ = 0.0;
	*r++ = 0.0;
	*r++ = 1.0;

}

// implements the rotation matrix, as shown in OpenGL Programming Guide,
// Third Edition, p.672f
// COLUMN major order (like in OpenGL)
// param:
//  angle: angle in radians
//  x, y, z: rotation axis
//  r: result 4x4 matrix, stored in column major order
void MtrCreateRotation4x4fc (const float angle, const float x, const float y, const float z, float *r)
{
	double norm;              // norm of the axis vector
	double xn, yn, zn;        // components from normalized axis vector
	double xnyn, xnzn, ynzn;  // xn * yn, xn * zn, yn * zn
	double cosa, sina;        // cos(angle), sin(angle)

	
	norm = sqrt (x * x + y * y + z * z);
	xn = x / norm;
	yn = y / norm;
	zn = z / norm;

	xnyn = xn * yn;
	xnzn = xn * zn;
	ynzn = yn * zn;

	cosa = cos (angle);
	sina = sin (angle);	

	// m00
	*r++ = (float)(xn * xn + cosa * (1.0 - xn * xn));
	// m01
	*r++ = (float)(xnyn    - cosa * xnyn            + sina * zn);
	// m02
	*r++ = (float)(xnzn    - cosa * xnzn            + sina * (-yn));
	*r++ = 0.0f;

	// m10	
	*r++ = (float)(xnyn    - cosa * xnyn            + sina * (-zn));
	// m11
	*r++ = (float)(yn * yn + cosa * (1.0 - yn * yn));
	// m12
	*r++ = (float)(ynzn    - cosa * ynzn            + sina * xn);
	*r++ = 0.0f;

	// m20	
	*r++ = (float)(xnzn    - cosa * xnzn            + sina * yn);
	// m21
	*r++ = (float)(ynzn    - cosa * ynzn            + sina * (-xn));
	// m22
	*r++ = (float)(zn * zn + cosa * (1.0 - zn * zn));
	*r++ = 0.0f;

	// last column
	*r++ = 0.0;
	*r++ = 0.0;
	*r++ = 0.0;
	*r++ = 1.0;

}


float MtrGetUniformScalingFactorSquared4x4f (const float *a) {
	return a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
}


float MtrGetUniformScalingFactor4x4f (const float *a) {
	return sqrt (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}





	



// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
