// Title:   SurfelGPFrameBuffer.cpp
// Created: Thu Sep 25 14:51:37 2003
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
#include <limits.h>
#include <float.h>

#include <qimage.h>
#include <qsize.h>
#include <qthread.h>

#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/Configuration/src/Configuration.h"
#include "../../../Core/Scene/src/Scene.h"
#include "../../../Utilities/src/Matrix.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../../DataTypes/src/Vector2D.h"
#include "SurfelPipeline_SurfSplat/ZBuffer_SurfSplat.h"
#include "SurfelGPFrameBuffer.h"
#include <vector>

// *********
// constants
// *********

// each time the (per pixel) visibleSplatIndices array is full, MORE_VISIBLE_ELEMENTS 
// are additionally allocated 
static const uint MORE_VISIBLE_ELEMENTS = 32;

static const uint MORE_SPLATS = 1024;

// **************
// public methods
// **************

SurfelGPFrameBuffer::SurfelGPFrameBuffer (const QSize size) {

	valid   = false;

	this->size = size;
	height_1 = size.height()-1;
	width = size.width();
	height = size.height();
	
	// allocate frame buffer and image, if needed
	buffer = 0;
	image  = 0;
	if (size.isNull() == false) {

		buffer = new BufferEntry[width * height];
		image = new QImage (size, 32);

	}
	else {
		buffer = 0;
		image = 0;
	}

	this->initBuffer();	

}

SurfelGPFrameBuffer::~SurfelGPFrameBuffer() {
	
	if (buffer != 0) {
		this->removeBuffer();
	}

	if (image != 0) {
		delete image;
	}

}

void SurfelGPFrameBuffer::setSurfelContext(SrfContext *srf) {
	srfFrameBuffer = SrfGetInternalFrameBuffer(srf);
}

void SurfelGPFrameBuffer::setAttributes (const int newAttributes) {
	attributes = newAttributes;
}

int SurfelGPFrameBuffer::getAttributes() const {
	return attributes;
}

void SurfelGPFrameBuffer::setValid (const bool valid) {
	
	// clear the pixel data buffer, if necessary
	if (this->valid == true && valid == false) {
		this->resetBuffer();
	}

	this->valid = valid;	

}

bool SurfelGPFrameBuffer::isValid() const {
	return valid;
}

void SurfelGPFrameBuffer::setSize (const QSize newSize) {

	if (size == newSize) {
		return;
	}

	// delete the old buffer first
	if (buffer != 0) {
		this->removeBuffer();
	}

	size = newSize;
	height_1 = size.height()-1;
	width = size.width();
	height = size.height();

	buffer = new BufferEntry[width * height];
	this->initBuffer();

	// resize the image as well, if necessary
	if (image != 0) {
		image->reset();
		image->create (size, 32);
	}
	else {
		image = new QImage (size, 32);
	}

	valid = false;
	
}

QSize SurfelGPFrameBuffer::getSize() const {
	return size;
}

const QImage *SurfelGPFrameBuffer::getImage() const {
	return image;
}

void SurfelGPFrameBuffer::setColor (const int x, const int y, const QRgb newPixelColor) {
	*((uint *)image->scanLine(y) + x) = newPixelColor;
}

QRgb SurfelGPFrameBuffer::getColor (const int x, const int y) const {
	return *((uint *)image->scanLine(y) + x);	
}

void SurfelGPFrameBuffer::clearImage (const QRgb clearColor) {
	image->fill (clearColor);
}

void SurfelGPFrameBuffer::resetPosition (const int x, const int y) {
	
	BufferEntry *bufferPosition;

	if (0 <= x && x < width && 0 <= y && y < height) {

		bufferPosition = buffer + y * width + x;
	
		bufferPosition->nofVisible = 0;
		if (bufferPosition->visibleSurfels != 0) {
			delete bufferPosition->visibleSurfels;
			bufferPosition->visibleSurfels = 0;
		}

	}

}

void SurfelGPFrameBuffer::addWeight (const int x, const int y, const float pixelWeightIncrement) {
	srfFrameBuffer[(height_1 - y) * width + x].w += pixelWeightIncrement;
}

