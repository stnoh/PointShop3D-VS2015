// Title:   RendererManager.cpp
// Created: Thu Sep 25 14:11:35 2003
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

#include <stdlib.h>  // for exit()
#include <time.h>

#include <qmessagebox.h>
#include <qsize.h>
#include <qapplication.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../Configuration/src/Configuration.h"
#include "../../Scene/src/Scene.h"
#include "../../Configuration/src/Configuration.h"
#include "../../Transformer/src/Transformer.h"
#include "RendererRegistry.h"
#include "RendererDirWalk.h"
#include "RendererManager.h"

// *********
// constants
// *********

static const int NOF_TIMINGS = 25;

// ******
// global
// ******

// the single RendererManager instance, only
// accesible by getInstance() and
// destroyInstance()
static RendererManager *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

RendererManager::RendererManager() {

	Configuration *configuration;
	Scene         *scene;

	mainRenderer       = 0;
	auxiliaryRenderer  = 0;
	previewRenderer    = 0;
	mainRendererFPS    = 0.0f;
	previewRendererFPS = 0.0f;
	isPreviewRendererActive = false;

	this->initializeTimings();

	configuration = Configuration::getInstance();
	// get informed whenever the renderer plugins directory changes, as to re-initialize this RendererManager
	this->connect (configuration, SIGNAL (rendererPluginDirectoryChanged (const QString)),
		           this, SLOT (handleRendererPluginDirectoryChanged (const QString)));

	// get informed whenever the main renderer type changes
	this->connect (configuration, SIGNAL (mainRendererTypeChanged (const QString)),
		           this, SLOT (handleMainRendererTypeChanged (const QString)));

	// get informed whenever the auxiliary renderer type changes
	this->connect (configuration, SIGNAL (auxiliaryRendererTypeChanged (const QString)),
		           this, SLOT (handleAuxiliaryRendererTypeChanged (const QString)));

	// get informed whenever the preview renderer type changes
	this->connect (configuration, SIGNAL (previewRendererTypeChanged (const QString)),
		           this, SLOT (handlePreviewRendererTypeChanged (const QString)));

	// get informed whenever the view port size changed
	this->connect (configuration, SIGNAL (viewPortSizeChanged (const QSize)),
		           this, SLOT (handleViewPortSizeChanged (const QSize)));

	// get informed whenever the view frustum changed
	this->connect (configuration, SIGNAL (viewFrustumChanged (const MyDataTypes::ViewFrustum)),
		           this, SLOT (handleViewFrustumChanged (const MyDataTypes::ViewFrustum)));

	// get informed whenever the light direction changed changed
	this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		           this, SLOT (handleLightDirectionChanged (const Vector3D)));

	this->connect (configuration, SIGNAL (backgroundColorChanged (const QRgb)),
		           this, SLOT (handleBackgroundColorChanged (const QRgb)));

	this->connect (configuration, SIGNAL (previewRendererEnabled (const bool)),
		           this, SLOT (handlePreviewRendererEnabled (const bool)));

	this->connect (configuration, SIGNAL (mainShadingChanged (const bool)),
		           this, SLOT (handleMainShadingChanged (const bool)));

	this->connect (configuration, SIGNAL (auxiliaryShadingChanged (const bool)),
		           this, SLOT (handleAuxiliaryShadingChanged (const bool)));

	this->connect (configuration, SIGNAL (previewShadingChanged (const bool)),
		           this, SLOT (handlePreviewShadingChanged (const bool)));

	scene = Scene::getInstance();

	// get informed whenever the Scene moved
	this->connect (scene, SIGNAL (sceneMoved (const float*)),
		           this, SLOT (handleSceneMoved (const float*)));

	this->connect (scene, SIGNAL (objectMoved()),
		           this, SLOT (handleObjectMoved()));

	// get informed whenever the Scene is to be rendered
	this->connect (scene, SIGNAL (renderFrame()),
		           this, SLOT (handleRenderFrame()));
	
};

