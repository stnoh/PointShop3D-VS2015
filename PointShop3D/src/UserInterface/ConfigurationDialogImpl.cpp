// Title:   ConfigurationDialogImpl.cpp
// Created: Thu Sep 25 14:43:14 2003
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

#include <qlineedit.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qstringlist.h>
#include <qcolordialog.h> 

#include "../DataTypes/src/Vector3D.h"
#include "../DataTypes/src/MyDataTypes.h"
#include "../Core/Configuration/src/Configuration.h"
#include "../Core/RendererManager/src/RendererManager.h"
#include "../Core/Interfaces/src/RendererConfigurationInterface.h"
#include "../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "ConfigurationDialogImpl.h"
#include "../Utilities/UserMessages/src/StatusBar.h"


// ******
// global
// ******

// the single ConfigurationDialogImpl instance, only
// accesible by getInstance() and
// destroyInstance()
static ConfigurationDialogImpl *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

ConfigurationDialogImpl::ConfigurationDialogImpl (QWidget *parent, const char *name, bool modal, WFlags flags)
    : ConfigurationDialogBase (parent, name, modal, flags) {

	// get informed whenever the renderer type changes - we apply these settings IMMEDIATELY so we
	// can update this configuration dialog box as well, since we have to display the renderer specific
	// settings
	this->connect (mainRendererTypesComboBox, SIGNAL (activated (const QString &)),
		           this, SLOT (handleMainRendererTypeChosen (const QString &)));
	this->connect (previewRendererTypesComboBox, SIGNAL (activated (const QString &)),
		           this, SLOT (handlePreviewRendererTypeChosen (const QString &)));

}

ConfigurationDialogImpl::~ConfigurationDialogImpl() {
    // no need to delete child widgets, Qt does it all for us
}

// **************
// public methods
// **************

ConfigurationDialogImpl *ConfigurationDialogImpl::getInstance() {

	if (instance == 0) {
		instance = new ConfigurationDialogImpl();
	}
	return instance;

}

void ConfigurationDialogImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}

// ************
// public slots
// ************
void ConfigurationDialogImpl::clickedLoadOriginalSettings() {

	bitmapsLineEdit->setText("Bitmaps");
	brushesLineEdit->setText("Brushes");
	toolsLineEdit->setText("Tools");
	pluginsLineEdit->setText("Plugins");
	dataLineEdit->setText("Models");
	rendererPluginLineEdit->setText("Renderers");
			
	mainShadingCheckBox->setChecked(true);
	previewShadingCheckBox->setChecked(true);
	aspectRatioLineEdit->setText("1");
	fieldOfViewLineEdit->setText("30");
	nearPlaneLineEdit->setText("10");
	farPlaneLineEdit->setText("100000");

	lightDirectionXLineEdit->setText("0");
	lightDirectionYLineEdit->setText("0");
	lightDirectionZLineEdit->setText("-1");
	updateLightDirectionXSlider();
	updateLightDirectionYSlider();
	updateLightDirectionZSlider();

	guiBitmapWidthSpinBox->setValue(32);
	guiBitmapHeightSpinBox->setValue(32);
	guiShowBitmapsCheckBox->setChecked(true);
	guiShowNamesCheckBox->setChecked(true);
	guiUseAlphaMasksCheckBox->setChecked(true);
	rendererBackgroundColor = QColor(0x008598b5); 
	backgroundColorToolButton->setPalette(QPalette(rendererBackgroundColor, this->backgroundColor()));

	autoSaveCheckBox->setChecked(false);
}




