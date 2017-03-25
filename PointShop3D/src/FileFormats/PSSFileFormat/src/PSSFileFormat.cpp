// Title:   PSSFileFormat.cpp
// Created: Thu Sep 25 14:47:25 2003
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

#include <string.h>

#include <qbuffer.h>
#include <qcstring.h>
#include <qdatastream.h>
#include <qfile.h>

#include "../../../Core/Scene/src/Scene.h"
#include "../../../Core/Scene/src/Object.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/RendererManager/src/RendererManager.h"
#include "../../../Core/Interfaces/src/FileFormatPluginInterface.h"
#include "../../../Utilities/UserMessages/src/StatusBar.h"

#include "PSSFileFormat.h"

const char *MAGIC = "PointShop3D Scene";

// **************
// public methods
// **************

PSSFileFormat::PSSFileFormat() {}

PSSFileFormat::~PSSFileFormat(){}

bool PSSFileFormat::hasWriteProtection() {
	return true;
}

bool PSSFileFormat::isReadable (const FileHeader fileHeader) {

	QByteArray byteArray (sizeof (FileHeader));
	uint       i;
	QString    magicHeader;
	char       *magic;
	
	// construct a QDataStream from the fileHeader
	for (i = 0; i < sizeof (FileHeader); i++) {
		byteArray[(int)i] = fileHeader[(int)i];
	}
	
	QBuffer     buffer (byteArray);
	QDataStream dataStream (&buffer);

	if (buffer.open (IO_ReadOnly) == false) {
		return false;
	}

	// check the "magic header"
	magic = new char[strlen (MAGIC) + 1];
	dataStream.readRawBytes (magic, strlen (MAGIC) + 1);	
	buffer.close();
	
	if (strncmp (magic, MAGIC, strlen (MAGIC) + 1) == 0) {
		delete[] magic;
		return true;
	}
	else {
		delete[] magic;
		return false;
	}

}

bool PSSFileFormat::readFromFile (const QString filePath, const bool mergeWithCurrent) {

	QFile                               file (filePath);
	QDataStream                         stream (&file);
	uint                                i;
	QString                             magicHeader;
	float                               theVoid;   // data which is ignored goes here
	SurfelInterface::PropertyDescriptor existingPropertyDescriptor;
	Scene                               *scene;
	MyDataTypes::CameraPosition         cameraPosition;
	Object                              *object;
	char                                *magic;

	StatusBar::getInstance()->resetProgress();

	if (file.open (IO_ReadOnly ) == false) {
		return false;
	}

	// "magic header"
	magic = new char[strlen (MAGIC) + 1];
	stream.readRawBytes (magic, strlen (MAGIC) + 1);	
	if (strncmp (magic, MAGIC, strlen (MAGIC) + 1) != 0) {
		file.close();
		delete[] magic;
		return false;
	}

	scene = Scene::getInstance();
	scene->setAutoDelete (true);
	// decide wheter to merge or not with current scene
	if (mergeWithCurrent == false) {

		// remove the old objects
		scene->reset (true, false);
		
		stream >> propertyDescriptor;
		scene->setSurfelPropertyDescriptor (propertyDescriptor);

		for (i = 0; i < 16; i++) {
			stream >> cameraPosition.scaleTranslationMatrix[i];
		}

		for (i = 0; i < 16; i++) {
			stream >> cameraPosition.rotationMatrix[i];
		}
		scene->setCameraPosition (cameraPosition, false);

		stream >> nofObjects;
		for (currentObject = 0; currentObject < nofObjects - 1; currentObject++) {

			object = new Object();
			this->readObjectFromStream (stream, object);
			scene->addObject (object, false, false);

		}

		// for the last object we emit the signal and make it active
		object = new Object();
		this->readObjectFromStream (stream, object);
		scene->addObject (object, true, true);

	}
	else {

		// we read in the property descriptor ...
		stream >> propertyDescriptor;
		// ... but use the already existing property descriptor, if possible
		existingPropertyDescriptor = scene->getSurfelPropertyDescriptor();

		// if no object is loaded yet, we use the property descriptor just read from the file
		if (existingPropertyDescriptor == SurfelInterface::NO_PROPERTIES) {
			scene->setSurfelPropertyDescriptor (propertyDescriptor);
		}

		// we keep the old objects		

		// now read in the new data
		// we keep the old transformation matrices
		for (i = 0; i < 32; i++) {
			stream >> theVoid;
		}

		stream >> nofObjects;
		for (currentObject = 0; currentObject < nofObjects - 1; currentObject++) {

			object = new Object();
			this->readObjectFromStream (stream, object);
			scene->addObject (object, false, false);

		}

		object = new Object();
		this->readObjectFromStream (stream, object);
		// for the last object we emit the signal and make it active
		scene->addObject (object, true, true);

	}

	// check file state
	if (file.status() != IO_Ok) {
		file.close();
		return false;
	}
	else {
		file.close();
		return true;
	}	

}

