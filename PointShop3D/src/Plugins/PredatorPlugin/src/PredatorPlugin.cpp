// Title:   PredatorPlugin.cpp
// Created: Thu Sep 25 14:45:40 2003
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
#include "../../../Utilities/UserMessages/src/StatusBar.h"
#include "../../../Core/Interfaces/src/SurfelInterface.h"
#include "PredatorPlugin.h"

// ******************
// 'What's this' text
// ******************

static const char *whatsThisText = "Converts the color of the selection or the whole <em>active</em> object "
                                   "to a heat map, like in the movie Predator.";

// **************
// public methods
// **************

PredatorPlugin::PredatorPlugin() {}
PredatorPlugin::~PredatorPlugin() {}

void PredatorPlugin::execute() {

	StatusBar                *statusBar;
	SurfelInterface          *surfel;
	SurfelCollection         *activeSurfelCollection;
	Scene                    *scene;
	uint                     i,
		                     nofSurfels;
	QRgb                     color;
	float                    Y709;
	QColor                   heatMap;
	std::vector<SurfelInterface*> *selection;
	SelectionTool            *selectionTool;

	activeSurfelCollection = Scene::getInstance()->getActiveObject()->getSurfelCollection();
	statusBar    = StatusBar::getInstance();
	scene        = Scene::getInstance();

	statusBar->resetProgress();
	statusBar->showMessage ("Converting to heat map...", 0);

	selectionTool = CoreTools::getInstance()->getSelectionTool();
	if (selectionTool->hasSelection() == false) {
		
		nofSurfels = activeSurfelCollection->getNofSurfels();
		for (i = 0, surfel = activeSurfelCollection->getFirstSurfel(); i < nofSurfels - 1; i++, surfel = activeSurfelCollection->getNextSurfel()) {
			
			color = surfel->getDiffuseColor();

			Y709   = (qRed  (color) / 255.0f * 0.2126f
			  	   + qGreen (color) / 255.0f * 0.7152f
				   + qBlue  (color) / 255.0f * 0.0722f);
			
			heatMap.setHsv ((int)(Y709 * 360.0f + 0.5f), 255, 255);
			scene->setSurfelDiffuseColor (surfel, heatMap.rgb(), false);
			
			// update the progress bar
			if (i % (nofSurfels / 100) == 0) {
				statusBar->setProgress ((int)((float)i / (float)nofSurfels * 100.0f), false);
			}

		}
		
		color = surfel->getDiffuseColor();

		Y709   = (qRed  (color) / 255.0f * 0.2126f
			   + qGreen (color) / 255.0f * 0.7152f
			   + qBlue  (color) / 255.0f * 0.0722f);
		
		heatMap.setHsv ((int)(Y709 * 360.0f + 0.5f), 255, 255);
		// for the last surfel we emit the signal
		scene->setSurfelDiffuseColor (surfel, heatMap.rgb(), true);

	}
	else {

		selection = selectionTool->getSelection();
		nofSurfels = selection->size();
		for (i = 0; i < nofSurfels - 1; i++) {
			
			surfel = selection->at(i);
			color = surfel->getDiffuseColor();

			Y709   = (qRed  (color) / 255.0f * 0.2126f
			  	   + qGreen (color) / 255.0f * 0.7152f
				   + qBlue  (color) / 255.0f * 0.0722f);
			
			heatMap.setHsv ((int)(Y709 * 360.0f + 0.5f), 255, 255);
			scene->setSurfelDiffuseColor (surfel, heatMap.rgb(), false);
			
			// update the progress bar
			if (i % (nofSurfels / 100) == 0) {
				statusBar->setProgress ((int)((float)i / (float)nofSurfels * 100.0f), false);
			}

		}
		
		surfel = selection->at(i);
		color = surfel->getDiffuseColor();

		Y709   = (qRed  (color) / 255.0f * 0.2126f
			   + qGreen (color) / 255.0f * 0.7152f
			   + qBlue  (color) / 255.0f * 0.0722f);
		
		heatMap.setHsv ((int)(Y709 * 360.0f + 0.5f), 255, 255);
		// for the last surfel we emit the signal
		scene->setSurfelDiffuseColor (surfel, heatMap.rgb(), true);

	}

	statusBar->setProgress (100, true);
	statusBar->showMessage ("", 1);
	
}

bool PredatorPlugin::isAvailable() const {
	return this->checkPluginAvailability();
}

// ***************
// private methods
// ***************

bool PredatorPlugin::checkPluginAvailability() const {
	return (Scene::getInstance()->getNofObjects() > 0);
}

// ****************
// access functions
// ****************

PLUGIN_API PluginInterface *createPlugin() {
	return new PredatorPlugin();
}

PLUGIN_API const char *getPluginName() {
	return "Predator";
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