// Title:   AnimationDialogImpl.cpp
// Created: Thu Sep 25 14:43:24 2003
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

#include <qthread.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qfiledialog.h>
#include <qimage.h>
#include <qsize.h>
#include <qlabel.h>

#include "../Core/Scene/src/Scene.h"
#include "../DataTypes/src/Vector3D.h"
#include "../Utilities/src/FileTools.h"
#include "../Utilities/src/StringTools.h"
#include "../Utilities/src/Common.h"
#include "../Core/Configuration/src/Configuration.h"
#include "../Core/RendererManager/src/RendererManager.h"
#include "AnimationDialogImpl.h"

// frames per second drop down list id
enum {
	FPS_1,
	FPS_2,
	FPS_5,
	FPS_10,
	FPS_12,
	FPS_15,
	FPS_20,
	FPS_24,
	FPS_25,
	FPS_30
};

static const uint SEQUENCE_NUMBER_LENGTH = 5;

// ******
// global
// ******

// the single AnimationDialogImpl instance, only
// accesible by getInstance() and
// destroyInstance()
static AnimationDialogImpl *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

AnimationDialogImpl::AnimationDialogImpl (QWidget *parent, const char *name, bool modal, WFlags flags)
    : AnimationDialogBase (parent, name, modal, flags) {

	this->initDialog();

}

AnimationDialogImpl::~AnimationDialogImpl() {
    // no need to delete child widgets, Qt does it all for us
	
	// wait until the (possibly still running) worker thread has finished
	continueAnimation = false;
	this->wait();

}

// **************
// public methods
// **************

AnimationDialogImpl *AnimationDialogImpl::getInstance() {

	if (instance == 0) {
		instance = new AnimationDialogImpl();
	}
	return instance;

}

void AnimationDialogImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}

// ************
// public slots
// ************

void AnimationDialogImpl::clickedAnimate() {

	continueAnimation = true;
	this->setWorkMode (true);
	this->start();

}

void AnimationDialogImpl::clickedStop() {
	continueAnimation = false;
	this->wait();
	this->setWorkMode (false);
}

void AnimationDialogImpl::clickedReset() {

	// TODO: implement this
}

void AnimationDialogImpl::clickedBrowse() {


	QString directoryName;
	
	directoryName = QFileDialog::getExistingDirectory (directoryLineEdit->text(),
		                                               this,
												       "AnimationDirBrowser",
												       "Select animation directory",
												       true);
    if (directoryName.isEmpty() == false) {
		directoryLineEdit->setText (directoryName);
		directory = directoryName;
		this->updateFormatTextLabel();
	}
	
}

void AnimationDialogImpl::handleDegreesChanged() {

	bool  isNumber;
	float newDegrees;
	
	newDegrees = degreesLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		degreesLineEdit->setText (QString::number (degrees));
	}
	else {
		degrees = newDegrees;
	}

	this->updateFormatTextLabel();

}
	
void AnimationDialogImpl::handleStartPositionChanged() {

	bool  isNumber;
	float newStartPosition;
	
	newStartPosition = startPositionLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		startPositionLineEdit->setText (QString::number (startPosition));
	}
	else {
		startPosition = newStartPosition;
	}

	this->updateFormatTextLabel();

}

void AnimationDialogImpl::handleEndPositionChanged() {

	bool  isNumber;
	float newEndPosition;
	
	newEndPosition = endPositionLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		endPositionLineEdit->setText (QString::number (endPosition));
	}
	else {
		endPosition = newEndPosition;
	}

	this->updateFormatTextLabel();

}



void AnimationDialogImpl::handleStartCutOffChanged() {

	bool  isNumber;
	float newStartCutOff;
	
	newStartCutOff = startCutOffLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		startCutOffLineEdit->setText (QString::number (startCutOff));
	}
	else {
		startCutOff = newStartCutOff;
	}

	this->updateFormatTextLabel();

}

void AnimationDialogImpl::handleEndCutOffChanged() {

	bool  isNumber;
	float newEndCutOff;
	
	newEndCutOff = endCutOffLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		endCutOffLineEdit->setText (QString::number (endCutOff));
	}
	else {
		endCutOff = newEndCutOff;
	}

	this->updateFormatTextLabel();

}






