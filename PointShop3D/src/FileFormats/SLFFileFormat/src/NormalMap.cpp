// Title:   NormalMap.cpp
// Created: Thu Sep 25 14:48:03 2003
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
#include <assert.h>

#include "../../../Utilities/src/Common.h"
#include "NormalMap.h"

NormalMap::NormalMap (const int nofSubdivisions) {

	int    i, j, k;
	int    triPerRow;						// number of triangles per row
	double longStepP, longStepE;			// radial increments on row in longitude for pole and equator side
	double lattStep;						// radial increments in lattitude
	double triLong[3], triLatt[3];			// angles for current spherical triangle on hemisphere
	bool   flip;

	this->nofSubdivisions = nofSubdivisions;

	// calculate size of table, number of rows
	tableSize = 8;
	rows = 1;
	for(i = 0; i < nofSubdivisions; i++) {
		tableSize *= 4;
		rows      *= 2;
	}

	// allocate memory for row indices
	rowIndices = new unsigned int[2 * rows * sizeof(unsigned int)];

	// allocate memory for normal table 
	table = new Vector3D[tableSize * sizeof (Vector3D)];
	
	// calculate row indices for upper hemisphere
	rowIndices[0] = 0;
	triPerRow     = 4;
	for (i = 1; i < rows; i++) {
		rowIndices[i] = rowIndices[i-1] + triPerRow;
		triPerRow += 8;
	}
	rowIndices[rows] = rowIndices[rows-1] + triPerRow;

	// indices for lower hemisphere
	for(i = 1; i < rows; i++) {
		rowIndices[i + rows] = rowIndices[i + rows - 1] + triPerRow;
		triPerRow -=8;
	}

	// radial increment from one row to the next
	lattStep = M_PI / (2 * rows);

	// calculate normal at center of each spherical triangle

	// upper and lower pole
	triLatt[0] = 0.0; 
	triLong[0] = 0.0;
	triLatt[1] = triLatt[2] = lattStep;
	triLong[1] = 0.0;
	longStepE = (2 * M_PI) / 4.0;

	for(i = 0; i<4; i++) {

		triLong[2] = triLong[1] + longStepE;
		triLong[0] = (triLong[1] + triLong[2]) * 0.5;
		this->setNormalInTable (0, i, 0, triLatt, triLong);	// calculates index and normal vector for upper and lower hemisphere
		triLong[1] = triLong[2];

	}

	// rows between pole caps
	triPerRow = 3;										// this is now triangles per row and quadrant
	for (i = 1; i < rows; i++) {

		longStepP = (2 * M_PI) / (4.0 * (double)i);		// init longitude step on border of row towords pole
		longStepE = (2 * M_PI) / (4.0 * (double)(i+1));	// init longitude step on border of row towards equator

		for(k=0; k<4; k++)	{							// for all quadrants
		
			triLong[0] = (double)k * M_PI / 2.0;		// init first two points of current triangle
			triLatt[0] = (double)i * lattStep;
			triLong[1] = (double)k * M_PI / 2.0;
			triLatt[1] = (double)(i + 1) * lattStep;

			flip = true;								// flag indicating whether to step forward on border of row towards pole or equator
			for (j = 0; j < triPerRow; j++) { 
				
				if (flip == true) {

					flip = false;							// flip flag
					triLong[2] = triLong[1] + longStepE;	// step on border of row towards equator
					triLatt[2] = triLatt[1];
					this->setNormalInTable (i, k, j, triLatt, triLong);
					triLong[1] = triLong[2];
					triLatt[1] = triLatt[2];
				}
				else {

					flip = true;							// flip flag
					triLong[2] = triLong[0] + longStepP;	// step on border of row towards pole
					triLatt[2] = triLatt[0];
					this->setNormalInTable (i, k, j, triLatt, triLong);
					triLong[0] = triLong[2];
					triLatt[0] = triLatt[2];

				}

			}

		}

		triPerRow += 2;									// increment counter for triangles per row and quadrant
	}

}

NormalMap::~NormalMap() {
	
	delete[] table;
	delete[] rowIndices;
	
}

