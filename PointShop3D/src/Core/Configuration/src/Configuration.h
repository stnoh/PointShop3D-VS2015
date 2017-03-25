// Title:   Configuration.h
// Created: Thu Sep 25 14:31:28 2003
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

#ifndef __CONFIGURATION_H_
#define __CONFIGURATION_H_

#include <qobject.h>
#include <qsize.h>
#include <qstring.h>
#include <qstringlist.h>
#include <stdio.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../Interfaces/src/RendererInterface.h"
#include "../../src/CoreDLL.h"

/**
 * This class keeps track of all global settings such as renderer type and light position.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class CORE_API Configuration : public QObject {

	Q_OBJECT

public:

	/**
	 * All settings which may be stored/loaded from a file
	 */
	typedef struct fileSettings {

		char bitmapsDir[255], brushesDir[255], toolsDir[255], pluginsDir[255], dataDir[255], rendererPluginsDir[255];
		char mainRendererName[255], previewRendererName[255];
		int bMainShading, bPreviewShading;
		char aspectRatio[255], fieldOfView[255], nearPlane[255], farPlane[255];
		int bitmapWidth, bitmapHeight;
		char lightX[255], lightY[255], lightZ[255];
		int bShowBitmaps, bShowNames, bUseAlphaMasks, backgroundColor;
		int bAutoSave;

	} FileSettings;