void SurfelGPFrameBuffer::setWeight (const int x, const int y, const float newPixelWeight) {
	srfFrameBuffer[(height_1 - y) * width + x].w = newPixelWeight;
}

float SurfelGPFrameBuffer::getWeight (const int x, const int y) const {
	return srfFrameBuffer[(height_1 - y) * width + x].w;
}

void SurfelGPFrameBuffer::addVisibleSurfel (const int x, const int y, SurfelInterface *visibleSurfel) {

	BufferEntry *bufferPosition;
	uint        nofVisible;

	if (0 <= x && x < width && 0 <= y && y < height) {
	
		bufferPosition = buffer + y * width + x;
		nofVisible     = bufferPosition->nofVisible;

		// allocate the index array first, if necessary
		if (bufferPosition->visibleSurfels == 0) {
			bufferPosition->visibleSurfels = new std::vector<SurfelInterface *> (MORE_VISIBLE_ELEMENTS);
		}
		else if (nofVisible >= bufferPosition->visibleSurfels->size()) {
			// we have to allocate more memory first
			bufferPosition->visibleSurfels->resize (nofVisible + MORE_VISIBLE_ELEMENTS);
		}
		bufferPosition->visibleSurfels->at (nofVisible) = visibleSurfel;
		bufferPosition->nofVisible++;

	}

}

void SurfelGPFrameBuffer::getVisibleSurfels (const int x, const int y, const std::vector<SurfelInterface *> **visibleSurfels, uint *nofVisible) const {

	BufferEntry *bufferPosition;
	
	if (0 <= x && x < width && 0 <= y && y < height) {

		bufferPosition = buffer + y * width + x;

		*nofVisible     = bufferPosition->nofVisible;		
		*visibleSurfels = bufferPosition->visibleSurfels;		

	}
	else {
		*nofVisible     = 0;
		*visibleSurfels = 0;
	}

}

bool SurfelGPFrameBuffer::isPixelCovered (const int x, const int y) const {

	BufferEntry *bufferPosition;

	if (0 <= x && x < width && 0 <= y && y < height) {
		
		bufferPosition = buffer + y * width + x;

		if (srfFrameBuffer[(height_1 - y) * width + x].w == 0.0f) {
			return false;
		}
		else {
			return true;
		}

	}
	else {
		return false;
	}

}


bool SurfelGPFrameBuffer::allPixelsCovered () {

	int x, y;

	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			if (srfFrameBuffer[(height_1 - y) * width + x].w == 0.0f) {
				return false;
			}
		}
	}
	return true;
}

Object* SurfelGPFrameBuffer::getObjectAtPixel (const int x, const int y) const {

	BufferEntry* bufferPosition = buffer + y * width + x;

	return bufferPosition->object;
}

void SurfelGPFrameBuffer::setObjectAtPixel (const int x, const int y, Object* object) {

	BufferEntry* bufferPosition = buffer + y * width + x;
	
	bufferPosition->object = object;
}

void SurfelGPFrameBuffer::setDepth (const int x, const int y, const float z) {

	// flip z-axis to convert to left handed coordinate system (the surfel renderer uses
	// a left handed system)
	// note: de-normalize z-value, the surfel renderer frame buffer is de-normalized!
	srfFrameBuffer[(height_1 - y) * width + x].z = -z * srfFrameBuffer[(height_1 - y) * width + x].w;
}

float SurfelGPFrameBuffer::getDepth (const int x, const int y) const {

	// flip z-axis to convert to right handed coordinate system (the surfel renderer uses
	// a left handed system)
	// note: normalize z-value, the surfel renderer frame buffer is not normalized!
	return -srfFrameBuffer[(height_1 - y) * width + x].z / srfFrameBuffer[(height_1 - y) * width + x].w;
}


