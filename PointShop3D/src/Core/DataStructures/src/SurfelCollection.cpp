// Title:   SurfelCollection.cpp
// Created: Thu Sep 25 14:19:40 2003
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

#include "SurfelCollection.h"

#include <qdatastream.h>	
#if QT_VERSION >= 0x030000
# include <qptrlist.h>
# ifndef QList
#  define QList QPtrList
# endif
# ifndef QListIterator
#  define QListIterator QPtrListIterator
# endif
#else
# include <qlist.h>
#endif
#include "../../../Utilities/UserMessages/src/StatusBar.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../MarkerManager/src/MarkerManager.h"

#ifdef __WIN32
 #ifdef NDEBUG
	#define AT(vectorPointer, pos) ((*vectorPointer)[pos])
 #else
	#define AT(vectorPointer, pos) vectorPointer->at(pos)
 #endif
#else
	#define AT(vectorPointer, pos) ((*vectorPointer)[pos])
#endif


// **************
// static members
// **************

const MyDataTypes::TextureCoordinate SurfelCollection::defaultTextureCoordinate = {0.0f, 0.0f};

// **************
// public methods
// **************

SurfelCollection::SurfelCollection (const SurfelInterface::PropertyDescriptor nativePropertyDescriptor, 
									const int capacity,
									const float incrCapacityFactor) {

	surfels					= NULL;
	positions				= NULL;
	normals					= NULL;
	axes					= NULL;
	radius					= NULL;
	diffuseColors			= NULL;
	specularColors			= NULL;
	ambientCoefficients		= NULL;
	diffuseCoefficients		= NULL;
	specularCoefficients	= NULL;
	shininess				= NULL;	
	textureCoordinates		= NULL;
	flags					= NULL;
	associatedSurfels		= NULL;
	details					= NULL;

	nOfSurfels				= 0;
	this->nCapacity			= capacity;
	setIncrementCapacityFactor(incrCapacityFactor);
	currentSurfelIndex		= -1;

	normalAxesSystem.normal = normalAxesSystem.axisOne = normalAxesSystem.axisTwo = Vector3D(0,0,0);
	this->nativePropertyDescriptor = SurfelInterface::NO_PROPERTIES;

	setPropertyDescriptor(nativePropertyDescriptor);

}


SurfelCollection::~SurfelCollection() {
	int i;

	if (surfels) {
		for (i = 0; i < nOfSurfels; i++) {
			delete AT(surfels,i);
		}
		delete surfels;
	}
	if (positions) {
		delete positions;
	}
	if (normals) {
		delete normals;
	}
	if (axes) {
		delete axes;
	}
	if (radius) {
		delete radius;
	}
	if (diffuseColors) {
		delete diffuseColors;
	}
	if (specularColors) {
		delete specularColors;
	}
	if (ambientCoefficients) {
		delete ambientCoefficients;
	}
	if (diffuseCoefficients) {
		delete diffuseCoefficients;
	}
	if (specularCoefficients) {
		delete specularCoefficients;
	}
	if (shininess) {
		delete shininess;
	}
	if (textureCoordinates) {
		delete textureCoordinates;
	}
	if (flags) {
		delete flags;
	}
	if (associatedSurfels) {
		for (i = 0; i < nOfSurfels; i++) {
			AT(associatedSurfels,i).destroy();
		}
		delete associatedSurfels;
	}
	if (details) {
		delete details;
	}

}

