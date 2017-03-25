// Title:   GreyscalePlugin.cpp
// Created: Thu Sep 25 14:44:47 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003, Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General
// Public License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330,
// Boston, MA 02111-1307 USA
// 
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//



#include <qobject.h>

#include "../../../Core/Scene/src/Scene.h"
#include "../../../Core/Scene/src/Object.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/CoreTools/src/CoreTools.h"
#include "../../../Core/CoreTools/SelectionTool/src/SelectionTool.h"
#include "../../../Core/Interfaces/src/SurfelInterface.h"
#include "../../../Utilities/UserMessages/src/StatusBar.h"
#include "GreyscalePlugin.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "Converts the color of the selection or the whole <em>active</em> object "
                                   "to greyscale.";

// **************
// public methods
// **************

GreyscalePlugin::GreyscalePlugin() {}
GreyscalePlugin::~GreyscalePlugin() {}

void GreyscalePlugin::execute() {

	StatusBar        *statusBar;
	SurfelInterface  *surfel;
	SurfelCollection *activeSurfelCollection;
	Scene            *scene;
	uint             i,
		             nofSurfels;
	QRgb             color,
		             greyscale;
	int              Y709;
	std::vector<SurfelInterface*> *selection;
	SelectionTool            *selectionTool;

	activeSurfelCollection = Scene::getInstance()->getActiveObject()->getSurfelCollection();
	statusBar    = StatusBar::getInstance();
	scene        = Scene::getInstance();

	statusBar->resetProgress();
	statusBar->showMessage ("Converting to greyscale...", 0);

	selectionTool = CoreTools::getInstance()->getSelectionTool();
	if (selectionTool->hasSelection() == false) {
		
		nofSurfels = activeSurfelCollection->getNofSurfels();
		for (i = 0, surfel = activeSurfelCollection->getFirstSurfel(); i < nofSurfels - 1; i++, surfel = activeSurfelCollection->getNextSurfel()) {
			
			color = surfel->getDiffuseColor();

			// coefficients from Poynton's Color FAQ:
			// http://www.inforamp.net/~poynton/PDFs/ColorFAQ.pdf
			Y709   = (int)(qRed  (color) * 0.2126f
						+ qGreen (color) * 0.7152f
						+ qBlue  (color) * 0.0722f + 0.5f);

			greyscale = qRgb (Y709, Y709, Y709);

			scene->setSurfelDiffuseColor (surfel, greyscale, false);
			
			// update the progress bar
			if (i % (nofSurfels / 100) == 0) {
				statusBar->setProgress ((int)((float)i / (float)nofSurfels * 100.0f), false);
			}

		}
		
		color = surfel->getDiffuseColor();

		// coefficients from Poynton's Color FAQ:
		// http://www.inforamp.net/~poynton/PDFs/ColorFAQ.pdf
		Y709   = (int)(qRed  (color) * 0.2126f
					+ qGreen (color) * 0.7152f
					+ qBlue  (color) * 0.0722f + 0.5f);

		greyscale = qRgb (Y709, Y709, Y709);

		// for the last surfel we emit the signal
		scene->setSurfelDiffuseColor (surfel, greyscale, true);

	}
	else {

		selection = selectionTool->getSelection();
		nofSurfels = selection->size();
		for (i = 0; i < nofSurfels - 1; i++) {
			
			surfel = selection->at(i);
			color = surfel->getDiffuseColor();

			// coefficients from Poynton's Color FAQ:
			// http://www.inforamp.net/~poynton/PDFs/ColorFAQ.pdf
			Y709   = (int)(qRed  (color) * 0.2126f
						+ qGreen (color) * 0.7152f
						+ qBlue  (color) * 0.0722f + 0.5f);

			greyscale = qRgb (Y709, Y709, Y709);
			scene->setSurfelDiffuseColor (surfel, greyscale, false);
			
			// update the progress bar
			if (i % (nofSurfels / 100) == 0) {
				statusBar->setProgress ((int)((float)i / (float)nofSurfels * 100.0f), false);
			}

		}
		
		surfel = selection->at(i);
		color = surfel->getDiffuseColor();

		// coefficients from Poynton's Color FAQ:
		// http://www.inforamp.net/~poynton/PDFs/ColorFAQ.pdf
		Y709   = (int)(qRed  (color) * 0.2126f
					+ qGreen (color) * 0.7152f
					+ qBlue  (color) * 0.0722f + 0.5f);

		greyscale = qRgb (Y709, Y709, Y709);
		// for the last surfel we emit the signal
		scene->setSurfelDiffuseColor (surfel, greyscale, true);

	}

	scene->emitObjectModifiedSignal(false);
	scene->emitRenderFrameSignal();

	statusBar->setProgress (100, true);
	statusBar->showMessage ("", 1);
	
}

bool GreyscalePlugin::isAvailable() const {
	return this->checkPluginAvailability();
}

// ***************
// private methods
// ***************

bool GreyscalePlugin::checkPluginAvailability() const {
	return (Scene::getInstance()->getNofObjects() > 0);
}

// ****************
// access functions
// ****************

PLUGIN_API PluginInterface *createPlugin() {
	return new GreyscalePlugin();
}

PLUGIN_API const char *getPluginName() {
	return "Greyscale";
}

PLUGIN_API const char *getPluginDescription() {
	return whatsThisText;
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