void AnimationDialogImpl::handleDurationChanged() {

	bool  isNumber;
	float newDuration;
	
	newDuration = durationLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		durationLineEdit->setText (QString::number (duration));
	}
	else {
		duration = newDuration;
	}

	this->updateFormatTextLabel();

}
	
void AnimationDialogImpl::handleFPSChanged (int newItem) {

	switch (newItem) {

	case FPS_1:
		{
			fps = 1.0f;
		}
		break;

	case FPS_2:
		{
			fps = 2.0f;
		}
		break;

	case FPS_5:
		{
			fps = 5.0f;
		}
		break;

	case FPS_10:
		{
			fps = 10.0f;
		}
		break;

	case FPS_12:
		{
			fps = 12.0f;
		}
		break;

	case FPS_15:
		{
			fps = 15.0f;
		}
		break;

	case FPS_20:
		{
			fps = 20.0f;
		}
		break;

	case FPS_24:
		{
			fps = 24.0f;
		}
		break;

	case FPS_25:
		{
			fps = 25.0f;
		}
		break;

	case FPS_30:
		{
			fps = 30.0f;
		}
		break;

	default:
		break;

	}

	this->updateFormatTextLabel();

}

void AnimationDialogImpl::handleWidthChanged() {

	bool isNumber;
	int  newWidth;
	
	newWidth = widthLineEdit->text().toInt (&isNumber);
	if (isNumber == false) {
		// reset value
		widthLineEdit->setText (QString::number (dimension.width()));
	}
	else {
		dimension.setWidth (newWidth);
		Configuration::getInstance()->setViewPortSize (dimension);
	}

	this->updateFormatTextLabel();

}

void AnimationDialogImpl::handleHeightChanged() {

	bool isNumber;
	int  newHeight;
	
	newHeight = heightLineEdit->text().toInt (&isNumber);
	if (isNumber == false) {
		// reset value
		heightLineEdit->setText (QString::number (dimension.height()));
	}
	else {
		dimension.setHeight (newHeight);
		Configuration::getInstance()->setViewPortSize (dimension);
	}

	this->updateFormatTextLabel();

}


	
void AnimationDialogImpl::handleBaseFileNameChanged() {
	baseFileName = baseFileNameLineEdit->text();
	this->updateFormatTextLabel();
}

void AnimationDialogImpl::handleDirectoryChanged() {
	directory = directoryLineEdit->text();
	this->updateFormatTextLabel();
}
	
void AnimationDialogImpl::handleStoreAnimationChanged (bool enable) {
	storeAnimation = enable;
	this->updateFormatTextLabel();
}

// *****************
// protected methods
// *****************