void SurfelCollection::setPropertyDescriptor (const SurfelInterface::PropertyDescriptor newPropertyDescriptor, bool emitSignal) {

	if (nativePropertyDescriptor == newPropertyDescriptor) {
		return;
	}

	int i;

	computedPropertyDescriptor = 0;

	StatusBar::getInstance()->showMessage ("Converting surfel properties...", -1);

	// *************************************
	// CREATE AND INITIALIZE DATA STRUCTURES
	// *************************************
	
	// SURFEL REFERENCES
	if (!surfels) {
		surfels = new std::vector<SurfelInterface*>(nCapacity);
	}
	
	// POSITION
	if ((newPropertyDescriptor & SurfelInterface::POSITION) && !positions) {
		positions = new std::vector<Vector3D>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(positions,i) = (this->*_getPosition)(i);
		}
	}

	// NORMAL
	if ((newPropertyDescriptor & SurfelInterface::NORMAL) && !normals) {
		normals = new std::vector<Vector3D>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(normals,i) = (this->*_getNormal)(i);
		}
	}

	// AXES
	if ((newPropertyDescriptor & SurfelInterface::TANGENT_AXES) && !axes) {
		axes = new std::vector<Axes>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(axes,i).axisOne = (this->*_getAxisOne)(i);
			AT(axes,i).axisTwo = (this->*_getAxisTwo)(i);
		}
	}

	// RADIUS
	if ((newPropertyDescriptor & SurfelInterface::RADIUS) && !radius) {
		radius = new std::vector<float>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(radius,i) = (this->*_getRadius)(i);
		}
	}

	// DIFFUSE COLOR
	if ((newPropertyDescriptor & SurfelInterface::DIFFUSE_COLOR) && !diffuseColors) {
		diffuseColors = new std::vector<QRgb>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(diffuseColors,i) = (this->*_getDiffuseColor)(i);
		}
	}

	// SPECULAR_COLOR
	if ((newPropertyDescriptor & SurfelInterface::SPECULAR_COLOR) && !specularColors) {
		specularColors = new std::vector<QRgb>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(specularColors,i) = (this->*_getSpecularColor)(i);
		}
	}

	// AMBIENT_COEFFICIENT
	if ((newPropertyDescriptor & SurfelInterface::AMBIENT_COEFFICIENT) && !ambientCoefficients) {
		ambientCoefficients = new std::vector<float>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(ambientCoefficients,i) = (this->*_getAmbientCoefficient)(i);
		}
	}

	// DIFFUSE_COEFFICIENT
	if ((newPropertyDescriptor & SurfelInterface::DIFFUSE_COEFFICIENT) && !diffuseCoefficients) {
		diffuseCoefficients = new std::vector<float>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(diffuseCoefficients,i) = (this->*_getDiffuseCoefficient)(i);
		}
	}

	// SPECULAR_COEFFICIENT
	if ((newPropertyDescriptor & SurfelInterface::SPECULAR_COEFFICIENT) && !specularCoefficients) {
		specularCoefficients = new std::vector<float>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(specularCoefficients,i) = (this->*_getSpecularCoefficient)(i);
		}
	}

	// SHININESS
	if ((newPropertyDescriptor & SurfelInterface::SHININESS) && !shininess) {
		shininess = new std::vector<float>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(shininess,i) = (this->*_getShininess)(i);
		}
	}

	// TEXTURE_COORDINATE
	if ((newPropertyDescriptor & SurfelInterface::TEXTURE_COORDINATE) && !textureCoordinates) {
		textureCoordinates = new std::vector<MyDataTypes::TextureCoordinate>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(textureCoordinates,i) = (this->*_getTextureCoordinate)(i);
		}
	}

	// FLAGS
	if ((newPropertyDescriptor & SurfelInterface::FLAGS) && !flags) {
		flags = new std::vector<SurfelInterface::Flags>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(flags,i) = (this->*_getFlags)(i);
		}
	}

	// ASSOCIATED_SURFELS
	if ((newPropertyDescriptor & SurfelInterface::ASSOCIATED_SURFELS) && !associatedSurfels) {
		associatedSurfels = new std::vector<AssociatedSurfels>(nCapacity);
		for (int i = 0; i < nCapacity; i++) {
				AT(associatedSurfels,i).init();
		}
	}

	// DETAIL
	if ((newPropertyDescriptor & SurfelInterface::DETAIL) && !details) {
		details = new std::vector<Detail>(nCapacity);
		for (i = 0; i < nOfSurfels; i++) {
			AT(details,i).detailVector = (this->*_getDetailVector)(i);
			AT(details,i).detailCoefficient = (this->*_getDetailCoefficient)(i);
		}
	}


	// ************************************************
	// DELETE DATA STRUCTURES AND SET FUNCTION POINTERS
	// ************************************************

	// POSITION
	if (newPropertyDescriptor & SurfelInterface::POSITION) {
		_setPosition = &SurfelCollection::setPosition;
		_getPosition = &SurfelCollection::getPositionNative;
	}
	else { 
		if (positions) {
			delete positions;
			positions = NULL;
		}
		_setPosition = &SurfelCollection::setPositionEmpty;
		_getPosition = &SurfelCollection::getPositionEmpty;
	}

	// NORMAL
	if (newPropertyDescriptor & SurfelInterface::NORMAL) {

		_getNormal = &SurfelCollection::getNormalNative;

		if (newPropertyDescriptor & SurfelInterface::TANGENT_AXES) {
			if (newPropertyDescriptor & SurfelInterface::RADIUS) {
				// NORMAL & TANGENT_AXES & RADIUS
				// compute axes orthogonal to normal
				// length of axes is set equal to radius
				_setNormal = &SurfelCollection::setNormalComputeAxesFromNormalAndRadius;
			}
			else {
				// NORMAL & TANGENT_AXES & ~RADIUS
				// compute axes orthogonal to normal
				// length of axes is computed as average of old axes length
				_setNormal = &SurfelCollection::setNormalComputeAxesFromNormal;
			}
		}
		else {
			_setNormal = &SurfelCollection::setNormal;
		}

	}
	else {
		
		if (normals) {
			delete normals; 
			normals = NULL;
		}

		if (newPropertyDescriptor & SurfelInterface::TANGENT_AXES) {

			if (newPropertyDescriptor & SurfelInterface::RADIUS) {
				// recompute tangent axes from normal
				_setNormal = &SurfelCollection::setTangentAxesFromNormalAndRadius;
			}
			else {
				_setNormal = &SurfelCollection::setTangentAxesFromNormal;
			}

			// compute normal from tangent axes
			_getNormal = &SurfelCollection::getNormalDerivedFromTangentAxes;

			computedPropertyDescriptor |= SurfelInterface::NORMAL;
		}
		else {	
			_getNormal = &SurfelCollection::getNormalEmpty;
			_setNormal = &SurfelCollection::setNormalEmpty;
		}
	}

	// AXES
	if (newPropertyDescriptor & SurfelInterface::TANGENT_AXES) {

		_getAxisOne = &SurfelCollection::getAxisOneNative;
		_getAxisTwo = &SurfelCollection::getAxisTwoNative;

		if (newPropertyDescriptor & SurfelInterface::RADIUS) {
			
			if (newPropertyDescriptor & SurfelInterface::NORMAL) {
				// TANGENT_AXES & RADIUS & NORMAL
				// ->compute radius and normal from tangent axes
				_setTangentAxes = &SurfelCollection::setTangentAxesComputeNormal_Radius;
				_setNormalTangentAxesSystem = &SurfelCollection::setNormalTangentAxesSystemComputeRadius;
			}
			else {
				// TANGENT_AXES & RADIUS & ~NORMAL
				// ->compute radius from tangent axes
				_setTangentAxes = &SurfelCollection::setTangentAxesComputeRadius;
				_setNormalTangentAxesSystem = &SurfelCollection::setTangentAxesComputeRadius;			
			}
		}
		else {
			
			if (newPropertyDescriptor & SurfelInterface::NORMAL) {
				// TANGENT_AXES & NORMAL
				// -> compute normal from tangent axes
				_setTangentAxes = &SurfelCollection::setTangentAxesComputeNormal;
				_setNormalTangentAxesSystem = &SurfelCollection::setNormalTangentAxesSystem;
			}
			else {
				// TANGENT_AXES only
				_setTangentAxes = &SurfelCollection::setTangentAxes;
				_setNormalTangentAxesSystem = &SurfelCollection::setTangentAxes;			
			}
		}
		
	}
	else {
		
		if (axes) {
			delete axes;
			axes = NULL;
		}
		
		_setTangentAxes = &SurfelCollection::setTangentAxesEmpty;
		_setNormalTangentAxesSystem = &SurfelCollection::setNormalTangentAxesSystemEmpty;

		if (newPropertyDescriptor & (SurfelInterface::NORMAL | SurfelInterface::RADIUS)) {
			// compute axes from normal and radius
			_getAxisOne = &SurfelCollection::getAxesOneDerivedFromNormal;
			_getAxisTwo = &SurfelCollection::getAxesTwoDerivedFromNormal;

			computedPropertyDescriptor |= SurfelInterface::TANGENT_AXES;
		}
		else {
			_getAxisOne		= &SurfelCollection::getAxisOneEmpty;
			_getAxisTwo		= &SurfelCollection::getAxisTwoEmpty;
			
		}
	}

	// RADIUS
	if (newPropertyDescriptor & SurfelInterface::RADIUS) {
		
		if (newPropertyDescriptor & SurfelInterface::TANGENT_AXES) {
			// scale axes if radius is set
			_setRadius = &SurfelCollection::setRadiusScaleAxes;
		}
		else {
			_setRadius = &SurfelCollection::setRadius;
		}

		_getRadius = &SurfelCollection::getRadiusNative;
	}
	else {
		if (radius) {
			delete radius;
			radius = NULL;
		}

		if (newPropertyDescriptor & SurfelInterface::TANGENT_AXES) {
			// compute radius from tangent axes
			_getRadius = &SurfelCollection::getRadiusDerivedFromTangentAxes;
			// scale axes
			_setRadius = &SurfelCollection::scaleAxes;

			computedPropertyDescriptor |= SurfelInterface::RADIUS;
		}
		else {
			_setRadius = &SurfelCollection::setRadiusEmpty;
			_getRadius = &SurfelCollection::getRadiusEmpty;
		}
	}

	// DIFFUSE COLOR
	if (newPropertyDescriptor & SurfelInterface::DIFFUSE_COLOR) {

		_setDiffuseColor = &SurfelCollection::setDiffuseColor;
		_getDiffuseColor = &SurfelCollection::getDiffuseColorNative;
	}
	else {

		if (diffuseColors) {
			delete diffuseColors;
			diffuseColors = NULL;
		}
		_setDiffuseColor = &SurfelCollection::setDiffuseColorEmpty;
		_getDiffuseColor = &SurfelCollection::getDiffuseColorEmpty;
	}

	// SPECULAR_COLOR
	if (newPropertyDescriptor & SurfelInterface::SPECULAR_COLOR) {

		_setSpecularColor = &SurfelCollection::setSpecularColor;
		_getSpecularColor = &SurfelCollection::getSpecularColorNative;
	}
	else {

		if (specularColors) {
			delete specularColors;
			specularColors = NULL;
		}

		_setSpecularColor = &SurfelCollection::setSpecularColorEmpty;
		_getSpecularColor = &SurfelCollection::getSpecularColorEmpty;
	}

	// AMBIENT_COEFFICIENT
	if (newPropertyDescriptor & SurfelInterface::AMBIENT_COEFFICIENT) {
		
		_setAmbientCoefficient = &SurfelCollection::setAmbientCoefficient;
		_getAmbientCoefficient = &SurfelCollection::getAmbientCoefficientNative;
	}
	else {

		if (ambientCoefficients) {
			delete ambientCoefficients;
			ambientCoefficients = NULL;
		}

		_setAmbientCoefficient = &SurfelCollection::setAmbientCoefficientEmpty;
		_getAmbientCoefficient = &SurfelCollection::getAmbientCoefficientEmpty;
	}

	// DIFFUSE_COEFFICIENT
	if (newPropertyDescriptor & SurfelInterface::DIFFUSE_COEFFICIENT) {

		_setDiffuseCoefficient = &SurfelCollection::setDiffuseCoefficient;
		_getDiffuseCoefficient = &SurfelCollection::getDiffuseCoefficientNative;
	}
	else {

		if (diffuseCoefficients) {
			delete diffuseCoefficients;
			diffuseCoefficients = NULL;
		}

		_setDiffuseCoefficient = &SurfelCollection::setDiffuseCoefficientEmpty;
		_getDiffuseCoefficient = &SurfelCollection::getDiffuseCoefficientEmpty;
	}

	// SPECULAR_COEFFICIENT
	if (newPropertyDescriptor & SurfelInterface::SPECULAR_COEFFICIENT) {

		_setSpecularCoefficient = &SurfelCollection::setSpecularCoefficient;
		_getSpecularCoefficient = &SurfelCollection::getSpecularCoefficientNative;
	}
	else {

		if (specularCoefficients) {
			delete specularCoefficients;
			specularCoefficients = NULL;
		}

		_setSpecularCoefficient = &SurfelCollection::setSpecularCoefficientEmpty;
		_getSpecularCoefficient = &SurfelCollection::getSpecularCoefficientEmpty;
	}

	// SHININESS
	if (newPropertyDescriptor & SurfelInterface::SHININESS) {

		_setShininess = &SurfelCollection::setShininess;
		_getShininess = &SurfelCollection::getShininessNative;
	}
	else {

		if (shininess) {
			delete shininess;
			shininess = NULL;
		}
		
		_setShininess = &SurfelCollection::setShininessEmpty;
		_getShininess = &SurfelCollection::getShininessEmpty;
	}

	// TEXTURE_COORDINATE
	if (newPropertyDescriptor & SurfelInterface::TEXTURE_COORDINATE) {

		_setTextureCoordinate = &SurfelCollection::setTextureCoordinate;
		_getTextureCoordinate = &SurfelCollection::getTextureCoordinateNative;
	}
	else {

		if (textureCoordinates) {
			delete textureCoordinates;
			textureCoordinates = NULL;
		}

		_setTextureCoordinate = &SurfelCollection::setTextureCoordinateEmpty;
		_getTextureCoordinate = &SurfelCollection::getTextureCoordinateEmpty;
	}

	// FLAGS
	if (newPropertyDescriptor & SurfelInterface::FLAGS) {

		_setFlags = &SurfelCollection::setFlags;
		_setFlagOn = &SurfelCollection::setFlagOn;
		_getFlags = &SurfelCollection::getFlagsNative;
		_isFlagOn = &SurfelCollection::isFlagOnNative;
		_isSelected = &SurfelCollection::isSelectedNative;
	}
	else {

		if (flags) {
			delete flags;
			flags = NULL;
		}

		_setFlags = &SurfelCollection::setFlagsEmpty;
		_setFlagOn = &SurfelCollection::setFlagOnEmpty;
		_getFlags = &SurfelCollection::getFlagsEmpty;
		_isFlagOn = &SurfelCollection::isFlagOnEmpty;
		_isSelected = &SurfelCollection::isSelectedEmpty;
	}


	// ASSOCIATED_SURFELS
	if (newPropertyDescriptor & SurfelInterface::ASSOCIATED_SURFELS) {

		_addAssociatedSurfel    = &SurfelCollection::addAssociatedSurfel;
		_clearAssociatedSurfels = &SurfelCollection::clearAssociatedSurfels;
		_getAssociatedSurfel	= &SurfelCollection::getAssociatedSurfelNative;
		_getNofAssociatedSurfels = &SurfelCollection::getNofAssociatedSurfelsComputed;

	}
	else {
		if (associatedSurfels) {
			for (int i = 0; i < nOfSurfels; i++) {
				AT(associatedSurfels,i).destroy();
			}
			delete associatedSurfels;
			associatedSurfels = NULL;
		}

		_addAssociatedSurfel    = &SurfelCollection::addAssociatedSurfelEmpty;
		_clearAssociatedSurfels = &SurfelCollection::clearAssociatedSurfelsEmpty;
		_getAssociatedSurfel	= &SurfelCollection::getAssociatedSurfelEmpty;
		_getNofAssociatedSurfels = &SurfelCollection::getNofAssociatedSurfelsEmpty;
	}

	// DETAIL
	if (newPropertyDescriptor & SurfelInterface::DETAIL) {

		_setDetailVector = &SurfelCollection::setDetailVector;
		_getDetailVector = &SurfelCollection::getDetailVectorNative;
		_setDetailCoefficient = &SurfelCollection::setDetailCoefficient;
		_getDetailCoefficient = &SurfelCollection::getDetailCoefficientNative;
	}
	else {

		if (details) {
			delete details;
			details = NULL;
		}

		_setDetailVector = &SurfelCollection::setDetailVectorEmpty;
		_getDetailVector = &SurfelCollection::getDetailVectorEmpty;
		_setDetailCoefficient = &SurfelCollection::setDetailCoefficientEmpty;
		_getDetailCoefficient = &SurfelCollection::getDetailCoefficientEmpty;
	}

	nativePropertyDescriptor = newPropertyDescriptor;

	//preserve iterator
	int oldSurfelIndex = currentSurfelIndex;

	if (emitSignal) {
		emit surfelTypeChanged (nativePropertyDescriptor);
	}

	currentSurfelIndex = oldSurfelIndex;

	StatusBar::getInstance()->showMessage ("Surfel properties converted.");
}