QImage *SurfelGPFrameBuffer::getZImage() const {

	int x, y;
	BufferEntry *bufferPosition;
	QImage *zImage = new QImage (size, 8, 256);
	float z;

	// find maximum and minimum
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;
	for (x = 0; x < width; x++) {

		for (y = 0; y < height; y++) {

			if (isPixelCovered (x, y)) {
				
				bufferPosition = buffer + y * width + x;
				
				z = -srfFrameBuffer[(height_1 - y) * width + x].z / srfFrameBuffer[(height_1 - y) * width + x].w;

				if(z < minZ) {
					minZ = z;
				}
				else if (z > maxZ) {
					maxZ = z;
				}
			}
		}
	}
	// init color table
	for (x = 0; x < 256; x++) {
		zImage->setColor (x, qRgb(x,x,x));
	} 

	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {

			bufferPosition = buffer + y * width + x;

			z = -srfFrameBuffer[(height_1 - y) * width + x].z / srfFrameBuffer[(height_1 - y) * width + x].w;

			if (isPixelCovered (x, y)) {
				*(zImage->scanLine(y) + x) = (unsigned char)(255.0f * (z - minZ) / (maxZ - minZ));
			}
			else {
				*(zImage->scanLine(y) + x) = 0;
			}
		}
	}

	return zImage;

}

void SurfelGPFrameBuffer::setNormal (const int x, const int y, const Vector3D newNormal) {

	// flip z-axis to convert to left handed coordinate system (the surfel renderer uses
	// a left handed system)
	float *n = srfFrameBuffer[(height_1 - y) * width + x].n;
	n[0] = newNormal[0];
	n[1] = newNormal[1];
	n[2] = -newNormal[2];
}

void SurfelGPFrameBuffer::setNormal (const int x, const int y, const float nx, const float ny, const float nz) {

	// flip z-axis to convert to left handed coordinate system (the surfel renderer uses
	// a left handed system)
	float *n = srfFrameBuffer[(height_1 - y) * width + x].n;
	n[0] = nx;
	n[1] = ny;
	n[2] = -nz;
}

Vector3D SurfelGPFrameBuffer::getNormal (const int x, const int y) const {
	
	// flip z-axis to convert to right handed coordinate system (the surfel renderer uses
	// a left handed system)
	// note: surfel frame buffer does not store normalized normals, therefore, normalize here!
	float *n = srfFrameBuffer[(height_1 - y) * width + x].n;
	float r = 1/(float)sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	return Vector3D(n[0]*r, n[1]*r, -n[2]*r);
}

SurfelInterface* SurfelGPFrameBuffer::getSurfel(const int x, const int y) {
	return (SurfelInterface*)srfFrameBuffer[(height_1 - y) * width + x].userData;
}

QList<SurfelInterface>* SurfelGPFrameBuffer::getAllVisibleSurfels() {

	uint                            i;
	QList<SurfelInterface>          *allSurfels;
	const std::vector<SurfelInterface *> *visibleSurfels;
	uint                            nofVisible;
    SurfelInterface  			    *surfel;
	int                             x,
		                            y;
	Scene                           *scene;


	allSurfels = new QList<SurfelInterface>();
	scene = Scene::getInstance();

	// mark splats
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			getVisibleSurfels (x, y,  &visibleSurfels, &nofVisible);

			for (i = 0; i < nofVisible; i++) {
				scene->setSurfelFlagOn (visibleSurfels->at(i), SurfelInterface::COVERED, false, false);
			}
		}
	}

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			getVisibleSurfels (x, y,  &visibleSurfels, &nofVisible);
			for (i = 0; i < nofVisible; i++) {
				
				surfel = visibleSurfels->at (i);
				
				if (surfel->isFlagOn (SurfelInterface::COVERED) == false) {
					
					// add the covered surfel to the selection
					allSurfels->append (surfel);
					// set the COVERED flag, so it won't be inserted the next time
					scene->setSurfelFlagOn (surfel, SurfelInterface::COVERED, false, false);
				}
			}

		}  // width

	}  // height

	return allSurfels;
	
}



void SurfelGPFrameBuffer::markAllSurfels() {

	uint                               i;
	const std::vector<SurfelInterface *>    *visibleSurfels;
	uint                               nofVisible;
	int                                x,
		                               y;


	// mark splats
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			getVisibleSurfels (x, y,  &visibleSurfels, &nofVisible);

			for (i = 0; i < nofVisible; i++) {
				Scene::getInstance()->setSurfelFlagOn (visibleSurfels->at(i), SurfelInterface::COVERED, true, false);
			}
		}
	}
}