void ConfigurationDialogImpl::clickedLoadDefaultSettings() {

	FILE *file;
	Configuration::FileSettings fileSettings;
	QStringList availableRendererTypes;
	Configuration *configuration;

	file = fopen(Configuration::defaultFileName, "r" ); 

	if (file) {
		configuration = Configuration::getInstance();

		if (Configuration::readSettingsFromFile(file, fileSettings)) {
		
			bitmapsLineEdit->setText(fileSettings.bitmapsDir);
			brushesLineEdit->setText(fileSettings.brushesDir);
			toolsLineEdit->setText(fileSettings.toolsDir);
			pluginsLineEdit->setText(fileSettings.pluginsDir);
			dataLineEdit->setText(fileSettings.dataDir);
			rendererPluginLineEdit->setText(fileSettings.rendererPluginsDir);
			
			availableRendererTypes = configuration->getAvailableRendererTypes();
			mainRendererTypesComboBox->setCurrentItem (this->getRendererTypeIndex (fileSettings.mainRendererName, availableRendererTypes));
			previewRendererTypesComboBox->setCurrentItem (this->getRendererTypeIndex (fileSettings.previewRendererName, availableRendererTypes));
			mainShadingCheckBox->setChecked(fileSettings.bMainShading);
			previewShadingCheckBox->setChecked(fileSettings.bPreviewShading);
			aspectRatioLineEdit->setText(fileSettings.aspectRatio);
			fieldOfViewLineEdit->setText(fileSettings.fieldOfView);
			nearPlaneLineEdit->setText(fileSettings.nearPlane);
			farPlaneLineEdit->setText(fileSettings.farPlane);

			lightDirectionXLineEdit->setText(fileSettings.lightX);
			lightDirectionYLineEdit->setText(fileSettings.lightY);
			lightDirectionZLineEdit->setText(fileSettings.lightZ);
			updateLightDirectionXSlider();
			updateLightDirectionYSlider();
			updateLightDirectionZSlider();

			guiBitmapWidthSpinBox->setValue(fileSettings.bitmapWidth);
			guiBitmapHeightSpinBox->setValue(fileSettings.bitmapHeight);
			guiShowBitmapsCheckBox->setChecked(fileSettings.bShowBitmaps);
			guiShowNamesCheckBox->setChecked(fileSettings.bShowNames);
			guiUseAlphaMasksCheckBox->setChecked(fileSettings.bUseAlphaMasks);
			rendererBackgroundColor = QColor(fileSettings.backgroundColor); 
			backgroundColorToolButton->setPalette(QPalette(rendererBackgroundColor, this->backgroundColor()));

			autoSaveCheckBox->setChecked(fileSettings.bAutoSave);

		}
		else {

			StatusBar::getInstance()->showMessage("Could not load settings from file");
		}

		fclose(file);
	}
}


void ConfigurationDialogImpl::clickedSaveDefaultSettings() {



	FILE *file;
	Configuration::FileSettings fileSettings;

	file = fopen(Configuration::defaultFileName, "w" ); 
	
	if (file) {

		copyString(fileSettings.bitmapsDir, bitmapsLineEdit->text());
		copyString(fileSettings.brushesDir, brushesLineEdit->text());
		copyString(fileSettings.toolsDir, toolsLineEdit->text());
		copyString(fileSettings.pluginsDir, pluginsLineEdit->text());
		copyString(fileSettings.dataDir, dataLineEdit->text());
		copyString(fileSettings.rendererPluginsDir, rendererPluginLineEdit->text());
			
		// Renderer General
		copyString(fileSettings.mainRendererName, mainRendererTypesComboBox->currentText());
		copyString(fileSettings.previewRendererName, previewRendererTypesComboBox->currentText());
		fileSettings.bMainShading = mainShadingCheckBox->isChecked();
		fileSettings.bPreviewShading = previewShadingCheckBox->isChecked();
		copyString(fileSettings.aspectRatio, aspectRatioLineEdit->text());
		copyString(fileSettings.fieldOfView, fieldOfViewLineEdit->text());
		copyString(fileSettings.nearPlane, nearPlaneLineEdit->text());
		copyString(fileSettings.farPlane, farPlaneLineEdit->text());

		// Lighting
		copyString(fileSettings.lightX, lightDirectionXLineEdit->text());
		copyString(fileSettings.lightY, lightDirectionYLineEdit->text());
		copyString(fileSettings.lightZ, lightDirectionZLineEdit->text());

		// GUI
		fileSettings.bitmapWidth = guiBitmapWidthSpinBox->value();
		fileSettings.bitmapHeight = guiBitmapHeightSpinBox->value();
		fileSettings.bShowBitmaps = guiShowBitmapsCheckBox->isChecked();
		fileSettings.bShowNames = guiShowNamesCheckBox->isChecked();
		fileSettings.bUseAlphaMasks = guiUseAlphaMasksCheckBox->isChecked();
		fileSettings.backgroundColor = rendererBackgroundColor.rgb();

		// Settings
		fileSettings.bAutoSave = autoSaveCheckBox->isChecked();

		Configuration::writeSettingsToFile(file, fileSettings);
		fclose(file);		
	}

}