int NormalMap::getIndexFromVector (const Vector3D vector) const {

	double lattP, longP;					// lattitude and longitude of given normal vector
	double lattStep;						// radial increment of subdivision in lattitudal direction
	double longStepE, longStepP;			// radial increment of subdivision in longitudal direction, on border to pole and to equator of a row 
	double longCheck[2], lattCheck[2];		// points needed to check in which triangle the point lies
	double longDiff, lattDiff;				// vector that connects the check points in spherical coordinates
	double longN, lattN;					// normal vector in spherical coordinates
	double longQuad;						// zero-longitude for current quadrant
	double r;
	int    row, index, perrow, quad;		// row number, final index, triangles per row, current quadrant
	int    indP, indE;						// indices for the two possible triangles
	bool   lower;							// flag indicating point P lies on lower hemisphere
	double x,
		   y,
		   z;

	lower = false;

	// normalize the given normal vector
	r = vector.getLength();
	x = vector[0] / r;
	y = vector[1] / r;
	z = vector[2] / r;

	// calculate spherical coordinates (long/latt) of the point P corresponding to the given normal vector
	lattP = acos(z);
	r = sqrt (x * x + y * y);
	if (r < Constants::EPSILON2) {
		longP = 0.0;
	}
	else {

		longP = acos (x / r);
		if(y < 0.0) {
			longP = 2.0 * M_PI - longP;
		}

	}

	// calculate index for point P given in spherical coordinates (long/latt)

	// calculate row number
	lattStep = M_PI / (2.0 * rows);
	row = (int)floor (lattP / lattStep);
	// if in lower hemisphere, get the row number in the upper hemisphere
	// and flip lattitude to upper hemisphere
	if (row >= rows) {

		lower = true;
		row = 2 * rows - row - 1;
		lattP = M_PI - lattP;
	
	}
	
	if (row < 0) {	// might happen since my PI is not exact
		row = 0;
	}
	
	if (row > 0) {											// point is not within triangle fan at pole
	
		longStepP = (2.0 * M_PI) / (double)(row * 4);		// radial increment on row border towards pole			
		longStepE = (2.0 * M_PI) / (double)((row + 1) * 4);	// radial increment on row border towards equator
		perrow = row * 2 + 1;

		quad = (int)floor (longP / (M_PI / 2.0));			// current quadrant
		if (quad == 4) {
			quad = 0;
		}
		longQuad = quad * M_PI / 2;							// zero-longitude of quad
	
		indP = (int)floor ((longP - longQuad) / longStepP);	// possible row index for triangle with side on border to pole
		indE = (int)floor ((longP - longQuad) / longStepE);	// possible row index for triangle with side on border to equator

		// check in which of the two possible triangles the point P actually lies
		// use projection onto normal vector of common edge to determine on which side the point P lies

		// get long/latt coordinates of vertices of common edge (check points) of two possible triangles
		assert (indP <= indE);
		if(indP == indE) {
		
			longCheck[0] = longQuad + indP * longStepP;
			lattCheck[0] = row * lattStep;
			longCheck[1] = longQuad + (indE + 1) * longStepE;
			lattCheck[1] = (row + 1) * lattStep;
		
		}
		else if (indP < indE) {

			longCheck[0] = longQuad + indE * longStepE;
			lattCheck[0] = (row + 1) * lattStep;
			longCheck[1] = longQuad + (indP + 1) * longStepP;
			lattCheck[1] = row * lattStep;

		} 

		// calculate a vector from the first check point to the point P
		longDiff = longP - longCheck[0];
		lattDiff = lattP - lattCheck[0];
	
		// calculate the normal vector to the common edge given by the check points
		longN =   lattCheck[1] - lattCheck[0];
		lattN = -(longCheck[1] - longCheck[0]);

		if(longN * longDiff + lattN * lattDiff > 0) {		// scalar product to project onto normal vector
			index = quad * perrow + indP * 2 + 1;			// point lies on pole side triangle
		}
		else {
			index = quad * perrow + indE * 2;				// point lies on equator side triangle
		}

	}
	else {							                        // point is within triangle fan at pole
		longStepP = (2 * M_PI) / 4.0;     					// calculate radial increment in longitudal direciton
		index = (int)floor (longP / longStepP);				// get the index for point P
	}

	if (lower == true) {									// point P was originally in lower hemisphere
		row = 2 * rows - row - 1;						    // recalculate original row number
	}

	return rowIndices[row] + index;
	
}

int NormalMap::getMainDirection (const int index) const {

	float x,
		  y,
		  z;

	x = table[index][0];
	y = table[index][1];
	z = table[index][2];

	// calculate absolute values of coordinates
	if (x < 0) {
		x = -x;
	}
	if (y < 0) {
		y = -y;
	}
	if (z < 0){
		z = -z;
	}

	// find biggest coordinate
	if (x > y) {

		if (x > z) {
			// x is the biggest coordinate
			return 0;
		}
		else {
			// z is the biggest coordinate
			return 2;
		}

	}
	else if (y > z) {
		// y is the biggest coordinate
		return 1;
	} 
	else {
		// z is the biggest coordinate
		return 2;
	}

}

Vector3D NormalMap::getNormalVector (const uint index) const {
	return table[index];
}

const Vector3D *NormalMap::getTable() const {
	return table;
}

uint NormalMap::getTableSize() const {
	return tableSize;
}


int NormalMap::getNofSubdivisions() const {
	return nofSubdivisions;
}


// ***************
// private methods
// ***************

void NormalMap::setNormalInTable (const int row, const int quadr, const int inrow,
								  const double *triLatt, const double *triLong) {

	double cLatt,
	       cLong;			// lattitude and longitude of center of triangle
	double t;
	int    perquadr,
		   index;

	t = 1.0 / 3.0;

	// number of triangles on row in one quadrant
	perquadr = 2 * row + 1;

	// upper hemisphere
	cLatt = t * triLatt[0] + t * triLatt[1] + t * triLatt[2];
	cLong = t * triLong[0] + t * triLong[1] + t * triLong[2];
	
	index = rowIndices[row] + quadr * perquadr + inrow;
	table[index][0] = (float) (cos (cLong) * sin (cLatt));
	table[index][1] = (float) (sin (cLong) * sin (cLatt));
	table[index][2] = (float)  cos (cLatt);

	// lower hemisphere
	cLatt = M_PI - cLatt;

	index = rowIndices[2 * rows - row - 1] + quadr * perquadr + inrow;
	table[index][0] = (float) (cos (cLong) * sin (cLatt));
	table[index][1] = (float) (sin (cLong) * sin (cLatt));
	table[index][2] = (float)  cos (cLatt);

}

	

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
