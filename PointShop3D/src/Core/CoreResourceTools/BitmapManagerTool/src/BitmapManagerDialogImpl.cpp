// Title:   BitmapManagerDialogImpl.cpp
// Created: Thu Sep 25 14:23:11 2003
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
#include <qfile.h>
#include <qbuttongroup.h>
#include <qstringlist.h>
#include <qcolordialog.h>
#include <qslider.h>
#include <qapplication.h>

#include "BrushScrollView.h"


#include "../../../Scene/src/Scene.h"
#include "../../../CoreResourceTools/src/CoreResourceTools.h"
#include "../../BitmapResourceTool/src/BitmapResourceTool.h"
#include "../../../../Utilities/UserMessages/src/StatusBar.h"

#include "BitmapManagerDialogImpl.h"
#include "NewBrushDialogImpl.h"
#include "SelectShapeDialogImpl.h"


// *********
// constants
// *********

static const char *CONFIGURATION_FILE_NAME = "bitmapComb.dat";

// ******
// global
// ******

// the single ResourceBitmapDialogImpl instance, only
// accesible by getInstance() and
// destroyInstance()
static BitmapManagerDialogImpl *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

BitmapManagerDialogImpl::BitmapManagerDialogImpl (QWidget* parent,
												  const char* name,
												  bool modal,
												  WFlags flags)
    : BitmapManagerDialogBase (parent, name, modal, flags) {


	(void)CONFIGURATION_FILE_NAME;
	
	QApplication::setOverrideCursor (Qt::waitCursor);

	
	constBitmap = new QImage (1, 1, 32);
	constAlpha = new QImage (1, 1, 32);
	gaussianAlpha = new QImage (100, 100, 32);

	propertyResourceTool = 0;
	alphaResourceTool    = 0;

	this->initDialog();

	QApplication::restoreOverrideCursor();

}


BitmapManagerDialogImpl::~BitmapManagerDialogImpl() {
    // no need to delete child widgets, Qt does it all for us

	// delete all local bitmaps
	for (int i = 0; i < numOfProperties; i++) {
		if(propertyBitmaps[i] != 0) {
			delete propertyBitmaps[i];
		}
	
		if(alphaBitmaps[i] != 0) {
			alphaBitmaps[i] = 0;
		}
	}

	if (propertyResourceTool != 0) {
		delete propertyResourceTool;
	}

	if (alphaResourceTool !=0) {
		delete alphaResourceTool;
	}

	delete constBitmap;
	delete constAlpha;
	delete gaussianAlpha;

}



// **************
// public methods
// **************

BitmapManagerDialogImpl *BitmapManagerDialogImpl::getInstance () {

	if (instance == 0) {
		instance = new BitmapManagerDialogImpl ();
	}
	return instance;

}

void BitmapManagerDialogImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}



// ************
// public slots
// ************

void BitmapManagerDialogImpl::handleGetAlphaFromProperty () {
		
	// get alpha channel from current property bitmap
	QImage *alphaBitmap = extractAlpha(propertyBitmaps[currentProperty]);

	// this method makes a deep copy of the alpha bitmap!
	handleAlphaBitmapChanged (alphaBitmap);

	delete alphaBitmap;
}


void BitmapManagerDialogImpl::handleApplySizeFromCurrentBitmap() {

	if (propertyBitmaps[currentProperty]) {	
		this->brushWidthSpinBox->setValue (propertyBitmaps[currentProperty]->width());
		this->brushHeightSpinBox->setValue (propertyBitmaps[currentProperty]->height());
	}
	this->setBrushModifiedState (true);
}


void BitmapManagerDialogImpl::handlePropertySelectionChanged () {

	currentProperty = this->propertyComboBox->currentItem();

	// set pixmaps for display
	if (propertyBitmaps[currentProperty] != 0) {
		QPixmap pixmap;
		pixmap.convertFromImage (*propertyBitmaps[currentProperty]);
		propertyPixmapLabel->setPixmap (pixmap);
	}
	else {
		propertyPixmapLabel->clear();
	}

	if (alphaBitmaps[currentProperty] != 0) {
		QPixmap pixmap;
		pixmap.convertFromImage(*alphaBitmaps[currentProperty]);
		alphaPixmapLabel->setPixmap (pixmap);	
	}
	else {
		alphaPixmapLabel->clear();
	}

	const QStringList propertiesList = SurfelInterface::getEditableProperties();
	SurfelInterface::Property surfelProperty = SurfelInterface::getSystemProperty(propertiesList[currentProperty]);

	if (SurfelInterface::getNumOfPropertyBrushComponents (surfelProperty) == 3) {
		this->propertyConstPushButton->setText ("Color");
		this->propertyConstPushButton->repaint();
		this->propertyColorChooserPushButton->setEnabled (true);
	}
	else {
		this->propertyConstPushButton->setText ("Shape");
		this->propertyConstPushButton->repaint();
		this->propertyColorChooserPushButton->setEnabled (false);
	}
}


