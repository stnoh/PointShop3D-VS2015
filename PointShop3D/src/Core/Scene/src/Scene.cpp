// Title:   Scene.cpp
// Created: Thu Sep 25 14:11:03 2003
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

#include "../../../Utilities/src/FileTools.h"
#include "../../../Utilities/src/StringTools.h"
#include "../../../Utilities/src/Matrix.h"
#include "../../../Utilities/src/FileTools.h"
#include "../../Configuration/src/Configuration.h"
#include "../../ToolManager/src/ToolManager.h"
#include "../../PluginManager/src/PluginManager.h"
#include "Scene.h"

// *********
// constants
// *********

// number of digits for sequence numbers
static const uint NOF_SEQ_DIGITS = 5;

// ******
// global
// ******

// the single Scene instance, only
// accesible by getInstance() and
// destroyInstance()
static Scene *instance = 0;

// **************
// public methods
// **************

Scene *Scene::getInstance() {

	if (instance == 0) {
		instance = new Scene();
	}
	return instance;

}

void Scene::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void Scene::setAutoDelete (const bool enable) {
	objects.setAutoDelete (enable);
}

bool Scene::isAutoDelete() const {
	return objects.autoDelete();
}

uint Scene::getNofObjects() const {
	return objects.count();
}

uint Scene::getNofEnabledObjects() {

	Object *object;
	uint   nofEnabledObjects = 0;

	for (object = objects.first(); object != 0; object = objects.next()) {
		if (object->isFlagOn (Object::ENABLED)) {
			nofEnabledObjects++;
		}
	}

	
	return nofEnabledObjects;
}


uint Scene::getNofSurfels() {

	uint   nofSurfels;
	Object *object;

	nofSurfels = 0;
	for (object = objects.first(); object != 0; object = objects.next()) {
		nofSurfels += object->getSurfelCollection()->getNofSurfels();
	}

	return nofSurfels;

}

QString Scene::getUniqueName (const QString name) {

	Object  *object;
	bool    found;
	QString objectName,
		    uniqueName;
	uint    size;
	int	    sequenceNumber,
			highestSequenceNumber;

	// compare the name with the name of each object, find out the highest used sequence number for that name
	found                 = false;
	size                  = name.length();
	highestSequenceNumber = -1;
	for  (object = objects.first(); object != 0; object = objects.next()) {

		objectName = object->getName();
		if (strncmp (name.latin1(), objectName.latin1(), size) == 0) {

			// the same name has been found, but it hasn't necessarily a sequence number yet
			found = true;
			sequenceNumber = StringTools::getSequenceNumber (objectName, NOF_SEQ_DIGITS, name.length());
			if (sequenceNumber > highestSequenceNumber) {
				highestSequenceNumber = sequenceNumber;
			}

		}

	}

	if (found == true) {

		if (highestSequenceNumber > -1) {
			uniqueName = StringTools::appendSequenceNumber (name, highestSequenceNumber + 1, NOF_SEQ_DIGITS);
		}
		else {
			// we start numbering at 2, i.e. name, name0002, name0003 etc.
			uniqueName = StringTools::appendSequenceNumber (name, 2, NOF_SEQ_DIGITS);
		}

	}
	else {
		uniqueName = name;
	}

	return uniqueName; 
}

QString Scene::addObject (Object *newObject, const bool makeActive, const bool emitSignal) {

	QString uniqueName;

	uniqueName = this->getUniqueName (newObject->getName());
	newObject->setName (uniqueName, false);

	// update the active object
	if (objects.count() > 0) {
		if (makeActive == true) {
			emit activeObjectAboutToChange();

			activeObject = newObject;
			// updates also the current item
			objects.append (newObject);
			activeObjectID = objects.at();

			emit activeObjectChanged();
		}
		else {
			// just append it, without updating activeObject(ID)
			objects.append (newObject);
		}
	}
	else {
		emit activeObjectAboutToChange();

		// if this is the first object it becomes active in any case
		activeObject = newObject;
		// updates also the current item
		objects.append (newObject);
		activeObjectID = objects.at();

		emit activeObjectChanged();
	}
	
	// get informed whenever this object is modified
	this->connect (newObject, SIGNAL (objectModified ()),
				   this, SLOT (handleObjectModified ()));

	this->connect (newObject, SIGNAL (objectRenamed ()),
				   this, SLOT (handleObjectRenamed ()));

	// get informed whenever this object has been moved
	this->connect (newObject, SIGNAL (objectMoved()),
				   this, SLOT (handleObjectMoved()));

	// get informed whenever the SurfelCollection of this Object changes its surfel type
	this->connect (newObject->getSurfelCollection(), SIGNAL (surfelTypeChanged (const SurfelInterface::PropertyDescriptor)),
				   this, SLOT (handleSurfelTypeChanged (const SurfelInterface::PropertyDescriptor)));	

	if (emitSignal == true) {
		emit objectAdded();
		emit renderFrame();
	}

	// update tool-availability
	ToolManager::getInstance()->checkToolAvailability();
	
	// update plugin-availability
	PluginManager::getInstance()->checkPluginAvailability();
	
	return uniqueName;

}