bool PSSFileFormat::writeToFile (const QString filePath, const bool onlyActiveObject) {

	QFile                       file (filePath);
	QDataStream                 stream (&file);
	uint                        i;
	Object                      *object;
	Scene                       *scene;
	MyDataTypes::CameraPosition cameraPosition;

	StatusBar::getInstance()->resetProgress();

	if (file.open (IO_WriteOnly ) == false) {
		return false;
	}

	scene = Scene::getInstance();

	// "magic header"
	stream.writeRawBytes (MAGIC, strlen (MAGIC) + 1);

	propertyDescriptor = scene->getSurfelPropertyDescriptor();
	stream << propertyDescriptor;
		
	cameraPosition = scene->getCameraPosition();
	for (i = 0; i < 16; i++) {
		stream << cameraPosition.scaleTranslationMatrix[i];
	}

	for (i = 0; i < 16; i++) {
		stream << cameraPosition.rotationMatrix[i];
	}

	if (onlyActiveObject == false) {
	
		nofObjects = scene->getNofObjects();
		stream << nofObjects;
		currentObject = 0;
		for (object = scene->getFirstObject(); object != 0; object = scene->getNextObject()) {
			this->writeObjectToStream (object, stream);
			currentObject++;
		}

	}
	else {
		nofObjects    = 1;
		currentObject = 0;
		stream << nofObjects;
		this->writeObjectToStream (scene->getActiveObject(), stream);
	}

	// check file state
	if (file.status() != IO_Ok) {
		// remove this corrupt file (it is also closed)
		file.remove();
		return false;
	}
	else {
		file.close();            
		return true;
	}

}

QString PSSFileFormat::getFileFilter() const {
	return QString ("PointShop Scene (*.pss)");
}

QString PSSFileFormat::getFileSuffix() const {
	return QString ("pss");
}

void PSSFileFormat::customize (QWidget *) {}

// ***************
// private methods
// ***************

void PSSFileFormat::readObjectFromStream (QDataStream &stream, Object *object) {

	uint                                 i;
	QString                              name;
	MyDataTypes::TransformationMatrix16f scaleTranslationMatrix,
		                                 rotationMatrix;

	stream >> name;
	object->setName (name, false);

	for (i = 0; i < 16; i++) {
		stream >> scaleTranslationMatrix[i];
	}

	for (i = 0; i < 16; i++) {
		stream >> rotationMatrix[i];
	}

	object->setScale (scaleTranslationMatrix[0], scaleTranslationMatrix[5], scaleTranslationMatrix[10], false);
	object->setPosition (scaleTranslationMatrix[12], scaleTranslationMatrix[13], scaleTranslationMatrix[14], false);
	object->setRotationMatrix (rotationMatrix, false);

	this->readSurfelCollectionFromStream (stream, object->getSurfelCollection());
	
}

void PSSFileFormat::writeObjectToStream (Object *object, QDataStream &stream) {

	uint i;
	QString                              name;
	MyDataTypes::TransformationMatrix16f scaleTranslationMatrix,
		                                 rotationMatrix;
	float                                dx,
		                                 dy,
										 dz;

	
	stream << object->getName();

	object->getScaleMatrix (scaleTranslationMatrix);
	object->getPosition (&dx, &dy, &dz);
	scaleTranslationMatrix[12] = dx;
	scaleTranslationMatrix[13] = dy;
	scaleTranslationMatrix[14] = dz;
	object->getRotationMatrix (rotationMatrix);
	for (i = 0; i < 16; i++) {
		stream << scaleTranslationMatrix[i];
	}

	for (i = 0; i < 16; i++) {
		stream << rotationMatrix[i];
	}

	this->writeSurfelCollectionToStream (object->getSurfelCollection(), stream);
	
}

