// Title:   SLFFileFormat.cpp
// Created: Thu Sep 25 14:47:55 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003, Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General
// Public License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330,
// Boston, MA 02111-1307 USA
// 
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

// Title:   SLFFileFormat.cpp
// Created: Thu Sep 25 14:47:55 2003
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

#include <qfile.h>
#include <qdatastream.h>
#include <qstring.h>

#include "../../../Core/Scene/src/Scene.h"
#include "../../../Core/Scene/src/Object.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/RendererManager/src/RendererManager.h"
#include "../../../Core/Interfaces/src/FileFormatPluginInterface.h"
#include "../../../Utilities/src/FileTools.h"
#include "../../../Utilities/src/Matrix.h"
#include "../../../Utilities/UserMessages/src/StatusBar.h"

#include "NormalMap.h"
#include "SLFFileFormat.h"

// *********
// constants
// *********

static const int NOF_SUBDIVISIONS = 6;

// **************
// public methods
// **************

SLFFileFormat::SLFFileFormat() {}

SLFFileFormat::~SLFFileFormat(){}

bool SLFFileFormat::hasWriteProtection() {
	return true;
}

bool SLFFileFormat::isReadable (const FileHeader fileHeader) {
	return (strncmp ("SLF", fileHeader, 3) == 0);
}