void Scene::removeObject (Object *object, const bool emitSignal) {

	if (this->isAutoDelete() == false) {
		object->disconnect (this);
	}
	objects.remove (object);

	// update tool-availability
	ToolManager::getInstance()->checkToolAvailability();
	

	// update the active object, if necessary
	if (activeObject == object) {
		emit activeObjectAboutToChange();

		activeObject   = objects.first();
		activeObjectID = objects.at();

		emit activeObjectChanged();
	}	

	if (emitSignal == true) {
		emit objectRemoved();
		emit renderFrame();
	}

}

void Scene::reset (const bool emitSignal, const bool doRenderFrame) {

	emit activeObjectAboutToChange(); 

	objects.clear();
	activeObject   = 0;
	activeObjectID = -1;
	cameraPosition = Configuration::getInstance()->getHomePosition();
	isTransformationMatrixValid = false;

	emit activeObjectChanged(); 

	if (emitSignal == true) {
		emit objectRemoved();

		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
	}

	if (doRenderFrame == true) {
		emit renderFrame();
	}

}

Object *Scene::getFirstObject() {
	return objects.first();
}

Object *Scene::getNextObject() {
	return objects.next();
}

void Scene::setActiveObject (const QString activeName, const bool doRenderFrame) {

	Object *object;
	bool   found;

	object = objects.first();
	found  = false;
	while (object != 0 && found == false) {

		if (activeName == object->getName()) {
			found = true;
		}
		else {
			object = objects.next();
		}

	}

	if (found == true) {

		emit activeObjectAboutToChange();

		activeObject   = object;
		activeObjectID = objects.at();

		emit activeObjectChanged();

		if (doRenderFrame == true) {
			emit renderFrame();
		}

	}
	else {
		activeObjectID = -1;
	}

}

Object *Scene::getActiveObject() const {
	return activeObject;
}

int Scene::getActiveObjectID() const {
	return activeObjectID;
}

void Scene::setRotation (const float angle, const float x, const float y, const float z, const bool emitSignal) {

    MtrCreateRotation4x4fc (angle, x, y, z, cameraPosition.rotationMatrix);

	isTransformationMatrixValid = false;
	if (emitSignal == true) {
		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
		emit renderFrame();
	}

}

void Scene::rotate (const float dAngle, const float x, const float y, const float z, const bool emitSignal) {

	MyDataTypes::TransformationMatrix16f userRotation,
		                                 finalRotation;
	
    MtrCreateRotation4x4fc (dAngle, x, y, z, userRotation);
	MtrMult4x4f (userRotation, cameraPosition.rotationMatrix, finalRotation);
	MtrCopy4x4f (finalRotation, cameraPosition.rotationMatrix);

	isTransformationMatrixValid = false;
	if (emitSignal == true) {
		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
		emit renderFrame();
	}

}

void Scene::setTranslation (const float x, const float y, const float z, const bool emitSignal) {

	cameraPosition.scaleTranslationMatrix[12] = x;
	cameraPosition.scaleTranslationMatrix[13] = y;
	cameraPosition.scaleTranslationMatrix[14] = z;

	isTransformationMatrixValid = false;
	if (emitSignal == true) {
		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
		emit renderFrame();
	}

}

void Scene::translate (const float dx, const float dy, const float dz, const bool emitSignal) {

	cameraPosition.scaleTranslationMatrix[12] += dx;
	cameraPosition.scaleTranslationMatrix[13] += dy;
	cameraPosition.scaleTranslationMatrix[14] += dz;

	isTransformationMatrixValid = false;
	if (emitSignal == true) {
		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
		emit renderFrame();
	}

}

void Scene::setScale (const float scaleX, const float scaleY, const float scaleZ, const bool emitSignal) {

	cameraPosition.scaleTranslationMatrix[0]  = scaleX;
	cameraPosition.scaleTranslationMatrix[5]  = scaleY;
	cameraPosition.scaleTranslationMatrix[10] = scaleZ;

	isTransformationMatrixValid = false;
	if (emitSignal == true) {
		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
		emit renderFrame();
	}

}

