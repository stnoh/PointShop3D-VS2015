// Title:   BrushChannel.h
// Created: Thu Sep 25 14:31:34 2003
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

#ifndef __BRUSHCHANNEL_H_
#define __BRUSHCHANNEL_H_

#include <qsize.h>
#include <stdio.h>

#include "../../src/CoreDLL.h"


/**
 * Brush channels store the data for one surface property that is used when the surface
 * is edited, e.g., using the <code>BrushTool</code>. A channel has a certain number of
 * components, each component is a float array of data values.
 *
 * @author Matthias Zwicker
 * @version 1.2
 */

/**
 * Channel component: stores and provides access to a single float array of
 * data values.
 */
class ChannelComponent {

public:

	/**
	 * Instantiate a component with a given size.
	 */
	CORE_API ChannelComponent(QSize s);
	CORE_API virtual ~ChannelComponent();

	/**
	 * Get the number of data values in the component.
	 */
	CORE_API inline int getNOfValues() { return nValues; };

	/**
	 * Get a pointer to the float array of data values of the component.
	 */
	CORE_API inline float* getValues() { return values; };

	/**
	 * Get a data value from the component.
	 * Caution: no range checking on this access function!
	 */
	CORE_API inline float getValue(int x, int y) { return values[x + width * y]; };

	/**
	 * Set a data value in the component.
	 * Caution: no range checking on this access function!
	 */
	CORE_API inline void setValue(int x, int y, float f) { values[x + width * y] = f; };

private:

	int width, height;
	int nValues;

	float *values;

};


/**
 * Brush channel: manages a single channel of a brush. There is a one-to-one 
 * correspondence between brush channels and surfel properties.
 *
 * NOTE: a brush channel also stores the bitmap that was used to generate the channel.
 * this bitmap is used for displaying the brush in the brush builder.
 */
class BrushChannel {

public:

	/**
	 * Generate a brush channel from a bitmap. 
	 * 
	 * - If nComponents==1 or nComponents==2, the bitmap is converted to gray scale and 
	 * the scalars are assigned to the first channel component. In case nComponents==2, the second 
	 * channel component is not initialized, it is provided for user specific, temporary use. If 
	 * nComponents==3, the bitmap RGB channels are each written to a brush channel component.
	 * - All components are scaled to contain values in the range [0..1].
	 */
	CORE_API BrushChannel(QSize s, int nComponents, float scale, const QImage *bitmap);

	/**
	 * Default constructor.
	 */
	CORE_API BrushChannel();

	/**
	 * Construct a channel by copying the data from a given channel.
	 */
	CORE_API BrushChannel(const BrushChannel* src);

	CORE_API virtual ~BrushChannel();

	/**
	 * Load a brush channel from a .pbd (Pointshop3D brush descriptor) file.
	 */
	CORE_API int load (FILE *file, QString currentDirectory, int nComponents, QSize size);

	/**
	 * Save a brush channel to a .pbd (Pointshop3D brush descriptor) file.
	 */
	CORE_API int save (FILE *file, QString currentDirectory, QString channelFileName);

	/**
	 * Return the bitmap that is stored in the brush channel.
	 */
	CORE_API const QImage* getBitmap();

	/**
	 * Get the number of components in the brush channel.
	 */
	CORE_API int getNOfComponents();

	/**
	 * Get a channel component.
	 */
	CORE_API ChannelComponent* getComponent(int n);

private:

	void setComponents(QSize s, int n, float scale, const QImage *bitmap);
	void clearComponents();

	QSize size;

	float scale;					// scaling parameter

	int nComponents;
	ChannelComponent **components;
	QImage *bitmap;
	
};


#endif  // __BRUSHCHANNEL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
