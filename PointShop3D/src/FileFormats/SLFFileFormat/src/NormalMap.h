// Title:   NormalMap.h
// Created: Thu Sep 25 14:48:00 2003
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
// Normal map from the subdivision of a sphere
//
// NormalMap.h 
//-------------------------------------------------------------------------

#ifndef __NORMALMAP_H_
#define __NORMALMAP_H_

#include <qobject.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../DataTypes/src/Vector3D.h"

/**
 * This class implements a normal map from the subdivision of a sphere.
 *
 * @version 1.2
 * @author Oliver Knoll
 */
class NormalMap {

public:

	/**
	 * Creates this <code>NormalMap</code> for a sphere with <code>nofSubdivisions</code> subdivisions. The
	 * number of subdivisions influence the memory size of the map.
	 * 
	 * @param nofSufDivisions
	 *        the number of subdivisions for the sphere
	 */
	NormalMap (const int nofSubDivisions);
	virtual ~NormalMap();	

	/**
	 * Returns the index in the map for the <code>vector</code>
	 *
	 * @param vector
	 *        a <code>Vector3f</code> for which the index has to be returned
	 * @return the index in the map for the <code>vector</code>
	 */
	int getIndexFromVector (const Vector3D vector) const;

	/**
	 * Returns the main direction from the map entry identified by <code>index</code>, which
	 * is the biggest absolute component of the normal vector at this <code>index</code>.
	 *
	 * The returned value is encoded as follows:
	 * <ul>
	 *   <li>0: the x-component is the biggest absolute value</li>
	 *   <li>1: the y-component is the biggest absolute value</li>
	 *   <li>2: the z-component is the biggest absolute value</li>
	 * </ul>
	 * 
	 * @param index
	 *        the index into the normal map
	 * @return the main direction of the normal vector in the map entry <code>index</code>
	 */
	int getMainDirection (const int index) const;

	/**
	 * Returns the normal vector which corresponds to <code>index</code>.
	 *
	 * @param index
	 *        specifies the normal to be returned
	 * @return a <code>Vector3D</code> containing the normal vector
	 */
	Vector3D getNormalVector (const uint index) const;

	/**
	 * Returns the whole normal table.
	 *
	 * @return a pointer to the normal table which contains <code>Vector3D</coce> normals
	 * @see #getTableSize
	 */
	const Vector3D *getTable() const;

	/**
	 * Returns the size of the normal table.
	 *
	 * @return the size of the normal table
	 * @see #getTable
	 */
	uint getTableSize() const;

	/**
	 * Returns the number of subdivisions.
	 *
	 * @return the number of subdivisions
	 */
	int getNofSubdivisions() const;

private:

	int                   nofSubdivisions,
		                  rows;
	unsigned int          tableSize;
	Vector3D *table;
	unsigned int          *rowIndices;
	
	// Calculate normal vector for spherical triangle and set it in table
	void setNormalInTable (const int row, const int perquadr, const int inrow,
		                   const double *triLatt, const double *triLong);

};


#endif  // __NORMALMAP_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