void PSSFileFormat::readSurfelCollectionFromStream (QDataStream &stream, SurfelCollection *surfelCollection) {

	SurfelInterface                     *surfel;
	Scene                               *scene;
	uint                                i,
		                                nofSurfels;
	Vector3D                            position,
		                                normal,
										axisOne,
										axisTwo;
	float                               radius,
		                                ambientCoefficient,
										diffuseCoefficient,
										specularCoefficient,
										shininess;
	QRgb                                diffuseColor,
		                                specularColor;
	SurfelInterface::Flags              flags;
	MyDataTypes::TextureCoordinate      textureCoordinate;
	StatusBar                           *statusBar;
	int                                 progress;

	scene = Scene::getInstance();
	statusBar   = StatusBar::getInstance();

	stream >> nofSurfels;

	surfelCollection->setPropertyDescriptor (propertyDescriptor);

	if (propertyDescriptor == SurfelInterface::BaseSurfel_PropertyDescriptor) {
			
		for (i = 0; i < nofSurfels; i++) {

			surfel = surfelCollection->addSurfel (false);

			// read properties
			stream >> position[0];
			stream >> position[1];
			stream >> position[2];

			stream >> normal[0];
			stream >> normal[1];
			stream >> normal[2];

			stream >> radius;
			stream >> diffuseColor;
			stream >> specularColor;
			stream >> flags;

			scene->setSurfelPosition (surfel, position, false);
			scene->setSurfelNormal (surfel, normal, false);
			scene->setSurfelRadius (surfel, radius, false);
			scene->setSurfelDiffuseColor (surfel, diffuseColor, false);
			scene->setSurfelSpecularColor (surfel, specularColor, false);
			scene->setSurfelFlags (surfel, flags, false);

			// update the progress bar
			progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)i * 100.0f / (float)nofSurfels + 0.5f);
			if (progress > statusBar->getProgress()) {
				statusBar->setProgress (progress, false);
			}

		}
		
	}
	else if (propertyDescriptor == SurfelInterface::Surfel1_PropertyDescriptor) {

		for (i = 0; i < nofSurfels; i++) {

			surfel = surfelCollection->addSurfel (false);

			// read properties
			stream >> position[0];
			stream >> position[1];
			stream >> position[2];

			stream >> normal[0];
			stream >> normal[1];
			stream >> normal[2];

			stream >> radius;
			stream >> diffuseColor;
			stream >> specularColor;
			stream >> flags;

			stream >> ambientCoefficient;
			stream >> diffuseCoefficient;
			stream >> specularCoefficient;
			stream >> shininess;
			stream >> textureCoordinate.u;
			stream >> textureCoordinate.v;

			scene->setSurfelPosition (surfel, position, false);
			scene->setSurfelNormal (surfel, normal, false);
			scene->setSurfelRadius (surfel, radius, false);
			scene->setSurfelDiffuseColor (surfel, diffuseColor, false);
			scene->setSurfelSpecularColor (surfel, specularColor, false);
			scene->setSurfelFlags (surfel, flags, false);
			scene->setSurfelAmbientCoefficient (surfel, ambientCoefficient, false);
			scene->setSurfelDiffuseCoefficient (surfel, diffuseCoefficient, false);
			scene->setSurfelSpecularCoefficient (surfel, specularCoefficient, false);
			scene->setSurfelShininess (surfel, shininess, false);
			scene->setSurfelTextureCoordinate (surfel, textureCoordinate, false);

			// update the progress bar
			progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)i * 100.0f / (float)nofSurfels + 0.5f);
			if (progress > statusBar->getProgress()) {
				statusBar->setProgress (progress, false);
			}

		}

	}
	else if (propertyDescriptor == SurfelInterface::ElSurfel_PropertyDescriptor) {

		for (i = 0; i < nofSurfels; i++) {

			surfel = surfelCollection->addSurfel (false);

			// read properties
			stream >> position[0];
			stream >> position[1];
			stream >> position[2];

			stream >> normal[0];
			stream >> normal[1];
			stream >> normal[2];

			stream >> axisOne[0];
			stream >> axisOne[1];
			stream >> axisOne[2];

			stream >> axisTwo[0];
			stream >> axisTwo[1];
			stream >> axisTwo[2];
			
			stream >> diffuseColor;
			stream >> flags;

			scene->setSurfelPosition (surfel, position, false);
			scene->setSurfelNormalTangentAxesSystem (surfel, normal, axisOne, axisTwo, false);
			scene->setSurfelDiffuseColor (surfel, diffuseColor, false);
			scene->setSurfelFlags (surfel, flags, false);

			// update the progress bar
			progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)i * 100.0f / (float)nofSurfels + 0.5f);
			if (progress > statusBar->getProgress()) {
				statusBar->setProgress (progress, false);
			}

		}

	}	

	statusBar->setProgress (((float)(currentObject + 1) / (float)nofObjects) * 100, true);

}

