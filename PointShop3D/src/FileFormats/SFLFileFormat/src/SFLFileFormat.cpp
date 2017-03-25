// Title:   SFLFileFormat.cpp
// Created: Thu Sep 25 15:45:55 2003
// Author:  Tim Weyrich
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


#include <memory.h>

#include <sfl.h>
#include <sfl/gff.h>

#include <qstring.h>
#include <qfile.h>
#include <qapplication.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qptrdict.h>

#include "../../../Core/Scene/src/Scene.h"
#include "../../../Core/Scene/src/Object.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/Interfaces/src/FileFormatPluginInterface.h"
#include "../../../Core/Configuration/src/Configuration.h"
#include "../../../Core/MarkerManager/src/MarkerManager.h"
#include "../../../Core/MarkerManager/src/MarkerInterface.h"
#include "../../../Core/MarkerManager/src/Marker2D.h"
#include "../../../Core/MarkerManager/src/Marker3D.h"
#include "../../../Core/CoreTools/src/CoreTools.h"
#include "../../../Core/CoreTools/SelectionTool/src/SelectionTool.h"
#include "../../../Utilities/UserMessages/src/StatusBar.h"
#include "../UI/SFLCustomizeDialog.h"
#include "SFLFileFormat.h"

// *********
// constants
// *********


// ****************
// static funcitons
// ****************
static int  cmp_magic(uint32 magic, const char *magic_ref) {
	return (magic == *((const uint32*)magic_ref));
}
	
// **************
// public methods
// **************

SFLFileFormat::SFLFileFormat() {
	storeMarkers = false;
	normalCompression = sfl::CUBIC_32;
}

SFLFileFormat::~SFLFileFormat(){}

bool SFLFileFormat::isReadable (const FileHeader fileHeader) {

	gff_header gffHeader;

	memcpy ((void *)&gffHeader, fileHeader, sizeof (gff_header));
	return cmp_magic (gffHeader.gff_magic, GFF_MAGIC);

}

bool SFLFileFormat::readFromFile (const QString filePath, const bool mergeWithCurrent) {

	sfl::InStream                        *in;

	Object                               *object;
	QString                              magicHeader;
	SurfelInterface::PropertyDescriptor  propertyDescriptor;
	Scene                                *scene;
	MyDataTypes::CameraPosition          cameraPosition;

	bool  ok = false;

	scene = Scene::getInstance();
	StatusBar::getInstance()->resetProgress();
	if ((in = sfl::InStream::open (filePath.latin1())) != 0) {

        in->setMatrixApiColumnMajor();

        in->setLocalRightVector( 1, 0, 0);
        in->setLocalUpVector   ( 0, 1, 0);
        in->setLocalBackVector ( 0, 0, 1);

	    //if (in->seekSurfelSet(0) == true) 
		{

		    // decide wheter to merge or not with current scene
		    if (mergeWithCurrent == false) {

			    // remove the old objects
				scene->reset (true, false);
				scene->setAutoDelete (true);				

			    // read transformation
			    if (in->querySceneCameraTransform() == true) {
				    in->getSceneCameraScalationTranslation4x4 (cameraPosition.scaleTranslationMatrix);
				    in->getSceneCameraRotation4x4 (cameraPosition.rotationMatrix);
			    }
			    else {
					cameraPosition = Configuration::getInstance()->getHomePosition();
			    }
				scene->setCameraPosition (cameraPosition, false);

			    nofObjects = in->getNumSurfelSets();

				for (currentObject = 0; currentObject < nofObjects - 1; currentObject++) {

				    if (in->seekSurfelSet (currentObject) == true) {

						propertyDescriptor = this->sflPropertiesToPropertyDescriptor (in->getSurfelSetProperties(0));
						object = new Object(QString(), propertyDescriptor);
				        this->readObjectFromStream (in, object);

					    // it is important that the objects have unique names already!
					    // as long as the Scene was manipulated only through this Scene.h API
					    // this is guaranteed.
						scene->addObject (object, false, false);

				    } 
					else {
					    // Something is wrong!
					    break;
					}

			    }

				// for the last object we emit the signal and make it active
				if (in->seekSurfelSet (nofObjects - 1) == true) {

				    propertyDescriptor = this->sflPropertiesToPropertyDescriptor (in->getSurfelSetProperties(0));
					object = new Object(QString(), propertyDescriptor);
				    this->readObjectFromStream (in, object);

					// it is important that the objects have unique names already!
					// as long as the Scene was manipulated only through this Scene.h API
					// this is guaranteed.
					scene->addObject (object, true, true);
					currentObject++;

				} 

			    ok = currentObject == nofObjects;

		    }
		    else {

			    // we keep the old objects

				scene->setAutoDelete (true);

				nofObjects = in->getNumSurfelSets();

			    for (currentObject = 0; currentObject < nofObjects - 1; currentObject++) {

				    if (in->seekSurfelSet (currentObject) == true) {

				        propertyDescriptor = this->sflPropertiesToPropertyDescriptor (in->getSurfelSetProperties(0));
						object = new Object(QString(), propertyDescriptor);
				        this->readObjectFromStream (in, object);

					    // it is important that the objects have unique names already!
					    // as long as the Scene was manipulated only through this Scene.h API
					    // this is guaranteed.
						scene->addObject (object, false, false);

				    } 
					else {
					    // Something is wrong!
					    break;
					}

			    }

				// for the last object we emit the signal and make it active
				if (in->seekSurfelSet (nofObjects - 1) == true) {

				    propertyDescriptor = this->sflPropertiesToPropertyDescriptor (in->getSurfelSetProperties(0));
					object = new Object(QString(), propertyDescriptor);
				    this->readObjectFromStream (in, object);

					// it is important that the objects have unique names already!
					// as long as the Scene was manipulated only through this Scene.h API
					// this is guaranteed.					
					scene->addObject (object, true, true);
					currentObject++;

				} 

			    ok = currentObject == nofObjects;

		    }

	    }

        sfl::InStream::close(in);
	}

	return ok;
	
}