SurfelInterface::PropertyDescriptor SurfelCollection::getPropertyDescriptor() const {
	return nativePropertyDescriptor;
}

SurfelInterface::PropertyDescriptor SurfelCollection::getComputedPropertyDescriptor() const {
	return computedPropertyDescriptor;
}

void SurfelCollection::setIncrementCapacityFactor(const float incrCapacityFactor) {
	this->incrCapacityFactor = (incrCapacityFactor >= 0.0f) ? incrCapacityFactor : 1.0f;
}


// ***********
// Data access
// ***********

SurfelInterface* SurfelCollection::getFirstSurfel() {

	if (nOfSurfels > 0) {
		currentSurfelIndex = 0;
		return AT(surfels, currentSurfelIndex);
	}
	else {
		currentSurfelIndex = -1;
		return NULL;
	}
}

SurfelInterface* SurfelCollection::getCurrentSurfel() const {
	if (currentSurfelIndex >= 0) {
		return AT(surfels, currentSurfelIndex);
	}
	else {
		return NULL;
	}
}

SurfelInterface* SurfelCollection::getNextSurfel() {
	if (currentSurfelIndex + 1 < nOfSurfels) {
		currentSurfelIndex++;
		return AT(surfels, currentSurfelIndex);
	}
	else {
		return NULL;
	}
}

