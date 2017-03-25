// Title:   CameraPositionMenu.cpp
// Created: Thu Sep 25 14:43:20 2003
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

#include "../Utilities/src/Matrix.h"
#include "../Core/Scene/src/Scene.h"

#include "CameraPositionMenu.h"

// *************
// public static
// *************

const int CameraPositionMenu::NOF_CAMERA_POSITIONS = 10;

// **************
// public methods
// **************

CameraPositionMenu::CameraPositionMenu (QWidget *parent, const char *name) : QPopupMenu (parent, name) {

	storeMenu   = new QPopupMenu (this);
	restoreMenu = new QPopupMenu (this);

	this->insertItem ("Store", storeMenu);
	this->insertItem ("Restore", restoreMenu);

	// add menu items
	storeMenu->insertItem ("Position 1", this, SLOT (handleStore1()), CTRL + Key_1, 10);
	storeMenu->insertItem ("Position 2", this, SLOT (handleStore2()), CTRL + Key_2, 11);
	storeMenu->insertItem ("Position 3", this, SLOT (handleStore3()), CTRL + Key_3, 12);
	storeMenu->insertItem ("Position 4", this, SLOT (handleStore4()), CTRL + Key_4, 13);
	storeMenu->insertItem ("Position 5", this, SLOT (handleStore5()), CTRL + Key_5, 14);
	storeMenu->insertItem ("Position 6", this, SLOT (handleStore6()), CTRL + Key_6, 15);
	storeMenu->insertItem ("Position 7", this, SLOT (handleStore7()), CTRL + Key_7, 16);
	storeMenu->insertItem ("Position 8", this, SLOT (handleStore8()), CTRL + Key_8, 17);
	storeMenu->insertItem ("Position 9", this, SLOT (handleStore9()), CTRL + Key_9, 18);
	storeMenu->insertItem ("Position 10", this, SLOT (handleStore10()), CTRL + Key_0, 19);
	
	restoreMenu->insertItem ("Position 1", this, SLOT (handleRestore1()), Key_1, 0);
	restoreMenu->insertItem ("Position 2", this, SLOT (handleRestore2()), Key_2, 1);
	restoreMenu->insertItem ("Position 3", this, SLOT (handleRestore3()), Key_3, 2);
	restoreMenu->insertItem ("Position 4", this, SLOT (handleRestore4()), Key_4, 3);
	restoreMenu->insertItem ("Position 5", this, SLOT (handleRestore5()), Key_5, 4);
	restoreMenu->insertItem ("Position 6", this, SLOT (handleRestore6()), Key_6, 5);
	restoreMenu->insertItem ("Position 7", this, SLOT (handleRestore7()), Key_7, 6);
	restoreMenu->insertItem ("Position 8", this, SLOT (handleRestore8()), Key_8, 7);
	restoreMenu->insertItem ("Position 9", this, SLOT (handleRestore9()), Key_9, 8);
	restoreMenu->insertItem ("Position 10", this, SLOT (handleRestore10()), Key_0, 9);

	setRestoreMenuItem(0);
	setRestoreMenuItem(1);
	setRestoreMenuItem(2);
	setRestoreMenuItem(3);
	setRestoreMenuItem(4);
	setRestoreMenuItem(5);
	setRestoreMenuItem(6);
	setRestoreMenuItem(7);
	setRestoreMenuItem(8);
	setRestoreMenuItem(9);
	
}

CameraPositionMenu::~CameraPositionMenu() {}

void CameraPositionMenu::setCameraPosition (const MyDataTypes::CameraPosition newCameraPosition, const int index) {
	
	QString fileName, num;	
	FILE *stream;
	
	if (index >= 0 && index < NOF_CAMERA_POSITIONS) {
		fileName = "camera_";
		num.setNum(index);
		fileName.append(num);

		stream = fopen(fileName, "w" );
		if (stream) {
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.rotationMatrix[0], newCameraPosition.rotationMatrix[1], newCameraPosition.rotationMatrix[2], newCameraPosition.rotationMatrix[3]);
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.rotationMatrix[4], newCameraPosition.rotationMatrix[5], newCameraPosition.rotationMatrix[6], newCameraPosition.rotationMatrix[7]);
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.rotationMatrix[8], newCameraPosition.rotationMatrix[9], newCameraPosition.rotationMatrix[10], newCameraPosition.rotationMatrix[11]);
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.rotationMatrix[12], newCameraPosition.rotationMatrix[13], newCameraPosition.rotationMatrix[14], newCameraPosition.rotationMatrix[15]);
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.scaleTranslationMatrix[0], newCameraPosition.scaleTranslationMatrix[1], newCameraPosition.scaleTranslationMatrix[2], newCameraPosition.scaleTranslationMatrix[3]);
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.scaleTranslationMatrix[4], newCameraPosition.scaleTranslationMatrix[5], newCameraPosition.scaleTranslationMatrix[6], newCameraPosition.scaleTranslationMatrix[7]);
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.scaleTranslationMatrix[8], newCameraPosition.scaleTranslationMatrix[9], newCameraPosition.scaleTranslationMatrix[10], newCameraPosition.scaleTranslationMatrix[11]);
			fprintf(stream, "%f;%f;%f;%f\n", newCameraPosition.scaleTranslationMatrix[12], newCameraPosition.scaleTranslationMatrix[13], newCameraPosition.scaleTranslationMatrix[14], newCameraPosition.scaleTranslationMatrix[15]);

			fclose(stream);
			
			restoreMenu->setItemEnabled (index, true);
		}
	}

}

