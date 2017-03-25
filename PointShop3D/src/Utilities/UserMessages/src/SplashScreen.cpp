// Title:   SplashScreen.cpp
// Created: Thu Sep 25 15:03:57 2003
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

#include <qapplication.h>
#include <qlabel.h>
#include <qstring.h>
#include <qpainter.h>

#include "../../src/Version.h"
#include "SplashScreen.h"

// ******
// global
// ******

// the single SplashScreen instance, only
// accesible by getInstance() and
// destroyInstance()
static SplashScreen *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

SplashScreen::SplashScreen() {

	QRect rect;
	
	splash = new QLabel (0, "splash", Qt::WDestructiveClose | Qt::WStyle_Customize | Qt::WStyle_NoBorder);
	splash->setLineWidth(0);
	splash->setMidLineWidth(0);

	splashPixmap = QPixmap ("Resources/Splash.png");
	splash->setPixmap (splashPixmap);
			
	splash->adjustSize();
	splash->setCaption ("Pointshop3D");
	rect = QApplication::desktop()->geometry();
	splash->move (rect.center() - splash->rect().center());	

}

SplashScreen::~SplashScreen() {
	delete splash;
}

// **************
// public methods
// **************

SplashScreen *SplashScreen::getInstance() {

	if (instance == 0) {
		instance = new SplashScreen();
	}
	return instance;

}

void SplashScreen::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void SplashScreen::showText (const QString text) {

	QPainter painter;		

	painter.begin (splash);
	// clear previous text
	painter.setBackgroundColor (Qt::white);

	float fontSize = (float)painter.font().pointSize();

	painter.eraseRect ((int)(0.05f * (float)splashPixmap.width() + 0.5f),
		               (int)(0.75f  * (float)splashPixmap.height() + 0.5f) - fontSize*1.1f,
					   splashPixmap.width() - (int)(0.10f * (float)splashPixmap.width() + 0.5f),
					   (int)(2.0f * fontSize));
	painter.setPen (Qt::black);
	painter.drawText ((int)(0.05f * (float)splashPixmap.width() + 0.5f),
		              (int)(0.75f  * (float)splashPixmap.height() + 0.5f),
					  text);

	painter.end();
  

}

void SplashScreen::show() {

	QPainter painter;		

	splash->show();
	splash->repaint (false);
	QApplication::flushX();
	
	painter.begin (splash);
	painter.setPen (Qt::gray);

	float fontSize = (float)painter.font().pointSize();

	painter.drawText ((int)(0.83f   * (float)splashPixmap.width() + 0.5f),
		              (int)(0.04f * (float)splashPixmap.height() + 0.5f),
					  "Version " + Version::toString());	

	painter.drawText ((int)(0.025f * (float)splashPixmap.width() + 0.5f),
		              (int)(0.970f * (float)splashPixmap.height() + 0.5f),
					  Version::getVersionDate().toString() + "   \"" + Version::getCodeName() + "\"");

	painter.drawText ((int)(0.05f * (float)splashPixmap.width() + 0.5f),
		              (int)(0.8f * (float)splashPixmap.height() + 0.5f),
					  "Pointshop3D by Richard Keiser, Oliver Knoll, Mark Pauly, Tim Weyrich and Matthias Zwicker");
	painter.drawText ((int)(0.05f * (float)splashPixmap.width() + 0.5f),
		              (int)(0.8 * (float)splashPixmap.height() + 3.6f * fontSize + 0.5f),
					  "(c) CGL, ETH Zurich, 2001 - 2004");


#if defined _DEBUG
	painter.setPen (Qt::red);
	painter.drawText ((int)(0.9f * (float)splashPixmap.width() + 0.5f),
		              (int)(0.9f * (float)splashPixmap.height() + 0.5f),
					  "DEBUG");
#endif
	painter.end();	
	
}

void SplashScreen::hide() {
	splash->hide();
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