void ConfigurationDialogImpl::clickedOkay() {

	this->applyDialogSettings();

	if (autoSaveCheckBox->isChecked()) {
		clickedSaveDefaultSettings();
	}
	this->accept();

}

void ConfigurationDialogImpl::clickedApply() {
	this->applyDialogSettings();
}

void ConfigurationDialogImpl::clickedCancel() {

	Configuration *configuration;

	configuration = Configuration::getInstance();
	if (configuration->getLightDirection() != originalLightDirection) {
		configuration->setLightDirection (originalLightDirection);
	}
	if (originalRendererBackgroundColor != configuration->getBackgroundColor()) {
		configuration->setBackgroundColor(originalRendererBackgroundColor.rgb());
	}

	// for the next time this dialog is being shown, we need to re-initialize
	// the light source settings
	this->initLightSourceValues();
	this->reject();

}

void ConfigurationDialogImpl::clickedHelp() {
	// TODO: implement this (HTML help)
}

void ConfigurationDialogImpl::clickedBackgroundColorToolButton() {
	QPalette pal;
	QColor newColor;
	
	newColor = QColorDialog::getColor(rendererBackgroundColor, this, "Background Color");
	if (newColor.isValid()) {
		rendererBackgroundColor = newColor;
		pal = QPalette(rendererBackgroundColor, this->backgroundColor());
		this->backgroundColorToolButton->setPalette(pal);	
	}	
}

void ConfigurationDialogImpl::clickedBitmapsBrowseButton() {

	QString directoryName;
	
	directoryName = QFileDialog::getExistingDirectory (bitmapsLineEdit->text(),
		                                               this,
												       "BitmapDirBrowser",
												       "Select bitmap directory",
												       true);
    if (directoryName.isEmpty() == false) {
		bitmapsLineEdit->setText (directoryName);
	}
	
}

void ConfigurationDialogImpl::clickedBrushesBrowseButton() {

	QString directoryName;
	
	directoryName = QFileDialog::getExistingDirectory (brushesLineEdit->text(),
		                                               this,
												       "BrushDirBrowser",
												       "Select brush directory",
												       true);
    if (directoryName.isEmpty() == false) {
		brushesLineEdit->setText (directoryName);
	}

}

void ConfigurationDialogImpl::clickedToolsBrowseButton() {

	QString directoryName;
	
	directoryName = QFileDialog::getExistingDirectory (toolsLineEdit->text(),
		                                               this,
												       "ToolDirBrowser",
												       "Select tool directory",
												       true);
    if (directoryName.isEmpty() == false) {
		toolsLineEdit->setText (directoryName);
	}

}

void ConfigurationDialogImpl::clickedPluginsBrowseButton() {

	QString directoryName;
	
	directoryName = QFileDialog::getExistingDirectory (pluginsLineEdit->text(),
		                                               this,
												       "PluginDirBrowser",
												       "Select plugin directory",
												       true);
    if (directoryName.isEmpty() == false) {
		pluginsLineEdit->setText (directoryName);
	}

}

void ConfigurationDialogImpl::clickedDataBrowseButton() {

	QString directoryName;
	
	directoryName = QFileDialog::getExistingDirectory (dataLineEdit->text(),
		                                               this,
												       "DataDirBrowser",
												       "Select data directory",
												       true);
    if (directoryName.isEmpty() == false) {
		dataLineEdit->setText (directoryName);
	}

}

void ConfigurationDialogImpl::clickedRenderersBrowseButton() {

	QString directoryName;
	
	directoryName = QFileDialog::getExistingDirectory (rendererPluginLineEdit->text(),
		                                               this,
												       "RenderersDirBrowser",
												       "Select renderer directory",
												       true);
    if (directoryName.isEmpty() == false) {
		rendererPluginLineEdit->setText (directoryName);
	}

}

// light position X values

void ConfigurationDialogImpl::handleLightDirectionXChanged() {
	this->updateLightDirectionXSlider();
}

