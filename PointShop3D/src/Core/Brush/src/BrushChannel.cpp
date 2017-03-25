// Title:   BrushChannel.cpp
// Created: Thu Sep 25 14:23:26 2003
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

#include "BrushChannel.h"

#include <qimage.h>
#include <qdir.h>

// **************
// public methods
// **************


ChannelComponent::ChannelComponent(QSize s) {

	width = s.width();
	height = s. height();
	nValues = width * height;

	values = new float[nValues];
}


ChannelComponent::~ChannelComponent() {

	delete[] values;
}


BrushChannel::BrushChannel(QSize s, int n, float scale, const QImage *newBitmap) {


	setComponents(s, n, scale, newBitmap);
}


BrushChannel::BrushChannel() {

	bitmap = 0;
	components = 0;
}


BrushChannel::BrushChannel(const BrushChannel* src) {

	setComponents(src->size, src->nComponents, src->scale, src->bitmap);
}


BrushChannel::~BrushChannel() {
	
	clearComponents();

	if(bitmap != 0) {
		delete bitmap;
	}
}


int BrushChannel::load (FILE *file, QString currentDirectory, int nComponents, QSize size) {

	char bitmapFileName[512];
	QImage fileBitmap;
	QString filePath;

	// read bitmap for channel
	fscanf(file, "%s", bitmapFileName);
	filePath = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + bitmapFileName;
	if( fileBitmap.load(filePath) == false) {
		return -1;
	}

	clearComponents();
	setComponents(size, nComponents, 1.f, &fileBitmap);

	return 1;
}



int BrushChannel::save (FILE *file, QString directory, QString channelFileName) {

	QString filePath;

	// write bitmap for channel
	fprintf(file, "%s", channelFileName.latin1());

	filePath = QDir::convertSeparators (directory) + QDir::convertSeparators ("/") + channelFileName;

	if( bitmap->save (filePath, "BMP") == false) {
		return -1;
	}

	return 1;
}




void BrushChannel::setComponents(QSize s, int n, float scale, const QImage *newBitmap) {

	int i, j;
	int height, width;
	float f;
	QRgb color = qRgb(0,0,0);
	QImage scaledBitmap;

	// set channel status 
	this->scale = scale;

	size = s;
	height = size.height();
	width = size.width();

	// save a copy of the bitmap
	bitmap = new QImage();
	(*bitmap) = newBitmap->copy();

	// scale bitmap to width and height of the brush
	scaledBitmap = newBitmap->smoothScale (width, height);


	// allocate component arrays
	nComponents = n;
	components = new ChannelComponent*[nComponents];

	for(i=0; i<nComponents; i++) {
		components[i] = new ChannelComponent(s);
	}

	// initialize component arrays with bitmap data
	// NOTE: we handle only 8 and 32 bpp
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++) {

			// get color from bitmap
			if (scaledBitmap.depth() == 8) {
				color = scaledBitmap.color (*(scaledBitmap.scanLine (i) + j));
			} else 
			if (scaledBitmap.depth() == 32) {
				color = *((QRgb *)(scaledBitmap.scanLine (i)) + j);
			}

			if(nComponents == 1 || nComponents == 2) {
				// convert rgb to greyscale	
				f = (0.3f * (float)qRed(color) + 0.59f * (float)qGreen(color) + 0.11f * (float)qBlue(color)) / 255.f;
				// write data to first component
				components[0]->setValue(j, i, f);
				// in case nComponents==2, the second component is not initialized. it is provided 
				// for user specific, temporary use only (e.g., as a displacement map used for carving).

			} 
			else if(nComponents == 3) {
				// set rgb components
				components[0]->setValue(j, i, (float)qRed(color)/255.f);
				components[1]->setValue(j, i, (float)qGreen(color)/255.f);
				components[2]->setValue(j, i, (float)qBlue(color)/255.f);
			}
		}
	}
}


void BrushChannel::clearComponents() {

	int i;

	if(components != 0) {
		for( i=0; i<nComponents; i++ ) {
			delete components[i];
		}

		delete[] components;
	}
}


const QImage* BrushChannel::getBitmap() {

	return bitmap;
}


int BrushChannel::getNOfComponents() {

	return nComponents;
}


ChannelComponent* BrushChannel::getComponent(int n) {

	if(0 <= n && n < nComponents) {
		return components[n];
	} else {
		return 0;
	}
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