void AnimationDialogImpl::run() {
	
	int           currentFrame,
		          totalFrames;
	float         rotationStep,
		          zoomStep,
				  cutOffRadius,
				  cutOffStep;
	bool          finished;
	QString       filePath,
		          fileFormat;
	Configuration *configuration;
	QPixmap       previewPixmap;
	Scene         *scene;

	scene = Scene::getInstance();

	// at this point we're not interested in SceneMoved events, since we
	// are going to modify it ourselves
	scene->disconnect (this);

	configuration = Configuration::getInstance();

	currentFrame = 0;
	totalFrames  = (int)(duration * fps + 0.5f);
	if (continuousCheckBox->isChecked() == true) {
		rotationStep = degrees / (float)(totalFrames);
	}
	else {
		rotationStep = degrees / (float)(totalFrames - 1);
	}
	zoomStep = (endPosition - startPosition) / (duration * fps);

	cutOffRadius = startCutOff;
	cutOffStep = (endCutOff - startCutOff) / (duration * fps);

	animationProgressBar->setTotalSteps (totalFrames);
	animationProgressBar->setProgress (currentFrame);
	scene->setTranslation (0.0f, 0.0f, startPosition, false);
	fileFormat = fileFormatComboBox->currentText();
	
	finished = false;
	while (continueAnimation == true && finished == false) {

		// check if we are finished
		if (currentFrame < totalFrames) {

			if (currentFrame != 0) {
			
				// translate		
				//modelView->preTranslate (ModelView::VIEW2, 0.f, 0.0f, zoomStep, false);
				scene->translate (0.0f, 0.0f, zoomStep, false);

				RendererManager::getInstance()->getMainRenderer()->setCutoffRadius (cutOffRadius);
				cutOffRadius += cutOffStep;

				// rotate
				if (xAxisRadioButton->isChecked() == true) {
					scene->rotate (rotationStep * PI_180, 1.0f, 0.0f, 0.0f, updateDisplayCheckBox->isChecked());
				}
				else if (yAxisRadioButton->isChecked() == true) {
					scene->rotate (rotationStep * PI_180, 0.0f, 1.0f, 0.0f, updateDisplayCheckBox->isChecked());
				}
				else {
					scene->rotate (rotationStep * PI_180, 0.0f, 0.0f, 1.0f, updateDisplayCheckBox->isChecked());
				}

			}

			if (storeAnimation == true) {

				filePath = directory + baseFileName;
				filePath = StringTools::appendSequenceNumber (filePath, currentFrame, SEQUENCE_NUMBER_LENGTH);
				filePath.append ("." + fileFormat.lower());
				RendererManager::getInstance()->getMainRenderer()->getRenderedImage().save (filePath,
					                                                                        fileFormatComboBox->currentText(),
																						    -1);
			}

			currentFrame++;
			animationProgressBar->setProgress (currentFrame);

		}
		else {
			finished = true;
			animationProgressBar->setProgress (totalFrames);
		}		
		

	}

	this->setWorkMode (false);

	// get informed again whenever the model view matrix has changed
	this->connect (scene, SIGNAL (sceneMoved (const float*)),
	               this, SLOT (handleSceneMoved (const float*)));

}

// ***************
// protected slots
// ***************

void AnimationDialogImpl::showEvent (QShowEvent *) {

	Configuration *configuration;
	Scene         *scene;
	MyDataTypes::TransformationMatrix16f transformationMatrix;

	configuration = Configuration::getInstance();
	scene         = Scene::getInstance();
	
	this->connect (scene, SIGNAL (sceneMoved (const float*)),
	               this, SLOT (handleSceneMoved (const float*)));
	scene->getTransformationMatrix (transformationMatrix);

	
	startPosition  = transformationMatrix[14];
	endPosition    = transformationMatrix[14];
	startPositionLineEdit->setText (QString::number (startPosition));
	endPositionLineEdit->setText (QString::number (endPosition));

	startCutOff = endCutOff = RendererManager::getInstance()->getMainRenderer()->getCutoffRadius();
	startCutOffLineEdit->setText (QString::number (startCutOff));
	endCutOffLineEdit->setText (QString::number (endCutOff));


	// get informed whenever the render size has changed
	this->connect (configuration, SIGNAL (viewPortSizeChanged (const QSize)),
		           this, SLOT (handleViewPortSizeChanged (const QSize)));

	dimension = configuration->getViewPortSize();
	widthLineEdit->setText (QString::number (dimension.width()));
	heightLineEdit->setText (QString::number (dimension.height()));

	this->updateFormatTextLabel();

}

void AnimationDialogImpl::hideEvent (QHideEvent *) {

	// we are no longer interested in these events
	Configuration::getInstance()->disconnect (this);
	Scene::getInstance()->disconnect (this);

}

// ***************
// private methods
// ***************