SurfelInterface *SurfelCollection::getSurfel(const int index) {
	return AT(surfels, index);
}

std::vector<SurfelInterface*>* SurfelCollection::getSurfelVector() {

	return surfels;
}

std::vector<Vector3D>* SurfelCollection::getPositionVector() {

	return positions;
}

std::vector<Vector3D>* SurfelCollection::getNormalVector() {

	return normals;
}

// ********************
// Surfel manipulations
// ********************
bool SurfelCollection::contains (const SurfelInterface *surfel) {

	return (surfel->index < nOfSurfels && AT(surfels, surfel->index) == surfel);
}

SurfelInterface *SurfelCollection::addSurfel (const Vector3D position, const Vector3D normal,
										 const Vector3D axisOne, const Vector3D axisTwo,
										 const float r, const QRgb diffuseColor, const QRgb specularColor, 
										 const SurfelInterface::Flags flgs,
										 const float ambientCoefficient, const float diffuseCoefficient,
										 const float specularCoefficient, const float shin,
										 const MyDataTypes::TextureCoordinate textureCoordinate,
										 const bool emitSignal) {

	SurfelInterface *newSurfel;

	if (nOfSurfels >= nCapacity) {
		reserve(nOfSurfels + 1 + (nCapacity*incrCapacityFactor));
	}
	newSurfel = createNewSurfel(nOfSurfels);

	if (positions)				AT(positions, nOfSurfels) = position;
	if (normals)				AT(normals, nOfSurfels) = normal;
	if (axes) {
		AT(axes, nOfSurfels).axisOne = axisOne;
		AT(axes, nOfSurfels).axisTwo = axisTwo;
	}
	if (radius) 				AT(radius, nOfSurfels) = r;
	if (diffuseColors)			AT(diffuseColors, nOfSurfels) = diffuseColor;
	if (specularColors) 		AT(specularColors, nOfSurfels) = specularColor;
	if (ambientCoefficients)	AT(ambientCoefficients, nOfSurfels) = ambientCoefficient;
	if (diffuseCoefficients) 	AT(diffuseCoefficients, nOfSurfels) = diffuseCoefficient;
	if (specularCoefficients) 	AT(specularCoefficients, nOfSurfels) = specularCoefficient;
	if (shininess)				AT(shininess, nOfSurfels) = shin;
	if (textureCoordinates) 	AT(textureCoordinates, nOfSurfels) = textureCoordinate;
	if (flags) 					AT(flags, nOfSurfels) = flgs;

	nOfSurfels++;

	if (emitSignal == true) {
		emit surfelCollectionChanged();
	}
	return newSurfel;
}

SurfelInterface *SurfelCollection::addSurfel (const bool emitSignal) {

	SurfelInterface *newSurfel;

	nOfSurfels++;
	if (nOfSurfels > nCapacity) {
		reserve(nOfSurfels + (nCapacity*incrCapacityFactor));
	}
	newSurfel = createNewSurfel(nOfSurfels-1);

	if (flags) {
		AT(flags, nOfSurfels-1) = SurfelInterface::NO_FLAGS;
	}

	if (emitSignal == true) {
		emit surfelCollectionChanged();
	}
	return newSurfel;

}


SurfelInterface *SurfelCollection::addSurfel (const SurfelInterface *surfel, const bool emitSignal) {

	SurfelInterface *newSurfel;

	nOfSurfels++;
	if (nOfSurfels > nCapacity) {
		reserve(nOfSurfels + (nCapacity*incrCapacityFactor));
	}
	newSurfel = createNewSurfel(nOfSurfels-1);
	this->copySurfelProperties(nOfSurfels-1, surfel);

	if (emitSignal == true) {
		emit surfelCollectionChanged();
	}
	return newSurfel;

}

void SurfelCollection::addSurfels (std::vector<SurfelInterface*> *newSurfels, const bool emitSignal) {

	std::vector<SurfelInterface*>::iterator iter_surfels;

	reserve(nOfSurfels + newSurfels->size());

	for (iter_surfels = newSurfels->begin(); iter_surfels != newSurfels->end(); ++iter_surfels) {
		createNewSurfel(nOfSurfels);
		this->copySurfelProperties(nOfSurfels, *iter_surfels);
		nOfSurfels++;
	}

	if (emitSignal) {
		emit surfelCollectionChanged();
	}
}

void SurfelCollection::merge (SurfelCollection *newSurfels, const bool emitSignal) {

	int nOfNewSurfels = newSurfels->getNofSurfels();
	SurfelInterface *surfel;

	reserve(nOfNewSurfels + nOfSurfels);
	
	for (surfel = newSurfels->getFirstSurfel(); surfel; surfel = newSurfels->getNextSurfel()) {
		if (associatedSurfels) {
			AT(associatedSurfels, nOfSurfels).init();
		}
		this->copySurfelProperties(nOfSurfels, surfel);
		surfel->init(nOfSurfels, this);
		AT(surfels, nOfSurfels) = surfel;
		nOfSurfels++;
	}

	newSurfels->detach();

	if (emitSignal) {
		emit surfelCollectionChanged();
	}
}


void SurfelCollection::resize(const int newNofSurfels, const bool emitSignal) {

	if (newNofSurfels == nOfSurfels) {
		return;
	}
	
	if (newNofSurfels < nOfSurfels) {
		// data loss
		for (int i = newNofSurfels; i < nOfSurfels; i++) {
			delete AT(surfels, i);
		}
		if (associatedSurfels) {
			for (int i = newNofSurfels; i < nOfSurfels; i++) {
				AT(associatedSurfels,i).destroy();
			}
		}

		if (currentSurfelIndex > newNofSurfels) {
			currentSurfelIndex = -1;
		}

	}
	else {

		reserve(newNofSurfels);
	
		for (int i = nOfSurfels; i < newNofSurfels; i++) {
			createNewSurfel(i);
		}
	}

	nOfSurfels = newNofSurfels;

	if (emitSignal) {
		emit surfelCollectionChanged();
	}
	
}