public:

	/**
	 * Creates an instance of this <code>Configuration</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>Configuration</code>
	 */
	static Configuration *getInstance();

	/**
	 * Destroys the instance of this <code>Configuration</code>, if there is any.
	 */
	static void destroyInstance();

	/**
	 * Name of the file with the default settings
	 */
	static QString defaultFileName;

	/**
	 * reads the setting from a <code>file</code>
 	 * @param file
	 *		the settings file
	 * @param fileSettings
	 *		the file settings to return
	 * @return true if the file could be read successfully, false otherwise
	 */
	static bool readSettingsFromFile(FILE *file, FileSettings& fileSettings);

	/**
	 * writes the setting to a <code>file</code>
	 *
	 * @param file
	 *		the settings file
	 * @param fileSettings
	 *		the file settings to store (might change)
	 */
	static void writeSettingsToFile(FILE *file, FileSettings &fileSettings);
	
	// ********
	// settings
	// ********

	/**
	 * Sets the type of the <em>main</em> renderer to be used for rendering. Call <code>getAvailableRendererTypes</code>
	 * for a list of available renderers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>mainRendererTypeChanged</code></li>
	 * </ul>
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the type of the <em>main</em> renderer
	 * @see #getAvailableRendererTypes
	 * @see #mainRendererTypeChanged
	 */
	void setMainRendererType (const QString newRendererType);

	/**
	 * Returns the type of the <em>main</em> renderer.
	 *
	 * @return a <code>QString</code> specifying the type of the <em>main</em> renderer
	 */
	QString getMainRendererType() const;

	/**
	 * Sets the type of the <em>auxiliary</em> renderer to be used for rendering. Call <code>getAvailableRendererTypes</code>
	 * for a list of available renderers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>auxiliaryRendererTypeChanged</code></li>
	 * </ul>
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the type of the <em>auxiliary</em> renderer
	 * @see #getAvailableRendererTypes
	 * @see #auxiliaryRendererTypeChanged
	 */
	void setAuxiliaryRendererType (const QString newRendererType);

	/**
	 * Returns the type of the <em>auxiliary</em> renderer.
	 *
	 * @return a <code>QString</code> specifying the type of the <em>auxiliary</em> renderer
	 */
	QString getAuxiliaryRendererType() const;

	/**
	 * Sets the type of the <em>preview</em> renderer to be used for rendering. Call <code>getAvailableRendererTypes</code>
	 * for a list of available renderers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>previewRendererTypeChanged</code></li>
	 * </ul>
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the type of the <em>preview</em> renderer
	 * @see #getAvailableRendererTypes
	 * @see #previewRendererTypeChanged
	 */
	void setPreviewRendererType (const QString newRendererType);

	/**
	 * Returns the type of the <em>preview</em> renderer.
	 *
	 * @return a <code>QString</code> specifying the type of the <em>preview</em> renderer
	 */
	QString getPreviewRendererType() const;

	/**
	 * Sets the available renderer types.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>availableRendererTypesChanged</code></li>
	 * </ul>
	 *
	 * @param newAvailableRendererTypes
	 *        the <code>QStringList</code> specifying the newly available renderer types
	 * @see #availableRendererTypesChanged
	 */
	void setAvailableRendererTypes (const QStringList newAvailableRendererTypes);

	/**
	 * Returns the available renderer types.
	 *
	 * @return a <code>QStringList</code> containing the description of available renderer types
	 */
	QStringList getAvailableRendererTypes() const;

	/**
	 * Enables/disables the <em>preview</em> renderer according to <code>enable</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>previewRendererEnabled</code></li>
	 * </ul>
	 *
	 * @param enable
	 *        set to <code>true</code> if rendering should be done by the <em>preview</em> renderer;
	 *        false if rendering should be done by the <em>main</em> renderer
	 * @see #previewRendererEnabled
	 */
	void setPreviewRendererEnabled (const bool enable);

	/**
	 * Returns <code>true</code> if the <em>preview</em> render is enabled.
	 *
	 * @return <code>true</code> if the <em>preview</em> render is enabled; <code>false</code> if
	 *         the <em>main</em> renderer is enabled
	 */
	bool isPreviewRendererEnabled() const;

	/**
	 * Sets the viewport size in pixels.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>viewPortSizeChanged</code></li>
	 * </ul>
	 *
	 * @param newViewPortSize
	 *        the <code>QSize</code> which specifies the width and height of the
	 *        viewport size in pixels
	 * @see #viewPortSizeChanged
	 */
	void setViewPortSize (const QSize newViewPortSize);

	/**
	 * Returns the viewport size in pixels.
	 *
	 * @return the <code>QSize</code> which specifies the width and height of the
	 *        viewport size in pixels
	 */
	QSize getViewPortSize() const;

	/**
	 * Returns the minimum viewport size a renderer <code>QWidget</code> must have in pixels.
	 *
	 * @return the <code>QSize</code> which specifies the minimum width and height of the
	 *        viewport (renderer <code>QWidget</code>) size in pixels
	 * @see RendererInterface#getRendererWidget
	 */
	QSize getMinimumViewPortSize() const;

	/**
	 * Sets the <code>ViewFrustum</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>viewFrustumChanged</code></li>
	 * </ul>
	 *
	 * @param newViewFrustum
	 *        the new <code>ViewFrustum</code> to be used
	 * @see #viewFrustumChanged
	 */
	void setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum);

	/**
	 * Returns the <code>ViewFrustum</code>.
	 *
	 * @return the <code>ViewFrustum</code>
	 */
	MyDataTypes::ViewFrustum getViewFrustum() const;

	/**
	 * Enables/disables shading for the <em>main</em> renderer.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>mainShadingChanged</code></li>
	 * </ul>
	 *
	 * @param enable
	 *        set to <code>true</code> as to enable shading for the <em>main</em> renderer;
	 *        <code>false</code> else
	 * @see #mainShadingChanged
	 */
	void setMainShadingEnabled (const bool enable);

	/**
	 * Returns <code>true</code> if shading for the <em>main</em> renderer is enabled.
	 *
	 * @param <code>true</code> if shading is enabled for the <em>main</em> renderer; <code>false</code>
	 *        else
	 */
	bool isMainShadingEnabled() const;

	/**
	 * Enables/disables shading for the <em>auxiliary</em> renderer.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>auxiliaryShadingChanged</code></li>
	 * </ul>
	 *
	 * @param enable
	 *        set to <code>true</code> as to enable shading for the <em>auxiliary</em> renderer;
	 *        <code>false</code> else
	 * @see #auxiliaryShadingChanged
	 */
	void setAuxiliaryShadingEnabled (const bool enable);

	/**
	 * Returns <code>true</code> if shading for the <em>auxiliary</em> renderer is enabled.
	 *
	 * @param <code>true</code> if shading is enabled for the <em>auxiliary</em> renderer; <code>false</code>
	 *        else
	 */
	bool isAuxiliaryShadingEnabled() const;

	/**
	 * Enables/disables shading for the <em>preview</em> renderer.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>previewShadingChanged</code></li>
	 * </ul>
	 *
	 * @param enable
	 *        set to <code>true</code> as to enable shading for the <em>preview</em> renderer;
	 *        <code>false</code> else
	 * @see #previewShadingChanged
	 */
	void setPreviewShadingEnabled (const bool enable);

	/**
	 * Returns <code>true</code> if shading for the <em>preview</em> renderer is enabled.
	 *
	 * @param <code>true</code> if shading is enabled for the <em>preview</em> renderer; <code>false</code>
	 *        else
	 */
	bool isPreviewShadingEnabled() const;

	/**
	 * Sets the path to the plugin directory where the plugins are expected to be.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>pluginDirectoryChanged</code></li>
	 * </ul>
	 *
	 * @param newPluginDirectory
	 *        the <code>QString</code> specifying the relative or absolute path
	 *        to the plugin directory
	 * @see #pluginDirectoryChanged
	 */
	void setPluginDirectory (const QString newPluginDirectory);
	
	/**
	 * Returns the path to the plugin directory.
	 *
	 * @return the path to the plugin directory
	 */
	QString getPluginDirectory() const;

	/**
	 * Sets the path to the renderer plugin directory where the renderer plugins are expected to be.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>rendererPluginDirectoryChanged</code></li>
	 * </ul>
	 *
	 * @param newRendererPluginDirectory
	 *        the <code>QString</code> specifying the relative or absolute path
	 *        to the renderer plugin directory
	 * @see #rendererPluginDirectoryChanged
	 */
	void setRendererPluginDirectory (const QString newRendererPluginDirectory);

	/**
	 * Returns the path to the renderer plugin directory.
	 *
	 * @return the path to the renderer plugin directory
	 */
	QString getRendererPluginDirectory() const;

	/**
	 * Sets the path to the tool directory where the tools are expected to be.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>toolDirectoryChanged</code></li>
	 * </ul>
	 *
	 * @param newToolDirectory
	 *        the <code>QString</code> specifying the relative or absolute path
	 *        to the tool directory
	 * @see #toolDirectoryChanged
	 */
	void setToolDirectory (const QString newToolDirectory);
	
	/**
	 * Returns the path to the tool directory.
	 *
	 * @return the path to the tool directory
	 */
	QString getToolDirectory() const;

	/**
	 * Sets the path to the file format plugin directory where the file format plugins are expected to be.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fileFormatPluginDirectoryChanged</code></li>
	 * </ul>
	 *
	 * @param newFileFormatPluginDirectory
	 *        the <code>QString</code> specifying the relative or absolute path
	 *        to the file format plugin directory
	 * @see #fileFormatPluginDirectoryChanged
	 */
	void setFileFormatPluginDirectory (const QString newFileFormatPluginDirectory);

	/**
	 * Returns the path to the file format plugin directory.
	 *
	 * @return the path to the file format plugin directory
	 */
	QString getFileFormatPluginDirectory() const;

	/**
	 * Sets the path to the brush directory where the brushes are expected to be.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>brushDirectoryChanged</code></li>
	 * </ul>
	 *
	 * @param newBrushDirectory
	 *        the <code>QString</code> specifying the relative or absolute path
	 *        to the brush directory
	 * @see #brushDirectoryChanged
	 */
	void setBrushDirectory (const QString newBrushDirectory);

	/**
	 * Returns the path to the brush directory.
	 *
	 * @return the path to the brush directory
	 */
	QString getBrushDirectory() const;

	/**
	 * Sets the path to the bitmap directory where the bitmaps are expected to be.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>bitmapDirectoryChanged</code></li>
	 * </ul>
	 *
	 * @param newBitmapDirectory
	 *        the <code>QString</code> specifying the relative or absolute path
	 *        to the bitmap directory
	 * @see #bitmapDirectoryChanged
	 */
	void setBitmapDirectory (const QString newBitmapDirectory);	

	/**
	 * Returns the path to the bitmap directory.
	 *
	 * @return the path to the bitmap directory
	 */
	QString getBitmapDirectory() const;
	
	/**
	 * Sets the path to the data directory where the data (model) files are expected to be.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>dataDirectoryChanged</code></li>
	 * </ul>
	 *
	 * @param newDataDirectory
	 *        the <code>QString</code> specifying the relative or absolute path
	 *        to the data directory
	 * @see #dataDirectoryChanged
	 */
	void setDataDirectory (const QString newDataDirectory);
	
	/**
	 * Returns the path to the data directory.
	 *
	 * @return the path to the data directory
	 */
	QString getDataDirectory() const;

	/**
	 * Sets the initial camera position (the <em>home</em> position).
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>homePositionChanged</code></li>
	 * </ul>
	 *
	 * @param newHomePosition
	 *        a <code>CameraPosition</code> specifying the <em>home</em> position
	 * @see #homePositionChanged
	 */
	void setHomePosition (const MyDataTypes::CameraPosition newHomePosition);

	/**
	 * Returns the home position.
	 *
	 * @return a <code>CameraPosition</code> specifying the <em>home</em> position
	 */
	MyDataTypes::CameraPosition getHomePosition() const;

	/**
	 * Sets the light direction in a right-handed world coordinate system (like OpenGL).
	 *
	 * @param newLightDirection
	 *        a <code>Vector3D</code> specifying the direction of the light source in a right-handed
	 *        camera coordinate system
	 */
	void setLightDirection (const Vector3D newLightDirection);

	/**
	 * Returns the light direction in a right-handed camera coordinate system (like OpenGL).
	 *
	 * @return a <code>Vector3D</code> specifying the direction of the light source in a right-handed
	 *         camera coordinate system
	 */
	Vector3D getLightDirection() const;

	/**
	 * Sets the background color
	 *
	 * @param newBackgroundColor
	 *		the new background color of the renderer
	 */
	void setBackgroundColor(const QRgb newBackgroundColor);

	/**
	 * Gets the background color of the renderers
	 *
	 * @return the background color
	 */
	QRgb getBackgroundColor() const;