bool SFLFileFormat::writeToFile (const QString filePath, const bool onlyActiveObject) {

	sfl::OutStream              *out;
	Object                      *object;
	int                         res;
	MyDataTypes::CameraPosition cameraPosition;
	Scene                       *scene;
	QFile                       file (filePath);

	res = 0;
	StatusBar::getInstance()->resetProgress();
	if ((out = sfl::OutStream::open (filePath.latin1()))) {
		
        out->setMatrixApiColumnMajor();
		
        if ((res = out->beginHeader()) == 0) {

			scene = Scene::getInstance();
			out->setTitle("PointShop3D Scene");
			out->setRightVec3 (1, 0, 0);
			out->setUpVec3    (0, 1, 0);
			out->setBackVec3  (0, 0, 1);
			cameraPosition = scene->getCameraPosition();
	    	out->setSceneCameraTransformTwice4x4 (cameraPosition.scaleTranslationMatrix,
												  cameraPosition.rotationMatrix);
			
        	if ((res = out->endHeader()) == 0) {

				if (onlyActiveObject == false) {

					nofObjects = scene->getNofObjects();
        			if ((res = out->beginSurfelSetList (nofObjects)) == 0) {
				  		
						currentObject = 0;
						for (object = scene->getFirstObject(); object != 0; object = scene->getNextObject()) {
							this->writeObjectToStream (object, out);
							currentObject++;
				  		}
				  		  
				  		res = out->endSurfelSetList();
					}

				}
				else {

					nofObjects    = 1;
					currentObject = 0;
					if ((res = out->beginSurfelSetList(1)) == 0) {
						this->writeObjectToStream (scene->getActiveObject(), out);
				  		res = out->endSurfelSetList();
					}

				}

			}

		}

        sfl::OutStream::close (out);
		
		if (res != 0) {
			file.remove();
		}
		// check file state
		return (res == 0);

	}
	else {
		return false;
	}

}

QString SFLFileFormat::getFileFilter() const {
	return QString ("Surfel File Format (*.sfl)");
}

QString SFLFileFormat::getFileSuffix() const {
	return QString ("sfl");
}

void SFLFileFormat::customize (QWidget *parent) {

	SFLCustomizeDialog *sflCustomizeDialog;

	sflCustomizeDialog = new SFLCustomizeDialog (parent, "customizeDialog", true);
	// initialize
	sflCustomizeDialog->storeMarkersCheckBox->setChecked (storeMarkers);
	sflCustomizeDialog->normalNoneRadioButton->setChecked (normalCompression == sfl::NONE);
	sflCustomizeDialog->normalCube16RadioButton->setChecked (normalCompression == sfl::CUBIC_16);
	sflCustomizeDialog->normalCube32RadioButton->setChecked (normalCompression == sfl::CUBIC_32);

	sflCustomizeDialog->exec();
	
	storeMarkers = sflCustomizeDialog->storeMarkersCheckBox->isChecked();

	if (sflCustomizeDialog->normalNoneRadioButton->isChecked()) {
		normalCompression = sfl::NONE;
	}
	else if (sflCustomizeDialog->normalCube16RadioButton->isChecked()) {
		normalCompression = sfl::CUBIC_16;
	}
	else	if (sflCustomizeDialog->normalCube32RadioButton->isChecked()) {
		normalCompression = sfl::CUBIC_32;
	}



	// remove unused resources
	delete sflCustomizeDialog;

}

// ***************
// private methods
// ***************

void SFLFileFormat::readObjectFromStream (sfl::InStream *in, Object *object) {

	QString name;
	char    *ident;
	MyDataTypes::TransformationMatrix16f scaleTranslationMatrix,
		                                 rotationMatrix;
		
	ident = in->getSurfelSetIdentifier();    
	name = ident;
	object->setName (name, false);
	sfl::memFree (ident);

    in->getSurfelSetScalationTranslation4x4 (scaleTranslationMatrix);
    in->getSurfelSetRotation4x4 (rotationMatrix);

	object->setScale (scaleTranslationMatrix[0], scaleTranslationMatrix[5], scaleTranslationMatrix[10], false);
	object->setPosition (scaleTranslationMatrix[12], scaleTranslationMatrix[13], scaleTranslationMatrix[14], false);
	object->setRotationMatrix (rotationMatrix, false);

	this->readSurfelCollectionFromStream (in, object->getSurfelCollection());
	
}

void SFLFileFormat::writeObjectToStream (Object *object, sfl::OutStream *out) {

	MyDataTypes::TransformationMatrix16f scaleTranslationMatrix,
		                                 rotationMatrix;
	float                                sx,
		                                 sy,
										 sz;

	object->getTranslationMatrix (scaleTranslationMatrix);
	object->getScale (&sx, &sy, &sz);
	scaleTranslationMatrix[0]  = sx;
	scaleTranslationMatrix[5]  = sy;
	scaleTranslationMatrix[10] = sz;
	object->getRotationMatrix (rotationMatrix);
	this->writeSurfelCollectionToStream (object->getSurfelCollection(), object->getName(),
		                                 scaleTranslationMatrix, rotationMatrix, out);
	
}

//#define  HACKED_SCALATION  10000.0