float SurfelGPFrameBuffer::getMaxZValue() const {

	int i;
	float maxZValue = FLT_MIN;
	float z;

	for (i = 0; i < width * height; i++) {
		z = srfFrameBuffer[i].z / srfFrameBuffer[i].w;
		if (z > maxZValue) {
			maxZValue = z;
		}
	}

	return maxZValue;

}

float SurfelGPFrameBuffer::getMinZValue() const {

	int i;
	float minZValue = FLT_MAX;
	float z;

	for (i = 0; i < width * height; i++) {
		z = srfFrameBuffer[i].z / srfFrameBuffer[i].w;
		if (z < minZValue) {
			minZValue = z;
		}
	}
	return minZValue;

}

// ***************
// private methods
// ***************

void SurfelGPFrameBuffer::initBuffer() {	

	BufferEntry *entry;
	uint        length,
		        i;

	length = width * height;
	entry = buffer;
	for (i = 0; i < length; i++) {
		entry->nofVisible = 0;
		entry->visibleSurfels = 0;
		entry++;
	}

	nSplats = 0;
	surfels.clear();
	texCoords.clear();
	splats.clear();

	valid = false;
}

void SurfelGPFrameBuffer::resetBuffer() {	

	BufferEntry *entry;
	uint        length,
		        i;

	// only delete per pixel surfel lists if necessary
	if(attributes & FrameBufferInterface::PERPIXEL_SURFELLISTS) {
		
		length = width * height;
		entry = buffer;
		for (i = 0; i < length; i++) {
			entry->nofVisible = 0;
			if (entry->visibleSurfels != 0) {
				delete entry->visibleSurfels;
				entry->visibleSurfels = 0;
			}
			entry++;
		}

		// delete per pixel surfel lists from frame buffer attributes mask
		int mask = 0xffffffff ^ FrameBufferInterface::PERPIXEL_SURFELLISTS;
		attributes &= mask;
	}

	nSplats = 0;
	surfels.clear();
	texCoords.clear();
	splats.clear();

	valid = false;
}

void SurfelGPFrameBuffer::removeBuffer() {

	BufferEntry *entry;
	uint        length,
		        i;

	// remove dynamically allocated visible surfels arrays
	length = width * height;
	entry = buffer;
	for (i = 0; i < length; i++) {

		if (entry->visibleSurfels != 0) {
			delete entry->visibleSurfels;
		}
		entry++;

	}
	
	// finally remove the buffer
	delete[] buffer;
	buffer = 0;
	valid = false;

}



int SurfelGPFrameBuffer::getNofSplatInfoEntries() const {
	return nSplats;
}




std::vector<SurfelInterface *>* SurfelGPFrameBuffer::getSurfelsInViewFrustum() {
	return &surfels;
}

std::vector<MyDataTypes::TextureCoordinate>* SurfelGPFrameBuffer::getTextureCoordinatesInViewFrustum() {
	return &texCoords;
}

std::vector<MyDataTypes::Splat>* SurfelGPFrameBuffer::getSplatsInViewFrustum() {
	return &splats;
}


void SurfelGPFrameBuffer::setSplatInfo (SurfelInterface *surfel, const float x, const float y, const float z,
										const float a, const float b_2, const float c, const int bbox[4]) {

	MyDataTypes::TextureCoordinate texCoord;
	MyDataTypes::Splat splat;
	
	texCoord.u = x;
	texCoord.v = height - y;
	
	splat.conic[0] = a;
	splat.conic[1] = b_2;
	splat.conic[2] = b_2;
	splat.conic[3] = c;
	splat.z = z;
	splat.bbox[0] = bbox[0];
	splat.bbox[1] = bbox[1];
	splat.bbox[2] = bbox[2];
	splat.bbox[3] = bbox[3];

	surfels.push_back (surfel);
	texCoords.push_back(texCoord);
	splats.push_back(splat);

	nSplats++;

}





// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