void SurfelCollection::reserve(const int newCapacity) {

	if (newCapacity > nCapacity) {

		surfels->resize(newCapacity);
		if (positions)				positions->resize(newCapacity);
		if (normals)				normals->resize(newCapacity);
		if (axes)					axes->resize(newCapacity);
		if (radius) 				radius->resize(newCapacity);
		if (diffuseColors)			diffuseColors->resize(newCapacity); 
		if (specularColors) 		specularColors->resize(newCapacity);
		if (ambientCoefficients)	ambientCoefficients->resize(newCapacity);
		if (diffuseCoefficients) 	diffuseCoefficients->resize(newCapacity);
		if (specularCoefficients) 	specularCoefficients->resize(newCapacity);
		if (shininess)				shininess->resize(newCapacity);
		if (textureCoordinates) 	textureCoordinates->resize(newCapacity);
		if (flags) 					flags->resize(newCapacity);
		if (associatedSurfels) 		associatedSurfels->resize(newCapacity);
		if (details) 				details->resize(newCapacity);

		nCapacity = newCapacity;	
		
	}
}

void SurfelCollection::removeCurrentSurfel() {
	if (currentSurfelIndex >= 0) {
		removeSurfel(currentSurfelIndex);

		if (currentSurfelIndex == nOfSurfels) {
			// set to last surfel
			currentSurfelIndex--;
		}
	}	
}

bool SurfelCollection::removeSurfel (SurfelInterface *surfel, const bool emitSignal) {
	
	if (surfel->index < nOfSurfels && AT(surfels, surfel->index) == surfel) {
		// collection contains surfel
		removeSurfel(surfel->index);

		if (emitSignal) {
			emit surfelCollectionChanged();
		}
		return true;
	}
	
	return false;
}

void SurfelCollection::removeSurfels (const QList<SurfelInterface> *removeSurfels, const bool emitSignal) {

	QListIterator<SurfelInterface> it(*removeSurfels);

	for ( ; it.current() != 0; ++it) {
		removeSurfel(it.current()->index);
	}

	if (emitSignal == true) {
		emit surfelCollectionChanged();
	}

}

void SurfelCollection::removeSurfels (const std::vector<SurfelInterface *>* removeSurfels, const bool emitSignal) {

	std::vector<SurfelInterface*>::const_iterator iter_surfels;

	for (iter_surfels = removeSurfels->begin(); iter_surfels != removeSurfels->end(); ++iter_surfels) {
		removeSurfel((*iter_surfels)->index);
	}

	if (emitSignal == true) {
		emit surfelCollectionChanged();
	}
}

void SurfelCollection::copy (SurfelCollection *sourceCollection, const bool emitSignal) {

	SurfelInterface  *surfel;

	setPropertyDescriptor(sourceCollection->nativePropertyDescriptor);
	clear();

	reserve(sourceCollection->getNofSurfels());

	for (surfel = sourceCollection->getFirstSurfel(); surfel; surfel = sourceCollection->getNextSurfel()) {
		addSurfel(surfel, false);		// make a copy and insert the surfel
	}

	if (emitSignal) {
		emit surfelCollectionChanged();
	}

}

void SurfelCollection::clear (const bool emitSignal) {

	
	for (int i = 0; i < nOfSurfels; i++) {
		delete AT(surfels,i);
	}

	if (associatedSurfels) {
		for (int i = 0; i < nOfSurfels; i++) {
			AT(associatedSurfels,i).destroy();
		}
	}
	
	nOfSurfels = 0;
	currentSurfelIndex = -1;
		
	if (emitSignal == true) {
		emit surfelCollectionChanged();
	}
}

NeighbourHood* SurfelCollection::buildNeighbourHood () {

	int nofSurfels = getNofSurfels();
	NeighbourHood *neighbourHood;

	if (positions && (positions->size() > 0) ) {	

		neighbourHood = new NeighbourHood (&(*(positions->begin())), nofSurfels);
		return neighbourHood;
	}
	else {
		return NULL;
	}
}


// *****************
// protected methods
// *****************
void SurfelCollection::detach() {

	if (associatedSurfels) {
		for (int i = 0; i < nOfSurfels; i++) {
			AT(associatedSurfels,i).destroy();
		}
	}

	nOfSurfels = 0;
	currentSurfelIndex = -1;

}


// *************************
// Function implementations
// *************************


Vector3D SurfelCollection::getPositionNative(const int index) {
	return AT(positions,index);
}

Vector3D SurfelCollection::getNormalNative(const int index) {
	return AT(normals,index);
}

Vector3D SurfelCollection::getNormalDerivedFromTangentAxes(const int index) {
	Vector3D normal;
	normal = Vector3D::crossProduct(AT(axes,index).axisOne, AT(axes,index).axisTwo);
	normal.normalize();
	return normal;
}

Vector3D SurfelCollection::getAxisOneNative(const int index) {
	return AT(axes,index).axisOne;
}

Vector3D SurfelCollection::getAxisTwoNative(const int index) {
	return AT(axes,index).axisTwo;
}

Vector3D SurfelCollection::getAxesOneDerivedFromNormal(const int index) {
	if (normalAxesSystem.normal != AT(normals,index) || normalAxesSystem.radius != AT(radius,index)) {
		//update normal axes system
		normalAxesSystem.normal = AT(normals,index);
		normalAxesSystem.radius = AT(radius,index);
		normalAxesSystem.normal.getTwoOrthogonals (normalAxesSystem.axisOne, normalAxesSystem.axisTwo);
		normalAxesSystem.axisOne.normalize();
		normalAxesSystem.axisTwo.normalize();
		normalAxesSystem.axisOne *= normalAxesSystem.radius;
		normalAxesSystem.axisTwo *= normalAxesSystem.radius;
	}

	return normalAxesSystem.axisOne;
}

Vector3D SurfelCollection::getAxesTwoDerivedFromNormal(const int index) {

	if (normalAxesSystem.normal != AT(normals,index) || normalAxesSystem.radius != AT(radius,index)) {
		//update normal axes system
		normalAxesSystem.normal = AT(normals,index);
		normalAxesSystem.radius = AT(radius,index);
		normalAxesSystem.normal.getTwoOrthogonals (normalAxesSystem.axisOne, normalAxesSystem.axisTwo);
		normalAxesSystem.axisOne.normalize();
		normalAxesSystem.axisTwo.normalize();
		normalAxesSystem.axisOne *= normalAxesSystem.radius;
		normalAxesSystem.axisTwo *= normalAxesSystem.radius;
	}

	return normalAxesSystem.axisTwo;
}

float SurfelCollection::getRadiusNative(const int index) {
	return AT(radius,index);
}

float SurfelCollection::getRadiusDerivedFromTangentAxes(const int index) {
	float len1 = AT(axes,index).axisOne.getLength();
	float len2 = AT(axes,index).axisTwo.getLength();
	return (len1 + len2) / 2.0f;
}

QRgb SurfelCollection::getDiffuseColorNative(const int index) {
	return AT(diffuseColors,index);
}

QRgb SurfelCollection::getSpecularColorNative(const int index) {
	return AT(specularColors,index);
}

bool SurfelCollection::isFlagOnNative(const int index, const SurfelInterface::Flags flgs)  {
	return (AT(flags,index) & flgs) == flgs;
}

bool SurfelCollection::isSelectedNative(const int index) {
	return AT(flags,index) & (SurfelInterface::SELECTED1 | SurfelInterface::SELECTED2 | SurfelInterface::SELECTED3);
}

SurfelInterface::Flags SurfelCollection::getFlagsNative(const int index) {
	return AT(flags,index);
}