void ConfigurationDialogImpl::handleLightDirectionXSliderChanged (int value) {

	Vector3D      lightDirection;
	Configuration *configuration;
	float         x;

	configuration = Configuration::getInstance();

	lightDirection    = configuration->getLightDirection();
	x                 = (float)value / (float)lightDirectionXSlider->maxValue();
	lightDirection[0] = x;

	// update the x position line edit
	lightDirectionXLineEdit->setText (QString::number (x));
	this->disconnect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		              this, SLOT (handleLightDirectionChanged (const Vector3D)));
	configuration->setLightDirection (lightDirection);
	this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		           this, SLOT (handleLightDirectionChanged (const Vector3D)));

}

// light position Y values

void ConfigurationDialogImpl::handleLightDirectionYChanged() {
	this->updateLightDirectionYSlider();
}

void ConfigurationDialogImpl::handleLightDirectionYSliderChanged (int value) {

	Vector3D      lightDirection;
	Configuration *configuration;
	float         y;

	configuration = Configuration::getInstance();

	lightDirection    = configuration->getLightDirection();
	y                 = (float)value / (float)lightDirectionYSlider->maxValue();
	lightDirection[1] = y;

	// update the y position line edit
	lightDirectionYLineEdit->setText (QString::number (y));
	this->disconnect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		              this, SLOT (handleLightDirectionChanged (const Vector3D)));
	configuration->setLightDirection (lightDirection);
	this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		           this, SLOT (handleLightDirectionChanged (const Vector3D)));

}

// light position Z values

void ConfigurationDialogImpl::handleLightDirectionZChanged() {
	this->updateLightDirectionZSlider();
}

void ConfigurationDialogImpl::handleLightDirectionZSliderChanged (int value) {

	Vector3D      lightDirection;
	Configuration *configuration;
	float         z;

	configuration = Configuration::getInstance();

	lightDirection    = configuration->getLightDirection();
	z                 = (float)value / (float)lightDirectionZSlider->maxValue();
	lightDirection[2] = z;

	// update the z position line edit
	lightDirectionZLineEdit->setText (QString::number (z));
	this->disconnect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		              this, SLOT (handleLightDirectionChanged (const Vector3D)));
	configuration->setLightDirection (lightDirection);
	this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		           this, SLOT (handleLightDirectionChanged (const Vector3D)));

}

// ***************
// protected slots
// ***************

void ConfigurationDialogImpl::showEvent (QShowEvent *) {

	Configuration *configuration;

	configuration = Configuration::getInstance();
	// TODO: connect here every property change in the Configuration which might be changed
	//       from "outside" this dialog box, so this dialog box gets updated properly!

	// get informed whenever new renderer types are available
	this->connect (configuration, SIGNAL (availableRendererTypesChanged (const QStringList)),
		           this, SLOT (handleAvailableRendererTypesChanged (const QStringList)));

	// get informed whenever the view frustum changes
	this->connect (configuration, SIGNAL (viewFrustumChanged (const MyDataTypes::ViewFrustum)),
		           this, SLOT (handleViewFrustumChanged (const MyDataTypes::ViewFrustum)));

	this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		           this, SLOT (handleLightDirectionChanged (const Vector3D)));

	this->initDialog();

}

void ConfigurationDialogImpl::hideEvent (QHideEvent *) {

	// we are no longer interested in these events
	Configuration::getInstance()->disconnect (this);

}

