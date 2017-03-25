// Title:   ImageExportDialogImpl.cpp
// Created: Thu Sep 25 14:43:10 2003
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

#include <qlineedit.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qslider.h>
#include <qimage.h>
#include <qstringlist.h>
#include <qmessagebox.h>

#include "../Utilities/src/FileTools.h"
#include "../Core/Configuration/src/Configuration.h"
#include "../Core/RendererManager/src/RendererManager.h"
#include "ImageExportDialogImpl.h"

// ******
// global
// ******

// the single ImageExportDialogImpl instance, only
// accesible by getInstance() and
// destroyInstance()
static ImageExportDialogImpl *instance = 0;

// *******************
// private c'tor/d'tor
// *******************

ImageExportDialogImpl::ImageExportDialogImpl (QWidget* parent, const char* name, bool modal, WFlags fl)
    : ImageExportDialogBase( parent, name, modal, fl ) {

	this->initDialog();

}

ImageExportDialogImpl::~ImageExportDialogImpl() {
    // no need to delete child widgets, Qt does it all for us
}

// **************
// public methods
// **************

ImageExportDialogImpl *ImageExportDialogImpl::getInstance() {

	if (instance == 0) {
		instance = new ImageExportDialogImpl ();
	}
	return instance;

}

void ImageExportDialogImpl::destroyInstance() {

	if (instance != 0) {
		instance->close();
		delete instance;
	}
	instance = 0;

}

// ***************
// private methods
// ***************

void ImageExportDialogImpl::initDialog() {

	QStringList formatList;

	formatList = QImage::outputFormatList();

	fileFormatComboBox->insertStringList (formatList);
	

}

void ImageExportDialogImpl::applyDialogSettings() {

    QString       fileName;
	Configuration *configuration;

	fileName = fileNameLineEdit->text();
	
	QFile file (fileName);
	if (fileName.length() > 0) {

		if (file.exists() == true) {

			if (QMessageBox::warning (this, "File exists",
				                      QString ("The file\n%1\nalready exists! Do you want to overwrite it?").arg (fileName),
									  "Yes", "No", QString::null, 0, 1) == 1) {
				return;
			}			

		}

		configuration = Configuration::getInstance();

		if (defaultQualityCheckBox->isChecked() == true) {
			RendererManager::getInstance()->getMainRenderer()->getRenderedImage().save (fileName,
					                                                                        fileFormatComboBox->currentText(),
																						    -1);
		}
		else {

			RendererManager::getInstance()->getMainRenderer()->getRenderedImage().save (fileName,
					                                                                        fileFormatComboBox->currentText(),
																						    qualitySlider->value());

		}
	
	}	

}	

// ************
// public slots
// ************

void ImageExportDialogImpl::clickedOkay() {
	this->applyDialogSettings();
	this->accept();
}

void ImageExportDialogImpl::clickedApply() {
	this->applyDialogSettings();
}

void ImageExportDialogImpl::clickedHelp() {}

void ImageExportDialogImpl::clickedBrowseButton() {

	QString fileName;
	
	fileName = QFileDialog::getSaveFileName (fileNameLineEdit->text(), QString ("*." + fileFormatComboBox->currentText()).lower(), this );
    if (fileName.isEmpty() == false) {
        fileNameLineEdit->setText (FileTools::autoCompleteSuffix (fileName, QString (fileFormatComboBox->currentText()).lower()));
    }
	
}

void ImageExportDialogImpl::handleFileFormatChanged (const QString &newFileFormat) {

	QString fileName;

	fileName = fileNameLineEdit->text();

	if (fileName.length() > 0) {
		fileNameLineEdit->setText (FileTools::replaceSuffix (fileName, newFileFormat.lower()));
	}

}

void ImageExportDialogImpl::handleFileNameChanged() {

	QString fileName,
		    fileFormat;

	fileName = fileNameLineEdit->text();
	fileName = FileTools::autoCompleteSuffix (fileName, QString (fileFormatComboBox->currentText()).lower());
	fileNameLineEdit->setText (fileName);

}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
