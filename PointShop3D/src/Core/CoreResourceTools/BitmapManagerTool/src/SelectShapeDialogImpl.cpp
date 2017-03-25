// Title:   SelectShapeDialogImpl.cpp
// Created: Thu Sep 25 14:22:54 2003
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

#include <qlabel.h>
#include <qslider.h>
#include <qradiobutton.h>
#include <qimage.h>

#include <math.h>

#include "SelectShapeDialogImpl.h"


// **************
// public methods
// **************


SelectShapeDialogImpl::SelectShapeDialogImpl() : SelectShapeDialogBase (0, 0, true) {


	imageSizeX = this->shapePixmapLabel->width();
	imageSizeY = this->shapePixmapLabel->height();
	shapeImage = new QImage (imageSizeX, imageSizeY, 32);

	this->scaleValueSlider->setValue (255);
	this->sizeValueSlider->setValue (100);
}


QImage* SelectShapeDialogImpl::getShapeImage () {
	return shapeImage;
}



// ************
// public slots
// ************



void SelectShapeDialogImpl::handleSliderValueChanged() {


	int x, y;
	float scaledX, scaledY;
	int scale = this->scaleValueSlider->value();

	if (constRadioButton->isChecked()) {

		this->sizeValueSlider->setEnabled (false);
		this->sizeTextLabel->setEnabled (false);

		for (x = 0; x < imageSizeX; x++) {
			for (y = 0; y < imageSizeY; y++) {
				shapeImage->setPixel (x, y, qRgb (scale, scale, scale));
			}
		}
	}
	else if (circleRadioButton->isChecked()) {

		this->sizeValueSlider->setEnabled (true);
		this->sizeTextLabel->setEnabled (true);

		float radius2 = (float) this->sizeValueSlider->value() / (255.0f * 2.0f);
		radius2 *= radius2;

		for (x = 0; x < imageSizeX; x++) {
			for (y = 0; y < imageSizeY; y++) {				
				scaledX = (float) x / (float) imageSizeX - 0.5;
				scaledY = (float) y / (float) imageSizeY - 0.5;				
					
				if (scaledX * scaledX + scaledY * scaledY < radius2) {
					shapeImage->setPixel (x, y, qRgb(scale, scale, scale));
				}
				else {
					shapeImage->setPixel (x, y, qRgb(0, 0, 0));
				}
			}
		}
	}
	else if (gaussianRadioButton->isChecked()) {	

		this->sizeValueSlider->setEnabled (true);
		this->sizeTextLabel->setEnabled (true);

		float gaussian;
		float variance = 200.0f / ((float) this->sizeValueSlider->value() + 1);	
		variance *= variance;
		for (x = 0; x < imageSizeX; x++) {
			for (y = 0; y < imageSizeY; y++) {				
				scaledX = (float) x / (float) imageSizeX - 0.5;
				scaledY = (float) y / (float) imageSizeY - 0.5;				
				gaussian = (float) scale * exp (-variance * (scaledX * scaledX + scaledY * scaledY));
				shapeImage->setPixel ((int)x, (int)y, qRgb((int)gaussian, (int)gaussian, (int)gaussian));
			}
		}
	}
	QPixmap pixmap;
	pixmap.convertFromImage (*shapeImage);
	this->shapePixmapLabel->setPixmap (pixmap);
}




// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