signals:

	/**
	 * Emitted whenever the type of the <em>main</em> renderer has been changed.
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the new type of the <em>main</em> renderer
	 */
	void mainRendererTypeChanged (const QString newRendererType);

	/**
	 * Emitted whenever the type of the <em>auxiliary</em> renderer has been changed.
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the new type of the <em>auxiliary</em> renderer
	 */
	void auxiliaryRendererTypeChanged (const QString newRendererType);

	/**
	 * Emitted whenever the type of the <em>preview</em> renderer has been changed.
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the new type of the <em>preview</em> renderer
	 */
	void previewRendererTypeChanged (const QString newRendererType);

	/**
	 * Emitted whenever the available renderer types have been changed.
	 *
	 * @param newAvailableRendererTypes
	 *        the <code>QStringList</code> specifying the new available renderer types
	 */
	void availableRendererTypesChanged (const QStringList newAvailableRendererTypes);

	/**
	 * Emitted whenever the <em>preview</em> renderer has been enabled/disabled.
	 *
	 * @param isEnabled
	 *        <code>true</code> if the <em>preview</em> renderer is enabled; <code>false</code> if
	 *        the <em>main</em> renderer is enabled
	 */
	void previewRendererEnabled (const bool isEnabled);

	/**
	 * Emitted whenever the size of the viewport area has been changed.
	 *
	 * @param newViewPortSize
	 *        the <code>QSize</code> specifying the new width and height of the
	 *        viewport in pixels
	 */
	void viewPortSizeChanged (const QSize newViewPortSize);

	/**
	 * Emitted whenever the <code>ViewFrustum</code> has changed.
	 *
	 * @param newViewFrustum
	 *        the new <code>ViewFrustum</code> which defines the volume to be rendered
	 * @see MyDataTypes#viewFrustum
	 */
	void viewFrustumChanged (const MyDataTypes::ViewFrustum newViewFrustum);

	/**
	 * Emitted whenever shading for the <em>main</em> renderer has been enabled/disabled.
	 * 
	 * @param isEnabled
	 *        <code>true</code> if shading has been enabled for the <em>main</em> renderer;
	 *        <code>false</code> else
	 */
	void mainShadingChanged (const bool isEnabled);

	/**
	 * Emitted whenever shading for the <em>auxiliary</em> renderer has been enabled/disabled.
	 * 
	 * @param isEnabled
	 *        <code>true</code> if shading has been enabled for the <em>auxiliary</em> renderer;
	 *        <code>false</code> else
	 */
	void auxiliaryShadingChanged (const bool isEnabled);

	/**
	 * Emitted whenever shading for the <em>preview</em> renderer has been enabled/disabled.
	 * 
	 * @param isEnabled
	 *        <code>true</code> if shading has been enabled for the <em>preview</em> renderer;
	 *        <code>false</code> else
	 */
	void previewShadingChanged (const bool isEnabled);

	/**
	 * Emitted whenever the path to the plugin directory has been changed.
	 *
	 * @param newPluginDirectory
	 *        the <code>QString</code> specifying the new path to the plugin directory
	 */
	void pluginDirectoryChanged (const QString newPluginDirectory);

	/**
	 * Emitted whenever the path to the renderer plugin directory has been changed.
	 *
	 * @param newRendererPluginDirectory
	 *        the <code>QString</code> specifying the new path to the renderer plugin directory
	 */
	void rendererPluginDirectoryChanged (const QString newRendererPluginDirectory);

	/**
	 * Emitted whenever the path to the tool directory has been changed.
	 *
	 * @param newToolDirectory
	 *        the <code>QString</code> specifying the new path to the tool directory
	 */
	void toolDirectoryChanged (const QString newToolDirectory);

	/**
	 * Emitted whenever the path to the file format plugin directory has been changed.
	 *
	 * @param newFileFormatPluginDirectory
	 *        the <code>QString</code> specifying the new path to the file format plugin directory
	 */
	void fileFormatPluginDirectoryChanged (const QString newFileFormatPluginDirectory);

	/**
	 * Emitted whenever the path to the brush directory has been changed.
	 *
	 * @param newBrushDirectory
	 *        the <code>QString</code> specifying the new path to the brush directory
	 */
	void brushDirectoryChanged (const QString newBrushDirectory);

	/**
	 * Emitted whenever the path to the bitmap directory has been changed.
	 *
	 * @param newBitmapDirectory
	 *        the <code>QString</code> specifying the new path to the bitmap directory
	 */
	void bitmapDirectoryChanged (const QString newBitmapDirectory);

	/**
	 * Emitted whenever the path to the data directory has been changed.
	 *
	 * @param newDataDirectory
	 *        the <code>QString</code> specifying the new path to the data directory
	 */
	void dataDirectoryChanged (const QString newDataDirectory);

	/**
	 * Emitted whenever the home position has been changed.
	 *
	 * @param newHomePosition
	 *        a <code>Vector3D</code> specifying the new home position
	 */
	void homePositionChanged (const MyDataTypes::CameraPosition newHomePosition);

	/**
	 * Emitted whenever the light direction has been changed.
	 *
	 * @param newLightDirection
	 *        a <code>Vector3D</code> specifying the new light direction
	 */
	void lightDirectionChanged (const Vector3D newLightDirection);

	/**
	 * Emitted whenever the background color has been changed
	 *
	 * @param newBackgroundColor
	 *        the new background color
	 */
	void backgroundColorChanged(const QRgb newBackgroundColor);

private:

	QSize                         viewPortSize,
		                          minimumViewPortSize;   // render area in pixels

	bool                          mainShadingEnabled,
		                          auxiliaryShadingEnabled,
								  previewShadingEnabled,
								  previewRendererActivated;

	QString                       mainRendererType,
		                          auxiliaryRendererType,
								  previewRendererType,
		                          pluginDirectory,
		                          rendererPluginDirectory,
	                              toolDirectory,
								  fileFormatPluginDirectory,
								  brushDirectory,
	                              bitmapDirectory,
	                              dataDirectory;

	QStringList                   availableRendererTypes;

	MyDataTypes::CameraPosition   homePosition;
	Vector3D                      lightDirection;
	MyDataTypes::ViewFrustum      viewFrustum;

	QRgb						  backgroundColor;

	// Singleton-pattern: c'tor/d'tor private
	Configuration();
protected:  // To suppress warnings...
	virtual ~Configuration();
};

#endif  // __CONFIGURATION_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
