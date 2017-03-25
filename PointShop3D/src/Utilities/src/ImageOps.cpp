// Title:   ImageOps.cpp
// Created: Thu Sep 25 15:01:49 2003
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

#include <qimage.h>
#include <qpainter.h>
#include <qpixmap.h>

#include "ImageOps.h"

QImage *ImageOps::combineImages (const QImage *image32, const QImage *grayscale8) {

	QImage   brushAlphaImage;
	QImage   *newImage32;
	int      x,
		     y;
	uchar    *alphaSource;
	uchar    *textureSource,
		     *textureDestination;
	uchar    alpha;

	newImage32 = new QImage (image32->size(), 32);

	// make the grayscale8 the same size like the image32
	brushAlphaImage = grayscale8->smoothScale (image32->width(), image32->height());
	
	// construct the new texture brush by combining the texture image and the alpha channel
	for (y = 0; y < image32->height(); y++) {

		for (x = 0; x < image32->width(); x++) {

			textureSource      = (uchar *)((uint *)image32->scanLine(y) + x);
			//alphaSource        = (uint *)brushAlphaImage.scanLine(y) + x;
			alphaSource        = (uchar *)brushAlphaImage.scanLine(y) + x;
			textureDestination = (uchar *)((uint *)newImage32->scanLine(y) + x);
			
			// red
			*textureDestination++ = *textureSource++;
			// green
			*textureDestination++ = *textureSource++;
			// blue
			*textureDestination++ = *textureSource++;
			// alpha
			alpha = qBlue (brushAlphaImage.color (*alphaSource++));
			*textureDestination++ = alpha;
			// skip the alpha channel of the source texture image
			textureSource++;

		}

	}

	return newImage32;

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