RendererManager::~RendererManager() {

	// it is important to delete the main and auxiliary renderer instances first...
	if (mainRenderer != 0) {
		delete mainRenderer;
	}
	if (auxiliaryRenderer != 0) {
		delete auxiliaryRenderer;
	}
	if (previewRenderer != 0) {
		delete previewRenderer;
	}

	// ... then unload all dynamic libraries
	RendererRegistry::destroyInstance();

};

// **************
// public methods
// **************

RendererManager *RendererManager::getInstance() {

	if (instance == 0) {
		instance = new RendererManager();
	}
	return instance;

}

void RendererManager::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

bool RendererManager::initialize (const QString rendererPluginDirectory, QWidget *parent) {

	uint             nofDefaultRenderers,     // should be 1, could be 0 or > 1
		             nofPreviewRenderers,     // should be 1, could be 0 or > 1
					 nofAdditionalRenderers;

	RendererDirWalk  rendererDirWalk (rendererPluginDirectory);
	RendererDirWalk  defaultDirWalk (rendererPluginDirectory + QDir::convertSeparators ("/") + "default");
	RendererDirWalk  previewDirWalk (rendererPluginDirectory + QDir::convertSeparators ("/") + "preview");
	RendererUserData rendererUserData (&availableRendererTypes);

	Configuration    *configuration;

	bool             renderersFound;

	this->parent = parent;

	// clear the registry first...
	RendererRegistry::getInstance()->unregisterAllRendererPlugins();
	availableRendererTypes.clear();
	mainRenderer      = 0;
	auxiliaryRenderer = 0;
	previewRenderer   = 0;

	// ...register the default renderer (the default renderer gets registered first)
	defaultDirWalk.execute (&rendererUserData);
	nofDefaultRenderers = availableRendererTypes.count();
	if (nofDefaultRenderers > 0) {

		mainRendererType           = availableRendererTypes.first();
		hasMainRendererTypeChanged = true;
		emit mainRendererChanged();

	}
	
	// ...register the preview renderer
	previewDirWalk.execute (&rendererUserData);
	nofPreviewRenderers = availableRendererTypes.count() - nofDefaultRenderers;
	if (nofPreviewRenderers > 0) {

		previewRendererType = availableRendererTypes [nofDefaultRenderers];
		hasPreviewRendererTypeChanged = true;
		emit previewRendererChanged();

	}
	
	// ... then update the registry with additional renderers
	rendererDirWalk.execute (&rendererUserData);
	nofAdditionalRenderers = availableRendererTypes.count() - nofDefaultRenderers - nofPreviewRenderers;
	if (nofAdditionalRenderers > 0) {

		if (nofDefaultRenderers > 0) {
			// if we have found a default renderer, use it as the auxiliary renderer as well...
			auxiliaryRendererType = availableRendererTypes.first();
		}
		else {
			// ...else use the first found additional renderer as the auxiliary renderer
			auxiliaryRendererType = availableRendererTypes [nofDefaultRenderers + nofPreviewRenderers];
		}
		hasAuxiliaryRendererTypeChanged = true;
		emit previewRendererChanged();

	}
	else {

		auxiliaryRendererType = availableRendererTypes.first();
		hasAuxiliaryRendererTypeChanged = true;
		emit auxiliaryRendererChanged();

	}
	
	// if no default renderer has been found, we take the first renderer found
	// - from the additional renders, if available
	// - else the preview renderer, if available
	if (nofDefaultRenderers == 0) {

		if (nofAdditionalRenderers > 0) {
			mainRendererType    = availableRendererTypes[nofPreviewRenderers];
			hasMainRendererTypeChanged = true;
			emit mainRendererChanged();
		}
		else if (nofPreviewRenderers > 0) {
			mainRendererType    = availableRendererTypes.first();
			hasMainRendererTypeChanged = true;
			emit mainRendererChanged();
		}
		
	}
	
	// if no preview renderer has been found, we take the first renderer found
	// - from the default renderers, if available
	// - from the additional renderers, if available
	if (nofPreviewRenderers == 0) {

		if (nofDefaultRenderers > 0) {
			previewRendererType    = availableRendererTypes.first();
			hasPreviewRendererTypeChanged = true;
			emit previewRendererChanged();
		}
		else if (nofAdditionalRenderers > 0) {
			previewRendererType    = availableRendererTypes.first();
			hasPreviewRendererTypeChanged = true;
			emit previewRendererChanged();
		}
		
	}
	
	renderersFound = (nofDefaultRenderers > 0) || (nofPreviewRenderers > 0) || (nofAdditionalRenderers > 0);
	if (renderersFound == true) {

		// update the configuration
		configuration = Configuration::getInstance();
		configuration->setAvailableRendererTypes (availableRendererTypes);
		configuration->setMainRendererType (mainRendererType);
		configuration->setAuxiliaryRendererType (auxiliaryRendererType);
		configuration->setPreviewRendererType (previewRendererType);

	}

	return renderersFound;

}