void SFLFileFormat::readSurfelCollectionFromStream (sfl::InStream *in, SurfelCollection *surfelCollection) {

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
	uint8                               r,
		                                g,
								        b,
								        a;
	SurfelInterface::PropertyDescriptor actualPropertyDescriptor;
	float                               *markersU,
	                                    *markersV;
	uint32                              *markerIndices,
		                                surfelIndex,
										nofMarkers,
										nofFeatureRelations;
	MarkerManager                       *markerManager;
	Marker2D                            *marker2D;
	Marker3D                            *marker3D;
	SelectionTool                       *selectionTool;

	int	                                markerIndex;
	StatusBar                           *statusBar;
	int                                 progress;

	markerManager = MarkerManager::getInstance();
	statusBar     = StatusBar::getInstance();

	// read markers
	nofMarkers = in->getSurfelSetNumMarkers();

	if (nofMarkers > 0) {

		markersU      = new float[nofMarkers];
		markersV      = new float[nofMarkers];
		markerIndices = new uint32[nofMarkers];

		// reset the old markers
		markerManager->removeAllFittingConstraints2D();
		markerManager->removeAllFittingConstraints3D (false);

		// read the surfelIndices and UV coordinates
		for (i = 0; i < nofMarkers; i++) {			
			in->getSurfelSetMarkerSurfelIndex (i, markerIndices[i]);
			markersU[i] = (float)in->getSurfelSetMarkerU(i);
			markersV[i] = (float)in->getSurfelSetMarkerV(i);
		}

	}
	else {
		markersU      = 0;
		markersV      = 0;
		markerIndices = 0;
	}
	


	scene               = Scene::getInstance();
	selectionTool       = CoreTools::getInstance()->getSelectionTool();
	
	actualPropertyDescriptor = surfelCollection->getPropertyDescriptor();

	in->setSurfelSetPropertyHints (this->propertyDescriptorToSFLProperties (actualPropertyDescriptor));

	if (in->seekResolution (in->getSurfelSetRes0Index()) == true) {

		nofSurfels = in->getResolutionNumSurfels();
		surfelCollection->reserve(surfelCollection->capacity() + nofSurfels);

		if (actualPropertyDescriptor == SurfelInterface::BaseSurfel_PropertyDescriptor) {
		
			for (surfelIndex = 0; surfelIndex < nofSurfels; surfelIndex++) {

				surfel = surfelCollection->addSurfel (false);
				
				in->beginSurfel();

					// read properties
					in->readSurfelPosition3 (position[0], position[1], position[2]);
					in->readSurfelNormal3 (normal[0], normal[1], normal[2]);
					in->readSurfelRadius (radius);
					in->readSurfelColorRGBAuc (sfl::DIFFUSE, r, g, b, a);
					diffuseColor = qRgba (r, g, b, a);
					in->readSurfelColorRGBAuc (sfl::SPECULAR, r, g, b, a);
					specularColor = qRgba (r, g, b, a);
					flags = in->readSurfelUserFlags();

#ifdef HACKED_SCALATION
					position[0] *= HACKED_SCALATION;
					position[1] *= HACKED_SCALATION;
					position[2] *= HACKED_SCALATION;
					radius      *= HACKED_SCALATION;
#endif

				in->endSurfel();

				scene->setSurfelPosition (surfel, position, false);
				scene->setSurfelNormal (surfel, normal, false);
				scene->setSurfelRadius (surfel, radius, false);
				scene->setSurfelDiffuseColor (surfel, diffuseColor, false);
				scene->setSurfelSpecularColor (surfel, specularColor, false);
				scene->setSurfelFlags (surfel, flags, false);

				// check if this surfel is marked
				if (nofMarkers > 0 && (markerIndex = this->isSurfelMarked (surfelIndex, markerIndices, nofMarkers)) >= 0 ) {
					
					textureCoordinate.u = markersU[markerIndex];
					textureCoordinate.v = markersV[markerIndex];
					
					marker2D = new Marker2D (textureCoordinate, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE);
					marker3D = new Marker3D (surfel, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE); 
					markerManager->addFittingConstraint2D (marker2D);
					markerManager->addFittingConstraint3D (marker3D);

					markerIndex++;

				}

				// update the progress bar
				progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelIndex * 100.0f / (float)nofSurfels + 0.5f);
				if (progress > statusBar->getProgress()) {
					statusBar->setProgress (progress, false);
				}

			}
			
			selectionTool->emitAllSelectionsChanged();
			
		}
		else if (actualPropertyDescriptor == SurfelInterface::Surfel1_PropertyDescriptor) {
		
			for (surfelIndex = 0; surfelIndex < nofSurfels; surfelIndex++) {

				surfel = surfelCollection->addSurfel (false);

				in->beginSurfel();

					// read properties
					in->readSurfelPosition3 (position[0], position[1], position[2]);
					in->readSurfelNormal3 (normal[0], normal[1], normal[2]);
					in->readSurfelRadius (radius);
					in->readSurfelColorRGBAuc (sfl::DIFFUSE, r, g, b, a);
					diffuseColor = qRgba (r, g, b, a);
					in->readSurfelColorRGBAuc (sfl::SPECULAR, r, g, b, a);
					specularColor = qRgba (r, g, b, a);
					flags = in->readSurfelUserFlags();

					in->readSurfelColorCoefff (sfl::AMBIENT, ambientCoefficient);
					in->readSurfelColorCoefff (sfl::DIFFUSE, diffuseCoefficient);
					in->readSurfelColorCoefff (sfl::SPECULAR, specularCoefficient);
					in->readSurfelShininess (shininess);
					in->readSurfelTextureUV (textureCoordinate.u, textureCoordinate.v);

#ifdef HACKED_SCALATION
					position[0] *= HACKED_SCALATION;
					position[1] *= HACKED_SCALATION;
					position[2] *= HACKED_SCALATION;
					radius      *= HACKED_SCALATION;
#endif

				in->endSurfel();

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


				// check if this surfel is marked
				if (nofMarkers > 0 && (markerIndex = this->isSurfelMarked (surfelIndex, markerIndices, nofMarkers)) >= 0 ) {
				
					textureCoordinate.u = markersU[markerIndex];
					textureCoordinate.v = markersV[markerIndex];
					
					marker2D = new Marker2D (textureCoordinate, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE);
					marker3D = new Marker3D (surfel, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE); 
					markerManager->addFittingConstraint2D (marker2D);
					markerManager->addFittingConstraint3D (marker3D);

					markerIndex++;

				}

				// update the progress bar
				progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelIndex * 100.0f / (float)nofSurfels + 0.5f);
				if (progress > statusBar->getProgress()) {
					statusBar->setProgress (progress, false);
				}

			}

			selectionTool->emitAllSelectionsChanged();
				
		}

		else if (actualPropertyDescriptor == SurfelInterface::ElSurfel_PropertyDescriptor) {
		
			for (surfelIndex = 0; surfelIndex < nofSurfels; surfelIndex++) {

				surfel = surfelCollection->addSurfel (false);

				in->beginSurfel();

					// read properties
					in->readSurfelPosition3 (position[0], position[1], position[2]);
					in->readSurfelNormal3 (normal[0], normal[1], normal[2]);
					in->readSurfelTangentAxisOne3 (axisOne[0], axisOne[1], axisOne[2]);
					in->readSurfelTangentAxisTwo3 (axisTwo[0], axisTwo[1], axisTwo[2]);
					in->readSurfelColorRGBAuc (sfl::DIFFUSE, r, g, b, a);
					diffuseColor = qRgba (r, g, b, a);
					in->readSurfelColorRGBAuc (sfl::SPECULAR, r, g, b, a);
					specularColor = qRgba (r, g, b, a);
					flags = in->readSurfelUserFlags();
					in->readSurfelColorCoefff (sfl::AMBIENT, ambientCoefficient);
					in->readSurfelColorCoefff (sfl::DIFFUSE, diffuseCoefficient);
					in->readSurfelColorCoefff (sfl::SPECULAR, specularCoefficient);
					in->readSurfelShininess (shininess);
					in->readSurfelTextureUV (textureCoordinate.u, textureCoordinate.v);

#ifdef HACKED_SCALATION
					position[0] *= HACKED_SCALATION;
					position[1] *= HACKED_SCALATION;
					position[2] *= HACKED_SCALATION;
					axisOne[0] *= HACKED_SCALATION;
					axisOne[1] *= HACKED_SCALATION;
					axisOne[2] *= HACKED_SCALATION;
					axisTwo[0] *= HACKED_SCALATION;
					axisTwo[1] *= HACKED_SCALATION;
					axisTwo[2] *= HACKED_SCALATION;
#endif

				in->endSurfel();

				scene->setSurfelPosition (surfel, position, false);
				scene->setSurfelNormalTangentAxesSystem (surfel, normal, axisOne, axisTwo, false);
				scene->setSurfelDiffuseColor (surfel, diffuseColor, false);
				scene->setSurfelSpecularColor (surfel, specularColor, false);
				scene->setSurfelFlags (surfel, flags, false);
				scene->setSurfelAmbientCoefficient (surfel, ambientCoefficient, false);
				scene->setSurfelDiffuseCoefficient (surfel, diffuseCoefficient, false);
				scene->setSurfelSpecularCoefficient (surfel, specularCoefficient, false);
				scene->setSurfelShininess (surfel, shininess, false);
				scene->setSurfelTextureCoordinate (surfel, textureCoordinate, false);


				// check if this surfel is marked
				if (nofMarkers > 0 && (markerIndex = this->isSurfelMarked (surfelIndex, markerIndices, nofMarkers)) >= 0 ) {
				
					textureCoordinate.u = markersU[markerIndex];
					textureCoordinate.v = markersV[markerIndex];
					
					marker2D = new Marker2D (textureCoordinate, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE);
					marker3D = new Marker3D (surfel, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE); 
					markerManager->addFittingConstraint2D (marker2D);
					markerManager->addFittingConstraint3D (marker3D);

					markerIndex++;

				}

				// update the progress bar
				progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelIndex * 100.0f / (float)nofSurfels + 0.5f);
				if (progress > statusBar->getProgress()) {
					statusBar->setProgress (progress, false);
				}

			}

			selectionTool->emitAllSelectionsChanged();
				
		}
		else {

			bool bPosition				= actualPropertyDescriptor & SurfelInterface::POSITION;
			bool bNormal				= actualPropertyDescriptor & SurfelInterface::NORMAL;
			bool bRadius				= actualPropertyDescriptor & SurfelInterface::RADIUS;
			bool bDiffuseColor			= actualPropertyDescriptor & SurfelInterface::DIFFUSE_COLOR;
			bool bSpecularColor			= actualPropertyDescriptor & SurfelInterface::SPECULAR_COLOR;
			bool bFlags					= actualPropertyDescriptor & SurfelInterface::FLAGS;
			bool bAmbientCoefficient	= actualPropertyDescriptor & SurfelInterface::AMBIENT_COEFFICIENT;
			bool bDiffuseCoefficient	= actualPropertyDescriptor & SurfelInterface::DIFFUSE_COEFFICIENT;
			bool bSpecularCoefficient	= actualPropertyDescriptor & SurfelInterface::SPECULAR_COEFFICIENT;
			bool bShininess				= actualPropertyDescriptor & SurfelInterface::SHININESS;
			bool bTextureCoordinate		= actualPropertyDescriptor & SurfelInterface::TEXTURE_COORDINATE;
			bool bTangentAxes			= actualPropertyDescriptor & SurfelInterface::TANGENT_AXES;
			bool bAssociatedSurfels		= actualPropertyDescriptor & SurfelInterface::ASSOCIATED_SURFELS; 

			for (surfelIndex = 0; surfelIndex < nofSurfels; surfelIndex++) {

				surfel = surfelCollection->addSurfel (false);

				in->beginSurfel();

						// read properties
					if (bPosition) {
						in->readSurfelPosition3 (position[0], position[1], position[2]);
	#ifdef HACKED_SCALATION
						position[0] *= HACKED_SCALATION;
						position[1] *= HACKED_SCALATION;
						position[2] *= HACKED_SCALATION;
	#endif
						scene->setSurfelPosition (surfel, position, false);
					}

					
					if (bNormal & bTangentAxes) {
						in->readSurfelNormal3 (normal[0], normal[1], normal[2]);
						in->readSurfelTangentAxisOne3 (axisOne[0], axisOne[1], axisOne[2]);
						in->readSurfelTangentAxisTwo3 (axisTwo[0], axisTwo[1], axisTwo[2]);
	#ifdef HACKED_SCALATION
						axisOne[0] *= HACKED_SCALATION;
						axisOne[1] *= HACKED_SCALATION;
						axisOne[2] *= HACKED_SCALATION;
						axisTwo[0] *= HACKED_SCALATION;
						axisTwo[1] *= HACKED_SCALATION;
						axisTwo[2] *= HACKED_SCALATION;
	#endif
						scene->setSurfelNormalTangentAxesSystem (surfel, normal, axisOne, axisTwo, false);
					}
					else {
						if (bNormal) {
							in->readSurfelNormal3 (normal[0], normal[1], normal[2]);
							scene->setSurfelNormal (surfel, normal, false);
						}
						if (bRadius) {
							in->readSurfelRadius (radius);				
#ifdef HACKED_SCALATION
							radius *= HACKED_SCALATION;
#endif	
							scene->setSurfelRadius(surfel, radius, false);	
						}
						if (bTangentAxes) {
							in->readSurfelTangentAxisOne3 (axisOne[0], axisOne[1], axisOne[2]);
							in->readSurfelTangentAxisTwo3 (axisTwo[0], axisTwo[1], axisTwo[2]);
#ifdef HACKED_SCALATION
							axisOne[0] *= HACKED_SCALATION;
							axisOne[1] *= HACKED_SCALATION;
							axisOne[2] *= HACKED_SCALATION;
							axisTwo[0] *= HACKED_SCALATION;
							axisTwo[1] *= HACKED_SCALATION;
							axisTwo[2] *= HACKED_SCALATION;
#endif
							scene->setSurfelTangentAxes (surfel, axisOne, axisTwo, false);
						}
					}	

					if (bDiffuseColor) {
						in->readSurfelColorRGBAuc (sfl::DIFFUSE, r, g, b, a);
						diffuseColor = qRgba (r, g, b, a);
						scene->setSurfelDiffuseColor (surfel, diffuseColor, false);
					}
					if (bSpecularColor) {
						in->readSurfelColorRGBAuc (sfl::SPECULAR, r, g, b, a);
						specularColor = qRgba (r, g, b, a);
						scene->setSurfelSpecularColor (surfel, specularColor, false);
					}			
					if (bFlags) {
						flags = in->readSurfelUserFlags();
						scene->setSurfelFlags (surfel, flags, false);
					}
					if (bAmbientCoefficient) {
						in->readSurfelColorCoefff (sfl::AMBIENT, ambientCoefficient);
						scene->setSurfelAmbientCoefficient (surfel, ambientCoefficient, false);
					}
					if (bDiffuseCoefficient) {
						in->readSurfelColorCoefff (sfl::DIFFUSE, diffuseCoefficient);
						scene->setSurfelDiffuseCoefficient (surfel, diffuseCoefficient, false);
					}
					if (bSpecularCoefficient) {
						in->readSurfelColorCoefff (sfl::SPECULAR, specularCoefficient);
						scene->setSurfelSpecularCoefficient (surfel, specularCoefficient, false);
					}
					if (bShininess) {
						in->readSurfelShininess (shininess);
						scene->setSurfelShininess (surfel, shininess, false);
					}
					if (bTextureCoordinate) {
						in->readSurfelTextureUV (textureCoordinate.u, textureCoordinate.v);
						scene->setSurfelTextureCoordinate (surfel, textureCoordinate, false);
					}

				in->endSurfel();	
				
				// check if this surfel is marked
				if (nofMarkers > 0 && (markerIndex = this->isSurfelMarked (surfelIndex, markerIndices, nofMarkers)) >= 0 ) {
				
					textureCoordinate.u = markersU[markerIndex];
					textureCoordinate.v = markersV[markerIndex];
					
					marker2D = new Marker2D (textureCoordinate, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE);
					marker3D = new Marker3D (surfel, false, markerIndex + 1, MarkerInterface::FITTING_CONSTRAINT_STYLE); 
					markerManager->addFittingConstraint2D (marker2D);
					markerManager->addFittingConstraint3D (marker3D);

					markerIndex++;

				}

				// update the progress bar
				progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelIndex * 100.0f / (float)nofSurfels + 0.5f);
				if (progress > statusBar->getProgress()) {
					statusBar->setProgress (progress, false);
				}

			}

			selectionTool->emitAllSelectionsChanged();

		}
		
		
	}



	// load feature relations

	nofFeatureRelations = in->getSurfelSetExpFeatureRels();

	if (nofFeatureRelations > 0) {

		SurfelInterface** surfels = new SurfelInterface*[nofSurfels];

		for (i = 0, surfel = surfelCollection->getFirstSurfel(); i < nofSurfels; surfel = surfelCollection->getNextSurfel(), i++) {
			surfels[i] = surfel;
		}

		uint32 source, dest;
		for (i = 0; i < nofFeatureRelations; i++) {
			in->getSurfelSetExpFeatureRel (i, source, dest);
			scene->addAssociatedSurfel (surfels[source], surfels[dest], false);
		}

		delete[] surfels;
	}


	statusBar->setProgress ((int)(((float)(currentObject + 1) / (float)nofObjects) * 100), true);

	// remove unused resources
	if (markersU != 0) {
		delete[] markersU;
	}

	if (markersV != 0) {
		delete[] markersV;
	}

	if (markerIndices != 0) {
		delete[] markerIndices;
	}

}