void PSSFileFormat::writeSurfelCollectionToStream (SurfelCollection *surfelCollection, QDataStream &stream) {

	SurfelInterface                *surfel;
	Vector3D                       position,
		                           normal,
								   axisOne,
								   axisTwo;
	float                          radius,
		                           ambientCoefficient,
								   diffuseCoefficient,
								   specularCoefficient,
								   shininess;
	QRgb                           diffuseColor,
		                           specularColor;
	SurfelInterface::Flags         flags;
	MyDataTypes::TextureCoordinate textureCoordinate;
	StatusBar                      *statusBar;
	uint                           surfelCount,
		                           nofSurfels;
	int                            progress;

	statusBar = StatusBar::getInstance();

	stream << surfelCollection->getNofSurfels();

	surfelCount = 0;
	nofSurfels = surfelCollection->getNofSurfels();
	if (propertyDescriptor == SurfelInterface::BaseSurfel_PropertyDescriptor) {
		
		for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {

			position      = surfel->getPosition();
			normal        = surfel->getNormal();
			radius        = surfel->getRadius();
			diffuseColor  = surfel->getDiffuseColor();
			specularColor = surfel->getSpecularColor();
			flags         = surfel->getFlags();

			// write properties
			stream << position[0];
			stream << position[1];
			stream << position[2];

			stream << normal[0];
			stream << normal[1];
			stream << normal[2];

			stream << radius;
			stream << diffuseColor;
			stream << specularColor;
			stream << flags;
			
			// update the progress bar
			progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelCount * 100.0f / (float)nofSurfels + 0.5f);
			if (progress > statusBar->getProgress()) {
				statusBar->setProgress (progress, false);
			}
			surfelCount++;

		}
		
	}
	else if (propertyDescriptor == SurfelInterface::Surfel1_PropertyDescriptor) {

		for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {			

			position            = surfel->getPosition();
			normal              = surfel->getNormal();
			radius              = surfel->getRadius();
			diffuseColor        = surfel->getDiffuseColor();
			specularColor       = surfel->getSpecularColor();
			flags               = surfel->getFlags();
			ambientCoefficient  = surfel->getAmbientCoefficient();
			diffuseCoefficient  = surfel->getDiffuseCoefficient();
			specularCoefficient = surfel->getSpecularCoefficient();
			shininess           = surfel->getShininess();
			textureCoordinate   = surfel->getTextureCoordinate();

			// write properties
			stream << position[0];
			stream << position[1];
			stream << position[2];

			stream << normal[0];
			stream << normal[1];
			stream << normal[2];

			stream << radius;
			stream << diffuseColor;
			stream << specularColor;
			stream << flags;

			stream << ambientCoefficient;
			stream << diffuseCoefficient;
			stream << specularCoefficient;
			stream << shininess;
			stream << textureCoordinate.u;
			stream << textureCoordinate.v;
			
			// update the progress bar
			progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelCount * 100.0f / (float)nofSurfels + 0.5f);
			if (progress > statusBar->getProgress()) {
				statusBar->setProgress (progress, false);
			}
			surfelCount++;

		}	
		
	}	
	else if (propertyDescriptor == SurfelInterface::ElSurfel_PropertyDescriptor) {

		for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {			

			position     = surfel->getPosition();
			normal       = surfel->getNormal();
			axisOne      = surfel->getAxisOne();
			axisTwo      = surfel->getAxisTwo();
			diffuseColor = surfel->getDiffuseColor();
			flags        = surfel->getFlags();

			// write properties
			stream << position[0];
			stream << position[1];
			stream << position[2];

			stream << normal[0];
			stream << normal[1];
			stream << normal[2];

			stream << axisOne[0];
			stream << axisOne[1];
			stream << axisOne[2];

			stream << axisTwo[0];
			stream << axisTwo[1];
			stream << axisTwo[2];

			stream << diffuseColor;
			stream << flags;

			// update the progress bar
			progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelCount * 100.0f / (float)nofSurfels + 0.5f);
			if (progress > statusBar->getProgress()) {
				statusBar->setProgress (progress, false);
			}
			surfelCount++;

		}	
		
	}	

	statusBar->setProgress (((float)(currentObject + 1) / (float)nofObjects) * 100, true);

}

// ****************
// access functions
// ****************

FileFormatPluginInterface *createFileFormatPlugin() {
	return new PSSFileFormat();
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