void Scene::scale (const float dScaleX, const float dScaleY, const float dScaleZ, const bool emitSignal) {

	cameraPosition.scaleTranslationMatrix[0]  *= dScaleX;
	cameraPosition.scaleTranslationMatrix[5]  *= dScaleY;
	cameraPosition.scaleTranslationMatrix[10] *= dScaleZ;

	isTransformationMatrixValid = false;
	if (emitSignal == true) {
		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
		emit renderFrame();
	}

}

void Scene::getScaleMatrix (MyDataTypes::TransformationMatrix16f scaleMatrix) const {
	
	MtrUnity4x4f (scaleMatrix);
	scaleMatrix[0]  = cameraPosition.scaleTranslationMatrix[0];
	scaleMatrix[5]  = cameraPosition.scaleTranslationMatrix[5];
	scaleMatrix[10] = cameraPosition.scaleTranslationMatrix[10];

}

void Scene::getTranslationMatrix (MyDataTypes::TransformationMatrix16f translationMatrix) const {
	
	MtrUnity4x4f (translationMatrix);
	translationMatrix[12] = cameraPosition.scaleTranslationMatrix[12];
	translationMatrix[13] = cameraPosition.scaleTranslationMatrix[13];
	translationMatrix[14] = cameraPosition.scaleTranslationMatrix[14];

}

void Scene::getRotationMatrix (MyDataTypes::TransformationMatrix16f rotationMatrix) const {
	MtrCopy4x4f (cameraPosition.rotationMatrix, rotationMatrix);
}

void Scene::setCameraPosition (const MyDataTypes::CameraPosition newCameraPosition, const bool emitSignal) {

	cameraPosition = newCameraPosition;
	
	isTransformationMatrixValid = false;
	if (emitSignal == true) {
		this->updateTransformationMatrix();
		emit sceneMoved (transformationMatrix);
		emit renderFrame();
	}

}

MyDataTypes::CameraPosition Scene::getCameraPosition() const {
	return cameraPosition;
}

void Scene::getTransformationMatrix (MyDataTypes::TransformationMatrix16f transformationMatrix) {
	
	if (isTransformationMatrixValid == false) {
		this->updateTransformationMatrix();
	}
	memcpy (transformationMatrix, this->transformationMatrix, sizeof (MyDataTypes::TransformationMatrix16f));

}

void Scene::setSurfelPropertyDescriptor (const SurfelInterface::PropertyDescriptor newPropertyDescriptor) {
	this->setAllPropertyDescriptors (newPropertyDescriptor);
}

void Scene::requestSurfelPropertyDescriptor (const SurfelInterface::PropertyDescriptor requestedPropertyDescriptor) {

	Object                              *object;
	SurfelInterface::PropertyDescriptor supportedPropertyDescriptor;

	object = objects.getFirst();
	if (object != 0) {
		
		supportedPropertyDescriptor = object->getSurfelCollection()->getPropertyDescriptor();

		// check if we support the 'requestedPropertyDescriptor'
		if ((supportedPropertyDescriptor & requestedPropertyDescriptor) != requestedPropertyDescriptor) {
			// set the combination of supported and requested properties
			this->setAllPropertyDescriptors (supportedPropertyDescriptor | requestedPropertyDescriptor);
		}

	}

}

SurfelInterface::PropertyDescriptor Scene::getSurfelPropertyDescriptor() const {
	
	Object                *object;	

	object = objects.getFirst();
	if (object != 0) {
		return object->getSurfelCollection()->getPropertyDescriptor();
	}
	else {
		return SurfelInterface::NO_PROPERTIES;
	}

}