void SFLFileFormat::writeSurfelCollectionToStream (SurfelCollection *surfelCollection, const QString name,
												   const MyDataTypes::TransformationMatrix16f scaleTranslationMatrix,
												   const MyDataTypes::TransformationMatrix16f rotationMatrix,
												   sfl::OutStream *out) {

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
	const QList<Marker3D>          *markers3D;
	const QList<Marker2D>          *markers2D;
	Marker3D                       *currentMarker3D;	
	Marker2D                       *currentMarker2D;
	MarkerManager                  *markerManager;
	uint                           nofMarkers;
	float                          *markersU,
	                               *markersV;
	uint32                         *markersIndices,
								   *featureIndices,
	                               index,
								   featureIndex;
	uint                           surfelCount,
		                           nofSurfels;
	StatusBar                      *statusBar;
	int                            progress;
	SurfelInterface::PropertyDescriptor propertyDescriptor;

	propertyDescriptor = surfelCollection->getPropertyDescriptor();

	out->beginSurfelSet (this->propertyDescriptorToSFLProperties (propertyDescriptor)
                         | SFLPROP_COLOR_MODEL_RGBA
                         | (((unsigned)normalCompression) << SFLPROP_NORMAL_COMPR_SHIFT));
   
	out->setSurfelSetIdentifier (name.latin1());
	out->setSurfelSetTransformTwice4x4 (scaleTranslationMatrix, rotationMatrix);

	// write markers	

	markerManager = MarkerManager::getInstance();
	statusBar     = StatusBar::getInstance();
	markers3D     = markerManager->getFittingConstraints3D();
	markers2D     = markerManager->getFittingConstraints2D();

	QListIterator<Marker3D> markers3DIterator (*markers3D);
	QListIterator<Marker2D> markers2DIterator (*markers2D);

	nofMarkers = markerManager->getMinimumFittingConstraintListCount();
	if (nofMarkers > 0 && storeMarkers == true) {

		markersU       = new float[nofMarkers];
		markersV       = new float[nofMarkers];
		markersIndices = new uint32[nofMarkers];
		
		// check for all surfels whether they have an associated marker
		
		markers2DIterator.toFirst();
		markers3DIterator.toFirst();

		for (uint i = 0; i < nofMarkers; i++) {
			
			currentMarker3D = markers3DIterator.current();
			currentMarker2D = markers2DIterator.current();

			index = 0;

			for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {								
															
				if(surfel == currentMarker3D->getSurfel()) {					
					markersU[i] = currentMarker2D->getNormalX();
					markersV[i] = currentMarker2D->getNormalY();
					markersIndices[i] = index;					
				}
				index++;								
				
			}
			++markers3DIterator;		
			++markers2DIterator;									
		}

		// write markers array to slf file
		out->setSurfelSetMarkerSurfelIndices(nofMarkers, markersIndices);
		out->setSurfelSetMarkerSurfelUs(nofMarkers, markersU);
		out->setSurfelSetMarkerSurfelVs(nofMarkers, markersV);
		
		delete[] markersU;
		delete[] markersV;
		delete[] markersIndices;

	}

	// write feature relations
	if (propertyDescriptor & SurfelInterface::ASSOCIATED_SURFELS) {

		QPtrDict<uint32> dict;
		
		for (index = 0, surfel = surfelCollection->getFirstSurfel(); surfel != 0; index++, surfel = surfelCollection->getNextSurfel()) {
			dict.insert ((void*) surfel, (uint32*) index);
		}

		
		featureIndices = new uint32[dict.count() * 2];
		featureIndex = 0;
		for (index = 0, surfel = surfelCollection->getFirstSurfel(); surfel != 0; index++, surfel = surfelCollection->getNextSurfel()) {
			for (unsigned int j=0; j<surfel->getNofAssociatedSurfels(); j++) {
				featureIndices[featureIndex++] = index;
				featureIndices[featureIndex++] = (uint32) dict[surfel->getAssociatedSurfel(j)];	
			}
		}
		
		out->setSurfelSetExpFeatureRels(featureIndex>>1, featureIndices);
		delete[] featureIndices;
	}
	else {
		out->setSurfelSetExpFeatureRels(0, NULL);
	}

	out->beginResolution (surfelCollection->getNofSurfels());
	{
	    
		surfelCount = 0;
		nofSurfels = surfelCollection->getNofSurfels();
		if (propertyDescriptor == SurfelInterface::BaseSurfel_PropertyDescriptor) {
		
			for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {

				out->beginSurfel();

					position      = surfel->getPosition();
					normal        = surfel->getNormal();
					radius        = surfel->getRadius();
					diffuseColor  = surfel->getDiffuseColor();
					specularColor = surfel->getSpecularColor();
					flags         = surfel->getFlags();

					// write properties
					out->dumpSurfelPosition3 (position[0], position[1], position[2]);
					out->dumpSurfelNormal3 (normal[0], normal[1], normal[2]);
					out->dumpSurfelRadius (radius);

					out->dumpSurfelColorRGBAuc (sfl::DIFFUSE, 
												qRed(diffuseColor),
												qGreen(diffuseColor),
												qBlue(diffuseColor),
												qAlpha(diffuseColor));

					out->dumpSurfelColorRGBAuc (sfl::SPECULAR, 
												qRed(specularColor),
												qGreen(specularColor),
												qBlue(specularColor),
												qAlpha(specularColor));

					out->dumpSurfelUserFlags (flags);
				
				out->endSurfel();

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

				out->beginSurfel();

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
					out->dumpSurfelPosition3 (position[0], position[1], position[2]);
					out->dumpSurfelNormal3 (normal[0], normal[1], normal[2]);
					out->dumpSurfelRadius (radius);

					out->dumpSurfelColorRGBAuc (sfl::DIFFUSE, 
												qRed(diffuseColor),
												qGreen(diffuseColor),
												qBlue(diffuseColor),
												qAlpha(diffuseColor));

					out->dumpSurfelColorRGBAuc (sfl::SPECULAR, 
												qRed(specularColor),
												qGreen(specularColor),
												qBlue(specularColor),
												qAlpha(specularColor));

					out->dumpSurfelUserFlags (flags);	

					// write additional properties
					out->dumpSurfelColorCoefff (sfl::AMBIENT, ambientCoefficient);
					out->dumpSurfelColorCoefff (sfl::DIFFUSE, diffuseCoefficient);
					out->dumpSurfelColorCoefff (sfl::SPECULAR, specularCoefficient);
					out->dumpSurfelShininess (shininess);
					out->dumpSurfelTextureUV (textureCoordinate.u, textureCoordinate.v);

				out->endSurfel();

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

				out->beginSurfel();

					position            = surfel->getPosition();
					normal              = surfel->getNormal();
					axisOne             = surfel->getAxisOne();
					axisTwo             = surfel->getAxisTwo();
					diffuseColor        = surfel->getDiffuseColor();
					specularColor       = surfel->getSpecularColor();
					flags               = surfel->getFlags();
					ambientCoefficient  = surfel->getAmbientCoefficient();
					diffuseCoefficient  = surfel->getDiffuseCoefficient();
					specularCoefficient = surfel->getSpecularCoefficient();
					shininess           = surfel->getShininess();
					textureCoordinate   = surfel->getTextureCoordinate();

					// write properties
					out->dumpSurfelPosition3 (position[0], position[1], position[2]);
					out->dumpSurfelNormal3(normal[0], normal[1], normal[2]);
					out->dumpSurfelTangentAxisOne3 (axisOne[0], axisOne[1], axisOne[2]);
					out->dumpSurfelTangentAxisTwo3 (axisTwo[0], axisTwo[1], axisTwo[2]);
		
					out->dumpSurfelColorRGBAuc (sfl::DIFFUSE, 
												qRed(diffuseColor),
												qGreen(diffuseColor),
												qBlue(diffuseColor),
												qAlpha(diffuseColor));

					out->dumpSurfelColorRGBAuc (sfl::SPECULAR, 
												qRed(specularColor),
												qGreen(specularColor),
												qBlue(specularColor),
												qAlpha(specularColor));

					out->dumpSurfelUserFlags (flags);	

					out->dumpSurfelColorCoefff (sfl::AMBIENT, ambientCoefficient);
					out->dumpSurfelColorCoefff (sfl::DIFFUSE, diffuseCoefficient);
					out->dumpSurfelColorCoefff (sfl::SPECULAR, specularCoefficient);
					out->dumpSurfelShininess (shininess);
					out->dumpSurfelTextureUV (textureCoordinate.u, textureCoordinate.v);

				out->endSurfel();

				// update the progress bar
				progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelCount * 100.0f / (float)nofSurfels + 0.5f);
				if (progress > statusBar->getProgress()) {
					statusBar->setProgress (progress, false);
				}
				surfelCount++;
			}
		}
		else {

			bool bPosition				= propertyDescriptor & SurfelInterface::POSITION;
			bool bNormal				= propertyDescriptor & SurfelInterface::NORMAL;
			bool bRadius				= propertyDescriptor & SurfelInterface::RADIUS;
			bool bDiffuseColor			= propertyDescriptor & SurfelInterface::DIFFUSE_COLOR;
			bool bSpecularColor			= propertyDescriptor & SurfelInterface::SPECULAR_COLOR;
			bool bFlags					= propertyDescriptor & SurfelInterface::FLAGS;
			bool bAmbientCoefficient	= propertyDescriptor & SurfelInterface::AMBIENT_COEFFICIENT;
			bool bDiffuseCoefficient	= propertyDescriptor & SurfelInterface::DIFFUSE_COEFFICIENT;
			bool bSpecularCoefficient	= propertyDescriptor & SurfelInterface::SPECULAR_COEFFICIENT;
			bool bShininess				= propertyDescriptor & SurfelInterface::SHININESS;
			bool bTextureCoordinate		= propertyDescriptor & SurfelInterface::TEXTURE_COORDINATE;
			bool bTangentAxes			= propertyDescriptor & SurfelInterface::TANGENT_AXES;

			for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {			

				out->beginSurfel();

					if (bPosition) {
						position = surfel->getPosition();
						out->dumpSurfelPosition3 (position[0], position[1], position[2]);
					}
					if (bNormal) {
						normal = surfel->getNormal();
						out->dumpSurfelNormal3(normal[0], normal[1], normal[2]);
					}
					if (bTangentAxes) {
						axisOne = surfel->getAxisOne();
						axisTwo = surfel->getAxisTwo();
						out->dumpSurfelTangentAxisOne3 (axisOne[0], axisOne[1], axisOne[2]);
						out->dumpSurfelTangentAxisTwo3 (axisTwo[0], axisTwo[1], axisTwo[2]);
					}
					if (bRadius) {
						radius = surfel->getRadius();
						out->dumpSurfelRadius(radius);
					}
					if (bDiffuseColor) {
						diffuseColor = surfel->getDiffuseColor();
						out->dumpSurfelColorRGBAuc (sfl::DIFFUSE, 
													qRed(diffuseColor),
													qGreen(diffuseColor),
													qBlue(diffuseColor),
													qAlpha(diffuseColor));
					}
					if (bSpecularColor) {
						specularColor = surfel->getSpecularColor();
						out->dumpSurfelColorRGBAuc (sfl::SPECULAR, 
													qRed(specularColor),
													qGreen(specularColor),
													qBlue(specularColor),
													qAlpha(specularColor));
					}
					if (bFlags) {
						flags = surfel->getFlags();
						out->dumpSurfelUserFlags (flags);	
					}
					if (bAmbientCoefficient) {
						ambientCoefficient = surfel->getAmbientCoefficient();
						out->dumpSurfelColorCoefff (sfl::AMBIENT, ambientCoefficient);
					}
					if (bDiffuseCoefficient) {
						diffuseCoefficient = surfel->getDiffuseCoefficient();
						out->dumpSurfelColorCoefff (sfl::DIFFUSE, diffuseCoefficient);
					}	
					if (bSpecularCoefficient) {
						specularCoefficient = surfel->getSpecularCoefficient();
						out->dumpSurfelColorCoefff (sfl::SPECULAR, specularCoefficient);					
					}
					if (bShininess) {
						shininess = surfel->getShininess();
						out->dumpSurfelShininess (shininess);
					}
					if (bTextureCoordinate) {
						textureCoordinate   = surfel->getTextureCoordinate();
						out->dumpSurfelTextureUV (textureCoordinate.u, textureCoordinate.v);
					}				

				out->endSurfel();

				// update the progress bar
				progress = (int)(((float)(currentObject + 1) / (float)nofObjects) * (float)surfelCount * 100.0f / (float)nofSurfels + 0.5f);
				if (progress > statusBar->getProgress()) {
					statusBar->setProgress (progress, false);
				}
				surfelCount++;
			}

		}
   	}



	statusBar->setProgress ((int)(((float)(currentObject + 1) / (float)nofObjects) * 100), true);

    out->endResolution();
   
    out->endSurfelSet();

}