void BitmapManagerDialogImpl::handleClearPropertyBitmap () {

	handlePropertyBitmapChanged (NULL);
}


void BitmapManagerDialogImpl::handleClearAlphaMask () {

	handleAlphaBitmapChanged (NULL);
}


void BitmapManagerDialogImpl::handleCreateNewBrush() {

	Brush *newBrush;

	BrushChooserTool *brushChooserTool = CoreResourceTools::getInstance()->getBrushChooserTool();
	Brush *brush = brushChooserTool->getSelectedBrush();

	NewBrushDialogImpl *newBrushDialog = new NewBrushDialogImpl();
	
	if (newBrushDialog->exec() != 0) {

		if (newBrushDialog->copyBitmapsCheckBox->isChecked() &&( brush != 0)) {
			// copy selected brush
			newBrush = new Brush(brush);

			// set name
			newBrush->setName(newBrushDialog->brushNameLineEdit->text());
		}
		else {
			newBrush = new Brush (newBrushDialog->brushNameLineEdit->text(),
				                  QSize (brushWidthSpinBox->value(), brushHeightSpinBox->value()));
		}		

		// add new brush to brush chooser tool
		brushChooserTool->addAndSelectBrush (newBrush);

	} 

	delete newBrushDialog;
}


void BitmapManagerDialogImpl::handleUpdateBrush() {

	this->updateSelectedBrush();

	this->setBrushModifiedState (false);
}


void BitmapManagerDialogImpl::handleSelectedBrushChanged (const Brush *) {
	this->setSelectedBrush();
}


void BitmapManagerDialogImpl::handleConstProperty() {

	const QStringList propertiesList = SurfelInterface::getEditableProperties();
	SurfelInterface::Property surfelProperty = SurfelInterface::getSystemProperty(propertiesList[currentProperty]);

	if (SurfelInterface::getNumOfPropertyBrushComponents (surfelProperty) == 3) {

		QColor chosenColor;
		QColor oldColor;
		
		oldColor.setRgb (constBitmap->pixel (0, 0));
		
		chosenColor = QColorDialog::getColor (oldColor, this);
		
		if (chosenColor.isValid() == true) {
			
			constBitmap->setPixel (0, 0, chosenColor.rgb());
			handlePropertyBitmapChanged (constBitmap);
		}
	}
	else {
	
		SelectShapeDialogImpl *selectShapeDialog = new SelectShapeDialogImpl();	
		
		if (selectShapeDialog->exec() != 0) {
			
			handlePropertyBitmapChanged (selectShapeDialog->getShapeImage());
		}	
		delete selectShapeDialog;
	} 
}






void BitmapManagerDialogImpl::handleShapeAlpha() {

	SelectShapeDialogImpl *selectShapeDialog = new SelectShapeDialogImpl();	
	
	if (selectShapeDialog->exec() != 0) {
		
		handleAlphaBitmapChanged (selectShapeDialog->getShapeImage());
	}	
	delete selectShapeDialog;
}



void BitmapManagerDialogImpl::handleColorChooserProperty () {

	const QStringList propertiesList = SurfelInterface::getEditableProperties();
	SurfelInterface::Property surfelProperty = SurfelInterface::getSystemProperty(propertiesList[currentProperty]);

	if (SurfelInterface::getNumOfPropertyBrushComponents (surfelProperty) == 3) {

		QRgb color = CoreResourceTools::getInstance()->getColorChooserTool()->getFGColor();
		constBitmap->setPixel (0, 0, color);
		handlePropertyBitmapChanged (constBitmap);
	}
}




void BitmapManagerDialogImpl::handleClearAll () {

	int restoreCurrentProperty = currentProperty;
	
	for (currentProperty = 0; currentProperty < numOfProperties; currentProperty++) {
		handlePropertyBitmapChanged (NULL);
		handleAlphaBitmapChanged (NULL);
	}
	currentProperty = restoreCurrentProperty;
}
	


void BitmapManagerDialogImpl::handleSizeChanged () {
	this->setBrushModifiedState (true);
}


