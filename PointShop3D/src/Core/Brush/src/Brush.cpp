// Title:   Brush.cpp
// Created: Thu Sep 25 14:23:31 2003
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

#include <stdio.h>
#include <qdir.h>
#include <qimage.h>
#include <qregexp.h>

#include "../../../Utilities/src/FileTools.h"
#include "Brush.h"


const char Brush::PBD_FILE_ID[9]     = "PS3DBDFT";

// **************
// public methods
// **************

Brush::Brush (const QString brushName, const QSize brushSize) {

	initBrush(brushName, brushSize);
}

Brush::Brush() {
	 
	channels = 0;
	alphaChannels = 0;
	properties = 0;
	popupMenu = 0;

	directory = QString("");
}



Brush::Brush (const Brush *src) {

	initBrush(src->name, src->size);
	this->directory = src->directory;
	this->popupMenu = src->popupMenu;

	// copy channels
	for(unsigned int i = 0; i < nChannels; i++) {
		if(src->channels[i] != 0) {
			channels[i] = new BrushChannel(src->channels[i]);
		}

		if(src->alphaChannels[i] != 0) {
			alphaChannels[i] = new BrushChannel(src->alphaChannels[i]);
		}
	}
}

Brush::~Brush() {
	
	if(channels != 0) {
		removeAllChannels();
		delete[] channels;
	}

	if(alphaChannels != 0) {
		removeAllAlphaChannels();
		delete[] alphaChannels;
	}

	if(properties != 0) {
		delete[] properties;
	}
}

void Brush::removeAllChannels() {

	if(channels != 0) {
		for(unsigned int i = 0; i < nChannels; i++) {
			if( channels[i] != 0 ) {
				delete channels[i];
				channels[i] = 0;
			}
		}
	}
}

void Brush::removeAllAlphaChannels() {

	if(alphaChannels != 0) {
		for(unsigned int i = 0; i < nChannels; i++) {
			if( alphaChannels[i] != 0 ) {
				delete alphaChannels[i];
				alphaChannels[i] = 0;
			}
		}
	}
}


int Brush::load (const QString currentDirectory, const QString currentFileName) {

	FILE *file;
	char fileId[9];
	int width, height;
	unsigned int i, index;
	unsigned int nUsedChannels;
	char propertyName[128];
	int nComponents;
	QStringList editableProperties;
	SurfelInterface::Property surfelProperty = SurfelInterface::NO_PROPERTIES;
	QString filePath;

	// set brush name and directory
	name = FileTools::getFileNameWithoutSuffix(currentFileName);
	directory = currentDirectory;

	// construct complete file name (including path)
	filePath = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/") + currentFileName.latin1();

	if( (file = fopen(filePath.latin1(), "r")) == 0)
		return -1;

	// read .pbd file type identifier
	fread(fileId, 8, 1, file);
	fileId[8] = (char)0;

	// check file type
	if( QString(fileId) != QString(PBD_FILE_ID) ) {
		return -1;
	}

	// read brush size
	fscanf(file, "%i", &width);
	fscanf(file, "%i", &height);
	size.setWidth(width);
	size.setHeight(height);

	// allocate and initialize array for brush (and brush alpha) channels 
	nChannels = SurfelInterface::getNumOfEditableProperties();
	channels = new BrushChannel*[nChannels];
	alphaChannels = new BrushChannel*[nChannels];
	for(i = 0; i < nChannels; i++) {
		channels[i] = 0;
		alphaChannels[i] = 0;
	}
	
	// allocate and initialize array for corresponding property identifiers
	properties = new SurfelInterface::Property[nChannels];
	editableProperties = SurfelInterface::getEditableProperties();
	QStringList::Iterator it = editableProperties.begin();
	for (i = 0; i < nChannels; i++) {
		// translate property strings to property identifiers
		properties[i] = SurfelInterface::getSystemProperty ( (*it) );
		++it;
	}

	// read number of used channels
	fscanf(file, "%i", &nUsedChannels);

	// read channels
	for(i=0; i<nUsedChannels; i++) {

		// read property
		fscanf(file, "%i", (int*)&surfelProperty);
		// get internal index of current surfel property, abort if there is no channel
		// for the given property id
		if( (int)(index = getChannelIndex(surfelProperty)) == -1) {
			return -1;
		}

		// get number of channel components for current property
		nComponents = SurfelInterface::getNumOfPropertyBrushComponents(surfelProperty);

		// load channel
		channels[index] = new BrushChannel();
		if( (channels[index]->load(file, currentDirectory, nComponents, size)) == -1) {	
			delete channels[index];
			channels[index] = 0;
		}


		// load alpha channel
		fscanf(file, "%s", propertyName);
		if(QString(propertyName) == QString("alpha")) {

			alphaChannels[index] = new BrushChannel();
			if( (alphaChannels[index]->load(file, currentDirectory, 1, size)) == -1) {
				delete alphaChannels[index];
				alphaChannels[index] = 0;
			}
		}

	}

	fclose(file);

	return 1;
}