float SurfelCollection::getAmbientCoefficientNative(const int index) {
	return AT(ambientCoefficients,index);
}

float SurfelCollection::getDiffuseCoefficientNative(const int index) {
	return AT(diffuseCoefficients,index);
}

float SurfelCollection::getSpecularCoefficientNative(const int index) {
	return AT(specularCoefficients,index);
}

float SurfelCollection::getShininessNative(const int index) {
	return AT(shininess,index);
}

MyDataTypes::TextureCoordinate	SurfelCollection::getTextureCoordinateNative(const int index) {
	return AT(textureCoordinates,index);
}

unsigned int SurfelCollection::getNofAssociatedSurfelsComputed(const int index) {
	return AT(associatedSurfels,index).getNOfAssociatedSurfels();
}

SurfelInterface* SurfelCollection::getAssociatedSurfelNative(const int index, const int idx) {
	return AT(associatedSurfels,index).getAssociatedSurfel(idx);
}

Vector3D SurfelCollection::getDetailVectorNative(const int index) {
	return AT(details,index).detailVector;
}

float SurfelCollection::getDetailCoefficientNative(const int index) {
	return AT(details,index).detailCoefficient;
}

void SurfelCollection::setDetailVector(const int index, const Vector3D detailVector) {
	AT(details,index).detailVector = detailVector;
}

void SurfelCollection::setDetailCoefficient(const int index, const float detailCoefficient) {
	AT(details,index).detailCoefficient = detailCoefficient;
}

void SurfelCollection::setPosition(const int index, const Vector3D newPosition) {
	AT(positions,index) = newPosition;
}

void SurfelCollection::setNormal(const int index, const Vector3D newNormal) {
	AT(normals,index) = newNormal;
}

void SurfelCollection::setNormal(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(normals,index) = newNormal;
}

void SurfelCollection::setNormalComputeAxesFromNormalAndRadius(const int index, const Vector3D newNormal) {
	Axes newAxes;
	float newLength = AT(radius,index);

	newNormal.getTwoOrthogonals (newAxes.axisOne, newAxes.axisTwo);
	newAxes.axisOne.normalize();
	newAxes.axisTwo.normalize();
	newAxes.axisOne *= newLength;
	newAxes.axisTwo *= newLength;
	
	AT(normals,index) = newNormal;
	AT(axes,index) = newAxes;
}

void SurfelCollection::setNormalComputeAxesFromNormal(const int index, const Vector3D newNormal) {
	Axes newAxes, oldAxes;
	oldAxes = AT(axes,index);
	float newLength = (oldAxes.axisOne.getLength() + oldAxes.axisTwo.getLength()) / 2.0f;

	newNormal.getTwoOrthogonals (newAxes.axisOne, newAxes.axisTwo);
	newAxes.axisOne.normalize();
	newAxes.axisTwo.normalize();
	newAxes.axisOne *= newLength;
	newAxes.axisTwo *= newLength;
	
	AT(normals,index) = newNormal;
	AT(axes,index) = newAxes;
}

void SurfelCollection::setTangentAxes(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;
}


void SurfelCollection::setTangentAxes(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;
}

void SurfelCollection::setNormalTangentAxesSystem(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(normals,index) = newNormal;
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;
}

void SurfelCollection::setNormalTangentAxesSystemComputeRadius(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(normals,index) = newNormal;
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;

	float len1 = AT(axes,index).axisOne.getLength();
	float len2 = AT(axes,index).axisTwo.getLength();
	float r = (len1 + len2) / 2.0f;
	AT(radius,index) = r;
}

void SurfelCollection::setTangentAxesFromNormalAndRadius(const int index, const Vector3D newNormal) {
	Axes newAxes;

	newNormal.getTwoOrthogonals (newAxes.axisOne, newAxes.axisTwo);
	newAxes.axisOne.normalize();
	newAxes.axisTwo.normalize();
	newAxes.axisOne *= AT(radius,index);
	newAxes.axisTwo *= AT(radius,index);
	
	AT(axes,index) = newAxes;
}

void SurfelCollection::setTangentAxesFromNormal(const int index, const Vector3D newNormal) {
	Axes newAxes;
	float len;

	len = (AT(axes,index).axisOne.getLength() + AT(axes,index).axisTwo.getLength()) / 2.0f;
	newNormal.getTwoOrthogonals (newAxes.axisOne, newAxes.axisTwo);
	newAxes.axisOne.normalize();
	newAxes.axisTwo.normalize();
	newAxes.axisOne *= len;
	newAxes.axisTwo *= len;
	
	AT(axes,index) = newAxes;
}

void SurfelCollection::setTangentAxesComputeRadius(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;

	float len1 = AT(axes,index).axisOne.getLength();
	float len2 = AT(axes,index).axisTwo.getLength();
	float r = (len1 + len2) / 2.0f;
	AT(radius,index) = r;
}

void SurfelCollection::setTangentAxesComputeRadius(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;

	float len1 = AT(axes,index).axisOne.getLength();
	float len2 = AT(axes,index).axisTwo.getLength();
	AT(radius,index) = (len1 + len2) / 2.0f;
}

void SurfelCollection::setTangentAxesComputeNormal(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;

	Vector3D normal;
	normal = Vector3D::crossProduct(newAxisOne, newAxisTwo);
	normal.normalize();
	AT(normals,index) = normal;
}

void SurfelCollection::setTangentAxesComputeNormal_Radius(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	AT(axes,index).axisOne = newAxisOne;
	AT(axes,index).axisTwo = newAxisTwo;

	Vector3D normal;
	normal = Vector3D::crossProduct(newAxisOne, newAxisTwo);
	normal.normalize();
	AT(normals,index) = normal;

	float len1 = AT(axes,index).axisOne.getLength();
	float len2 = AT(axes,index).axisTwo.getLength();
	AT(radius,index) = (len1 + len2) / 2.0f;
}

void SurfelCollection::setRadius(const int index, float r) {
	AT(radius,index) = r;
}

void SurfelCollection::setRadiusScaleAxes(const int index, float r) {
	AT(radius,index) = r;
	AT(axes,index).axisOne.normalize();
	AT(axes,index).axisOne *= r;
	AT(axes,index).axisTwo.normalize();
	AT(axes,index).axisTwo *= r;
}

void SurfelCollection::scaleAxes(const int index, float r) {

	AT(axes,index).axisOne.normalize();
	AT(axes,index).axisOne *= r;
	AT(axes,index).axisTwo.normalize();
	AT(axes,index).axisTwo *= r;
}

void SurfelCollection::setDiffuseColor(const int index, const QRgb newDiffuseColor) {
	AT(diffuseColors,index) = newDiffuseColor;
}

void SurfelCollection::setSpecularColor(const int index, const QRgb newSpecularColor) {
	AT(specularColors,index) = newSpecularColor;
}

void SurfelCollection::setFlagOn(const int index, const SurfelInterface::Flags flgs, const bool on) {
	if (on) {
		AT(flags,index) |= flgs;
	}
	else {
		AT(flags,index) &= ~flgs;
	}
}

void SurfelCollection::setFlags(const int index, const SurfelInterface::Flags newFlags) {
	AT(flags,index) = newFlags;
}

