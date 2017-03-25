// Title:   Configuration.cpp
// Created: Thu Sep 25 14:23:22 2003
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

#include <qsize.h>
#include <qstring.h>
#include <qstringlist.h>

#include "../../../Utilities/src/Matrix.h"
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "Configuration.h"

// ******
// global
// ******

char* replaceSpace(char *str) {

	for (unsigned int i = 0; str[i] != '\0'; i++) {
		if (str[i] == ' ') {
			str[i] = '#';
		}
	}

	return str;
}

void insertSpace(char *str) {

	for (unsigned int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '#') {
			str[i] = ' ';
		}
	}
}


// the single configuration instance, only
// accesible by getInstance() and
// deleteInstance()
static Configuration *configuration = 0;

QString Configuration::defaultFileName = "DefaultSettings.txt";

bool Configuration::readSettingsFromFile(FILE *file, FileSettings& fileSettings) {

	bool b = false;
	
	if (file) {

		b = true;

		// Directories
		b &= (fscanf(file, "Bitmaps Dir: %s\n", fileSettings.bitmapsDir) > 0);
		b &= (fscanf(file, "Brush Dir: %s\n", fileSettings.brushesDir) > 0);
		b &= (fscanf(file, "Tool Dir: %s\n", fileSettings.toolsDir) > 0);
		b &= (fscanf(file, "Plugin Dir: %s\n", fileSettings.pluginsDir) > 0);
		b &= (fscanf(file, "Data Dir: %s\n", fileSettings.dataDir) > 0);
		b &= (fscanf(file, "RendererPlugin Dir: %s\n", fileSettings.rendererPluginsDir) > 0);

		insertSpace(fileSettings.bitmapsDir);
		insertSpace(fileSettings.brushesDir);
		insertSpace(fileSettings.toolsDir);
		insertSpace(fileSettings.pluginsDir);
		insertSpace(fileSettings.dataDir);
		insertSpace(fileSettings.rendererPluginsDir);

		// Renderer General
		b &= (fscanf(file, "Main Renderer: %s\n", fileSettings.mainRendererName) > 0);
		b &= (fscanf(file, "Preview Renderer: %s\n", fileSettings.previewRendererName) > 0);
		b &= (fscanf(file, "Main Shading: %i\n", &fileSettings.bMainShading) > 0);
		b &= (fscanf(file, "Preview Shading: %i\n", &fileSettings.bPreviewShading) > 0);
		b &= (fscanf(file, "Aspect Ratio: %s\n", fileSettings.aspectRatio) > 0);
		b &= (fscanf(file, "Field of View: %s\n", fileSettings.fieldOfView) > 0);
		b &= (fscanf(file, "Near Plane: %s\n", fileSettings.nearPlane) > 0);
		b &= (fscanf(file, "Far Plane: %s\n", fileSettings.farPlane) > 0);

		insertSpace(fileSettings.mainRendererName);
		insertSpace(fileSettings.previewRendererName);
		insertSpace(fileSettings.aspectRatio);
		insertSpace(fileSettings.fieldOfView);
		insertSpace(fileSettings.nearPlane);
		insertSpace(fileSettings.farPlane);	
		
		// Lighting
		b &= (fscanf(file, "Light X: %s\n", fileSettings.lightX) > 0);
		b &= (fscanf(file, "Light Y: %s\n", fileSettings.lightY) > 0);
		b &= (fscanf(file, "Light Z: %s\n", fileSettings.lightZ) > 0);

		insertSpace(fileSettings.lightX);
		insertSpace(fileSettings.lightY);
		insertSpace(fileSettings.lightZ);

		// GUI
		b &= (fscanf(file, "Bitmap Width: %i\n", &fileSettings.bitmapWidth) > 0);
		b &= (fscanf(file, "Bitmap Height: %i\n", &fileSettings.bitmapHeight) > 0);
		b &= (fscanf(file, "Show Bitmaps: %i\n", &fileSettings.bShowBitmaps) > 0);
		b &= (fscanf(file, "Show Names: %i\n", &fileSettings.bShowNames) > 0);
		b &= (fscanf(file, "Use Alpha Masks: %i\n", &fileSettings.bUseAlphaMasks) > 0);
		b &= (fscanf(file, "Background Color: %i\n", &fileSettings.backgroundColor) > 0);

		// Settings
		b &= (fscanf(file, "Auto Save: %i\n", &fileSettings.bAutoSave) > 0);		
	}

	return b;
}