RendererInterface *RendererManager::getCurrentRenderer() {

	if (isPreviewRendererEnabled()) {
		return previewRenderer;
	}
	else {
		return mainRenderer;
	}
}

RendererInterface *RendererManager::getMainRenderer() {

	RendererLoader::RendererAccess       rendererAccess;
	Configuration                        *configuration;
	MyDataTypes::TransformationMatrix16f sceneViewMatrix;

	if (mainRenderer != 0) {

		if (hasMainRendererTypeChanged == false) {
			return mainRenderer;
		}
		else {
			delete mainRenderer;
		}

	}

	// now there has never been a renderer instance before or another renderer instance has been requested
	rendererAccess = RendererRegistry::getInstance()->getRendererAccess (mainRendererType);
	// the renderer access is guaranteed to be valid, no need to check the 'isValid' field

	mainRenderer = rendererAccess.createRendererFunction();
	mainRenderer->initialize (true, parent);
	this->connect (mainRenderer, SIGNAL (rendererWidgetRepainted()),
		           this, SLOT (handleMainWidgetRepainted()));

	configuration = Configuration::getInstance();

	// setup the renderer
	mainRenderer->setViewFrustum (configuration->getViewFrustum());
	mainRenderer->setViewPortSize (configuration->getViewPortSize());
	mainRenderer->setTwoSidedNormalsEnabled (false);
	mainRenderer->setShadingEnabled (configuration->isMainShadingEnabled());
	mainRenderer->setLightDirection (configuration->getLightDirection());
	mainRenderer->setBackgroundColor(configuration->getBackgroundColor());

	// we have to initialize the renderer with the scene view of the new scene
	Scene::getInstance()->getTransformationMatrix (sceneViewMatrix);
	mainRenderer->setSceneView (sceneViewMatrix);

	hasMainRendererTypeChanged = false;

	return mainRenderer;

}

RendererInterface *RendererManager::getAuxiliaryRenderer() {

	RendererLoader::RendererAccess rendererAccess;

	if (auxiliaryRenderer != 0) {

		if (hasAuxiliaryRendererTypeChanged == false) {
			return auxiliaryRenderer;
		}
		else {
			delete auxiliaryRenderer;
		}

	}

	// now there has never been a renderer instance before or another renderer instance has been requested
	rendererAccess = RendererRegistry::getInstance()->getRendererAccess (auxiliaryRendererType);
	// the renderer access is guaranteed to be valid, no need to check the 'isValid' field

	auxiliaryRenderer = rendererAccess.createRendererFunction();
	auxiliaryRenderer->initialize (false, 0);
	hasAuxiliaryRendererTypeChanged = false;

	return auxiliaryRenderer;

}