bool SLFFileFormat::readFromFile (const QString filePath, const bool mergeWithCurrent) {

	QFile                                file (filePath);
	QDataStream                          stream (&file);
	uint                                 i;
	Object                               *object;
	SurfelCollection                     *surfelCollection;
	char                                 id[3];
	float                                theVoid;   // data which is ignored goes here
	int                                  nofSubDivisions;
	MyDataTypes::TransformationMatrix16f userTransformation;
	float                                dx,        // we apply the user transformation while
		                                 dy,        // loading the object, only translation is applied
										 dz;
	QString                              objectName;
	Scene                                *scene;

	StatusBar::getInstance()->resetProgress();

	if (file.open (IO_ReadOnly) == false) {
		return false;
	}
	stream.setByteOrder (QDataStream::LittleEndian);

	stream.readRawBytes (&(id[0]), 3);
	if (strncmp ("SLF", id, 3) != 0) {
		return false;
	}

	stream >> nofSubDivisions;

	// ignore the old model transformation
	for (i = 0; i < 16; i++) {
		stream >> userTransformation[i];
	}

	for (i = 0; i < 16; i++) {
		stream >> theVoid;
	}

	// assume row major order 
	dx = userTransformation[3];
	dy = userTransformation[7];
	dz = userTransformation[11];

	scene = Scene::getInstance();
	// decide wheter to merge or not with current scene
	objectName = FileTools::getFileName (filePath);
	objectName = FileTools::replaceSuffix (objectName, "");
	scene->setAutoDelete (true);

	SurfelInterface::PropertyDescriptor propertyDescriptor =	SurfelInterface::POSITION |
																SurfelInterface::NORMAL |
																SurfelInterface::DIFFUSE_COLOR |
																SurfelInterface::SPECULAR_COLOR |
																SurfelInterface::RADIUS |
																SurfelInterface::AMBIENT_COEFFICIENT |
																SurfelInterface::DIFFUSE_COEFFICIENT |
																SurfelInterface::SPECULAR_COEFFICIENT;

	if (mergeWithCurrent == false) {

		// remove the old objects, move camera into home position
		scene->reset (true, false);

		object = new Object(objectName, propertyDescriptor);		
		
		surfelCollection = object->getSurfelCollection();
		this->readSurfelCollectionFromStream (stream, dx, dy, dz, nofSubDivisions, surfelCollection);
		
		scene->addObject (object, true, true);		

	}
	else {

		// we keep the old transformation matrices

		object = new Object(objectName, propertyDescriptor);		
		
		surfelCollection = object->getSurfelCollection();
		this->readSurfelCollectionFromStream (stream, dx, dy, dz, nofSubDivisions, surfelCollection);
		
		// make this object active
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

bool SLFFileFormat::writeToFile (const QString filePath, const bool onlyActiveObject) {

	QFile                                file (filePath);
	QDataStream                          stream (&file);
	uint                                 i;
	Object                               *object;
	SurfelCollection                     *surfelCollection;
	MyDataTypes::TransformationMatrix16f userTransformation;

	QString                              objectName;
	Scene                                *scene;

	StatusBar::getInstance()->resetProgress();

	if (file.open (IO_WriteOnly) == false) {
		return false;
	}
	stream.setByteOrder (QDataStream::LittleEndian);

	stream.writeRawBytes ("SLF", 3);

	stream << NOF_SUBDIVISIONS;

	MtrUnity4x4f (userTransformation);
	// user transformation
	for (i = 0; i < 16; i++) {
		stream << userTransformation[i];
	}

	// inverse user transformation
	for (i = 0; i < 16; i++) {
		stream << userTransformation[i];
	}

	scene = Scene::getInstance();

	if (onlyActiveObject == false) {

		// only one object can be stored in the SLFFileFormat, so we just store the
		// active object
		object = scene->getActiveObject();
		
		surfelCollection = object->getSurfelCollection();
		this->writeSurfelCollectionToStream (surfelCollection, stream);	

	}
	else {

		// only one object can be stored in the SLFFileFormat, so we just store the
		// active object
		object = scene->getActiveObject();
		
		surfelCollection = object->getSurfelCollection();
		this->writeSurfelCollectionToStream (surfelCollection, stream);		

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





QString SLFFileFormat::getFileFilter() const {
	return QString ("Surfel List File (*.slf)");
}

QString SLFFileFormat::getFileSuffix() const {
	return QString ("slf");
}

void SLFFileFormat::customize (QWidget *) {}

// ***************
// private methods
// ***************

void SLFFileFormat::readSurfelCollectionFromStream (QDataStream &stream,
													const float dx, const float dy, const float dz,
													const uint nofSubDivisions,
													SurfelCollection *surfelCollection) {

	SurfelInterface *surfel;
	uint         i,
		         nofSurfels;
	Vector3D     position,
		         normal;
	int          normalIndex;
	QRgb         diffuseColor,
		         specularColor;    // no specular color supported yet
	float        radius,
		         diffuseCoefficient,
		         ambientCoefficient,
				 specularCoefficient;
	uchar        shininess;        // no shininess supported yet
	NormalMap    *normalMap;
	Scene        *scene;
	StatusBar    *statusBar;
	int          progress;

	scene = Scene::getInstance();
	statusBar = StatusBar::getInstance();

	// allocate the temporary normal map
	normalMap = new NormalMap (nofSubDivisions);

	stream >> nofSurfels;

	// we create a Surfel1 here...
	for (i = 0; i < nofSurfels; i++) {

		surfel = surfelCollection->addSurfel (false);

		stream >> position[0];
		stream >> position[1];
		stream >> position[2];

		// apply user transformation
		position[0] += dx;
		position[1] += dy;
		position[2] += dz;

		stream >> normalIndex;
		normal = normalMap->getNormalVector (normalIndex);

		// set alpha to 0xff, the rest is overwritten with the color values
		diffuseColor  = 0xff000000;
		specularColor = 0xff000000;
		stream.readRawBytes ((char *)&diffuseColor, 3);
		stream.readRawBytes ((char *)&specularColor, 3);
		
		stream >> diffuseCoefficient;
		stream >> ambientCoefficient;
		stream >> specularCoefficient;
		stream >> shininess;
		stream >> radius;

		position[1] = -position[1];
		normal[1] = -normal[1];

		scene->setSurfelPosition (surfel, position, false);
		scene->setSurfelNormal (surfel, normal, false);
		scene->setSurfelDiffuseColor (surfel, diffuseColor, false);
		scene->setSurfelSpecularColor (surfel, specularColor, false);
		scene->setSurfelRadius (surfel, radius, false);
		scene->setSurfelAmbientCoefficient (surfel, ambientCoefficient, false);
		scene->setSurfelDiffuseCoefficient (surfel, diffuseCoefficient, false);
		scene->setSurfelSpecularCoefficient (surfel, specularCoefficient, false);

		// update the progress bar
		progress = (int)((float)i * 100.0f / (float)nofSurfels + 0.5f);
		if (progress > statusBar->getProgress()) {
			statusBar->setProgress (progress, false);
		}

	}	

	statusBar->setProgress (100, true);

	// remove unused resources
	delete normalMap;

}

void SLFFileFormat::writeSurfelCollectionToStream (SurfelCollection *surfelCollection, QDataStream &stream) {

	SurfelInterface *surfel;

	Vector3D        position,
		            normal;
	int             normalIndex;
	QRgb            diffuseColor,
		            specularColor;    // no specular color supported yet
	float           radius,
		            diffuseCoefficient,
		            ambientCoefficient,
					specularCoefficient;
	uchar           shininess;        // no shininess supported yet
	NormalMap       *normalMap;
	Scene           *scene;
	uint            surfelCount,
		            nofSurfels;
	StatusBar       *statusBar;
	int             progress;

	scene = Scene::getInstance();

	// allocate the temporary normal map
	normalMap = new NormalMap (NOF_SUBDIVISIONS);
	statusBar = StatusBar::getInstance();

	stream << scene->getNofSurfels();

	surfelCount = 0;
	nofSurfels = surfelCollection->getNofSurfels();
	for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {

		position            = surfel->getPosition();
		normal              = surfel->getNormal();
		radius              = surfel->getRadius();
		diffuseColor        = surfel->getDiffuseColor();
		specularColor       = surfel->getSpecularColor();
		
		ambientCoefficient  = surfel->getAmbientCoefficient();
		diffuseCoefficient  = surfel->getDiffuseCoefficient();
		specularCoefficient = surfel->getSpecularCoefficient();
		shininess           = (unsigned char)surfel->getShininess();

		stream << position[0];
		stream << position[1];
		stream << position[2];

		normalIndex = normalMap->getIndexFromVector (normal);
		stream << normalIndex;

		stream.writeRawBytes ((char *)&diffuseColor, 3);
		stream.writeRawBytes ((char *)&specularColor, 3);
		
		stream << diffuseCoefficient;
		stream << ambientCoefficient;
		stream << specularCoefficient;
		stream << shininess;
		stream << radius;	

		// update the progress bar
		progress = (int)((float)surfelCount * 100.0f / (float)nofSurfels + 0.5f);
		if (progress > statusBar->getProgress()) {
			statusBar->setProgress (progress, false);
		}
		surfelCount++;

	}	

	statusBar->setProgress (100, true);

	// remove unused resources
	delete normalMap;

}

// ****************
// access functions
// ****************

FileFormatPluginInterface *createFileFormatPlugin() {
	return new SLFFileFormat();
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
