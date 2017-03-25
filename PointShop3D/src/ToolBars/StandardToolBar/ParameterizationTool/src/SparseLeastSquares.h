// Title:   SparseLeastSquares.h
// Created: Thu Sep 25 14:58:08 2003
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

/**
 * Sparse least squares class
 *
 * Solves a least squares optimization problem. The optimization problem
 * is given by an arbitrary set of linear constraints which are added to 
 * the system one by one. Internally, the constraints are accumulated into 
 * a sparse representation of the least squares matrix. A conjugate gradient
 * algorithm is provided to solve the system.
 *
 * @author Matthias Zwicker
 * @version 1.2
 */

#ifndef __SPARSELEASTSQUARES_H_
#define __SPARSELEASTSQUARES_H_

#include <vector>


struct dRowCompMatrix {
	double* values;
	int* colIndices;
	int* nCols;
	int* startRow;
	int n;
};


class SparseLeastSquares {

public:

	SparseLeastSquares (int n);
	virtual ~SparseLeastSquares();

	/**
	 * Add a linear constraint to the system. The constraint is given as
	 * sum[i]( a[I[i]] * x[I[i]] ) = b, where I is a set of indices, a are
	 * the coefficients of the constraint, x are the variables in the system,
	 * and b is the right hand side.
	 */
	void addConstraint(float b, std::vector<int>& I, std::vector<float> a, bool addFlag = true);

	/**
	 * Solve the least squares optimization problem. The parameter x must contain
	 * an initial solution, it will then be filled with the final solution.
	 * epsilon is a threshold for the precision, 10 is a good value.
	 */
	int solve (float *x, const unsigned int size, const float epsilon);

	/**
	 * prints the matrix (e.g. for debug purposes)
	 */
	void printFullMatrix();

private:

	int nUnknowns;

	void addContributionToMatrixElement(int i, int j, float c);
	void addContributionToRightHand(int i, float c);

	// the row compressed sparse matrix data structure
	std::vector<int>* colIndices;
	std::vector<float>* values;
	std::vector<int> nCols;

	// the right hand side vector
	std::vector<float> rightHandSide;

	// utility methods for the conjugate gradient method which is usedto solve 
	// the linear equation system
	float innerProduct(std::vector<float> &a, std::vector<float> &b);
	void matrixVectorProduct(std::vector<float> &b, std::vector<float> &r);
	void addVectors(std::vector<float> &a, std::vector<float> &b, std::vector<float> &r);
	void vectorScalarProduct(std::vector<float> &a, float s, std::vector<float> &r);

	int copyToDoubleArray(dRowCompMatrix& m);
	inline float innerProduct(int n, double* a, double* b);
	inline void matrixVectorProduct(const dRowCompMatrix& m, double* b, double* r);
	inline void addVectors(int n, double* a, double* b, double* r);
	inline void vectorScalarProduct(int n, double* a, double s, double* r);

};

#endif  // __SPARSELEASTSQUARES_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