int Brush::save (const QString currentDirectory) {

	FILE *file;
	unsigned int i;
	QString filePath;
	QString pbdFileName;
	QString channelFileName;
	QString nameWithoutSpaces;

	// construct complete file name (including path)
	// NOTE: if the brush does not contain its own path, use the directory given as argument
	// to this method
	if(directory.compare(QString("")) == 0) {
		filePath = QDir::convertSeparators (currentDirectory) + QDir::convertSeparators ("/");
	}
	else {
		filePath = QDir::convertSeparators (directory) + QDir::convertSeparators ("/");
	}
	pbdFileName = filePath + getName().latin1() + QString(".pbd");

	if( (file = fopen(pbdFileName.latin1(), "w")) == 0) {
		return -1;
	}

	// remove spaces from the brush name for generating file names for the brush channel bitmaps,
	// since the reader cannot handle spaces
	nameWithoutSpaces = getName();
	nameWithoutSpaces.replace(QRegExp(" "), "_");

	// write .pbd file type identifier
	fwrite (QString (PBD_FILE_ID).latin1(), 8, 1, file);	

	// write brush size
	fprintf (file, "\n%d ", size.width());
	fprintf (file, "%d\n", size.height());

	// write number of used channels
	fprintf (file, "%d\n", getNumOfUsedChannels());

	// write channels
	for (i = 0; i < nChannels; i++) {

		if (channels[i] != 0) {
			
			// write property
			fprintf(file, "%d ", properties[i]);			
			
			// save channel
			channelFileName = nameWithoutSpaces + SurfelInterface::getUserReadableProperty (properties[i]) + ".bmp";

			if( (channels[i]->save (file, filePath, channelFileName)) == -1) {	
				fclose (file);
				return -1;
			}

			if (alphaChannels[i] != 0) {
				
				// save alpha channel
				fprintf(file, "%s", " alpha ");

				channelFileName = nameWithoutSpaces + SurfelInterface::getUserReadableProperty (properties[i]) + "alpha.bmp";

				if( (alphaChannels[i]->save(file, filePath, channelFileName)) == -1) {
					fclose (file);
					return -1;
				}
			}
			else {
				fprintf (file, "%s", " noalpha");
			}
			fprintf (file, "\n");

		}
	}

	fclose(file);

	return 1;

}

void Brush::setChannel (const SurfelInterface::Property surfelProperty, const float scale, const QImage *bitmap) {

	int channelIndex;
	int nComponents;

	// given a surfel property, get the index of the corresponding channel. if there is no channel
	// for the property (i.e., the property is not editable), return.
	if( (channelIndex = getChannelIndex(surfelProperty)) == -1) {
		return;
	}

	// if a channel for the given property already existed, destroy it
	if( channels[channelIndex] != 0 ) {

		delete channels[channelIndex];
		channels[channelIndex] = 0;
	}

	// given a surfel property for the new channel, get the number of components needed
	nComponents = SurfelInterface::getNumOfPropertyBrushComponents(surfelProperty);

	// instantiate new brush channel
	channels[channelIndex] = new BrushChannel(size, nComponents, scale, bitmap);

}


void Brush::setAlphaChannel (const SurfelInterface::Property surfelProperty, const QImage *bitmap) {

	int channelIndex;

	// given a surfel property, get the index of the corresponding channel. if there is no channel
	// for the property (i.e., the property is not editable), return.
	if( (channelIndex = getChannelIndex(surfelProperty)) == -1) {
		return;
	}

	// if an alpha channel for the given property already existed, destroy it
	if( alphaChannels[channelIndex] != 0 ) {

		delete alphaChannels[channelIndex];
		alphaChannels[channelIndex] = 0;
	}

	// instantiate new alpha channel (alpha channels always have one component, scaling
	// is always 1.f!)
	alphaChannels[channelIndex] = new BrushChannel(size, 1, 1.f, bitmap);

}


void Brush::removeChannel (const SurfelInterface::Property surfelProperty) {

	int channelIndex;

	if( (channelIndex = getChannelIndex(surfelProperty)) == -1) {
		return;
	}

	if( channels[channelIndex] != 0 ) {

		delete channels[channelIndex];
		channels[channelIndex] = 0;
	}
}


void Brush::removeAlphaChannel (const SurfelInterface::Property surfelProperty) {

	int channelIndex;

	if( (channelIndex = getChannelIndex(surfelProperty)) == -1) {
		return;
	}

	if( alphaChannels[channelIndex] != 0 ) {

		delete alphaChannels[channelIndex];
		alphaChannels[channelIndex] = 0;
	}
}


bool Brush::hasChannel (const SurfelInterface::Property surfelProperty) const {

	int channelIndex;

	// get channel index for desired property. return false if there is no channel 
	// for the property (i.e., the property is not editable)
	if( (channelIndex = getChannelIndex(surfelProperty)) == -1 ) {
		return false;
	}

	// return false if the channel is not allocated
	if( channels[channelIndex] == 0) {
		return false;
	} else {
		return true;
	}
}


