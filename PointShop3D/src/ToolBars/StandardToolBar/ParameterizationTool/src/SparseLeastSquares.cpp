// Title:   SparseLeastSquares.cpp
// Created: Thu Sep 25 14:58:11 2003
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

#include "SparseLeastSquares.h"
#include <stdio.h>
//#include <qdatetime.h>
#include "../../../../Utilities/src/Common.h"


SparseLeastSquares::SparseLeastSquares(int n) {

	int i;

	// set the number of unknowns in the system
	nUnknowns = n;

	// init column indices and matrix element value arrays
	colIndices = new std::vector<int>[n];
	values = new std::vector<float>[n];
	for(i=0; i<n; i++) {
		colIndices[i].resize(0);
		values[i].resize(0);
	}

	// init number of columns (nCols)
	nCols.resize(n);
	for(i=0; i<n; i++) {
		nCols[i] = 0;
	}

	// init right hand side vector
	rightHandSide.resize(n);
	for(i=0; i<n; i++) {
		rightHandSide[i] = 0.f;
	}
}


SparseLeastSquares::~SparseLeastSquares() {
	delete[] colIndices;
	delete[] values;
}


void SparseLeastSquares::addConstraint(float b, std::vector<int>& I, std::vector<float> a, bool addFlag) {

	int i, j;
	int n = I.size();

	for(i=0; i<n; i++) {

		// add contributions to sparse matrix
		for(j=0; j<n; j++) {

			if(addFlag) {
				// add contribution
				addContributionToMatrixElement(I[i], I[j], a[i]*a[j]);
			} else {
				// subtract contribution
				addContributionToMatrixElement(I[i], I[j], -a[i]*a[j]);
			}
		}

		// add contribution to right hand side
		// NOTE: the sign of the contribution. this is consistent with "Constrained 
		// Texture Mapping", Levy, SIGGRAPH 2001 (Section 4)
		if(addFlag) {
			// add contribution
			addContributionToRightHand(I[i], -a[i]*b);
		} else {
			// subtract contribution
			addContributionToRightHand(I[i], a[i]*b);
		}
	}
}


void SparseLeastSquares::addContributionToMatrixElement(int i, int j, float c) {

	int k;

	// look for column index j in row i
	k = 0;
	while(k<nCols[i] && colIndices[i][k]!=j) {
		k++;
	}

	if(k==nCols[i]) {
		// column index j was not found in row i, hence add it
		nCols[i]++;
		colIndices[i].resize(nCols[i]);
		colIndices[i][k] = j;

		// init new matrix element
		values[i].resize(nCols[i]);
		values[i][k] = 0.f;
	}

	// add contribution to matrix element
	values[i][k] += c;
}


void SparseLeastSquares::addContributionToRightHand(int i, float c) {

	rightHandSide[i] += c;
}

/**
 * Do it yourself: Hestenes-Stiefel conjugate gradient method as described in
 * "Constrained Texture Mapping", Levy, SIGGRAPH 2001 (*)
 *
 * The vector x must contain an initial solution. The final solution
 * will be established in the same vector x.
 * NOTE: this algorithm minimizes || Gx + c ||, not || Gx - c || !
 */
int SparseLeastSquares::solve(float *x, const unsigned int size, float epsilon) {

	(void)size;

	double *g, *r, *p, *tmp;
	double *d_x, *d_rightHandSide;
	double t, tau, sigma, rho, gamma, threshold;

	int its = 0;
	int i;

	dRowCompMatrix M;

	g = new double[nUnknowns];
	r = new double[nUnknowns];
	p = new double[nUnknowns];
	tmp = new double[nUnknowns];

	// copy data to double arrays
	this->copyToDoubleArray(M);
	d_x = new double[nUnknowns];
	d_rightHandSide = new double[nUnknowns];
	for(i=0; i<nUnknowns; i++) {
		d_x[i] = x[i];
		d_rightHandSide[i] = rightHandSide[i];
	}

	// solve linear equations
	threshold = epsilon*epsilon * innerProduct(nUnknowns, d_rightHandSide, d_rightHandSide);

	// in (*): g = -(G*x + c) 
	matrixVectorProduct(M, d_x, g);
	addVectors(nUnknowns, g, d_rightHandSide, g);
	vectorScalarProduct(nUnknowns, g, -1.f, g);

	for(i=0; i<nUnknowns; i++) {
		r[i] = g[i];
	}
/*
	QTime timer;
	timer.start();
	qDebug("SparseLeastSquares::solve: start solving with %i unknowns", nUnknowns);
//*/
	float debug = innerProduct(nUnknowns, g, g);
//	qDebug("SparseLeastSquares::solve: initial error is %f", debug);

	while( (debug = innerProduct(nUnknowns, g, g)) > threshold) {

		// in (*): p = G*r
		matrixVectorProduct(M, r, p);
		rho = innerProduct(nUnknowns, p, p);
		sigma = innerProduct(nUnknowns, r, p);
		tau = innerProduct(nUnknowns, g, r);
		t = tau/sigma;
		// in (*): x = x + t*r
		vectorScalarProduct(nUnknowns, r, t, tmp);
		addVectors(nUnknowns, d_x, tmp, d_x);
		// in (*): g = g - t*p
		vectorScalarProduct(nUnknowns, p, -t, tmp);
		addVectors(nUnknowns, g, tmp, g);
		gamma = (t*t * rho - tau) / tau;
		// in (*): r = gamma*r + g
		vectorScalarProduct(nUnknowns, r, gamma, tmp);
		addVectors(nUnknowns, tmp, g, r);

		its++;
	}

//	qDebug("SparseLeastSquares::solve: reached residuum %f in %i iterations in %i milliseconds", debug, its, timer.elapsed());

	// write result 
	for(i=0; i<nUnknowns; i++) {
		x[i] = d_x[i];
	}

	// clean up
	delete g;
	delete r;
	delete p;
	delete tmp;

	delete d_x;
	delete d_rightHandSide;

	delete M.nCols;
	delete M.values;
	delete M.startRow;
	delete M.colIndices;

	return its; 

}