void BitmapManagerDialogImpl::handleFGColorChanged (QRgb newFGColor) {

	Brush *brush = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();
	
	if (brush) {		
		BrushChannel *brushChannel = brush->getChannel (SurfelInterface::DIFFUSE_COLOR);	
		if (brushChannel) {
			if ((brushChannel->getBitmap()->width() == 1) && (brushChannel->getBitmap()->height() == 1)) {
				QImage *image = new QImage (1, 1, 32);
				image->setPixel (0, 0, newFGColor);
				brush->setChannel (SurfelInterface::DIFFUSE_COLOR, 0, image);
				brush->save (CoreResourceTools::getInstance()->getBrushChooserTool()->getBrushDirectory());				
				CoreResourceTools::getInstance()->getBrushChooserTool()->updateBrushBitmap (brush);
			}
		}
	}
}

	




// ***************
// private methods
// ***************

void BitmapManagerDialogImpl::initDialog() {	

	Configuration *configuration;
	
	StatusBar::getInstance()->showMessage("Loading Bitmaps...", 10000);

	propertyResourceTool = new BitmapResourceTool (Configuration::getInstance()->getBitmapDirectory(),
		                                          (BitmapUserData::BitmapProperties) (BitmapUserData::DEPTH_8   |
												                                      BitmapUserData::DEPTH_32  |
																					  BitmapUserData::GRAYSCALE |
																					  BitmapUserData::COLOR));

	propertyResourceTool->addMenuToPushButton (this->propertySelectPushButton);

	alphaResourceTool = new BitmapResourceTool (Configuration::getInstance()->getBitmapDirectory(),
		                                          (BitmapUserData::BitmapProperties) (BitmapUserData::DEPTH_8   |
												                                      BitmapUserData::DEPTH_32  |
																					  BitmapUserData::GRAYSCALE |
																					  BitmapUserData::COLOR));
	alphaResourceTool->addMenuToPushButton (this->alphaSelectPushButton);

	// get informed whenever the property bitmap changed
	this->connect (propertyResourceTool, SIGNAL (bitmapChanged (const QImage *)),
		           this, SLOT (handlePropertyBitmapChanged (const QImage *)));

	// get informed whenever the alpha bitmap changed
	this->connect (alphaResourceTool, SIGNAL (bitmapChanged (const QImage *)),
		           this, SLOT (handleAlphaBitmapChanged (const QImage *)));

	// get informed whenever the current brush changed
	this->connect (CoreResourceTools::getInstance()->getBrushChooserTool(), SIGNAL (brushChanged (const Brush *)),
				   this, SLOT (handleSelectedBrushChanged (const Brush *)));

	configuration = Configuration::getInstance();
	
	// let the propertyResourceTool know whenever the bitmap directory changed
	this->connect (configuration, SIGNAL (bitmapDirectoryChanged (const QString)),
		           propertyResourceTool, SLOT (handleBitmapDirectoryChanged (const QString)));

	// let the alphaResourceTool know whenever the bitmap directory changed
	this->connect (configuration, SIGNAL (bitmapDirectoryChanged (const QString)),
		           alphaResourceTool, SLOT (handleBitmapDirectoryChanged (const QString)));

	// connect to color chooser tool. When the foreground color changes and the diffuse color
	// bitmap is 1 by 1 pixels, the its color is set to fgColor 
	this->connect (CoreResourceTools::getInstance()->getColorChooserTool(), SIGNAL (fgColorChanged (QRgb)),
		           this, SLOT (handleFGColorChanged (QRgb)));

	// get surfel properties
	SurfelInterface::PropertyDescriptor properties = Scene::getInstance()->getSurfelPropertyDescriptor();
	(void)properties;  // to suppress warnings, since we don't use it at the moment

	const QStringList propertiesList = SurfelInterface::getEditableProperties();
	this->propertyComboBox->insertStringList (propertiesList);

	numOfProperties = propertiesList.count();

	currentProperty = 0;
	this->propertyComboBox->setCurrentItem (currentProperty);

	propertyBitmaps = new QImage* [numOfProperties];

	alphaBitmaps = new QImage* [numOfProperties];

	// init property (alpha) bitmaps
	for (int i = 0; i < numOfProperties; i++) {
		propertyBitmaps[i] = 0;
		alphaBitmaps[i] = 0;	
	}

	setBrushModifiedState (false);

	StatusBar::getInstance()->showMessage("Bitmaps loaded");

}

void BitmapManagerDialogImpl::showEvent (QShowEvent *se) {
	(void)se;
	this->setSelectedBrush();
}

// *************
// private slots
// *************