int SFLFileFormat::isSurfelMarked (const uint surfelIndex, const uint32 *markerIndices, const uint nofMarkerIndices) const {

	uint i;
	int markerIndex = -1;

	i = 0;
	while (i < nofMarkerIndices && markerIndex < 0) {
		
		if (surfelIndex == markerIndices[i]) {
			markerIndex = i;
		}
		else {
			i++;
		}

	}

	return markerIndex;

}

// TODO: update this conversion method as soon as new surfel properties are added
SurfelInterface::PropertyDescriptor SFLFileFormat::sflPropertiesToPropertyDescriptor (const uint32 sflProps) {

	SurfelInterface::PropertyDescriptor result = 0x0;

	if (sflProps & SFLPROP_POSITION)       result |= SurfelInterface::POSITION;
	if (sflProps & SFLPROP_NORMAL)         result |= SurfelInterface::NORMAL;
	if (sflProps & SFLPROP_TANGENT_AXES)   result |= SurfelInterface::TANGENT_AXES;
	if (sflProps & SFLPROP_RADIUS)         result |= SurfelInterface::RADIUS;
	if (sflProps & SFLPROP_DIFFUSE_COLOR)  result |= SurfelInterface::DIFFUSE_COLOR;
	if (sflProps & SFLPROP_SPECULAR_COLOR) result |= SurfelInterface::SPECULAR_COLOR;
	if (sflProps & SFLPROP_USER_FLAGS)     result |= SurfelInterface::FLAGS;
	if (sflProps & SFLPROP_AMBIENT_COEFF)  result |= SurfelInterface::AMBIENT_COEFFICIENT;
	if (sflProps & SFLPROP_DIFFUSE_COEFF)  result |= SurfelInterface::DIFFUSE_COEFFICIENT;
	if (sflProps & SFLPROP_SPECULAR_COEFF) result |= SurfelInterface::SPECULAR_COEFFICIENT;
	if (sflProps & SFLPROP_SHININESS)      result |= SurfelInterface::SHININESS;
	if (sflProps & SFLPROP_TEXTURE_UV)     result |= SurfelInterface::TEXTURE_COORDINATE;
	
	return result;

}

