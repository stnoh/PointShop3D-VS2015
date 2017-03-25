// Title:   OpenGLRendererConfiguration.cpp
// Created: Thu Sep 25 15:45:02 2003
// Authors: Oliver Knoll, Mark Pauly, Tim Weyrich, Matthias Zwicker
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

#include <qcheckbox.h>
#include <qlineedit.h>

#include "../UI/OpenGLRendererConfigurationWidget.h"
#include "OpenGLRendererConfiguration.h"

OpenGLRendererConfiguration::OpenGLRendererConfiguration()
 : cutOffRadius(1.0f),
   constantPointSize(false),
   smoothPoint(true),
   useDisplayLists(true),
   useVertexShaders(true),
   configurationWidget(0)
{
	//setUseDisplayListsEnabled(true);
}

OpenGLRendererConfiguration::~OpenGLRendererConfiguration() 
{
}

void OpenGLRendererConfiguration::setCutOffRadius (const float newCutOffRadius) 
{
  cutOffRadius = newCutOffRadius;
  // update the configuration widget, if available yet
  if (configurationWidget != 0) {
      configurationWidget->cutOffRadiusLineEdit->setText (QString::number (newCutOffRadius));
  }
  emit cutOffRadiusChanged (newCutOffRadius);
}

float OpenGLRendererConfiguration::getCutOffRadius() const 
{
  return cutOffRadius;
}

void OpenGLRendererConfiguration::setConstantPointSizeEnabled (const bool enable) {

	constantPointSize = enable;
	// update the configuration widget, if available yet
	if (configurationWidget != 0) {
		configurationWidget->constantPointSizeCheckBox->setChecked (enable);
	}
	emit constantPointSizeChanged (enable);

}

bool OpenGLRendererConfiguration::isConstantPointSizeEnabled() const {
	return constantPointSize;
}

void OpenGLRendererConfiguration::setSmoothPointEnabled (const bool enable) {

	smoothPoint = enable;
	// update the configuration widget, if available yet
	if (configurationWidget != 0) {
		configurationWidget->smoothPointCheckBox->setChecked (enable);
	}
	emit smoothPointChanged (enable);

}

bool OpenGLRendererConfiguration::isSmoothPointEnabled() const {
	return smoothPoint;
}



void OpenGLRendererConfiguration::setUseDisplayListsEnabled (const bool enable) {

	useDisplayLists = enable;
	// update the configuration widget, if available yet
	if (configurationWidget != 0) {
		configurationWidget->useDisplayListsCheckBox->setChecked (enable);
	}
	emit useDisplayListsChanged (enable);

}

bool OpenGLRendererConfiguration::isUseDisplayListsEnabled() const {
	return useDisplayLists;
}

void OpenGLRendererConfiguration::setUseVertexShadersEnabled (const bool enable) {

	useVertexShaders = enable;
	// update the configuration widget, if available yet
	if (configurationWidget != 0) {
		configurationWidget->useVertexShadersCheckBox->setChecked (enable);
	}
	emit useVertexShadersChanged (enable);

}

bool OpenGLRendererConfiguration::isUseVertexShadersEnabled() const {
	return useVertexShaders;
}




QWidget *OpenGLRendererConfiguration::createConfigurationWidget (QWidget *parent) {	
	configurationWidget = new OpenGLRendererConfigurationWidget (parent);

	// initialize the configuration widget
	configurationWidget->cutOffRadiusLineEdit->setText (QString::number (cutOffRadius));
	configurationWidget->constantPointSizeCheckBox->setChecked (constantPointSize);
	configurationWidget->smoothPointCheckBox->setChecked (smoothPoint);
	configurationWidget->useDisplayListsCheckBox->setChecked (useDisplayLists);
	configurationWidget->useVertexShadersCheckBox->setChecked (useVertexShaders);

	return configurationWidget;
}

void OpenGLRendererConfiguration::apply() {

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

	if (constantPointSize != configurationWidget->constantPointSizeCheckBox->isChecked()) {
		constantPointSize = configurationWidget->constantPointSizeCheckBox->isChecked();
		emit constantPointSizeChanged (constantPointSize);
	}
	
	if (smoothPoint != configurationWidget->smoothPointCheckBox->isChecked()) {
		smoothPoint = configurationWidget->smoothPointCheckBox->isChecked();
		emit smoothPointChanged (smoothPoint);
	}

	if (useDisplayLists != configurationWidget->useDisplayListsCheckBox->isChecked()) {
		useDisplayLists = configurationWidget->useDisplayListsCheckBox->isChecked();
		emit useDisplayListsChanged (useDisplayLists);
	}

	if (useVertexShaders != configurationWidget->useVertexShadersCheckBox->isChecked()) {
		useVertexShaders = configurationWidget->useVertexShadersCheckBox->isChecked();
		emit useVertexShadersChanged (useVertexShaders);
	}



	emit configurationApplied();

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