// ***************
// private methods
// ***************
void ConfigurationDialogImpl::initDialog() {

	Configuration                  *configuration;
	MyDataTypes::ViewFrustum       viewFrustum;
	RendererConfigurationInterface *rendererConfiguration;
	QStringList                    availableRendererTypes;

	configuration = Configuration::getInstance();
	
	// store the original light position, so it can be restored in case 'Cancel' was clicked
	// (this value can be changed in the Configuration without clicking on 'Okay' or 'Apply')
	originalLightDirection = configuration->getLightDirection();	

	// *************
	// directory tab
	// *************

	bitmapsLineEdit->setText (configuration->getBitmapDirectory());
	brushesLineEdit->setText (configuration->getBrushDirectory());
	toolsLineEdit->setText (configuration->getToolDirectory());
	pluginsLineEdit->setText (configuration->getPluginDirectory());
	dataLineEdit->setText (configuration->getDataDirectory());
	rendererPluginLineEdit->setText (configuration->getRendererPluginDirectory());

	// ********************
	// renderer general tab
	// ********************

	availableRendererTypes = configuration->getAvailableRendererTypes();
	if (availableRendererTypes.count() > 0) {
		
		mainRendererTypesComboBox->clear();
		mainRendererTypesComboBox->insertStringList (availableRendererTypes);
		mainRendererTypesComboBox->setCurrentItem (this->getRendererTypeIndex (configuration->getMainRendererType(), availableRendererTypes));
		
		previewRendererTypesComboBox->clear();
		previewRendererTypesComboBox->insertStringList (availableRendererTypes);
		previewRendererTypesComboBox->setCurrentItem (this->getRendererTypeIndex (configuration->getPreviewRendererType(), availableRendererTypes));
	
	}
	else {
		mainRendererTypesComboBox->setEnabled (false);
		previewRendererTypesComboBox->setEnabled (false);
	}	

	mainShadingCheckBox->setChecked (configuration->isMainShadingEnabled());
	previewShadingCheckBox->setChecked (configuration->isPreviewShadingEnabled());

	viewFrustum = configuration->getViewFrustum();
	aspectRatioLineEdit->setText (QString::number (viewFrustum.aspectRatio));
	fieldOfViewLineEdit->setText (QString::number (viewFrustum.fieldOfView));
	nearPlaneLineEdit->setText (QString::number (viewFrustum.nearPlane));
	farPlaneLineEdit->setText (QString::number (viewFrustum.farPlane));

	// ***************************
	// renderer configuration tabs
	// ***************************

	rendererConfiguration = RendererManager::getInstance()->getMainRenderer()->getRendererConfiguration();
	rendererConfiguration->createConfigurationWidget (mainRendererConfigurationTab);

	rendererConfiguration = RendererManager::getInstance()->getPreviewRenderer()->getRendererConfiguration();
	rendererConfiguration->createConfigurationWidget (previewRendererConfigurationTab);

	// ************
	// lighting tab
	// ************

	this->initLightSourceValues();

	// ************
	// gui tab
	// ************

	unsigned int bitmapWidth,
		         bitmapHeight;
	bool         showBitmaps,
		         showNames,
				 useAlphaMasks;

	CoreResourceTools::getInstance()->getBrushChooserTool()->getDisplayParameters (bitmapWidth, bitmapHeight,
		showBitmaps, showNames, useAlphaMasks);

	this->guiBitmapWidthSpinBox->setValue (bitmapWidth);
	this->guiBitmapHeightSpinBox->setValue (bitmapHeight);
	this->guiShowBitmapsCheckBox->setChecked (showBitmaps);
	this->guiShowNamesCheckBox->setChecked (showNames);
	this->guiUseAlphaMasksCheckBox->setChecked (useAlphaMasks);
	rendererBackgroundColor = configuration->getBackgroundColor();
	originalRendererBackgroundColor = rendererBackgroundColor;
	backgroundColorToolButton->setPalette(QPalette(rendererBackgroundColor, this->backgroundColor()));

}

void ConfigurationDialogImpl::initLightSourceValues() {

	Vector3D      lightDirection;
	
	lightDirection = Configuration::getInstance()->getLightDirection();
	lightDirectionXLineEdit->setText (QString::number (lightDirection[0]));
	lightDirectionYLineEdit->setText (QString::number (lightDirection[1]));
	lightDirectionZLineEdit->setText (QString::number (lightDirection[2]));

	lightDirectionXSlider->setValue ((int)(lightDirection[0] * lightDirectionXSlider->maxValue()));	
	lightDirectionYSlider->setValue ((int)(lightDirection[1] * lightDirectionYSlider->maxValue()));
	lightDirectionZSlider->setValue ((int)(lightDirection[2] * lightDirectionZSlider->maxValue()));

}