// TODO: update this conversion method as soon as new surfel properties are added
uint32 SFLFileFormat::propertyDescriptorToSFLProperties (const SurfelInterface::PropertyDescriptor propDescr) {

	uint32 result = 0x0;

	if (propDescr &	SurfelInterface::POSITION)             result |= SFLPROP_POSITION;
	if (propDescr &	SurfelInterface::NORMAL)               result |= SFLPROP_NORMAL;
	if (propDescr &	SurfelInterface::TANGENT_AXES)         result |= SFLPROP_TANGENT_AXES;
	if (propDescr &	SurfelInterface::RADIUS)               result |= SFLPROP_RADIUS;
	if (propDescr &	SurfelInterface::DIFFUSE_COLOR)        result |= SFLPROP_DIFFUSE_COLOR;
	if (propDescr &	SurfelInterface::SPECULAR_COLOR)       result |= SFLPROP_SPECULAR_COLOR;
	if (propDescr &	SurfelInterface::FLAGS)                result |= SFLPROP_USER_FLAGS;
	if (propDescr &	SurfelInterface::AMBIENT_COEFFICIENT)  result |= SFLPROP_AMBIENT_COEFF;
	if (propDescr &	SurfelInterface::DIFFUSE_COEFFICIENT)  result |= SFLPROP_DIFFUSE_COEFF;
	if (propDescr &	SurfelInterface::SPECULAR_COEFFICIENT) result |= SFLPROP_SPECULAR_COEFF;
	if (propDescr &	SurfelInterface::SHININESS)            result |= SFLPROP_SHININESS;
	if (propDescr & SurfelInterface::TEXTURE_COORDINATE)   result |= SFLPROP_TEXTURE_UV;
	
	return result;

}

// ****************
// access functions
// ****************

FileFormatPluginInterface *createFileFormatPlugin() {
	return new SFLFileFormat();
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