void Configuration::writeSettingsToFile(FILE *file, FileSettings &fileSettings) {

	if (file) {

		// Directories
		fprintf(file, "Bitmaps Dir: %s\n", replaceSpace(fileSettings.bitmapsDir));
		fprintf(file, "Brush Dir: %s\n", replaceSpace(fileSettings.brushesDir));
		fprintf(file, "Tool Dir: %s\n", replaceSpace(fileSettings.toolsDir));
		fprintf(file, "Plugin Dir: %s\n", replaceSpace(fileSettings.pluginsDir));
		fprintf(file, "Data Dir: %s\n", replaceSpace(fileSettings.dataDir));
		fprintf(file, "RendererPlugin Dir: %s\n", replaceSpace(fileSettings.rendererPluginsDir));
				
		// Renderer General
		fprintf(file, "Main Renderer: %s\n", replaceSpace(fileSettings.mainRendererName));
		fprintf(file, "Preview Renderer: %s\n", replaceSpace(fileSettings.previewRendererName));
		fprintf(file, "Main Shading: %i\n", fileSettings.bMainShading);
		fprintf(file, "Preview Shading: %i\n", fileSettings.bPreviewShading);
		fprintf(file, "Aspect Ratio: %s\n", replaceSpace(fileSettings.aspectRatio));
		fprintf(file, "Field of View: %s\n", replaceSpace(fileSettings.fieldOfView));
		fprintf(file, "Near Plane: %s\n", replaceSpace(fileSettings.nearPlane));
		fprintf(file, "Far Plane: %s\n", replaceSpace(fileSettings.farPlane));
		
		// Lighting
		fprintf(file, "Light X: %s\n", replaceSpace(fileSettings.lightX));
		fprintf(file, "Light Y: %s\n", replaceSpace(fileSettings.lightY));
		fprintf(file, "Light Z: %s\n", replaceSpace(fileSettings.lightZ));

		// GUI
		fprintf(file, "Bitmap Width: %i\n", fileSettings.bitmapWidth);
		fprintf(file, "Bitmap Height: %i\n", fileSettings.bitmapHeight);
		fprintf(file, "Show Bitmaps: %i\n", fileSettings.bShowBitmaps);
		fprintf(file, "Show Names: %i\n", fileSettings.bShowNames);
		fprintf(file, "Use Alpha Masks: %i\n", fileSettings.bUseAlphaMasks);	
		fprintf(file, "Background Color: %i\n", fileSettings.backgroundColor);

		// Settings
		fprintf(file, "Auto Save: %i\n", fileSettings.bAutoSave);

	}

}