void ConfigurationDialogImpl::applyDialogSettings() {

	Configuration                  *configuration;
	bool                           isNumber;
	Vector3D                       newLightDirection,
	                               oldLightDirection;
	MyDataTypes::ViewFrustum       oldViewFrustum,
                                   newViewFrustum;
	RendererConfigurationInterface *rendererConfiguration;

	configuration = Configuration::getInstance();

	// *************
	// directory tab
	// *************

	// make only changes to Configuration if settings are different
	if (bitmapsLineEdit->text() != configuration->getBitmapDirectory()) {
		configuration->setBitmapDirectory (bitmapsLineEdit->text());
	}
	if (brushesLineEdit->text() != configuration->getBrushDirectory()) {
		configuration->setBrushDirectory (brushesLineEdit->text());
	}
	if (toolsLineEdit->text() != configuration->getToolDirectory()) {
		configuration->setToolDirectory (toolsLineEdit->text());
	}
	if (pluginsLineEdit->text() != configuration->getPluginDirectory()) {
		configuration->setPluginDirectory (pluginsLineEdit->text());
	}
	if (dataLineEdit->text() != configuration->getDataDirectory()) {
		configuration->setDataDirectory (dataLineEdit->text());
	}
	if (rendererPluginLineEdit->text() != configuration->getRendererPluginDirectory()) {
		configuration->setRendererPluginDirectory (rendererPluginLineEdit->text());
	};

	// ************
	// renderer tab
	// ************

	// we already have applied the renderer type settings in the slot handleRendererTypeChosen, no need to
	// do it here again

	oldViewFrustum = configuration->getViewFrustum();
	newViewFrustum.aspectRatio = aspectRatioLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		newViewFrustum.aspectRatio = oldViewFrustum.aspectRatio;
		aspectRatioLineEdit->setText (QString::number (oldViewFrustum.aspectRatio));
	}

	newViewFrustum.fieldOfView = fieldOfViewLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		newViewFrustum.fieldOfView = oldViewFrustum.fieldOfView;
		fieldOfViewLineEdit->setText (QString::number (oldViewFrustum.fieldOfView));
	}

	newViewFrustum.nearPlane = nearPlaneLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		newViewFrustum.nearPlane = oldViewFrustum.nearPlane;
		nearPlaneLineEdit->setText (QString::number (oldViewFrustum.nearPlane));
	}

	newViewFrustum.farPlane = farPlaneLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		newViewFrustum.farPlane = oldViewFrustum.farPlane;
		farPlaneLineEdit->setText (QString::number (oldViewFrustum.farPlane));
	}

	if (mainShadingCheckBox->isChecked() != configuration->isMainShadingEnabled()) {
		configuration->setMainShadingEnabled (mainShadingCheckBox->isChecked());
	}

	if (previewShadingCheckBox->isChecked() != configuration->isPreviewShadingEnabled()) {
		configuration->setPreviewShadingEnabled (previewShadingCheckBox->isChecked());
	}

	if ((newViewFrustum.aspectRatio != oldViewFrustum.aspectRatio) ||
		(newViewFrustum.fieldOfView != oldViewFrustum.fieldOfView) ||
		(newViewFrustum.nearPlane   != oldViewFrustum.nearPlane)   ||
		(newViewFrustum.farPlane    != oldViewFrustum.farPlane)) {
		configuration->setViewFrustum (newViewFrustum);
	}

	// **************************
	// renderer configuration tab
	// **************************

	// apply the renderer specific settings
	rendererConfiguration = RendererManager::getInstance()->getMainRenderer()->getRendererConfiguration();
	rendererConfiguration->apply();

	rendererConfiguration = RendererManager::getInstance()->getPreviewRenderer()->getRendererConfiguration();
	rendererConfiguration->apply();

	// ************
	// lighting tab
	// ************

	oldLightDirection = configuration->getLightDirection();

	newLightDirection[0] = lightDirectionXLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		newLightDirection[0] = oldLightDirection[0];		
		lightDirectionXLineEdit->setText (QString::number (oldLightDirection[0]));
	}

	newLightDirection[1] = lightDirectionYLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		newLightDirection[1] = oldLightDirection[1];		
		lightDirectionYLineEdit->setText (QString::number (oldLightDirection[1]));
	}

	newLightDirection[2] = lightDirectionZLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		newLightDirection[2] = oldLightDirection[2];		
		lightDirectionZLineEdit->setText (QString::number (oldLightDirection[2]));
	}

	if (newLightDirection != oldLightDirection) {

		configuration->setLightDirection (newLightDirection);

		// for the next time 'Cancel' is clicked
		originalLightDirection = newLightDirection;

		// update the sliders (temporarily disconnect this class from the slider signals)
		lightDirectionXSlider->disconnect (this);
		this->updateLightDirectionXSlider();
		this->connect (lightDirectionXSlider, SIGNAL (valueChanged (int)),
					   this, SLOT (handleLightDirectionXSliderChanged (int)));

		lightDirectionYSlider->disconnect (this);
		this->updateLightDirectionYSlider();
		this->connect (lightDirectionYSlider, SIGNAL (valueChanged (int)),
					   this, SLOT (handleLightDirectionYSliderChanged (int)));

		lightDirectionZSlider->disconnect (this);
		this->updateLightDirectionZSlider();
		this->connect (lightDirectionZSlider, SIGNAL (valueChanged (int)),
					   this, SLOT (handleLightDirectionZSliderChanged (int)));

	}

	// **************************
	// GUI tab
	// **************************

	CoreResourceTools::getInstance()->getBrushChooserTool()->setDisplayParameters (this->guiBitmapWidthSpinBox->value(),
		this->guiBitmapHeightSpinBox->value(), this->guiShowBitmapsCheckBox->isChecked(),
		this->guiShowNamesCheckBox->isChecked(), this->guiUseAlphaMasksCheckBox->isChecked());
	
	if (rendererBackgroundColor != configuration->getBackgroundColor()) {
		originalRendererBackgroundColor = rendererBackgroundColor;
		configuration->setBackgroundColor(rendererBackgroundColor.rgb());
	}

	if (!this->guiShowBitmapsCheckBox->isChecked() && !this->guiShowNamesCheckBox->isChecked()) {
		this->guiShowNamesCheckBox->setChecked (true);
	}

}	