void SurfelCollection::setAmbientCoefficient(const int index, const float newAmbientCoefficient) {
	AT(ambientCoefficients,index) = newAmbientCoefficient;
}

void SurfelCollection::setDiffuseCoefficient(const int index, const float newDiffuseCoefficient) {
	AT(diffuseCoefficients,index) = newDiffuseCoefficient;
}

void SurfelCollection::setSpecularCoefficient(const int index, const float newSpecularCoefficient) {
	AT(specularCoefficients,index) = newSpecularCoefficient;
}

void SurfelCollection::setShininess(const int index, const float newShininess) {
	AT(shininess,index) = newShininess;
}

void SurfelCollection::setTextureCoordinate(const int index, const MyDataTypes::TextureCoordinate newTextureCoordinate) {
	AT(textureCoordinates,index) = newTextureCoordinate;
}

void SurfelCollection::combineColorProperties(const int index, const SurfelInterface *copySurfel1, const SurfelInterface *copySurfel2, const float factor) {

	if (diffuseColors) {
		AT(diffuseColors,index) = mixColor(copySurfel1->getDiffuseColor(), copySurfel2->getDiffuseColor(), factor);
	}
	if (specularColors) {
		AT(specularColors,index) = mixColor(copySurfel1->getSpecularColor(), copySurfel2->getSpecularColor(), factor);
	}
	if (ambientCoefficients) {
		AT(ambientCoefficients,index) = factor*copySurfel1->getAmbientCoefficient() + (1.0 - factor)*copySurfel2->getAmbientCoefficient();
	}
	if (diffuseCoefficients) {
		AT(diffuseCoefficients,index) = factor*copySurfel1->getDiffuseCoefficient() + (1.0 - factor)*copySurfel2->getDiffuseCoefficient();
	}
	if (specularCoefficients) {
		AT(specularCoefficients,index) = factor*copySurfel1->getSpecularCoefficient() + (1.0 - factor)*copySurfel2->getSpecularCoefficient();
	}
	if (shininess) {
		AT(shininess,index) = factor*copySurfel1->getShininess() + (1.0 - factor)*copySurfel2->getShininess();
	}
}

void SurfelCollection::copySurfelProperties(const int index, const SurfelInterface* copySurfel) {

	if (positions)				AT(positions,index) = copySurfel->getPosition();
	if (normals)				AT(normals,index) = copySurfel->getNormal();
	if (axes) {
		Axes copyAxes;
		copyAxes.axisOne = copySurfel->getAxisOne();
		copyAxes.axisTwo = copySurfel->getAxisTwo();
		AT(axes,index)  = copyAxes;
	}
	if (radius) 				AT(radius,index) = copySurfel->getRadius();
	if (diffuseColors)			AT(diffuseColors,index) = copySurfel->getDiffuseColor();
	if (specularColors) 		AT(specularColors,index) = copySurfel->getSpecularColor();
	if (ambientCoefficients)	AT(ambientCoefficients,index) = copySurfel->getAmbientCoefficient();
	if (diffuseCoefficients) 	AT(diffuseCoefficients,index) = copySurfel->getDiffuseCoefficient();
	if (specularCoefficients) 	AT(specularCoefficients,index) = copySurfel->getSpecularCoefficient();
	if (shininess)				AT(shininess,index) = copySurfel->getShininess();
	if (textureCoordinates) 	AT(textureCoordinates,index) = copySurfel->getTextureCoordinate();
	if (flags) 					AT(flags,index) = copySurfel->getFlags();
	if (associatedSurfels)	    {
		AT(associatedSurfels,index).clearAssociatedSurfels();
		int nOfAssociatedSurfels = copySurfel->getNofAssociatedSurfels();
		for (int i = 0; i < nOfAssociatedSurfels; i++) {
			AT(associatedSurfels,index).addAssociatedSurfel(copySurfel->getAssociatedSurfel(i));
		}
	}
	if (details) {
		Detail detail;
		detail.detailCoefficient = copySurfel->getDetailCoefficient();
		detail.detailVector      = copySurfel->getDetailVector();
		AT(details,index) = detail;
	}

}

bool SurfelCollection::addAssociatedSurfel(const int index, SurfelInterface *surfel) {
	return AT(associatedSurfels,index).addAssociatedSurfel(surfel);	
}

void SurfelCollection::clearAssociatedSurfels(const int index) {
	AT(associatedSurfels,index).clearAssociatedSurfels();
}

Vector3D SurfelCollection::getPositionEmpty(const int index) {
	//qDebug("Function getPosition is not implemented");
	// return dummy value
	return Vector3D(0,0,0);
}

Vector3D SurfelCollection::getNormalEmpty(const int index) {
	//qDebug("Function getNormal is not implemented");
	// return dummy value
	return Vector3D(1, 0, 0);
}

Vector3D SurfelCollection::getAxisOneEmpty(const int index) {
	//qDebug("Function getAxisOne is not implemented");
	// return dummy value
	return Vector3D(0, 1, 0);
}

Vector3D SurfelCollection::getAxisTwoEmpty(const int index) {
	//qDebug("Function getAxisTwo is not implemented");
	// return dummy value
	return Vector3D(0, 0, 1);
}

float SurfelCollection::getRadiusEmpty(const int index) {
	//qDebug("Function getRadius is not implemented");
	// return dummy value
	return 1.0f;
}

QRgb SurfelCollection::getDiffuseColorEmpty(const int index) {
	//qDebug("Function getDiffuseColor is not implemented");
	// return dummy value
	return qRgb(240, 240, 240);
}

QRgb SurfelCollection::getSpecularColorEmpty(const int index) {
	//qDebug("Function getSpecularColor is not implemented");
	// return dummy value
	return 0;
}

bool SurfelCollection::isFlagOnEmpty(const int index, const SurfelInterface::Flags flags) {
	//qDebug("Function isFlagOn is not implemented");
	// return dummy value
	return false;
}

bool SurfelCollection::isSelectedEmpty(const int index) {
	//qDebug("Function isSelected is not implemented");
	// return dummy value
	return false;
}

SurfelInterface::Flags SurfelCollection::getFlagsEmpty(const int index) {
	return SurfelInterface::NO_FLAGS;
}

float SurfelCollection::getAmbientCoefficientEmpty(const int index) {
	//qDebug("Function getAmbientCoefficient is not implemented");
	// return dummy value
	return 0.5f;
}

float SurfelCollection::getDiffuseCoefficientEmpty(const int index) {
	//qDebug("Function getDiffuseCoefficient is not implemented");
	// return dummy value
	return 0.75f;
}

float SurfelCollection::getSpecularCoefficientEmpty(const int index) {
	//qDebug("Function getSpecularCoefficient is not implemented");
	// return dummy value
	return 0.25f;
}

float SurfelCollection::getShininessEmpty(const int index) {
	//qDebug("Function getShininess is not implemented");
	// return dummy value
	return 0.75f;
}

MyDataTypes::TextureCoordinate	SurfelCollection::getTextureCoordinateEmpty(const int index) {
	//qDebug("Function getTextureCoordinate is not implemented");
	//return dummy value
	MyDataTypes::TextureCoordinate textureCoordinate = {0.0f, 0.0f};
	return textureCoordinate;
}

