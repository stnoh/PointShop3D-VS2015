// Title:   ParameterizationDialogImpl.cpp
// Created: Thu Sep 25 14:58:21 2003
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

#include <qcombobox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
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
#include <qstring.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qapplication.h>

#include "../../../../Core/Brush/src/Brush.h"
#include "../../../../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "../../../../Core/CoreResourceTools/BitmapResourceTool/src/BitmapResourceTool.h"
#include "../../../../Core/CoreTools/src/CoreTools.h"

#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "../UI/ExtendedSettings.h"
#include "BitmapMarker.h"
#include "ParameterizationTool.h"
#include "ParameterizationDialogImpl.h"

// ******
// global
// ******

// the single ParameterizationDialogImpl instance, only
// accesible by getInstance() and
// destroyInstance()
static ParameterizationDialogImpl *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

ParameterizationDialogImpl::ParameterizationDialogImpl (ParameterizationTool *parameterizationTool,
									                    QWidget* parent,
									                    const char* name,
									                    bool modal,
									                    WFlags flags)
    : ParameterizationDialogBase (parent, name, modal, flags) {

	this->parameterizationTool  = parameterizationTool;
	bitmapMarker                = BitmapMarker::getInstance();
	currentBrush                = 0;

	extendedSettings = new ExtendedSettings (this);
	this->setOrientation (Vertical);
	this->setExtension (extendedSettings);

	this->initDialog();

}

ParameterizationDialogImpl::~ParameterizationDialogImpl() {
	BitmapMarker::destroyInstance();
}

// **************
// public methods
// **************

ParameterizationDialogImpl *ParameterizationDialogImpl::getInstance (ParameterizationTool *parameterizationTool) {

	if (instance == 0) {
		instance = new ParameterizationDialogImpl (parameterizationTool);
	}
	return instance;

}

void ParameterizationDialogImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}

// ************
// public slots
// ************

void ParameterizationDialogImpl::handleRemoveSelected2D() {
	MarkerManager::getInstance()->removeSelectedFittingConstraints2D();	
}

void ParameterizationDialogImpl::handleRemoveAll2D() {
	MarkerManager::getInstance()->removeAllFittingConstraints2D();
}

void ParameterizationDialogImpl::handleRemoveSelected3D() {
	MarkerManager::getInstance()->removeSelectedFittingConstraints3D();
}

void ParameterizationDialogImpl::handleRemoveAll3D() {
	MarkerManager::getInstance()->removeAllFittingConstraints3D (true);
}

void ParameterizationDialogImpl::handleMarkPropertyBitmap() {

	this->updatePixmaps();
	bitmapMarker->show();
	bitmapMarker->raise();
	

}

void ParameterizationDialogImpl::handlePropertyChosen (const QString &newProperty) {

	currentProperty = SurfelInterface::getSystemProperty (newProperty);
	this->updatePixmaps();

}

void ParameterizationDialogImpl::handleClearSelection() {
	CoreTools::getInstance()->getSelectionTool()->clearSelection (true);
}

void ParameterizationDialogImpl::handleSelectAll() {
	CoreTools::getInstance()->getSelectionTool()->selectAllVisible (true);
}

