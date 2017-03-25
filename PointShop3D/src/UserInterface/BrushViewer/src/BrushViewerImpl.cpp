// Title:   BrushViewerImpl.cpp
// Created: Thu Sep 25 14:42:21 2003
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

#include <qgrid.h>
#include <qscrollview.h>

#include "../../../Core/CoreResourceTools/BitmapManagerTool/src/BrushScrollView.h"
#include "../../../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "BrushViewerImpl.h"

// the single instance, only accesible by getInstance() and destroyInstance()
static BrushViewerImpl *instance = 0;

BrushViewerImpl::BrushViewerImpl (QWidget* parent, const char* name, bool modal, WFlags flags) :
	                              BrushViewer(parent, name, modal, flags) {

	int i;
	const QStringList propertiesList = SurfelInterface::getEditableProperties();

	this->connect (CoreResourceTools::getInstance()->getBrushChooserTool(), SIGNAL (brushChanged (const Brush *)),
				   this, SLOT (handleSelectedBrushChanged (const Brush *)));

	numOfProperties = propertiesList.count();

	currentProperty = 0;

	propertyBitmaps = new const QImage* [numOfProperties];
	alphaBitmaps = new const QImage* [numOfProperties];

	for (i = 0; i < numOfProperties; i++) {
		propertyBitmaps[i] = NULL;
		alphaBitmaps[i] = NULL;	
	}
	

	QGrid *grid = new QGrid (numOfProperties);
	grid->setGeometry (0,0, 700, 100);
	this->brushScollView->addChild (grid);


	textLabels     = new QLabel* [numOfProperties];
	propertyLabels = new QLabel* [numOfProperties];
	spacerLabels   = new QLabel* [numOfProperties];
	alphaLabels    = new QLabel* [numOfProperties];

	// property descriptor text
	for (i = 0; i < numOfProperties; i++) {
		textLabels[i] = new QLabel (propertiesList[i], grid);
		textLabels[i]->setFixedSize (70, 15);
		textLabels[i]->setScaledContents (true);
		textLabels[i]->setAlignment (AlignCenter | AlignVCenter);
		grid->insertChild (textLabels[i]);
	}

	// property bitmaps
	for (i = 0; i < numOfProperties; i++) {
		propertyLabels[i] = new QLabel ("", grid);
		propertyLabels[i]->setFixedSize (60, 60);
		propertyLabels[i]->setScaledContents (true);
		grid->insertChild (propertyLabels[i]);
	}

	// line of spacers
	for (i = 0; i < numOfProperties; i++) {
		spacerLabels[i] = new QLabel ("",grid);
		spacerLabels[i]->setFixedSize (70, 3);
		spacerLabels[i]->setScaledContents (true);
		grid->insertChild (spacerLabels[i]);
	}

	// alpha masks
	for (i = 0; i < numOfProperties; i++) {
		alphaLabels[i] = new QLabel ("", grid);
		alphaLabels[i]->setFixedSize (60, 60);
		alphaLabels[i]->setScaledContents (true);
		grid->insertChild (alphaLabels[i]);
	}

	this->brushScollView->setHScrollBarMode (QScrollView::AlwaysOn);
	this->brushScollView->setVScrollBarMode (QScrollView::AlwaysOff);

}

BrushViewerImpl::~BrushViewerImpl() {

	delete[] propertyBitmaps;
	delete[] alphaBitmaps;
	delete[] textLabels;
	delete[] propertyLabels;
	delete[] spacerLabels;
	delete[] alphaLabels;
}

BrushViewerImpl *BrushViewerImpl::getInstance () {

	if (instance == 0) {
		instance = new BrushViewerImpl ();
	}
	return instance;

}

void BrushViewerImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}


void BrushViewerImpl::handleSelectedBrushChanged (const Brush *) {

	setSelectedBrush();
}


void BrushViewerImpl::showEvent (QShowEvent *se) {

	(void)se;
	setSelectedBrush();
}


void BrushViewerImpl::setSelectedBrush() {
	
	Brush *brush = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();
	BrushChannel *brushChannel;
	const QImage *newPropertyBitmap;
	QPixmap pixmap;
	int i;

	if (brush != 0) {
	
		// set name of current brush in brush name text label
		QString brushName = brush->getName();
		brushNamePlaceHolder->setText ( brushName ); 

		// set size labels
		QString s;
		s = s.setNum(brush->getSize().width());
		widthPlaceHolder->setText(s);
		s = s.setNum(brush->getSize().height());
		heightPlaceHolder->setText(s);

		// set bitmaps
		const QStringList propertiesList = SurfelInterface::getEditableProperties();

		// set property bitmaps
		for(i=0; i<numOfProperties; i++) {	
			// get brush channel of current property
			brushChannel = brush->getChannel(SurfelInterface::getSystemProperty(propertiesList[i]));

			if (brushChannel != 0) {
				newPropertyBitmap = brushChannel->getBitmap();

				propertyBitmaps[i] = newPropertyBitmap;
				pixmap.convertFromImage (*newPropertyBitmap);
				propertyLabels[i]->setPixmap (pixmap);
			}
			else {
				propertyBitmaps[i] = 0;
				propertyLabels[i]->clear();
			}
		}

		// set alpha bitmaps
		for(i=0; i<numOfProperties; i++) {	
			// get brush alpha channel of current property
			brushChannel = brush->getAlphaChannel(SurfelInterface::getSystemProperty(propertiesList[i]));
			
			if (brushChannel != 0) {
				newPropertyBitmap = brushChannel->getBitmap();
				alphaBitmaps[i] = newPropertyBitmap;
				pixmap.convertFromImage (*newPropertyBitmap);
				alphaLabels[i]->setPixmap (pixmap);
			}
			else {
				alphaBitmaps[i] = 0;
				alphaLabels[i]->clear();
			}
		}

	}
	else {

		// set name of current brush in brush name text label
		QString brushName = "";
		brushNamePlaceHolder->setText (brushName); 

		// set size labels
		QString s;
		s = s.setNum(0);
		widthPlaceHolder->setText(s);
		s = s.setNum(0);
		heightPlaceHolder->setText(s);

		// set bitmaps
		const QStringList propertiesList = SurfelInterface::getEditableProperties();

		// clear property bitmaps
		for (i=0; i<numOfProperties; i++) {	

			propertyBitmaps[i] = 0;
			propertyLabels[i]->clear();
			alphaBitmaps[i] = 0;
			alphaLabels[i]->clear();

		}

	}

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
