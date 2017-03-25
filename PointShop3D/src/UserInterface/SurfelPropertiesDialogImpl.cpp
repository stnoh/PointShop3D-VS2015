// Title:   SurfelPropertiesDialogImpl.cpp
// Created: Thu Sep 25 14:42:53 2003
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
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgroupbox.h>

#include "../Core/Interfaces/src/SurfelInterface.h"
#include "../Core/Scene/src/Scene.h"
#include "../UserInterface/UI/SurfelPropertiesDialog.h"
#include "SurfelPropertiesDialogImpl.h"

// ******
// global
// ******

// the single SurfelPropertiesDialogImpl instance, only
// accesible by getInstance() and
// destroyInstance()
static SurfelPropertiesDialogImpl *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

SurfelPropertiesDialogImpl::SurfelPropertiesDialogImpl (QWidget *parent, const char *name, bool modal, WFlags flags)
    : SurfelPropertiesDialogBase (parent, name, modal, flags) {

	this->handleActiveObjectChanged();

	this->connect (Scene::getInstance(), SIGNAL (objectModified (bool)),
		           this, SLOT (handleObjectModified ()));

	this->connect (Scene::getInstance(), SIGNAL (objectAdded()),
		           this, SLOT (handleObjectAdded()));

	this->connect (Scene::getInstance(), SIGNAL (activeObjectChanged()),
					this, SLOT (handleActiveObjectChanged()));

}

SurfelPropertiesDialogImpl::~SurfelPropertiesDialogImpl() {
    // no need to delete child widgets, Qt does it all for us
}

// **************
// public methods
// **************

SurfelPropertiesDialogImpl *SurfelPropertiesDialogImpl::getInstance() {

	if (instance == 0) {
		instance = new SurfelPropertiesDialogImpl();
	}
	return instance;

}

void SurfelPropertiesDialogImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}

// ************
// public slots
// ************


void SurfelPropertiesDialogImpl::clickedApply() {
	this->applyDialogSettings();
}

void SurfelPropertiesDialogImpl::clickedHelp() {
	// TODO: implement this (HTML help)
}

// ***************
// private methods
// ***************

void SurfelPropertiesDialogImpl::initDialog() {

	Object* obj = Scene::getInstance()->getActiveObject();
	if (obj) {
		propertyDescriptor = obj->getSurfelCollection()->getPropertyDescriptor();
	}
	else {
		propertyDescriptor = SurfelInterface::NO_PROPERTIES;
	}

	// un-/check the checkboxes
	positionCheckBox->setChecked            ((propertyDescriptor & SurfelInterface::POSITION)             > 0);
	normalCheckBox->setChecked              ((propertyDescriptor & SurfelInterface::NORMAL)               > 0);
	tangentAxesCheckBox->setChecked         ((propertyDescriptor & SurfelInterface::TANGENT_AXES)         > 0);
	radiusCheckBox->setChecked              ((propertyDescriptor & SurfelInterface::RADIUS)               > 0);
	diffuseColorCheckBox->setChecked        ((propertyDescriptor & SurfelInterface::DIFFUSE_COLOR)        > 0);
	specularColorCheckBox->setChecked       ((propertyDescriptor & SurfelInterface::SPECULAR_COLOR)       > 0);
	flagsCheckBox->setChecked               ((propertyDescriptor & SurfelInterface::FLAGS)                > 0);
	ambientCoefficientCheckBox->setChecked  ((propertyDescriptor & SurfelInterface::AMBIENT_COEFFICIENT)  > 0);
	diffuseCoefficientCheckBox->setChecked  ((propertyDescriptor & SurfelInterface::DIFFUSE_COEFFICIENT)  > 0);
	specularCoefficientCheckBox->setChecked ((propertyDescriptor & SurfelInterface::SPECULAR_COEFFICIENT) > 0);
	shininessCheckBox->setChecked           ((propertyDescriptor & SurfelInterface::SHININESS)            > 0);
	textureCoordinateCheckBox->setChecked   ((propertyDescriptor & SurfelInterface::TEXTURE_COORDINATE)   > 0);
	associatedSurfelsCheckBox->setChecked	((propertyDescriptor & SurfelInterface::ASSOCIATED_SURFELS)   > 0);

}

void SurfelPropertiesDialogImpl::applyDialogSettings() {

	Object* obj = Scene::getInstance()->getActiveObject();
	if (obj) {
	
		propertyDescriptor = SurfelInterface::NO_PROPERTIES;

		if (positionCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::POSITION;
		}

		if (normalCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::NORMAL;
		}

		if (tangentAxesCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::TANGENT_AXES;
		}
		
		if (radiusCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::RADIUS;
		}

		if (diffuseColorCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::DIFFUSE_COLOR;
		}

		if (specularColorCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::SPECULAR_COLOR;
		}

		if (flagsCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::FLAGS;
		}

		if (ambientCoefficientCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::AMBIENT_COEFFICIENT;
		}

		if (diffuseCoefficientCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::DIFFUSE_COEFFICIENT;
		}

		if (specularCoefficientCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::SPECULAR_COEFFICIENT;
		}

		if (shininessCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::SHININESS;
		}

		if (textureCoordinateCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::TEXTURE_COORDINATE;
		}

		if (associatedSurfelsCheckBox->isChecked() == true) {
			propertyDescriptor |= SurfelInterface::ASSOCIATED_SURFELS;
		}

		obj->getSurfelCollection()->setPropertyDescriptor(propertyDescriptor);
	}

}

// *************
// private slots
// *************

void SurfelPropertiesDialogImpl::handleObjectModified () {

	Object* obj = Scene::getInstance()->getActiveObject();
	if (obj && obj->getSurfelCollection()->getPropertyDescriptor() != propertyDescriptor) {
		this->initDialog();
	}

}

void SurfelPropertiesDialogImpl::handleObjectAdded() {
	
	Object* obj = Scene::getInstance()->getActiveObject();
	if (obj && obj->getSurfelCollection()->getPropertyDescriptor() != propertyDescriptor) {
		this->initDialog();
	}

}

void SurfelPropertiesDialogImpl::handleActiveObjectChanged() {
	
	Object* obj = Scene::getInstance()->getActiveObject();
	if (obj) {
		activeObjectNameLabel->setText(obj->getName());
		if (obj->getSurfelCollection()->getPropertyDescriptor() != propertyDescriptor) {
			this->initDialog();
		}
		buttonApply->setEnabled(true);
		surfelPropertiesGroupBox->setEnabled(true);
	}
	else {
		activeObjectNameLabel->setText("No Active Object!");
		this->initDialog();
		buttonApply->setEnabled(false);
		surfelPropertiesGroupBox->setEnabled(false);
	}

}



// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