void ParameterizationDialogImpl::handleUVGeneration() {

	if (this->minDistortionRadioButton->isChecked()) {
		
		float newPrecision,
			newCoverageThreshold,
			newDisplacementScaling,
			newLowPassFilter,
			newFittingConstrWeights;
		bool  isNumber;
		uint  newNofLevels,
			newClusterSize;
		
		// okay, time for a coffee...
		QApplication::setOverrideCursor (Qt::waitCursor);
		
		// *******************
		// read the parameters
		// *******************
		
		// precision
		newPrecision = extendedSettings->precisionLineEdit->text().toFloat (&isNumber);
		if (isNumber == true) {
			
			if (newPrecision != precision) {
				parameterizationTool->parameterization->setPrecision (newPrecision);
				precision = newPrecision;
			}
			
		}
		else {
			// reset the precision
			extendedSettings->precisionLineEdit->setText (QString::number (precision));		
		}
		
		// coverage threshold
		newCoverageThreshold = extendedSettings->coverageThresholdLineEdit->text().toFloat (&isNumber);
		if (isNumber == true) {
			
			if (newCoverageThreshold != coverageThreshold) {
				parameterizationTool->parameterization->setCoverageThreshold (newCoverageThreshold);
				coverageThreshold = newCoverageThreshold;
			}
			
		}
		else {
			// reset the coverage threshold
			extendedSettings->coverageThresholdLineEdit->setText (QString::number (coverageThreshold));		
		}
		
		// number of levels
		newNofLevels = extendedSettings->numberOfLevelsLineEdit->text().toUInt (&isNumber);
		if (isNumber == true) {
			
			if (newNofLevels != nofLevels) {
				parameterizationTool->parameterization->setNofLevels (newNofLevels);
				nofLevels = newNofLevels;
			}
			
		} else {
			extendedSettings->numberOfLevelsLineEdit->setText (QString::number (nofLevels));
		}
		
		// cluster size
		newClusterSize = extendedSettings->clusterSizeLineEdit->text().toUInt (&isNumber);
		if (isNumber == true) {
			
			if (newClusterSize != clusterSize) {
				parameterizationTool->parameterization->setClusterSize (newClusterSize);
				clusterSize = newClusterSize;
			}
			
		} else {
			extendedSettings->clusterSizeLineEdit->setText (QString::number (clusterSize));
		}
		
		// low-pass filter
		newLowPassFilter= extendedSettings->lowPassFilterLineEdit->text().toFloat (&isNumber);
		if (isNumber == true) {
			
			if (newLowPassFilter != lowPassFilter) {
				parameterizationTool->parameterization->setLowPassFilter (newLowPassFilter);
				lowPassFilter = newLowPassFilter;
			}
			
		}
		else {
			extendedSettings->lowPassFilterLineEdit->setText (QString::number (lowPassFilter));		
		}

		// weights for fitting constraints
		newFittingConstrWeights = extendedSettings->fittingConstrWeightsLineEdit->text().toFloat (&isNumber);
		if (isNumber == true) {
			
			if (newFittingConstrWeights!= fittingConstrWeights) {
				parameterizationTool->parameterization->setFittingConstrWeights(newFittingConstrWeights);
				fittingConstrWeights= newFittingConstrWeights;
			}
			
		}
		else {
			extendedSettings->fittingConstrWeightsLineEdit->setText (QString::number (fittingConstrWeights));		
		}
		
		// displacement scaling
		newDisplacementScaling = sculptScaleLineEdit->text().toFloat (&isNumber);
		if (isNumber == true) {
			
			if (newDisplacementScaling != displacementScaling) {
				parameterizationTool->parameterization->setDisplacementScaling (newDisplacementScaling);
				displacementScaling = newDisplacementScaling;
			}
			
		}
		else {
			// reset the displacement scaling
			sculptScaleLineEdit->setText (QString::number (displacementScaling));		
		}

		parameterizationTool->parameterization->setFilterBrushEnabled(surfaceFilterCheckBox->isChecked());
		
		parameterizationTool->parameterization->generateUVCoordinates(samplingBitmapRadioButton->isChecked());
	}		

	else if (this->orthProjectRadioButton->isChecked()) {

		Scene            *scene;
		Object           *object;
		SurfelCollection *surfels;
		SurfelInterface  *surfel;
		MyDataTypes::TextureCoordinate param;
		
		
		scene = Scene::getInstance();
		object = scene->getActiveObject();
		
		if (object) {
			
			
			if (this->XYRadioButton->isChecked()) {
				orthoParameterize (0, 1);
			}
			else if (this->XZRadioButton->isChecked()) {
				orthoParameterize (0, 2);
			}
			else if (this->YZRadioButton->isChecked()) {
				orthoParameterize (1, 2);
			}
			
			
			std::vector<SurfelInterface *>* texturedSurfels = NULL;
			std::vector<MyDataTypes::TextureCoordinate>* textureCoordinates = NULL;
			int numOfTexturedSurfels;
			
			surfels = object->getSurfelCollection();

			uint              nofPositions;
				              
			
			Brush             *brush;
			
			brush = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();
			
			if (brush == 0) {
				return;
			}
			
			int brushSizeX = brush->getSize().width();
			int brushSizeY = brush->getSize().height();
			
			Painter *painter = new Painter (brush);
			
			nofPositions = CoreTools::getInstance()->getSelectionTool()->getSelection()->size();
			
			textureCoordinates->resize (nofPositions);
			texturedSurfels->resize (nofPositions);
			
			
			numOfTexturedSurfels = 0;

			for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {

				if (surfel->isFlagOn (SurfelInterface::SELECTED1)) {
										
					param = surfel->getTextureCoordinate();

					(*textureCoordinates)[numOfTexturedSurfels].u = brushSizeX * param.u;
					(*textureCoordinates)[numOfTexturedSurfels].v = brushSizeY * param.v;
					(*texturedSurfels)[numOfTexturedSurfels] = surfel;
					numOfTexturedSurfels++;			
				}
			}
			
			painter->paint (texturedSurfels, numOfTexturedSurfels, textureCoordinates);
			delete painter;
			
			scene->emitObjectModifiedSignal();
			scene->emitRenderFrameSignal();
		}
	}
		
	QApplication::restoreOverrideCursor();
		
}