bool Brush::hasAlphaChannel (const SurfelInterface::Property surfelProperty) const {

	int channelIndex;

	// get channel index for desired property. return false if there is no channel 
	// for the property (i.e., the property is not editable)
	if( (channelIndex = getChannelIndex(surfelProperty)) == -1 ) {
		return false;
	}

	// return false if the alpha channel is not allocated
	if( alphaChannels[channelIndex] == 0) {
		return false;
	} else {
		return true;
	}
}


BrushChannel* Brush::getChannel (const SurfelInterface::Property surfelProperty) const {

	int channelIndex;

	// get channel index for desired property. return 0 if there is no channel 
	// for the property (i.e., the property is not editable)
	if( (channelIndex = getChannelIndex(surfelProperty)) == -1 ) {
		return 0;
	} else {
		return channels[channelIndex];
	}
}


BrushChannel* Brush::getAlphaChannel (const SurfelInterface::Property surfelProperty) const {

	int channelIndex;

	// get channel index for desired property. return 0 if there is no channel 
	// for the property (i.e., the property is not editable)
	if( (channelIndex = getChannelIndex(surfelProperty)) == -1 ) {
		return 0;
	} else {
		return alphaChannels[channelIndex];
	}
}

QString Brush::getName() const {
	return name;
}

void Brush::setName (const QString name) {
	this->name = name;
}

QSize Brush::getSize() const {
	return size;
}

void Brush::setSize (const QSize size) {
	this->size = size;
}

int Brush::getChannelIndex (const SurfelInterface::Property surfelProperty) const {

	unsigned int i;

	// search channel containing the desired property
	i = 0;
	while(i < nChannels) {

		if(properties[i] == surfelProperty) {
			return i;
		} else {
			i++;
		}
	}

	// no channel with the desired property was found
	return -1;
}

void Brush::initBrush (const QString brushName, const QSize brushSize) {

	QStringList editableProperties;
	unsigned int i;

	popupMenu = 0;

	// set brush name and init directory
	name = brushName;
	directory = QString("");

	// set size
	size = brushSize;
	
	nChannels = SurfelInterface::getNumOfEditableProperties();

	// allocate and initialize array for brush (and brush alpha) channels 
	channels = new BrushChannel*[nChannels];
	alphaChannels = new BrushChannel*[nChannels];
	for(i = 0; i < nChannels; i++) {
		channels[i] = 0;
		alphaChannels[i] = 0;
	}
	
	// allocate and initialize array for corresponding property identifiers
	properties = new SurfelInterface::Property[nChannels];

	editableProperties = SurfelInterface::getEditableProperties();
	QStringList::Iterator it = editableProperties.begin();
	for (i = 0; i < nChannels; i++) {
		properties[i] = SurfelInterface::getSystemProperty ( (*it) );
		++it;
	}
}

QImage* Brush::createPreviewImage (unsigned int width, unsigned int height, bool useAlphaMasks) {

	QImage scaledPropertyImage;	
	QImage scaledAlphaImage;
	QImage *previewImage = new QImage (nChannels * width, height, 32);
	QRgb color;
	float alpha;

	unsigned int i, x, y;



	for (i = 0; i < nChannels; i++) {

		if (channels[i] != 0) {			
			scaledPropertyImage = channels[i]->getBitmap()->smoothScale (width, height);

			if (useAlphaMasks && (alphaChannels[i] != 0)) {
				scaledAlphaImage = alphaChannels[i]->getBitmap()->smoothScale (width, height);
			}
			
			for (x = 0; x < width; x++) {
				for (y = 0; y < height; y++) {					

					color = scaledPropertyImage.pixel (x, y);
					if (useAlphaMasks && (alphaChannels[i] != 0)) {
						alpha = (float) qRed (scaledAlphaImage.pixel (x, y)) / 255.0f;
						color = qRgb ((int)(alpha * (float) qRed (color)),
							      (int)(alpha * (float) qGreen (color)),
							      (int)(alpha * (float) qBlue (color)));
					}
					previewImage->setPixel (i*width + x, y, color);
				}
			}
		}
		else {
			for (x = 0; x < width; x++) {
				for (y = 0; y < height; y++) {					
					previewImage->setPixel (i*width + x, y, qRgb (0, 0, 0));
				}
			}
		}
	}
	return previewImage;
}

int Brush::getNumOfUsedChannels() const {

	unsigned int i;
	unsigned int numOfUsedChannels = 0;
	for (i = 0; i < nChannels; i++) {
		if (channels[i] != 0) {
			numOfUsedChannels++;
		}
	}

	return numOfUsedChannels;
}

int Brush::getMenuId() const {
	return menuId;
}

void Brush::setMenuId (const int newId) {
	menuId = newId;
}

void Brush::setPopupMenu(QPopupMenu *popupMenu) {
	this->popupMenu = popupMenu;
}

QPopupMenu* Brush::getPopupMenu() {
	return popupMenu;
}







// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