RendererInterface *RendererManager::getPreviewRenderer() {

	RendererLoader::RendererAccess      rendererAccess;
	Configuration                        *configuration;
	MyDataTypes::TransformationMatrix16f sceneViewMatrix;

	if (previewRenderer != 0) {

		if (hasPreviewRendererTypeChanged == false) {
			return previewRenderer;
		}
		else {
			delete previewRenderer;
		}

	}

	// now there has never been a renderer instance before or another renderer instance has been requested
	rendererAccess = RendererRegistry::getInstance()->getRendererAccess (previewRendererType);
	// the renderer access is guaranteed to be valid, no need to check the 'isValid' field

	previewRenderer = rendererAccess.createRendererFunction();
	previewRenderer->initialize (true, parent);
	this->connect (previewRenderer, SIGNAL (rendererWidgetRepainted()),
		           this, SLOT (handlePreviewWidgetRepainted()));

	configuration = Configuration::getInstance();

	// setup the renderer
	previewRenderer->setViewFrustum (configuration->getViewFrustum());
	previewRenderer->setViewPortSize (configuration->getViewPortSize());
	previewRenderer->setShadingEnabled (configuration->isPreviewShadingEnabled());
	previewRenderer->setLightDirection (configuration->getLightDirection());
	previewRenderer->setBackgroundColor(configuration->getBackgroundColor());

	// we have to initialize the renderer with the scene view of the new scene
	Scene::getInstance()->getTransformationMatrix (sceneViewMatrix);
	previewRenderer->setSceneView (sceneViewMatrix);

	hasPreviewRendererTypeChanged = false;

	return previewRenderer;

}

void RendererManager::setMainRendererType (const QString newMainRendererType) {
	
	if (mainRendererType != newMainRendererType) {
		mainRendererType = newMainRendererType;
		hasMainRendererTypeChanged = true;
		emit mainRendererChanged();
	}

}

QString RendererManager::getMainRendererType() const {
	return mainRendererType;
}

void RendererManager::setAuxiliaryRendererType (const QString newAuxiliaryRendererType) {
	
	if (auxiliaryRendererType != newAuxiliaryRendererType) {
		auxiliaryRendererType = newAuxiliaryRendererType;
		hasAuxiliaryRendererTypeChanged = true;
		emit auxiliaryRendererChanged();
	}

}

QString RendererManager::getAuxiliaryRendererType() const {
	return auxiliaryRendererType;
}

void RendererManager::setPreviewRendererType (const QString newPreviewRendererType) {
	
	if (previewRendererType != newPreviewRendererType) {
		previewRendererType = newPreviewRendererType;
		hasPreviewRendererTypeChanged = true;
		emit previewRendererChanged();
	}

}

QString RendererManager::getPreviewRendererType() const {
	return previewRendererType;
}

QStringList RendererManager::getAvailableRendererTypes() const {
	return availableRendererTypes;
}

void RendererManager::setPreviewRendererEnabled (const bool enable, const bool renderFrame) {
	
	Configuration                        *configuration;
	MyDataTypes::TransformationMatrix16f sceneViewMatrix;

	isPreviewRendererActive = enable;

	configuration = Configuration::getInstance();
	if (enable == false) {

		if (mainRenderer != 0) {
		
			// update the main renderer
			mainRenderer->setViewFrustum (configuration->getViewFrustum());
			mainRenderer->setViewPortSize (configuration->getViewPortSize());
			Scene::getInstance()->getTransformationMatrix (sceneViewMatrix);
			mainRenderer->setSceneView (sceneViewMatrix);
			mainRenderer->setLightDirection (configuration->getLightDirection());

		}

	}
	else {

		if (previewRenderer != 0) {

			// update the preview renderer
			previewRenderer->setViewFrustum (configuration->getViewFrustum());
			previewRenderer->setViewPortSize (configuration->getViewPortSize());
			Scene::getInstance()->getTransformationMatrix (sceneViewMatrix);
			previewRenderer->setSceneView (sceneViewMatrix);
			previewRenderer->setLightDirection (configuration->getLightDirection());

		}

	}

	if (renderFrame == true) {
		this->handleRenderFrame();
	}
	emit previewRendererEnabled (enable);

}

bool RendererManager::isPreviewRendererEnabled() const {
	return isPreviewRendererActive;
}

float RendererManager::getMainRendererFPS() const {
	return mainRendererFPS;
}

float RendererManager::getPreviewRendererFPS() const {
	return previewRendererFPS;
}

// ***************
// private methods
// ***************

void RendererManager::initializeTimings() {

	int i;

	mainRendererTimings.resize (NOF_TIMINGS);
	previewRendererTimings.resize (NOF_TIMINGS);

	for (i = 0; i < NOF_TIMINGS; i++) {
		mainRendererTimings.at(i)    = 0.0f;
		previewRendererTimings.at(i) = 0.0f;
	}

	mainRendererTimingsIndex    = 0;
	previewRendererTimingsIndex = 0;

}