void ParameterizationDialogImpl::handleExtensions (bool enable) {
	this->showExtension (enable);
}

void ParameterizationDialogImpl::handleShowAlpha (bool) {
	this->updatePixmaps();
}


void ParameterizationDialogImpl::handleParameterizationTypeChanged () {
	this->checkConditions();
}


// ***************
// private methods
// ***************

void ParameterizationDialogImpl::initDialog() {	

	QStringList   propertiesList;
	MarkerManager *markerManager;

	propertiesList = SurfelInterface::getEditableProperties();
	propertyComboBox->insertStringList (propertiesList);
	currentProperty = SurfelInterface::getSystemProperty (propertyComboBox->currentText());
	currentBrush    = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();

	markerManager = MarkerManager::getInstance();
	// get informed whenever a list changes
	this->connect (markerManager, SIGNAL (fittingConstraints3DChanged (const QList<Marker3D> *)),
		           this, SLOT (handleFittingConstraints3DChanged (const QList<Marker3D> *)));

	this->connect (markerManager, SIGNAL (fittingConstraints2DChanged (const QList<Marker2D> *)),
		           this, SLOT (handleFittingConstraints2DChanged (const QList<Marker2D> *)));

	
	// get informed whenever the selection changes
	this->connect (CoreTools::getInstance()->getSelectionTool(), SIGNAL (selectionChanged (SurfelInterface::Flag)),
		           this, SLOT (handleSelectionChanged (SurfelInterface::Flag)));


	// get informed whenver the brush has changed
	this->connect (CoreResourceTools::getInstance()->getBrushChooserTool(), SIGNAL (brushChanged (const Brush *)),
		           this, SLOT (handleBrushChanged (const Brush *)));

	fittingConstraint3DTextLabel->setText (QString::number (markerManager->getFittingConstraints3D()->count()));
	fittingConstraint2DTextLabel->setText (QString::number (markerManager->getFittingConstraints2D()->count()));

	precision = parameterizationTool->parameterization->getPrecision();
	extendedSettings->precisionLineEdit->setText (QString::number (precision));

	fittingConstrWeights = parameterizationTool->parameterization->getFittingConstrWeights();
	extendedSettings->fittingConstrWeightsLineEdit->setText (QString::number (fittingConstrWeights));

	coverageThreshold = parameterizationTool->parameterization->getCoverageThreshold();
	extendedSettings->coverageThresholdLineEdit->setText (QString::number (coverageThreshold));

	nofLevels = parameterizationTool->parameterization->getNofLevels();
	extendedSettings->numberOfLevelsLineEdit->setText (QString::number (nofLevels));

	clusterSize = parameterizationTool->parameterization->getClusterSize();
	extendedSettings->clusterSizeLineEdit->setText (QString::number (clusterSize));

	lowPassFilter = parameterizationTool->parameterization->getLowPassFilter();
	extendedSettings->lowPassFilterLineEdit->setText (QString::number (lowPassFilter));

	displacementScaling = parameterizationTool->parameterization->getDisplacementScaling();
	sculptScaleLineEdit->setText (QString::number (displacementScaling));

	surfaceFilterCheckBox->setChecked(parameterizationTool->parameterization->isFilterBrushEnabled());

	this->checkConditions();
	this->updatePixmaps();

}

