// Title:   AssociatedSurfels.h
// Created: Thu Sep 25 14:30:16 2003
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

#ifndef __SURFELINTERFACE_H_
	#include "../../Interfaces/src/SurfelInterface.h"
#endif

#ifndef __ASSOCIATEDSURFELS_H_
#define __ASSOCIATEDSURFELS_H_

class SurfelInterface;

#define NUM_ASSOCIATED_SURFELS 4

/**
 * Helper class for storing/managing the associated surfels
 *
 * @author Richard Keiser
 * @version 2.0
 */
class AssociatedSurfels {
	
public:

	/**
	 * initialize the associated surfels to zero
	 */
	inline void init();

	/**
	 * removes all associated surfels and frees memory
	 */
	inline void destroy();

	/**
	 * return number of associated surfels
	 *
	 * @return the number of associated surfels
	 */
	inline unsigned int getNOfAssociatedSurfels();

	/**
	 * returns true if associated surfels are stored
	 *
	 * @return true if associated surfels are stored
	 */
	inline bool hasAssociatedSurfels();

	/**
	 * returns an associated surfel at <code>index</code>
	 *
	 * @param index
	 *		  index of the associated surfel (e.g. index = 0 -> first associated surfel)
	 * @return the associated surfel
	 */
	inline SurfelInterface* getAssociatedSurfel(unsigned int index);

	/**
	 * adds an associated surfel at the end
	 *
	 * @param surfel
	 *		  the surfel to add
	 * @return true if adding successful
	 */
	inline bool addAssociatedSurfel(SurfelInterface *surfel);
	/**
	 * removes all associated surfels and frees memory
	 */
    inline void clearAssociatedSurfels();

	/**
	 * copies and returns the associated surfels and clears its own list of assicated surfels
	 *
	 * @return a copy of <code>AssociatedSurfels</code>
	 */
	inline AssociatedSurfels detach();

protected:
	SurfelInterface **assocSurfels;

};

inline AssociatedSurfels AssociatedSurfels::detach() {
	AssociatedSurfels assocSurfels;
	assocSurfels.assocSurfels = this->assocSurfels;
	this->assocSurfels = NULL;
	return assocSurfels;
}

inline void AssociatedSurfels::init() {
	assocSurfels = NULL;
}

inline void AssociatedSurfels::destroy() {
	if (assocSurfels) {
		delete[] assocSurfels;
	}
}

inline unsigned int AssociatedSurfels::getNOfAssociatedSurfels() {
	if (assocSurfels == 0) {
		return 0;
	}
	unsigned int i = 1;
	while (assocSurfels[i] != 0) {
		i++;
		if (i >= NUM_ASSOCIATED_SURFELS) {
			return i;
		}
	}
	return i;
}

inline bool AssociatedSurfels::hasAssociatedSurfels() {
	return assocSurfels != NULL;
}

inline SurfelInterface* AssociatedSurfels::getAssociatedSurfel(unsigned int index) {
	if (!assocSurfels || index >= NUM_ASSOCIATED_SURFELS ) {
		return 0;
	}
	else {
		return assocSurfels[index];
	}
}

inline bool AssociatedSurfels::addAssociatedSurfel(SurfelInterface *surfel) {
	unsigned int i;

	if (assocSurfels == 0) {
		assocSurfels = new SurfelInterface*[NUM_ASSOCIATED_SURFELS];
		assocSurfels[0] = surfel;

		for (i = 1; i < NUM_ASSOCIATED_SURFELS; i++) {
			assocSurfels[i] = 0;
		}
		return true;
	}

	i = 0;
	while (assocSurfels[i] != 0) {
		
		// check if surfel is already in array of associated surfels
		if (assocSurfels[i] == surfel) {
			return true;
		}
		i++;
		if (i >= NUM_ASSOCIATED_SURFELS) {
			return false;
		}
	}
	assocSurfels[i] = surfel;
	return true;
}

inline void AssociatedSurfels::clearAssociatedSurfels() {
	if (assocSurfels) {
		delete[] assocSurfels;	
		assocSurfels = NULL;
	}
}
	
#endif  // __ASSOCIATEDSURFELS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