void RendererManager::updateMainRendererFPS() {

	int i;

	mainRendererFPS = 0.0f;
	for (i = 0; i < NOF_TIMINGS; i++) {
		mainRendererFPS += mainRendererTimings[i];
	}

	mainRendererFPS = (float)NOF_TIMINGS / mainRendererFPS;
	emit mainRendererFPSChanged (mainRendererFPS);

}

void RendererManager::updatePreviewRendererFPS() {

	int i;

	previewRendererFPS = 0.0f;
	for (i = 0; i < NOF_TIMINGS; i++) {
		previewRendererFPS += previewRendererTimings[i];
	}

	previewRendererFPS = (float)NOF_TIMINGS / previewRendererFPS;
	emit previewRendererFPSChanged (previewRendererFPS);

}

double RendererManager::now() {
	return (double)clock() / CLOCKS_PER_SEC;	
}


// *************
// private slots
// *************

void RendererManager::handleRendererPluginDirectoryChanged (const QString newRendererPluginDirectory) {

	Configuration *configuration;

	configuration = Configuration::getInstance();
	
	// initialize again
	if (this->initialize (newRendererPluginDirectory, parent) == false) {
	
		qDebug ("RendererManager: handleRendererPluginDirectoryChanged: NO RENDERER FOUND in %s!!", newRendererPluginDirectory.latin1());
		
		exit (-1);

	}

}

void RendererManager::handleMainRendererTypeChanged (const QString newRendererType) {
	this->setMainRendererType (newRendererType);
}

void RendererManager::handleAuxiliaryRendererTypeChanged (const QString newRendererType) {
	this->setAuxiliaryRendererType (newRendererType);
}

void RendererManager::handlePreviewRendererTypeChanged (const QString newRendererType) {
	this->setPreviewRendererType (newRendererType);
}

void RendererManager::handleSceneMoved (const float* transformationMatrix) {

	Transformer::getSceneTransformer()->setSceneTransformationMatrix (transformationMatrix);
	if (isPreviewRendererActive == false) {
		if (mainRenderer != 0) {			
			mainRenderer->setSceneView (transformationMatrix);
		}
	}
	else {
		if (previewRenderer != 0) {
			previewRenderer->setSceneView (transformationMatrix);
		}
	}

}

void RendererManager::handleObjectMoved() {

	MyDataTypes::TransformationMatrix16f objectTransformationMatrix;
	Object                               *activeObject;

 	activeObject = Scene::getInstance()->getActiveObject();
	if (activeObject != 0) {
		activeObject->getTransformationMatrix (objectTransformationMatrix);
		Transformer::getSceneTransformer()->setObjectTransformationMatrix (objectTransformationMatrix);
	}

}

void RendererManager::handleRenderFrame() {	

	start = this->now();
	if ((isPreviewRendererActive == false) && (mainRenderer != 0)) {
		mainRenderer->renderFrame();
	}
	else if ((isPreviewRendererActive == true) && (previewRenderer != 0)) {
		previewRenderer->renderFrame();		
		qDebug ("handleRender----------------");
	}
}