float SparseLeastSquares::innerProduct(std::vector<float> &a, std::vector<float> &b) {

	uint   i;
	float r;

	// NOTE: vectors MUST have same length, otherwise result is not correct
	// or program crashes, since vector lengths are not checked.
	r = 0;
	for (i = 0; i < a.size(); i++) {
		r += a.at(i) * b.at(i);
	}

	return r;
}


void SparseLeastSquares::matrixVectorProduct (std::vector<float> &b, std::vector<float> &r) {

	int i, j;
	float s;

	// compute a matrix vector product. use the sparse representation of the system 
	// matrix and multiply it with the given vector b.
	for (i = 0; i < nUnknowns; i++) {

		s = 0.f;
		for(j=0; j<nCols[i]; j++) {

			s += values[i].at(j) * b[colIndices[i].at(j)];
		}

		r[i] = s;
	}

}


void SparseLeastSquares::addVectors(std::vector<float> &a, std::vector<float> &b, std::vector<float> &r) {

	uint i;

	// NOTE: vectors MUST have same length, otherwise result is not correct
	// or program crashes, since vector lengths are not checked.
	for (i = 0; i < a.size(); i++) {
		r.at(i) = a.at(i) + b.at(i);
	}
}


void SparseLeastSquares::vectorScalarProduct(std::vector<float> &a, float s, std::vector<float> &r) {

	uint i;

	// NOTE: vectors MUST have same length, otherwise result is not correct
	// or program crashes, since vector lengths are not checked.
	for (i = 0; i < a.size(); i++) {
		r.at(i) = s * a.at(i);
	}

}

int SparseLeastSquares::copyToDoubleArray(dRowCompMatrix& m) {

	int nonZeroElements;
	int i, j, k;

	m.n = nUnknowns;
	m.nCols = new int[nUnknowns];
	m.startRow = new int[nUnknowns];

	nonZeroElements = 0;
	for(i=0; i<nUnknowns; i++) {
		nonZeroElements += nCols[i];
	}

	m.values = new double[nonZeroElements];
	m.colIndices = new int[nonZeroElements];

	k = 0;
	for(i=0; i<nUnknowns; i++) {
		m.startRow[i] = k;
		m.nCols[i] = nCols[i];

		for(j=0; j<nCols[i]; j++) {
			m.values[k] = values[i].at(j);
			m.colIndices[k] = colIndices[i].at(j);
			k++;
		}
	}

	return nonZeroElements;
}

float SparseLeastSquares::innerProduct(int n, double* a, double* b) {

	int i;
	float r;

	// NOTE: vectors MUST have same length, otherwise result is not correct
	// or program crashes, since vector lengths are not checked.
	r = 0;
	for(i=0; i<n; i++) {
		r += a[i]*b[i];
	}

	return r;
}

void SparseLeastSquares::matrixVectorProduct(const dRowCompMatrix& m, double* b, double* r) {

	int i, j, k;
	float s;

	double* dValues = m.values;
	int* dColIndices = m.colIndices;
	int* dNCols = m.nCols; 
		
	// compute a matrix vector product. use the sparse representation of the system 
	// matrix and multiply it with the given vector b.
	k = 0;
	for(i=0; i<nUnknowns; i++) {

		s = 0.f;
		for(j=0; j<dNCols[i]; j++) {

			s += dValues[k] * b[dColIndices[k]];
			k++;
		}

		r[i] = s;
	}

}

void SparseLeastSquares::addVectors(int n, double* a, double* b, double* r) {

	int i;

	// NOTE: vectors MUST have same length, otherwise result is not correct
	// or program crashes, since vector lengths are not checked.
	for(i=0; i<n; i++) {
		r[i] = a[i]+b[i];
	}
}

void SparseLeastSquares::vectorScalarProduct(int n, double* a, double s, double* r) {

	int i;

	for(i=0; i<n; i++) {
		r[i] = a[i] * s;
	}
}


void SparseLeastSquares::printFullMatrix() {

	float* fullMatrix = new float[nUnknowns*nUnknowns];
	int i, j;

	// init full matrix
	for(i=0; i<nUnknowns*nUnknowns; i++) {
		fullMatrix[i] = 0;
	}

	// add non-zero values
	for(i=0; i<nUnknowns; i++) {
		for(j=0; j<nCols[i]; j++) {

			 fullMatrix[i*nUnknowns + colIndices[i].at(j)] = values[i].at(j);
		}
	}

	for(i=0; i<nUnknowns; i++) {
		for(j=0; j<nUnknowns; j++) {

			 printf(" %.3f ", fullMatrix[i*nUnknowns + j]);
		}
		printf("\n");
	}

	printf("\n");

	for(i=0; i<nUnknowns; i++) {
		printf(" %.3f ", rightHandSide[i]);
	}
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