void Scene::setSurfelPosition (SurfelInterface *surfel, const Vector3D newPosition, const bool emitSignal) {

	surfel->setPosition (newPosition);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelNormal (SurfelInterface *surfel, const Vector3D newNormal, const bool emitSignal) {

	surfel->setNormal (newNormal);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelTangentAxes (SurfelInterface *surfel, const Vector3D newAxisOne, const Vector3D newAxisTwo, const bool emitSignal) {

	surfel->setTangentAxes (newAxisOne, newAxisTwo);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelNormalTangentAxesSystem (SurfelInterface *surfel, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo, const bool emitSignal) {

	surfel->setNormalTangentAxesSystem(newNormal, newAxisOne, newAxisTwo);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelRadius (SurfelInterface *surfel, const float newRadius, const bool emitSignal) {

	surfel->setRadius (newRadius);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::combineSurfelColorProperties(SurfelInterface *surfel, SurfelInterface *copySurfel1, SurfelInterface *copySurfel2, float factor, const bool emitSignal) {

	surfel->combineColorProperties (copySurfel1, copySurfel2, factor);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}
}

void Scene::setSurfelDiffuseColor (SurfelInterface *surfel, const QRgb newDiffuseColor, const bool emitSignal) {

	surfel->setDiffuseColor (newDiffuseColor);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelSpecularColor (SurfelInterface *surfel, const QRgb newSpecularColor, const bool emitSignal) {

	surfel->setSpecularColor (newSpecularColor);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelFlagOn (SurfelInterface *surfel, const SurfelInterface::Flag flags,
										 const bool on, const bool emitSignal) {

	surfel->setFlagOn (flags, on);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelFlags (SurfelInterface *surfel, const SurfelInterface::Flags newFlags, const bool emitSignal) {
	
	surfel->setFlags (newFlags);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setAllSurfelFlagsOn (const SurfelInterface::Flags newFlags, const bool on, const bool emitSignal) {
	
	Object           *object;
	SurfelCollection *surfelCollection;
	SurfelInterface  *surfel;

	for (object = objects.first(); object != 0; object = objects.next()) {

		surfelCollection = object->getSurfelCollection();
		surfel = surfelCollection->getFirstSurfel();
		while (surfel != 0) {
			surfel->setFlagOn (newFlags, on);
			surfel = surfelCollection->getNextSurfel();
		}
	}

	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::clearAllAssociatedSurfels(const bool emitSignal) {

	Object           *object;
	SurfelCollection *surfelCollection;
	SurfelInterface  *surfel;

	for (object = objects.first(); object != 0; object = objects.next()) {

		surfelCollection = object->getSurfelCollection();
		for (surfel = surfelCollection->getFirstSurfel(); surfel; surfel = surfelCollection->getNextSurfel()) {
			surfel->clearAssociatedSurfels();
		}
	}

	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelAmbientCoefficient (SurfelInterface *surfel, const float newAmbientCoefficient, const bool emitSignal) {

	surfel->setAmbientCoefficient (newAmbientCoefficient);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelDiffuseCoefficient (SurfelInterface *surfel, const float newDiffuseCoefficient, const bool emitSignal) {

	surfel->setDiffuseCoefficient (newDiffuseCoefficient);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelSpecularCoefficient (SurfelInterface *surfel, const float newSpecularCoefficient, const bool emitSignal) {

	surfel->setSpecularCoefficient (newSpecularCoefficient);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelShininess (SurfelInterface *surfel, const float newShininess, const bool emitSignal) {

	surfel->setShininess (newShininess);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::setSurfelTextureCoordinate (SurfelInterface *surfel, const MyDataTypes::TextureCoordinate newTextureCoordinate, const bool emitSignal) {

	surfel->setTextureCoordinate (newTextureCoordinate);
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}



bool Scene::addAssociatedSurfel (SurfelInterface *surfel, SurfelInterface *associatedSurfel, const bool emitSignal) {

	if (surfel->getNofAssociatedSurfels() == 0) {

		//surfel->setDiffuseColor (QColor (255, 0, 0).rgb());
		//associatedSurfel->setDiffuseColor (QColor (255, 255, 0).rgb());

	}
	else if (surfel->getNofAssociatedSurfels() == 1) {
//		surfel->setDiffuseColor (QColor (0, 255, 0).rgb());
//		associatedSurfel->setDiffuseColor (QColor (0, 255, 255).rgb());
	}
	else if (surfel->getNofAssociatedSurfels() == 2) {
//		surfel->setDiffuseColor (QColor (0, 0, 255).rgb());
//		associatedSurfel->setDiffuseColor (QColor (255, 255, 0).rgb());
	}



	if (surfel->addAssociatedSurfel (associatedSurfel)) {
		if (emitSignal == true) {
			emit objectModified ();
			emit renderFrame();
		}
		return true;
	}
	else {
		return false;
	}
}


void Scene::clearAssociatedSurfels (SurfelInterface *surfel, const bool emitSignal) {
	surfel->clearAssociatedSurfels();
	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}
	
}




void Scene::removeSurfels (std::vector<SurfelInterface*> *surfels, const bool emitSignal) {

	Object                         *object;
	SurfelCollection               *surfelCollection;
	std::vector<SurfelInterface*>::iterator iter_surfels;

	for (object = objects.first(); object != 0; object = objects.next()) {

		surfelCollection = object->getSurfelCollection();

		for (iter_surfels = surfels->begin(); iter_surfels != surfels->end(); ++iter_surfels) {
			
			if (surfelCollection->contains (*iter_surfels)) {
				surfelCollection->removeSurfel (*iter_surfels, false);
			}        
			
        }

	}

	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::removeSurfels (SurfelInterface::Flag flag, const bool emitSignal) {

	Object           *object;
	SurfelCollection *surfelCollection;
	SurfelInterface  *surfel;

	for (object = objects.first(); object != 0; object = objects.next()) {

		surfelCollection = object->getSurfelCollection();
		surfel = surfelCollection->getFirstSurfel();
		while (surfel != 0) {

			if (surfel->isFlagOn (flag)) {
				// remove the current surfel, so for the next object
				// we have to check only the existing surfels - this also puts the internal iterator
				// to the next surfel, no need to call getNextSurfel here!!
				surfelCollection->removeCurrentSurfel();
				surfel = surfelCollection->getCurrentSurfel();
			}
			else {
				surfel = surfelCollection->getNextSurfel();
			}
		}

	}

	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::removeSurfels (Object *object, QList<SurfelInterface> *surfels, const bool emitSignal) {

	object->getSurfelCollection()->removeSurfels (surfels);

	if (emitSignal == true) {
		emit objectModified ();
		emit renderFrame();
	}

}

void Scene::removeSurfels (std::vector<SurfelInterface*> surfels, const int numOfSurfels, const bool emitSignal) {

	Object           *object;
	SurfelCollection *surfelCollection;

	for (object = objects.first(); object != 0; object = objects.next()) {

		surfelCollection = object->getSurfelCollection();
		int i;

		for (i = 0; i < numOfSurfels; i++) {
			// check if surfel is in current surfel collection. an attempt to remove the surfel otherwise
			// may cause a program crash
			if(surfelCollection->contains(surfels[i])) {
				surfelCollection->removeSurfel (surfels[i], false);
			}
		}
	}

	if (emitSignal == true) {
		emit objectModified();
		emit renderFrame();
	}

}

void Scene::emitObjectModifiedSignal (bool samplingPatternAltered) {	
	emit objectModified (samplingPatternAltered);
}

void Scene::emitRenderFrameSignal() {
	emit renderFrame();
}

// ***************
// private methods
// ***************

Scene::Scene() {

	this->reset (false, false);
	this->setAutoDelete (true);
	activeObjectID = -1;
	isTransformationMatrixValid = false;

}

Scene::~Scene() {}

void Scene::updateTransformationMatrix() {
	MtrMult4x4f (cameraPosition.scaleTranslationMatrix, cameraPosition.rotationMatrix, transformationMatrix);
	isTransformationMatrixValid = true;
}

void Scene::setAllPropertyDescriptors (const SurfelInterface::PropertyDescriptor newPropertyDescriptor) {

	Object           *object;
	SurfelCollection *surfelCollection;

	// when one Object's surfelCollection changes the type of it's surfels, change the surfel types
	// of all the other Objects as well - all Objects must have the same type of Surfel
	for (object = objects.first(); object != 0; object = objects.next()) {
	
		surfelCollection = object->getSurfelCollection();
		
		// temporarily disconnect from the surfelTypeChanged signal, so we won't get informed
		this->disconnect (surfelCollection, SIGNAL (surfelTypeChanged (const SurfelInterface::PropertyDescriptor)),
			              this, SLOT (handleSurfelTypeChanged (const SurfelInterface::PropertyDescriptor)));
		surfelCollection->setPropertyDescriptor (newPropertyDescriptor);
		
		// connect again
		this->connect (surfelCollection, SIGNAL (surfelTypeChanged (const SurfelInterface::PropertyDescriptor)),
			           this, SLOT (handleSurfelTypeChanged (const SurfelInterface::PropertyDescriptor)));

	}

	emit objectModified();
	emit renderFrame();

}

// *************
// private slots
// *************

void Scene::handleSurfelTypeChanged (const SurfelInterface::PropertyDescriptor newPropertyDescriptor) {
	//this->setAllPropertyDescriptors (newPropertyDescriptor);
	emit objectModified();
	emit renderFrame();
}

void Scene::handleObjectModified () {
	
	emit objectModified ();
	emit renderFrame();
}

void Scene::handleObjectRenamed () {	
	emit objectRenamed ();
}

void Scene::handleObjectMoved() {
	emit objectMoved();
	emit renderFrame();
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