Configuration::Configuration() {

	// try to read default values from file
	FILE *file;
	bool bSuccess = false;

	Configuration::FileSettings fileSettings;

	file = fopen(Configuration::defaultFileName, "r" ); 
	
	if (file) {

		bSuccess = this->readSettingsFromFile(file, fileSettings);
		fclose (file);
	}

	if (bSuccess) {
		viewPortSize.setWidth (800);
		viewPortSize.setHeight (600);

        minimumViewPortSize.setWidth(200);
        minimumViewPortSize.setHeight(200);

		viewFrustum.fieldOfView = QString(fileSettings.fieldOfView).toFloat();
		viewFrustum.aspectRatio = QString(fileSettings.aspectRatio).toFloat();
		viewFrustum.nearPlane   = QString(fileSettings.nearPlane).toFloat();
		viewFrustum.farPlane    = QString(fileSettings.farPlane).toFloat();

		mainShadingEnabled           = fileSettings.bMainShading;
		auxiliaryShadingEnabled      = false;
		previewShadingEnabled        = fileSettings.bPreviewShading;
		previewRendererActivated     = false;

		pluginDirectory               = fileSettings.pluginsDir;
		rendererPluginDirectory       = fileSettings.rendererPluginsDir;
		toolDirectory                 = fileSettings.toolsDir;
		fileFormatPluginDirectory     = QString ("FileFormatPlugins");
		brushDirectory				  = fileSettings.brushesDir;
		bitmapDirectory               = fileSettings.bitmapsDir;
		dataDirectory                 = fileSettings.dataDir;

		MtrUnity4x4f (homePosition.scaleTranslationMatrix);
		homePosition.scaleTranslationMatrix[14] = -1000.0f;
		MtrUnity4x4f (homePosition.rotationMatrix);

		lightDirection[0] = QString(fileSettings.lightX).toFloat();
		lightDirection[1] = QString(fileSettings.lightY).toFloat();
		lightDirection[2] = QString(fileSettings.lightZ).toFloat();

		backgroundColor					= fileSettings.backgroundColor;
	}
	else {
		// ************************ 
		// init with default values
		// ************************
		viewPortSize.setWidth (800);
		viewPortSize.setHeight (600);

        minimumViewPortSize.setWidth(200);
        minimumViewPortSize.setHeight(200);

		viewFrustum.fieldOfView = 30.0f;
		viewFrustum.aspectRatio = 1.0f;
		viewFrustum.nearPlane   = 10.0f;
		viewFrustum.farPlane    = 100000.0f;

		mainShadingEnabled           = true;
		auxiliaryShadingEnabled      = false;
		previewShadingEnabled        = true;
		previewRendererActivated     = false;

		// the available renderer types and the currently used renderers are to be initialized by the RendererManager

		pluginDirectory               = QString ("Plugins");
		rendererPluginDirectory       = QString ("Renderers");
		toolDirectory                 = QString ("Tools");
		fileFormatPluginDirectory     = QString ("FileFormatPlugins");
		brushDirectory				  = QString ("Brushes");
		bitmapDirectory               = QString ("Bitmaps");
		dataDirectory                 = QString ("Models");

		MtrUnity4x4f (homePosition.scaleTranslationMatrix);
		homePosition.scaleTranslationMatrix[14] = -1000.0f;
		MtrUnity4x4f (homePosition.rotationMatrix);

		lightDirection[0] = 0.0f;
		lightDirection[1] = 0.0f;
		lightDirection[2] = -1.0f;

		backgroundColor = 0x008598b5;
	}

}

Configuration::~Configuration() {}

// *****************
// Singleton Pattern
// *****************

Configuration *Configuration::getInstance() {

	if (configuration == 0) {
		configuration = new Configuration();
	}
	return configuration;

}

void Configuration::destroyInstance() {

	if (configuration != 0) {
		delete configuration;
	}
	configuration = 0;

}

void Configuration::setMainRendererType (const QString newRendererType) {
	mainRendererType = newRendererType;
	emit mainRendererTypeChanged (newRendererType);
}

QString Configuration::getMainRendererType() const {
	return mainRendererType;
}

void Configuration::setAuxiliaryRendererType (const QString newRendererType) {
	auxiliaryRendererType = newRendererType;
	emit auxiliaryRendererTypeChanged (newRendererType);
}

QString Configuration::getAuxiliaryRendererType() const {
	return auxiliaryRendererType;
}

void Configuration::setPreviewRendererType (const QString newRendererType) {
	previewRendererType = newRendererType;
	emit previewRendererTypeChanged (newRendererType);
}

QString Configuration::getPreviewRendererType() const {
	return previewRendererType;
}

void Configuration::setAvailableRendererTypes (const QStringList newAvailableRendererTypes) {
	availableRendererTypes = newAvailableRendererTypes;
	emit availableRendererTypesChanged (newAvailableRendererTypes);
}

QStringList Configuration::getAvailableRendererTypes() const {
	return availableRendererTypes;
}

void Configuration::setPreviewRendererEnabled (const bool enable) {
	previewRendererActivated = enable;
	emit previewRendererEnabled (enable);
}

bool Configuration::isPreviewRendererEnabled() const {
	return previewRendererActivated;
}

void Configuration::setViewPortSize (const QSize newViewPortSize) {
	viewPortSize = newViewPortSize;
	emit viewPortSizeChanged (newViewPortSize);
}

