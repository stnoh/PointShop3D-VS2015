// Title:   ColorChooserTool.cpp
// Created: Thu Sep 25 14:22:38 2003
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

#include <qtoolbar.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qpainter.h>
#include <qbitmap.h>
#include <qcolordialog.h>
#include <qwidget.h>

#include "../../../Interfaces/src/RendererInterface.h"
#include "../../../Configuration/src/Configuration.h"
#include "ColorChooserButton.h"

#include "ColorChooserTool.h"

// ******************
// 'What's this' text
// ******************

static const char *ColorChooserText = "<img source=\"chooser\"> "
"Click this button to choose a color:"
"<ul><li>Left button: choose foreground color</li>"
"<li>Right button: choose background color</li></ul>";

ColorChooserTool::ColorChooserTool() {

	toolCursor = new QCursor (ArrowCursor);

	// black
	fgColor = Qt::black.rgb();
	// white
	bgColor = Qt::white.rgb();

	pixmap = new QPixmap ("Resources/ColorChooserIcon.png");
	this->updatePixmap();

}

ColorChooserTool::~ColorChooserTool() {
	delete toolCursor;
}

// **************
// public methods
// **************

void ColorChooserTool::setFGColor (QRgb newFGColor) {
	
	fgColor = newFGColor;
	this->updatePixmap();
	colorChooserButton->setPixmap (*pixmap);
	emit fgColorChanged (newFGColor);

}

QRgb ColorChooserTool::getFGColor() {
	return fgColor;
}

void ColorChooserTool::setBGColor (QRgb newBGColor) {
	
	bgColor = newBGColor;
	this->updatePixmap();
	colorChooserButton->setPixmap (*pixmap);
	emit bgColorChanged (newBGColor);

}

QRgb ColorChooserTool::getBGColor() {
	return bgColor;
}

QToolButton *ColorChooserTool::addButtonToToolBar (QToolBar *toolBar) {

	colorChooserButton = new ColorChooserButton (*pixmap, "Color Chooser Tool", "Choose a color from a palette",
	                                             toolBar, "Color Chooser Tool");

	// connect the signals
	colorChooserButton->connect (colorChooserButton, SIGNAL (chooseFGColor()),
	                             this, SLOT (handleChooseFGColor()));
	colorChooserButton->connect (colorChooserButton, SIGNAL (chooseBGColor()),
	                             this, SLOT (handleChooseBGColor()));
	
	// add context sensitive help
	QWhatsThis::add (colorChooserButton, ColorChooserText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("chooser", *pixmap);

	return colorChooserButton;

}

void ColorChooserTool::addMenuToPushButton (QPushButton *pushButton) {
	(void)pushButton;
}

// ***************
// private methods
// ***************

void ColorChooserTool::updatePixmap() {

	QPainter painter (pixmap);

	painter.setPen (QPen (QColor ("black"), 1));

	// "blue box effect" -> gets transparent
	//painter.fillRect (0, 0, 16, 16, QColor ("blue"));

	// draw background color
	painter.fillRect (8, 8, 10, 10, QColor (bgColor));
	painter.drawRect (8, 8, 10, 10);

	// draw foreground color	
	painter.fillRect (4, 4, 10, 10, QColor (fgColor));
	painter.drawRect (4, 4, 10, 10);

}

// *************
// private slots
// *************

void ColorChooserTool::handleChooseFGColor() {
	
	QColor chosenColor;
	
	chosenColor = QColorDialog::getColor (fgColor, colorChooserButton);

	if (chosenColor.isValid() == true) {
		fgColor = chosenColor.rgb();
		this->updatePixmap();
		colorChooserButton->setPixmap (*pixmap);
		emit fgColorChanged (fgColor);
	}

	colorChooserButton->setDown (false);

}

void ColorChooserTool::handleChooseBGColor() {

	QColor chosenColor;
	
	chosenColor = QColorDialog::getColor (bgColor, colorChooserButton);

	if (chosenColor.isValid() == true) {
		bgColor = chosenColor.rgb();
		this->updatePixmap();
		colorChooserButton->setPixmap (*pixmap);
		emit bgColorChanged (bgColor);
	}

	colorChooserButton->setDown (false);

}






// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