void ParameterizationDialogImpl::updatePixmaps() {

	QPixmap       pixmap,
		          defaultPixmap;
	const QImage  *image;
	BrushChannel  *currentBrushChannel;


	defaultPixmap = QPixmap ("Icons/DefaultPixmap.png");
	// show property bitmap
	if (currentBrush != 0) {

		if (showAlphaCheckBox->isChecked() == false) {
			currentBrushChannel = currentBrush->getChannel (currentProperty);
		}
		else {
			currentBrushChannel = currentBrush->getAlphaChannel (currentProperty);
		}

		if (currentBrushChannel != 0) {

			image = currentBrushChannel->getBitmap();
			
			pixmap.convertFromImage (*image);
			propertyPixmapLabel->setPixmap (pixmap);

			// check and see if we also need to update the bitmap marker
			bitmapMarker->displayPixmap (&pixmap);

		}
		else {

			propertyPixmapLabel->setPixmap (defaultPixmap);
			// check and see if we also need to update the bitmap marker
			bitmapMarker->displayPixmap (&defaultPixmap);

		}

	}
	else {

		propertyPixmapLabel->setPixmap (defaultPixmap);
		bitmapMarker->displayPixmap (&defaultPixmap);

	}

}

void ParameterizationDialogImpl::checkConditions() {

	MarkerManager *markerManager;

	markerManager = MarkerManager::getInstance();
	if ((CoreTools::getInstance()->getSelectionTool()->getSelectionSize() > 0) &&
		(((markerManager->getFittingConstraints3D()->count() > 0) &&
		 (markerManager->getFittingConstraints3D()->count() == markerManager->getFittingConstraints2D()->count())) ||
		 orthProjectRadioButton->isChecked()))
	{
		generateUVButton->setEnabled (true);
	}
	else {
		generateUVButton->setEnabled (false);
	}

}



void ParameterizationDialogImpl::orthoParameterize (unsigned int uIndex,
													unsigned int vIndex) {


	Scene            *scene;
	Object           *object;
	SurfelCollection *surfels;
	SurfelInterface  *surfel;
	MyDataTypes::TextureCoordinate param;

	float            minU,
		             minV,
					 maxU,
		             maxV;

	scene = Scene::getInstance();
	object = scene->getActiveObject();
	
	if (object) {
		
		surfels = object->getSurfelCollection();
	
		minU = minV = FLT_MAX;
		maxU = maxV = -FLT_MAX;

		for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {			
			
			if (surfel->isFlagOn (SurfelInterface::SELECTED1)) {
				param.u = surfel->getPosition()[uIndex];
				param.v = surfel->getPosition()[vIndex];
				
				if (param.u < minU) { minU = param.u; }
				if (param.v < minV) { minV = param.v; }
				if (param.u > maxU) { maxU = param.u; }
				if (param.v > maxV) { maxV = param.v; }
				
				scene->setSurfelTextureCoordinate (surfel, param, false);
			}
		}

		for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {			
			
			if (surfel->isFlagOn (SurfelInterface::SELECTED1)) {				
				param = surfel->getTextureCoordinate();
				param.u = (param.u - minU) / (maxU - minU);
				param.v = (param.v - minV) / (maxV - minV);
				scene->setSurfelTextureCoordinate (surfel, param, false);
			}
		}
	}
}




// *************
// private slots
// *************

void ParameterizationDialogImpl::handleBrushChanged (const Brush *newBrush) {

	currentBrush = newBrush;
	if (currentBrush != 0) {
		markPropertyBitmapButton->setEnabled (true);
	}
	else {
		markPropertyBitmapButton->setEnabled (false);
	}	

	this->updatePixmaps();

}

void ParameterizationDialogImpl::handleFittingConstraints3DChanged (const QList<Marker3D> *fittingConstraint3DList) {
	fittingConstraint3DTextLabel->setText (QString::number (fittingConstraint3DList->count()));
	this->checkConditions();
	qDebug ("ParameterizationDialogImpl: handleFittingConstraints3DChanged: nof 3D constraints: %d", fittingConstraint3DList->count());
}

void ParameterizationDialogImpl::handleFittingConstraints2DChanged (const QList<Marker2D> *fittingConstraint2DList) {
	fittingConstraint2DTextLabel->setText (QString::number (fittingConstraint2DList->count()));
	this->checkConditions();
}


void ParameterizationDialogImpl::handleSelectionChanged (SurfelInterface::Flag selectionFlag) {
	if (selectionFlag == SurfelInterface::SELECTED1) {
		this->checkConditions();
	}
}




// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
