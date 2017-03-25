// Title:   SurfelGPRendererConfiguration.cpp
// Created: Thu Sep 25 14:51:19 2003
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

#include <qcheckbox.h>
#include <qlineedit.h>

#include "../UI/SurfelGPRendererConfigurationWidget.h"
#include "SurfelGPRendererConfiguration.h"

SurfelGPRendererConfiguration::SurfelGPRendererConfiguration() {

	cutOffRadius        = 1.0f;
	twoSidedNormals     = false;
	shadows             = false;
	configurationWidget = 0;

}

SurfelGPRendererConfiguration::~SurfelGPRendererConfiguration() {}

void SurfelGPRendererConfiguration::setCutOffRadius (const float newCutOffRadius) {

	cutOffRadius = newCutOffRadius;
	// update the configuration widget, if available yet
	if (configurationWidget != 0) {
		configurationWidget->cutOffRadiusLineEdit->setText (QString::number (newCutOffRadius));
	}
	emit cutOffRadiusChanged (newCutOffRadius);

}

float SurfelGPRendererConfiguration::getCutOffRadius() const {
	return cutOffRadius;
}

void SurfelGPRendererConfiguration::setTwoSidedNormalsEnabled (const bool enable) {
	
	twoSidedNormals = enable;

	// update the configuration widget, if available yet
	if (configurationWidget != 0) {
		configurationWidget->twoSidedNormalsCheckBox->setChecked (twoSidedNormals);
	}
	emit twoSidedNormalsChanged (twoSidedNormals);

}

bool SurfelGPRendererConfiguration::isTwoSidedNormalsEnabled() const {
	return twoSidedNormals;
}


void SurfelGPRendererConfiguration::setShadowsEnabled (const bool enable) {
	
	shadows = enable;

	// update the configuration widget, if available yet
	if (configurationWidget != 0) {
		configurationWidget->shadowsCheckBox->setChecked (shadows);
	}
}

bool SurfelGPRendererConfiguration::isShadowsEnabled() const {
	return shadows;
}




QWidget *SurfelGPRendererConfiguration::createConfigurationWidget (QWidget *parent) {	
	
	configurationWidget = new SurfelGPRendererConfigurationWidget (parent);

	// initialize the configuration widget
	// NOTE: this is actually not the way to do it. here, we should get the renderer parameters
	// from the renderer itself!
	configurationWidget->cutOffRadiusLineEdit->setText (QString::number (cutOffRadius));
	configurationWidget->twoSidedNormalsCheckBox->setChecked(false);
	configurationWidget->shadowsCheckBox->setChecked(false);

	return configurationWidget;

}

void SurfelGPRendererConfiguration::apply() {

	float newCutOffRadius;
	bool  isNumber;

	newCutOffRadius = configurationWidget->cutOffRadiusLineEdit->text().toFloat (&isNumber);
	if (isNumber == false) {
		// reset value
		configurationWidget->cutOffRadiusLineEdit->setText (QString::number (cutOffRadius));
	}
	else if (newCutOffRadius != cutOffRadius) {
		cutOffRadius = newCutOffRadius;
		emit cutOffRadiusChanged (newCutOffRadius);
	}

	if (twoSidedNormals != configurationWidget->twoSidedNormalsCheckBox->isChecked()) {
		twoSidedNormals = configurationWidget->twoSidedNormalsCheckBox->isChecked();
		emit twoSidedNormalsChanged (twoSidedNormals);
	}

	if (shadows != configurationWidget->shadowsCheckBox->isChecked()) {
		shadows = configurationWidget->shadowsCheckBox->isChecked();
	}



	// triggers rendering of scene
	emit configurationApplied();

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