MyDataTypes::CameraPosition CameraPositionMenu::getCameraPosition (const int index) const {

	MyDataTypes::CameraPosition undefinedPosition;
	QString fileName, num;
	FILE *stream;

	if (index >= 0 && index < NOF_CAMERA_POSITIONS) {
		fileName = "camera_";
		num.setNum(index);
		fileName.append(num);

		stream = fopen(fileName, "r" );
		if (stream) {
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.rotationMatrix[0], &undefinedPosition.rotationMatrix[1], &undefinedPosition.rotationMatrix[2], &undefinedPosition.rotationMatrix[3]);
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.rotationMatrix[4], &undefinedPosition.rotationMatrix[5], &undefinedPosition.rotationMatrix[6], &undefinedPosition.rotationMatrix[7]);
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.rotationMatrix[8], &undefinedPosition.rotationMatrix[9], &undefinedPosition.rotationMatrix[10], &undefinedPosition.rotationMatrix[11]);
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.rotationMatrix[12], &undefinedPosition.rotationMatrix[13], &undefinedPosition.rotationMatrix[14], &undefinedPosition.rotationMatrix[15]);
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.scaleTranslationMatrix[0], &undefinedPosition.scaleTranslationMatrix[1], &undefinedPosition.scaleTranslationMatrix[2], &undefinedPosition.scaleTranslationMatrix[3]);
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.scaleTranslationMatrix[4], &undefinedPosition.scaleTranslationMatrix[5], &undefinedPosition.scaleTranslationMatrix[6], &undefinedPosition.scaleTranslationMatrix[7]);
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.scaleTranslationMatrix[8], &undefinedPosition.scaleTranslationMatrix[9], &undefinedPosition.scaleTranslationMatrix[10], &undefinedPosition.scaleTranslationMatrix[11]);
			fscanf(stream, "%f;%f;%f;%f\n", &undefinedPosition.scaleTranslationMatrix[12], &undefinedPosition.scaleTranslationMatrix[13], &undefinedPosition.scaleTranslationMatrix[14], &undefinedPosition.scaleTranslationMatrix[15]);

			fclose(stream);
			
			return undefinedPosition;
		}
		
	}

	MtrUnity4x4f (undefinedPosition.rotationMatrix);
	MtrUnity4x4f (undefinedPosition.scaleTranslationMatrix);

	return undefinedPosition;

}

void CameraPositionMenu::restoreCameraPosition (const int index) {

	Scene::getInstance()->setCameraPosition (getCameraPosition(index));
}

// ***************
// private methods
// ***************

void CameraPositionMenu::setRestoreMenuItem(int index) {
	QString fileName, num;
	FILE *stream;

	fileName = "camera_";
	num.setNum(index);
	fileName.append(num);
	if (stream = fopen(fileName, "r")) {
		fclose(stream);
		restoreMenu->setItemEnabled (index, true);
	}
	else {
		restoreMenu->setItemEnabled (index, false);
	}
}

// *************
// private slots
// *************

void CameraPositionMenu::handleStore1() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 0);
}

void CameraPositionMenu::handleStore2() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 1);
}

void CameraPositionMenu::handleStore3() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 2);
}

void CameraPositionMenu::handleStore4() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 3);
}

void CameraPositionMenu::handleStore5() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 4);
}

void CameraPositionMenu::handleStore6() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 5);
}

void CameraPositionMenu::handleStore7() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 6);
}

void CameraPositionMenu::handleStore8() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 7);
}

void CameraPositionMenu::handleStore9() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 8);
}

void CameraPositionMenu::handleStore10() {
	this->setCameraPosition (Scene::getInstance()->getCameraPosition(), 9);
}

void CameraPositionMenu::handleRestore1() {
	this->restoreCameraPosition(0);
}

void CameraPositionMenu::handleRestore2() {
	this->restoreCameraPosition(1);
}

void CameraPositionMenu::handleRestore3() {
	this->restoreCameraPosition(2);
}

void CameraPositionMenu::handleRestore4() {
	this->restoreCameraPosition(3);
}

void CameraPositionMenu::handleRestore5() {
	this->restoreCameraPosition(4);
}

void CameraPositionMenu::handleRestore6() {
	this->restoreCameraPosition(5);
}

void CameraPositionMenu::handleRestore7() {
	this->restoreCameraPosition(6);
}

void CameraPositionMenu::handleRestore8() {
	this->restoreCameraPosition(7);
}

void CameraPositionMenu::handleRestore9() {
	this->restoreCameraPosition(8);
}

void CameraPositionMenu::handleRestore10() {
	this->restoreCameraPosition(9);
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