void ConfigurationDialogImpl::updateLightDirectionXSlider() {

	bool         isNumber;	
    float         xDirection;
	Vector3D      lightDirection;
	Configuration *configuration;

	configuration = Configuration::getInstance();
	xDirection = lightDirectionXLineEdit->text().toFloat(&isNumber);
	if (isNumber == true) {	

		lightDirection    = configuration->getLightDirection();
		
		// check min/max value
		if (xDirection < -1.0f) {
			xDirection = -1.0f;
			lightDirectionXLineEdit->setText (QString::number (xDirection));
		}
		else if (xDirection > 1.0f) {
			xDirection = 1.0f;
			lightDirectionXLineEdit->setText (QString::number (xDirection));
		}

		lightDirection[0] = xDirection;
		lightDirectionXSlider->setValue ((int)(xDirection * lightDirectionXSlider->maxValue()));

		this->disconnect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
						  this, SLOT (handleLightDirectionChanged (const Vector3D)));
		configuration->setLightDirection (lightDirection);
		this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		               this, SLOT (handleLightDirectionChanged (const Vector3D)));

	}
	else {
		// not a number, restore to old value
		lightDirectionXLineEdit->setText (QString::number (Configuration::getInstance()->getLightDirection()[0]));
	}

}

void ConfigurationDialogImpl::updateLightDirectionYSlider() {

	bool          isNumber;	
    float         yDirection;
	Vector3D      lightDirection;
	Configuration *configuration;

	configuration = Configuration::getInstance();
	yDirection = lightDirectionYLineEdit->text().toFloat(&isNumber);
	if (isNumber == true) {

		lightDirection    = configuration->getLightDirection();

		// check min/max value
		if (yDirection < -1.0f) {
			yDirection = -1.0f;
			lightDirectionYLineEdit->setText (QString::number (yDirection));
		}
		else if (yDirection > 1.0f) {
			yDirection = 1.0f;
			lightDirectionYLineEdit->setText (QString::number (yDirection));
		}

		lightDirection[1] = yDirection;
		lightDirectionYSlider->setValue ((int)(yDirection * lightDirectionYSlider->maxValue()));
	
		this->disconnect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
						  this, SLOT (handleLightDirectionChanged (const Vector3D)));
		configuration->setLightDirection (lightDirection);
		this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		               this, SLOT (handleLightDirectionChanged (const Vector3D)));		

	}
	else {
		// not a number, restore to old value
		lightDirectionYLineEdit->setText (QString::number (Configuration::getInstance()->getLightDirection()[1]));
	}

}