void RendererManager::handleViewPortSizeChanged (const QSize newViewPortSize) {
	
	Configuration            *configuration;
	MyDataTypes::ViewFrustum newViewFrustum;
	Transformer              *sceneTransformer;

	if ((isPreviewRendererActive == false) && (mainRenderer == 0) ||
		(isPreviewRendererActive == true)  && (previewRenderer == 0)) {
		return;
	}

	sceneTransformer = Transformer::getSceneTransformer();
	sceneTransformer->setViewPortSize (newViewPortSize);
	
	// also update the view frustum
	configuration = Configuration::getInstance();
	newViewFrustum = configuration->getViewFrustum();
	newViewFrustum.aspectRatio = (float)newViewPortSize.width() / (float)newViewPortSize.height();

	// temporarily disconnect, so we won't update the view frustum in the renderer twice...
	this->disconnect (configuration, SIGNAL (viewFrustumChanged (const MyDataTypes::ViewFrustum)),
		              this, SLOT (handleViewFrustumChanged (const MyDataTypes::ViewFrustum)));
	configuration->setViewFrustum (newViewFrustum);	
	sceneTransformer->setViewFrustum (newViewFrustum);
	
	// ... and connect again
	this->connect (configuration, SIGNAL (viewFrustumChanged (const MyDataTypes::ViewFrustum)),
		           this, SLOT (handleViewFrustumChanged (const MyDataTypes::ViewFrustum)));
	
	if (isPreviewRendererActive == false) {
		mainRenderer->setViewPortSize (newViewPortSize);
		mainRenderer->setViewFrustum (newViewFrustum);
		mainRenderer->renderFrame();
	}
	else {
		previewRenderer->setViewPortSize (newViewPortSize);
		previewRenderer->setViewFrustum (newViewFrustum);
		previewRenderer->renderFrame();
	}

}

void RendererManager::handleViewFrustumChanged (const MyDataTypes::ViewFrustum newViewFrustum) {

	if ((isPreviewRendererActive == false) && (mainRenderer == 0) ||
		(isPreviewRendererActive == true)  && (previewRenderer == 0)) {
		return;
	}

	Transformer::getSceneTransformer()->setViewFrustum (newViewFrustum);
	
	if (isPreviewRendererActive == false) {
		mainRenderer->setViewFrustum (newViewFrustum);
		mainRenderer->renderFrame();
	}
	else {
		previewRenderer->setViewFrustum (newViewFrustum);
		previewRenderer->renderFrame();
	}

}

void RendererManager::handleLightDirectionChanged (const Vector3D newLightDirection) {

	if ((isPreviewRendererActive == false) && (mainRenderer == 0) ||
		(isPreviewRendererActive == true)  && (previewRenderer == 0)) {
		return;
	}
	
	if (isPreviewRendererActive == false) {
		mainRenderer->setLightDirection (newLightDirection);
		mainRenderer->renderFrame();
	}
	else {
		previewRenderer->setLightDirection (newLightDirection);
		previewRenderer->renderFrame();
	}

}

void RendererManager::handleBackgroundColorChanged (const QRgb newBackgroundColor) {

	if (mainRenderer) {
		mainRenderer->setBackgroundColor(newBackgroundColor);
		if (isPreviewRendererActive == false) {
			mainRenderer->renderFrame();
		}
	}

	if (previewRenderer) {
		previewRenderer->setBackgroundColor(newBackgroundColor);
		if (isPreviewRendererActive == true) {
			previewRenderer->renderFrame();
		}
	}
}


void RendererManager::handlePreviewRendererEnabled (const bool isEnabled) {
	this->setPreviewRendererEnabled (isEnabled, true);
}

void RendererManager::handleMainShadingChanged (const bool isEnabled) {

	if (mainRenderer != 0) {
		mainRenderer->setShadingEnabled (isEnabled);
	}

}

void RendererManager::handleAuxiliaryShadingChanged (const bool isEnabled) {

	if (auxiliaryRenderer != 0) {
		auxiliaryRenderer->setShadingEnabled (isEnabled);
	}

}

void RendererManager::handlePreviewShadingChanged (const bool isEnabled) {

	if (previewRenderer != 0) {
		previewRenderer->setShadingEnabled (isEnabled);
	}

}

void RendererManager::handleMainWidgetRepainted() {

	double elapsed;

	end = this->now();
	elapsed = (float)(end - start);
	mainRendererTimings[mainRendererTimingsIndex] = elapsed;
	mainRendererTimingsIndex = (++mainRendererTimingsIndex) % NOF_TIMINGS;
	this->updateMainRendererFPS();

}

void RendererManager::handlePreviewWidgetRepainted() {

	double elapsed;

	end = this->now();
	elapsed = (float)(end - start);
	previewRendererTimings[previewRendererTimingsIndex] = elapsed;
	previewRendererTimingsIndex = (++previewRendererTimingsIndex) % NOF_TIMINGS;
	this->updatePreviewRendererFPS();

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