void BitmapManagerDialogImpl::handlePropertyBitmapChanged (const QImage *newPropertyBitmap) {

	// delete the current property bitmap
	if(propertyBitmaps[currentProperty] != 0) {
		delete propertyBitmaps[currentProperty];
	}
	propertyBitmaps[currentProperty] = 0;

	if (newPropertyBitmap != 0) {
		// get deep copy of the bitmap!
		propertyBitmaps[currentProperty] = new QImage();
		(*propertyBitmaps[currentProperty]) = newPropertyBitmap->copy();

		QPixmap pixmap;
		pixmap.convertFromImage (*propertyBitmaps[currentProperty]);
		propertyPixmapLabel->setPixmap (pixmap);
	}
	else {
		propertyPixmapLabel->clear();
	}

	this->setBrushModifiedState (true);
}

void BitmapManagerDialogImpl::handleAlphaBitmapChanged (const QImage *newAlphaBitmap) {

	// delete the current property alpha bitmap
	if(alphaBitmaps[currentProperty] != 0) {
		delete alphaBitmaps[currentProperty];
	}
	alphaBitmaps[currentProperty] = 0;

	if (newAlphaBitmap != 0) {
		// get deep copy of property alpha bitmap! at the same time, convert it to
		// an 8bit gray scale image
		alphaBitmaps[currentProperty] = convertTo8bppGrayScale(newAlphaBitmap);

		QPixmap pixmap;
		pixmap.convertFromImage (*alphaBitmaps[currentProperty]);
		alphaPixmapLabel->setPixmap (pixmap);	
	}
	else {
		alphaPixmapLabel->clear();
	}
	this->setBrushModifiedState (true);
}

QImage* BitmapManagerDialogImpl::extractAlpha(const QImage *input) {

	// generate a new bitmap containing the alpha channel of a bitmap
	// 32bit input: extract alpha channel from upper 8bits
	// 8bit input: convert to greyscale

	if (!input) {
		return 0;
	}

	QImage *newAlphaBitmap = new QImage (input->size(), 8, 256);;
				
	int x,
		y,
		width,
		height;
	QRgb color;
	
	width  = input->width();
	height = input->height();
	
	// initialize color table to greyscale image
	for (x = 0; x < 256; x++) {
		newAlphaBitmap->setColor (x, qRgb (x, x, x));
	}
	
	if (input->depth() == 32) {
		
		for (y = 0; y < height; y++) {
			
			for (x = 0; x < width; x++) {
				// extract alpha channel
				color = *((QRgb *)input->scanLine(y) + x);
				*(newAlphaBitmap->scanLine(y) + x) = (color & 0xff000000) >> 24;
			}
			
		}
		
	}
	else if (input->depth() == 8) {
		
		for (y = 0; y < height; y++) {
			
			for (x = 0; x < width; x++) {
				// convert to greyscale
				color = input->color (*(input->scanLine(y) + x));
				*(newAlphaBitmap->scanLine(y) + x) = qAlpha (color);
			}
			
		}
		
	}

	return newAlphaBitmap;
}


QImage* BitmapManagerDialogImpl::convertTo8bppGrayScale (const QImage *input) {

	// convert an image to an 8bpp gray scale image

	if (input == 0) {
		return 0;
	}

	QImage *output = new QImage (input->size(), 8, 256);;
				
	int x, y;
	int width, height;
	QRgb color;
	
	width  = input->width();
	height = input->height();
	
	// initialize color table to greyscale image
	for (x = 0; x < 256; x++) {
		output->setColor (x, qRgb (x, x, x));
	}
	
	if (input->depth() == 32) {	
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				// extract alpha channel
				color = *((QRgb *)input->scanLine(y) + x);
				color = (int) (0.3f * (float)qRed(color) + 0.59f * (float)qGreen(color) + 0.11f * (float)qBlue(color));
				*(output->scanLine(y) + x) = qRgb(color, color, color);
			}
		}
	}

	else if (input->depth() == 8) {
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				// convert to greyscale
				color = input->color (*(input->scanLine(y) + x));
				color = (int) (0.3f * (float)qRed(color) + 0.59f * (float)qGreen(color) + 0.11f * (float)qBlue(color));
				*(output->scanLine(y) + x) = color;
			}
		}
	}

	return output;
}