unsigned int SurfelCollection::getNofAssociatedSurfelsEmpty(const int index) {
	return 0;
}

SurfelInterface* SurfelCollection::getAssociatedSurfelEmpty(const int index, const int idx) {
	//qDebug("Function getAssociatedSurfel is not implemented");
	return NULL;
}

Vector3D SurfelCollection::getDetailVectorEmpty(const int index) {
	//qDebug("Function getDetailVector is not implemented");
	//return dummy value
	return Vector3D (1, 0, 0);
}

float SurfelCollection::getDetailCoefficientEmpty(const int index) {
	//qDebug("Function getDetailCoefficient is not implemented");
	// return dummy value
	return 0.0f;
}

void SurfelCollection::setDetailVectorEmpty(const int index, const Vector3D detailVector) {
	if (!(nativePropertyDescriptor & SurfelInterface::DETAIL)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::DETAIL);
		(this->*_setDetailVector)(index, detailVector);
	}
	else {
		//qDebug("Function setDetailVector is not implemented");
	}

	
}

void SurfelCollection::setDetailCoefficientEmpty(const int index, const float detailCoefficient) {
	if (!(nativePropertyDescriptor & SurfelInterface::DETAIL)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::DETAIL);
		(this->*_setDetailCoefficient)(index, detailCoefficient);
	}
	else {
		//qDebug("Function setDetailCoefficient is not implemented");
	}

}

void SurfelCollection::setPositionEmpty(const int index, const Vector3D newPosition) {
	if (!(nativePropertyDescriptor & SurfelInterface::POSITION)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::POSITION);
		(this->*_setPosition)(index, newPosition);
	}
	else {
		//qDebug("Function setPosition is not implemented");
	}

}

void SurfelCollection::setNormalEmpty(const int index, const Vector3D newNormal) {
	if (!(nativePropertyDescriptor & SurfelInterface::NORMAL)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::NORMAL);
		(this->*_setNormal)(index, newNormal);
	}
	else {
		//qDebug("Function setNormal is not implemented");
	}
}

void SurfelCollection::setTangentAxesEmpty(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	if (!(nativePropertyDescriptor & SurfelInterface::TANGENT_AXES)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::TANGENT_AXES);
		(this->*_setTangentAxes)(index, newAxisOne, newAxisTwo);
	}
	else {
		//qDebug("Function setTangentAxes is not implemented");
	}
}

void SurfelCollection::setNormalTangentAxesSystemEmpty(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	if (!(nativePropertyDescriptor & SurfelInterface::TANGENT_AXES)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::TANGENT_AXES);
		(this->*_setNormalTangentAxesSystem)(index, newNormal, newAxisOne, newAxisTwo);
	}
	else {
		//qDebug("Function setTangentAxesSystem is not implemented");
	}
}

void SurfelCollection::setRadiusEmpty(const int index, float r) {
	if (!(nativePropertyDescriptor & SurfelInterface::RADIUS)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::RADIUS);
		(this->*_setRadius)(index, r);
	}
	else {
		//qDebug("Function setRadius is not implemented");
	}

}

void SurfelCollection::setDiffuseColorEmpty(const int index, const QRgb newDiffuseColor) {
	if (!(nativePropertyDescriptor & SurfelInterface::DIFFUSE_COLOR)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::DIFFUSE_COLOR);
		(this->*_setDiffuseColor)(index, newDiffuseColor);
	}
	else {
		//qDebug("Function setDiffuseColor is not implemented");
	}
}

void SurfelCollection::setSpecularColorEmpty(const int index, const QRgb newSpecularColor) {
	if (!(nativePropertyDescriptor & SurfelInterface::SPECULAR_COLOR)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::SPECULAR_COLOR);
		(this->*_setSpecularColor)(index, newSpecularColor);
	}
	else {
		//qDebug("Function setSpecularColor is not implemented");
	}
}

void SurfelCollection::setFlagOnEmpty(const int index, const SurfelInterface::Flags flags, const bool on) {
	if (!(nativePropertyDescriptor & SurfelInterface::FLAGS)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::FLAGS);
		(this->*_setFlagOn)(index, flags, on);
	}
	else {
		//qDebug("Function setFlagOn is not implemented");
	}
}

void SurfelCollection::setFlagsEmpty(const int index, const SurfelInterface::Flags newFlags) {
	if (!(nativePropertyDescriptor & SurfelInterface::FLAGS)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::FLAGS);
		(this->*_setFlags)(index, newFlags);
	}
	else {
		//qDebug("Function setFlags is not implemented");
	}
}

void SurfelCollection::setAmbientCoefficientEmpty(const int index, const float newAmbientCoefficient) {
	if (!(nativePropertyDescriptor & SurfelInterface::AMBIENT_COEFFICIENT)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::AMBIENT_COEFFICIENT);
		(this->*_setAmbientCoefficient)(index, newAmbientCoefficient);
	}
	else {
		//qDebug("Function setAmbientCoefficient is not implemented");
	}
}

void SurfelCollection::setDiffuseCoefficientEmpty(const int index, const float newDiffuseCoefficient) {
	if (!(nativePropertyDescriptor & SurfelInterface::DIFFUSE_COEFFICIENT)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::DIFFUSE_COEFFICIENT);
		(this->*_setDiffuseCoefficient)(index, newDiffuseCoefficient);
	}
	else {
		//qDebug("Function setDiffuseCoefficient is not implemented");
	}
}

void SurfelCollection::setSpecularCoefficientEmpty(const int index, const float newSpecularCoefficient) {
	if (!(nativePropertyDescriptor & SurfelInterface::SPECULAR_COEFFICIENT)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::SPECULAR_COEFFICIENT);
		(this->*_setSpecularCoefficient)(index, newSpecularCoefficient);
	}
	else {
		//qDebug("Function setSpecularCoefficient is not implemented");
	}
}

void SurfelCollection::setShininessEmpty(const int index, const float newShininess) {
	if (!(nativePropertyDescriptor & SurfelInterface::SHININESS)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::SHININESS);
		(this->*_setShininess)(index, newShininess);
	}
	else {
		//qDebug("Function setShininess is not implemented");
	}
}

void SurfelCollection::setTextureCoordinateEmpty(const int index, const MyDataTypes::TextureCoordinate newTextureCoordinate) {
	if (!(nativePropertyDescriptor & SurfelInterface::TEXTURE_COORDINATE)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::TEXTURE_COORDINATE);
		(this->*_setTextureCoordinate)(index, newTextureCoordinate);
	}
	else {
		//qDebug("Function setTextureCoordinate is not implemented");
	}
}

bool SurfelCollection::addAssociatedSurfelEmpty(const int index, SurfelInterface *surfel) {
	if (!(nativePropertyDescriptor & SurfelInterface::ASSOCIATED_SURFELS)) {
		setPropertyDescriptor(nativePropertyDescriptor | SurfelInterface::ASSOCIATED_SURFELS);
		return (this->*_addAssociatedSurfel)(index, surfel);
	}
	else {
		//qDebug("Function addAssociatedSurfel is not implemented");
		return false;
	}	
}

void SurfelCollection::clearAssociatedSurfelsEmpty(const int index) {
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
