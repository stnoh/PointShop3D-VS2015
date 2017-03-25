// Title:   ConfigurationDialogImpl.h
// Created: Thu Sep 25 14:43:12 2003
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

#ifndef __CONFIGURATIONDIALOG_H_
#define __CONFIGURATIONDIALOG_H_

#include <qstring.h>
#include <stdio.h>
#include "../DataTypes/src/MyDataTypes.h"
#include "../DataTypes/src/Vector3D.h"
#include "../UserInterface/UI/ConfigurationDialog.h"  // must be ../UserInterface/ for linux portability

/**
 * This class implements a configuration dialog which allows the user to
 * modify all application relevant settings such as directories or render
 * quality settings.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ConfigurationDialogImpl : public ConfigurationDialogBase {
	
    Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>ConfigurationDialogImpl</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>ConfigurationDialogImpl</code>
	 */
	static ConfigurationDialogImpl *getInstance();

	/**
	 * Destroys the instance of this <code>ConfigurationDialogImpl</code>, if there
	 * is one.
	 */
	static void destroyInstance();

public slots:

	/**
	 * Called whenever the <em>Apply</em> button was clicked.
	 */
	void clickedApply();

	/**
	 * Called whenever the <em>Okay</em> button was clicked.
	 */
	void clickedOkay();

	/**
	 * Called whenever the <em>Cancel</em> button was clicked.
	 */
	void clickedCancel();

	/**
	 * Called whenever the <em>Help</em> button was clicked.
	 */
	void clickedHelp();

	/**
	 * Called whenever the <em>Browse..</em> button for the bitmap directory was clicked.
	 */
	void clickedBitmapsBrowseButton();

	/**
	 * Called whenever the <em>Browse..</em> button for the brushes directory was clicked.
	 */
	void clickedBrushesBrowseButton();

	/**
	 * Called whenever the <em>Browse..</em> button for the tool directory was clicked.
	 */
	void clickedToolsBrowseButton();

	/**
	 * Called whenever the <em>Browse..</em> button for the plugin directory was clicked.
	 */
	void clickedPluginsBrowseButton();

	/**
	 * Called whenever the <em>Browse..</em> button for the data directory was clicked.
	 */
	void clickedDataBrowseButton();

	/**
	 * Called whenever the <em>Browse..</em> button for the renderer directory was clicked.
	 */
	void clickedRenderersBrowseButton();

	/**
	 * Called whenever the <em>Choose Background Color</em> tool button was clicked
	 */
	void clickedBackgroundColorToolButton();

	void handleLightDirectionXChanged();
	void handleLightDirectionXSliderChanged (int value);

	void handleLightDirectionYChanged();
	void handleLightDirectionYSliderChanged (int value);

	void handleLightDirectionZChanged();
	void handleLightDirectionZSliderChanged (int value);

	void clickedLoadDefaultSettings();
	void clickedLoadOriginalSettings();
	void clickedSaveDefaultSettings();

protected slots:

	void showEvent (QShowEvent *se);
	void hideEvent (QHideEvent *he);

private:

	
	Vector3D                       originalLightDirection;  // the light direction may be changed within
	                                                        // the dialog without clicking 'Okay' or 'Apply'
	                                                        // -> we need to change the original light direction,
	                                                        // so we can restore it when 'Cancel' is clicked

	QColor							rendererBackgroundColor;
	QColor							originalRendererBackgroundColor;

	// singleton pattern
	ConfigurationDialogImpl (QWidget *parent = 0, const char *name = 0, bool modal = false, WFlags flags = 0);
protected:  // To suppress warnings...
    ~ConfigurationDialogImpl();
private:
	// initializes the dialog elements
	void initDialog();

	// initializes the min/max/current values for the light position line edits / sliders
	void initLightSourceValues();

	// stores the current settings in the Configuration
	void applyDialogSettings();

	// to be called whenever x position line edit has been changed
	void updateLightDirectionXSlider();

	// to be called whenever y position line edit has been changed
	void updateLightDirectionYSlider();

	// to be called whenever z position line edit has been changed
	void updateLightDirectionZSlider();

	// returns the index of 'rendererType' in 'availableRendererTypes' - returns -1 if 'rendererType'
	// is not contained in 'availableRendererTypes'
	static int getRendererTypeIndex (const QString rendererType, const QStringList availableRendererTypes);


private slots:

	void handleAvailableRendererTypesChanged (const QStringList newAvailableRendererTypes);
	void handleViewFrustumChanged (const MyDataTypes::ViewFrustum newViewFrustum);
	
	// connected to the 'mainRendererTypes' QComboBox - applies the settings immediately
	// and updates this configuration dialog by adding the configuration QWidget provided by the main
	// renderer
	void handleMainRendererTypeChosen (const QString &rendererType);

	// connected to the 'auxiliaryRendererTypes' QComboBox - applies the settings immediately
	// and updates this configuration dialog by adding the configuration QWidget provided by the auxiliary
	// renderer
	void handleAuxiliaryRendererTypeChosen (const QString &rendererType);

	// connected to the 'previewRendererTypes' QComboBox - applies the settings immediately
	// and updates this configuration dialog by adding the configuration QWidget provided by the preview
	// renderer
	void handlePreviewRendererTypeChosen (const QString &rendererType);

	void handleLightDirectionChanged (const Vector3D newLightDirection);

};

inline void copyString(char *copy, const char *str) {
	unsigned int i;
	for (i = 0; str[i] != '\0'; i++) {
		copy[i] = str[i];
	}
	copy[i] = '\0';
}

#endif  // __CONFIGURATIONDIALOG_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