void BitmapManagerDialogImpl::setSelectedBrush() {
	
	Brush *brush = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();
	BrushChannel *brushChannel;
	int i;

	if (brush != 0) {
	
		// set name of current brush in brush name text label
		QString brushName = brush->getName();
		brushNameLineEdit->setText ( brushName ); 
		brushNameLineEdit->setReadOnly ( true );

		// set size spin boxes
		brushWidthSpinBox->setValue(brush->getSize().width());
		brushHeightSpinBox->setValue(brush->getSize().height());

		// set bitmaps
		const QStringList propertiesList = SurfelInterface::getEditableProperties();

		// set property bitmaps
		for (i = 0; i < numOfProperties; i++) {	
			// get brush channel of current property
			brushChannel = brush->getChannel(SurfelInterface::getSystemProperty(propertiesList[i]));

			// delete old property bitmap
			if(propertyBitmaps[i] != 0) {
				delete propertyBitmaps[i];
			}
			propertyBitmaps[i] = 0;

			if (brushChannel != 0) {
				// NOTE: getBitmap() does not return a deep copy of the brush channel bitmap! therefore,
				// explicitly copy the bitmap
				propertyBitmaps[i] = new QImage();
				(*propertyBitmaps[i]) = brushChannel->getBitmap()->copy();
			}
		}

		// set alpha bitmaps
		for (i = 0; i < numOfProperties; i++) {	
			// get brush alpha channel of current property
			brushChannel = brush->getAlphaChannel(SurfelInterface::getSystemProperty(propertiesList[i]));

			// delete old property alpha bitmap
			if(alphaBitmaps[i] != 0) {
				delete alphaBitmaps[i];
			}
			alphaBitmaps[i] = 0;

			if (brushChannel != 0) {
				// NOTE: getBitmap() does not return a deep copy of the brush channel bitmap! therefore,
				// explicitly copy the bitmap
				alphaBitmaps[i] = new QImage();
				(*alphaBitmaps[i]) = brushChannel->getBitmap()->copy();
			}
		}

		// enable the update button
		updateBrushPushButton->setEnabled (true);

	}
	else {

		QString brushName = "";
		brushNameLineEdit->setText (brushName); 
		brushNameLineEdit->setReadOnly (true);

		// set size spin boxes
		brushWidthSpinBox->setValue(0);
		brushHeightSpinBox->setValue(0);

		// delete property bitmaps
		for (i = 0; i < numOfProperties; i++) {	

			// delete old property bitmap
			if (propertyBitmaps[i] != 0) {
				delete propertyBitmaps[i];
			}
			propertyBitmaps[i] = 0;

			// delete old property alpha bitmap
			if(alphaBitmaps[i] != 0) {
				delete alphaBitmaps[i];
			}
			alphaBitmaps[i] = 0;

		}

		// disable the update button
		updateBrushPushButton->setEnabled (false);

	}

	// this updates the bitmpas in the "bitmap selection" window
	this->handlePropertySelectionChanged ();

}

void BitmapManagerDialogImpl::updateSelectedBrush() {
	
	Brush *brush = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();
	BrushChannel *brushChannel;
	int i;
	SurfelInterface::Property surfelProperty;

	// set size
	brush->setSize(QSize(brushWidthSpinBox->value(), brushHeightSpinBox->value()));

	// set bitmaps
	const QStringList propertiesList = SurfelInterface::getEditableProperties();

	// update the brush channel for each property
	for(i = 0; i < numOfProperties; i++) {	
		// get brush channel of current property
		surfelProperty = SurfelInterface::getSystemProperty(propertiesList[i]);
		brushChannel = brush->getChannel(surfelProperty);

		brush->removeChannel(surfelProperty);
		if(propertyBitmaps[i]) {
			brush->setChannel(surfelProperty, 1.0f, propertyBitmaps[i]);
		} 
	}

	// update the brush alpha channel for each property
	for(i=0; i<numOfProperties; i++) {	
		// get brush alpha channel of current property
		surfelProperty = SurfelInterface::getSystemProperty(propertiesList[i]);
		brushChannel = brush->getAlphaChannel(surfelProperty);

		brush->removeAlphaChannel(surfelProperty);
		if(alphaBitmaps[i]) {
			brush->setAlphaChannel(surfelProperty, alphaBitmaps[i]);
		}
	}
	// save brush
	brush->save (CoreResourceTools::getInstance()->getBrushChooserTool()->getBrushDirectory());

	CoreResourceTools::getInstance()->getBrushChooserTool()->updateBrushBitmap (brush);
}



void BitmapManagerDialogImpl::setBrushModifiedState (bool brushModified) {

	if (brushModified) {
		this->updateBrushPushButton->setEnabled (true);
	}
	else {
		this->updateBrushPushButton->setEnabled (false);
	}
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