QSize Configuration::getViewPortSize() const {
	return viewPortSize;
}

QSize Configuration::getMinimumViewPortSize() const {
	return minimumViewPortSize;
}

void Configuration::setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum) {
	viewFrustum = newViewFrustum;
	emit viewFrustumChanged (newViewFrustum);
}

MyDataTypes::ViewFrustum Configuration::getViewFrustum() const {
	return viewFrustum;
}

void Configuration::setMainShadingEnabled (const bool enable) {
	mainShadingEnabled = enable;
	emit mainShadingChanged (enable);
}

bool Configuration::isMainShadingEnabled() const {
	return mainShadingEnabled;
}

void Configuration::setAuxiliaryShadingEnabled (const bool enable) {
	auxiliaryShadingEnabled = enable;
	emit auxiliaryShadingChanged (enable);
}

bool Configuration::isAuxiliaryShadingEnabled() const {
	return auxiliaryShadingEnabled;
}

void Configuration::setPreviewShadingEnabled (const bool enable) {
	previewShadingEnabled = enable;
	emit previewShadingChanged (enable);
}

bool Configuration::isPreviewShadingEnabled() const {
	return previewShadingEnabled;
}

void Configuration::setPluginDirectory (const QString newPluginDirectory) {
	pluginDirectory = newPluginDirectory;
	emit pluginDirectoryChanged (newPluginDirectory);
}

QString Configuration::getPluginDirectory() const {
	return pluginDirectory;
}

void Configuration::setRendererPluginDirectory (const QString newRendererPluginDirectory) {
	rendererPluginDirectory = newRendererPluginDirectory;
	emit rendererPluginDirectoryChanged (newRendererPluginDirectory);
}

QString Configuration::getRendererPluginDirectory() const {
	return rendererPluginDirectory;
}

void Configuration::setToolDirectory (const QString newToolDirectory) {
	toolDirectory = newToolDirectory;
	emit toolDirectoryChanged (newToolDirectory);
}

QString Configuration::getToolDirectory() const {
	return toolDirectory;
}

void Configuration::setBrushDirectory (const QString newBrushDirectory) {
	brushDirectory = newBrushDirectory;
	emit brushDirectoryChanged (newBrushDirectory);
}

QString Configuration::getBrushDirectory() const {
	return brushDirectory;
}

void Configuration::setFileFormatPluginDirectory (const QString newFileFormatPluginDirectory) {
	fileFormatPluginDirectory = newFileFormatPluginDirectory;
	emit fileFormatPluginDirectoryChanged (newFileFormatPluginDirectory);
}

QString Configuration::getFileFormatPluginDirectory() const {
	return fileFormatPluginDirectory;
}

void Configuration::setBitmapDirectory (const QString newBitmapDirectory) {
	bitmapDirectory = newBitmapDirectory;
	emit bitmapDirectoryChanged (newBitmapDirectory);
}

QString Configuration::getBitmapDirectory() const {
	return bitmapDirectory;
}

void Configuration::setDataDirectory (const QString newDataDirectory) {
	dataDirectory = newDataDirectory;
	emit dataDirectoryChanged (newDataDirectory);
}

QString Configuration::getDataDirectory() const {
	return dataDirectory;
}

void Configuration::setHomePosition (const MyDataTypes::CameraPosition newHomePosition) {
	
	homePosition = newHomePosition;
	emit homePositionChanged (newHomePosition);

}

MyDataTypes::CameraPosition Configuration::getHomePosition() const {
	return homePosition;
}

void Configuration::setLightDirection (const Vector3D newLightDirection) {
	lightDirection = newLightDirection;
	lightDirection.normalize();
	emit lightDirectionChanged (newLightDirection);
}

Vector3D Configuration::getLightDirection() const {
	return lightDirection;
}


void Configuration::setBackgroundColor(const QRgb newBackgroundColor) {
	backgroundColor = newBackgroundColor;
	emit backgroundColorChanged(newBackgroundColor);
}


QRgb Configuration::getBackgroundColor() const {
	return backgroundColor;
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
