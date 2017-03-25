// Title:   StatusBar.cpp
// Created: Thu Sep 25 15:03:53 2003
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

#include <qstatusbar.h>
#include <qprogressbar.h>
#include <qtimer.h>

#include "StatusBar.h"

static const uint PROGESS_BAR_TIMEOUT = 10000;  // 10 seconds

// ******
// global
// ******

// the single StatusBar instance, only
// accesible by getInstance() and
// destroyInstance()
static StatusBar *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

StatusBar::StatusBar() {

	statusBar          = 0;
	this->connect (&progressBarTimer, SIGNAL (timeout()),
		           this, SLOT (handleProgressBarTimer()));

}

StatusBar::~StatusBar() {}

// **************
// public methods
// **************

StatusBar *StatusBar::getInstance() {

	if (instance == 0) {
		instance = new StatusBar();
	}
	return instance;

}

void StatusBar::destroyInstance() {

	if (instance != 0) {
		delete instance;
	}
	instance = 0;

}

void StatusBar::showMessage (const QString text, const int milliSeconds) {
	
	if (statusBar != 0) {

		if (milliSeconds > 0) {
			statusBar->message (text, milliSeconds);
		}
		else {
			statusBar->message (text);
		}

	}	

}

/*
QProgressBar *StatusBar::getProgressBar() {
	return progressBar;
}
*/

void StatusBar::setProgress (const int progress, const bool autoReset) {

	progressBar->setProgress (progress);
	if (autoReset == true) {
		progressBarTimer.start (PROGESS_BAR_TIMEOUT, true);
	}
	else {
		progressBarTimer.stop();
	}

}

int StatusBar::getProgress() const {
	return progressBar->progress();
}

void StatusBar::resetProgress() {
	progressBar->setProgress(0);
}


// ***************
// private methods
// ***************

void StatusBar::setStatusBar (QStatusBar *newStatusBar) {

	statusBar   = newStatusBar;
	progressBar = new QProgressBar (statusBar);
	progressBar->setTotalSteps (100);
	progressBar->setMaximumWidth (100);
	statusBar->addWidget (progressBar, 0, true);

}

QStatusBar *StatusBar::getStatusBar() const {
	return statusBar;
}

// *************
// private slots
// *************

void StatusBar::handleProgressBarTimer() {
	progressBar->reset();
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