void AnimationDialogImpl::initDialog() {	

	QStringList formatList;

	degrees        = 360.0f;
	yAxisRadioButton->setChecked (true);	

	duration       = 5.0f;
	fps            = 30.0f;
	baseFileName   = "anim";
	directory      = "";

	storeAnimation = false;

	degreesLineEdit->setText (QString::number (degrees));	
	durationLineEdit->setText (QString::number (duration));

	fpsComboBox->insertItem ("1", FPS_1);
	fpsComboBox->insertItem ("2", FPS_2);
	fpsComboBox->insertItem ("5", FPS_5);
	fpsComboBox->insertItem ("10", FPS_10);
	fpsComboBox->insertItem ("12", FPS_12);
	fpsComboBox->insertItem ("15", FPS_15);
	fpsComboBox->insertItem ("20", FPS_20);
	fpsComboBox->insertItem ("24", FPS_24);
	fpsComboBox->insertItem ("25", FPS_25);
	fpsComboBox->insertItem ("30", FPS_30);
	fpsComboBox->setCurrentItem (FPS_30);

	baseFileNameLineEdit->setText (baseFileName);
	directoryLineEdit->setText (directory);
	this->setWorkMode (false);	

	formatList = QImage::outputFormatList();
	fileFormatComboBox->insertStringList (formatList);

	updateDisplayCheckBox->setChecked (true);

	// start/end position and dimension are updated as soon as this dialog is shown
	// (in method showEvent)

}

void AnimationDialogImpl::updateFormatTextLabel() {

	QString truncatedDirectory;

	if (storeAnimation == true) {
		formatTextLabel->setText (QString ("Storing %1 seconds (%2 frames) - Dimension: (%3, %4) - Rotation: %5 - Zoom: %6 to %7")
			                               .arg (QString::number (duration))
			                               .arg (QString::number (duration * fps))
										   .arg (QString::number (dimension.width()))
										   .arg (QString::number (dimension.height()))
										   .arg (QString::number (degrees))
										   .arg (QString::number (startPosition))
										   .arg (QString::number (endPosition)));
	}
	else {
		formatTextLabel->setText (QString ("Animating %1 seconds (%2 frames) - Dimension: (%3, %4) - Rotation: %5 - Zoom: %6 to %7")
			                               .arg (QString::number (duration))
		                                   .arg (QString::number (duration * fps))
										   .arg (QString::number (dimension.width()))
										   .arg (QString::number (dimension.height()))
										   .arg (QString::number (degrees))
										   .arg (QString::number (startPosition))
 										   .arg (QString::number (endPosition)));
	}

	if (directory.length() > 24) {
		truncatedDirectory = "..\"" + directory.right(22) + "\"";
	}
	else {
		truncatedDirectory = "\"" + directory.right(24) + "\"";
	}

	pathTextLabel->setText (QString ("Directory: %1 - Base File Name: \"%2\"").arg (truncatedDirectory).arg (baseFileName));

}

void AnimationDialogImpl::setWorkMode (const bool enable) {

	animatePushButton->setEnabled (enable != true);
	stopPushButton->setEnabled (enable == true);

	degreesLineEdit->setEnabled (enable != true);
	startPositionLineEdit->setEnabled (enable != true);
	endPositionLineEdit->setEnabled (enable != true);
	startCutOffLineEdit->setEnabled (enable != true);
	endCutOffLineEdit->setEnabled (enable != true);
	xAxisRadioButton->setEnabled (enable != true);
	yAxisRadioButton->setEnabled (enable != true);
	zAxisRadioButton->setEnabled (enable != true);
	continuousCheckBox->setEnabled (enable != true);
	durationLineEdit->setEnabled (enable != true);
	fpsComboBox->setEnabled (enable != true);
	widthLineEdit->setEnabled (enable != true);
	heightLineEdit->setEnabled (enable != true);
	fileFormatComboBox->setEnabled (enable != true);
	baseFileNameLineEdit->setEnabled (enable != true);
	directoryLineEdit->setEnabled (enable != true);
	browsePushButton->setEnabled (enable != true);
	storeAnimationCheckBox->setEnabled (enable != true);
	resetPushButton->setEnabled (enable != true);	

}

// *************
// private slots
// *************

void AnimationDialogImpl::handleViewPortSizeChanged (const QSize newRenderSize) {
	
	dimension = newRenderSize;
	widthLineEdit->setText (QString::number (dimension.width()));
	heightLineEdit->setText (QString::number (dimension.height()));
	this->updateFormatTextLabel();

}

void AnimationDialogImpl::handleSceneMoved (const float* newTransformationMatrix) {
 
	startPosition  = newTransformationMatrix[14];
	endPosition    = newTransformationMatrix[14];
	startPositionLineEdit->setText (QString::number (startPosition));
	endPositionLineEdit->setText (QString::number (endPosition));

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