void ConfigurationDialogImpl::updateLightDirectionZSlider() {

	bool          isNumber;	
    float         zDirection;
	Vector3D      lightDirection;
	Configuration *configuration;

	configuration = Configuration::getInstance();
	zDirection = lightDirectionZLineEdit->text().toFloat(&isNumber);
	if (isNumber == true) {

		lightDirection    = configuration->getLightDirection();

		// check min/max value
		if (zDirection < -1.0f) {
			zDirection = -1.0f;
			lightDirectionZLineEdit->setText (QString::number (zDirection));
		}
		else if (zDirection > 1.0f) {
			zDirection = 1.0f;
			lightDirectionZLineEdit->setText (QString::number (zDirection));
		}

		lightDirection[2] = zDirection;
		lightDirectionZSlider->setValue ((int)(zDirection * lightDirectionZSlider->maxValue()));
	
		this->disconnect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
						  this, SLOT (handleLightDirectionChanged (const Vector3D)));
		configuration->setLightDirection (lightDirection);
		this->connect (configuration, SIGNAL (lightDirectionChanged (const Vector3D)),
		               this, SLOT (handleLightDirectionChanged (const Vector3D)));

	}
	else {
		// not a number, restore to old value
		lightDirectionZLineEdit->setText (QString::number (Configuration::getInstance()->getLightDirection()[2]));
	}

}

int ConfigurationDialogImpl::getRendererTypeIndex (const QString rendererType, const QStringList availableRendererTypes) {

	int  i,
		 n;
	bool found;

	found = false;
	i     = 0;
	n     = availableRendererTypes.count();
	while (i < n && found == false) {

		if (rendererType == availableRendererTypes[i]) {
			found = true;
		}
		else {
			i++;
		}
	
	}

	if (i < n) {
		return i;
	}
	else {
		return -1;
	}

}


// *************
// private slots
// *************

void ConfigurationDialogImpl::handleAvailableRendererTypesChanged (const QStringList newAvailableRendererTypes) {

	mainRendererTypesComboBox->clear();
	previewRendererTypesComboBox->clear();

	mainRendererTypesComboBox->insertStringList (newAvailableRendererTypes);
	previewRendererTypesComboBox->insertStringList (newAvailableRendererTypes);

	if (newAvailableRendererTypes.isEmpty() == false) {
		mainRendererTypesComboBox->setEnabled (true);
		previewRendererTypesComboBox->setEnabled (true);
	}
	else {
		mainRendererTypesComboBox->setEnabled (false);
		previewRendererTypesComboBox->setEnabled (false);
	}

}

void ConfigurationDialogImpl::handleViewFrustumChanged (const MyDataTypes::ViewFrustum newViewFrustum) {

	aspectRatioLineEdit->setText (QString::number (newViewFrustum.aspectRatio));
	fieldOfViewLineEdit->setText (QString::number (newViewFrustum.fieldOfView));
	nearPlaneLineEdit->setText (QString::number (newViewFrustum.nearPlane));
	farPlaneLineEdit->setText (QString::number (newViewFrustum.farPlane));

}

void ConfigurationDialogImpl::handleMainRendererTypeChosen (const QString &rendererType) {

	Configuration                  *configuration;
	RendererConfigurationInterface *rendererConfiguration;

	configuration = Configuration::getInstance();

	// set the renderer type...
	if (configuration->getMainRendererType() != rendererType) {
		configuration->setMainRendererType (rendererType);
	}

	// ... and update this configuration dialog box (the old configuration QWidget has been removed
	// by the old renderer already)
	rendererConfiguration = RendererManager::getInstance()->getMainRenderer()->getRendererConfiguration();
	rendererConfiguration->createConfigurationWidget (mainRendererConfigurationTab);

}

void ConfigurationDialogImpl::handleAuxiliaryRendererTypeChosen (const QString &rendererType) {

	Configuration *configuration;

	configuration = Configuration::getInstance();

	// set the renderer type
	if (configuration->getAuxiliaryRendererType() != rendererType) {
		configuration->setAuxiliaryRendererType (rendererType);
	}

}


void ConfigurationDialogImpl::handlePreviewRendererTypeChosen (const QString &rendererType) {

	Configuration                  *configuration;
	RendererConfigurationInterface *rendererConfiguration;

	configuration = Configuration::getInstance();

	// set the renderer type...
	if (configuration->getPreviewRendererType() != rendererType) {
		configuration->setPreviewRendererType (rendererType);
	}

	// ... and update this configuration dialog box (the old configuration QWidget has been removed
	// by the old renderer already)
	rendererConfiguration = RendererManager::getInstance()->getPreviewRenderer()->getRendererConfiguration();
	rendererConfiguration->createConfigurationWidget (previewRendererConfigurationTab);

}

void ConfigurationDialogImpl::handleLightDirectionChanged (const Vector3D newLightDirection) {
	this->initLightSourceValues();
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
