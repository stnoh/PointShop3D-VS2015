// Title:   SplatBuffer.h
// Created: Thu Sep 25 14:58:01 2003
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

#ifndef __SPLATBUFFER_H_
#define __SPLATBUFFER_H_

#include <qimage.h>
#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Core/Brush/src/Brush.h"

/**
 * This class implements splatting methods, writing the data to a general pixel buffer
 *
 * @author Matthias Zwicker
 * @version 1.2
 */
class SplatBuffer {

public:

	SplatBuffer (const QSize size, float coverageThreshold);
	virtual ~SplatBuffer();

	void clearBuffer();

	void rasterizeSurfel (SurfelInterface *surfel, MyDataTypes::TextureCoordinate uv, float J[4], float localKernel[4],
			      Vector3D localX, Vector3D localY, float lowPassFilter);
	void rasterizeSurfelNearestNeighbor (SurfelInterface *surfel, MyDataTypes::TextureCoordinate uv, float J[4], Vector3D localX, Vector3D localY);
	
	/**
	 * Adds the resampled texture to the active <code>Object</code>.
	 */
	bool addTextureToActiveObject (Brush* brush, float displacementScaling, bool useEllipticalSurfels);

private:

	typedef struct __BufferEntry {

		float           weight;					// the accumulated weight in this pixel
		Vector3D	    position;				// the 3D position of this pixel
		Vector3D	    normal;					// the accumulated normal in this pixel
		float		    radius;					// the accumulated radius in this pixel
		float		    r;						// the accumulated red color component in this pixel
		float		    g;						// the accumulated green color component in this pixel
		float		    b;						// the accumulated blue color component in this pixel
		float		    uv[2];
		SurfelInterface *surfel;                // a pointer to the last rasterized surfel as to copy
		                                        // its properties to the newly created surfel

	} BufferEntry;

	QSize			bufferSize;

	BufferEntry		*buffer;				// the per pixel data which holds pixel weight, normal, etc.

	float			cutoffRadius;			// cutoff radius for rasterizing splats
	float			coverageThreshold;		// coverage threshold indicating full surface coverage (used for resampling at texture resolution)

	float*		    weightTable;
	int				weightTableSize;

};


#endif  // __SPLATBUFFER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
