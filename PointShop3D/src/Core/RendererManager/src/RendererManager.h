// Title:   RendererManager.h
// Created: Thu Sep 25 14:28:21 2003
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

#ifndef __RENDERERMANAGER_H_
#define __RENDERERMANAGER_H_

#include <qstring.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qobject.h>
#include <qwidget.h>
#include <qsize.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../Interfaces/src/RendererInterface.h"
#include "../../src/CoreDLL.h"

/**
 * The <code>RendererManager</code> manages all available renderer plugins. It gives access
 * to the <em>main</em>, <em>preview</em> and the <em>auxiliary</em> renderer. Additionally this
 * <code>RendererManager</code> connects to the signals <code>sceneMoved</code> and <code>objectMoved</code> as
 * sent by the <code>Scene</code> and updates the <em>main</em> or <em>preview</em> renderer, whichever is
 * activated.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see Scene#sceneMoved
 * @see Scene#objectMoved
 */
class RendererManager : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates an instance of this <code>RendererManager</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>RendererManager</code>
	 */
	CORE_API static RendererManager *getInstance();

	/**
	 * Destroys the instance of this <code>RendererManager</code>, if there is any.
	 */
	CORE_API static void destroyInstance();

	/**
	 * Initializes this <code>RendererManager</code>: the <code>rendererPluginDirectory</code> ist scanned for
	 * valid renderer plugins. The default renderer must be in a subdirectory called <code>default</code>.
	 * This method must be called before any other renderer access methods.
	 * Another call re-initializes this <code>RendererManager</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>mainRendererChanged</code></li>
	 *   <li><code>auxiliaryRendererChanged</code></li>
	 *   <li><code>previewRendererChanged</code></li>
	 * </ul>
	 *
	 * @param rendererPluginDirectory
	 *        a <code>QString</code> specifying the relative or absolute path of the
	 *        renderer plugin directory; this directory is scanned for
	 *        valid renderer plugins - invalid renderer plugins are ignored, but may lead to a system
	 *        error message, depending on the platform; the default renderer is looked for
	 *        in a subdirectory of <code>rendererPluginDirectory</code> called <code>default</code>
	 * @param parent
	 *        a pointer to a <code>QWidget</code>, the parent of all the <code>QWidget</code>s of each renderer
	 * @return <code>true</code>, if at least one valid renderer plugin could be found; <code>false</code> else
	 * @see #mainRendererChanged
	 * @see #auxiliaryRendererChanged
	 * @see #previewRendererChanged
	 */
	CORE_API bool initialize (const QString rendererPluginDirectory, QWidget *parent);

	/**
	 * Returns the <em>currently displaying</em> renderer instance which is used for rendering the image which is
	 * visible to the user.
	 *
	 * @return a pointer to a <code>RendererInterface</code>
	 * @see #getMainRenderer
	 * @see #getAuxiliaryRenderer
	 * @see #getPreviewRenderer
	 */
	CORE_API RendererInterface *getCurrentRenderer();

	/**
	 * Returns the current <em>main</em> renderer instance which is used for rendering the image which is
	 * visible to the user.
	 *
	 * @return a pointer to a <code>RendererInterface</code>
	 * @see #getAuxiliaryRenderer
	 * @see #getPreviewRenderer
	 */
	CORE_API RendererInterface *getMainRenderer();

	/**
	 * Returns the current <em>auxiliary</em> renderer instance which can be used for rendering the
	 * <code>Scene</code> from a different perspective than the user perspective.
	 *
	 * @return a pointer to a <code>RendererInterface</code>
	 * @see #getMainRenderer
	 * @see #getPreviewRenderer
	 */
	CORE_API RendererInterface *getAuxiliaryRenderer();

	/**
	 * Returns the current <em>preview</em> renderer instance which can be used for rendering the
	 * <code>Scene</code> with a faster renderer.
	 *
	 * @return a pointer to a <code>RendererInterface</code>
	 * @see #getMainRenderer
	 * @see #getAuxiliaryRenderer
	 */
	CORE_API RendererInterface *getPreviewRenderer();

	/**
	 * Sets the type of the <em>main</em> renderer, that is which renderer
	 * should be used. The next call to <code>getMainRenderer</code>
	 * returns an instance of this <code>rendererType</code>.
	 * Available types can be queried with <code>getAvailableTypes</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>mainRendererChanged</code></li>
	 * </ul>
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the type of the <em>main</em> renderer
	 * @see #getMainRenderer
	 * @see #getAvailableRendererTypes
	 * @see #mainRendererChanged
	 */
	CORE_API void setMainRendererType (const QString newRendererType);

	/**
	 * Returns the <em>type</em> (not the instance) of the <em>main</em> renderer.
	 *
	 * @return a <code>QString</code> specifying the type of the currently selected <em>main</em> renderer
	 * @see #getMainRenderer
	 */
	CORE_API QString getMainRendererType() const;
	
	/**
	 * Sets the type of the <em>auxiliary</em> renderer, that is which renderer
	 * should be used. The next call to <code>getAuxiliaryRenderer</code>
	 * returns an instance of this <code>rendererType</code>.
	 * Available types can be queried with <code>getAvailableTypes</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>auxiliaryRendererChanged</code></li>
	 * </ul>
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the type of the <em>auxiliary</em> renderer
	 * @see #getAuxiliaryRenderer
	 * @see #getAvailableRendererTypes
	 * @see #auxiliaryRendererChanged
	 */
	CORE_API void setAuxiliaryRendererType (const QString newRendererType);

	/**
	 * Returns the <em>type</em> (not the instance) of the <em>auxiliary</em> renderer.
	 *
	 * @return a <code>QString</code> specifying the type of the currently selected <em>auxiliary</em> renderer
	 * @see #getAuxiliaryRenderer
	 */
	CORE_API QString getAuxiliaryRendererType() const;

	/**
	 * Sets the type of the <em>preview</em> renderer, that is which renderer
	 * should be used. The next call to <code>getPreviewRenderer</code>
	 * returns an instance of this <code>rendererType</code>.
	 * Available types can be queried with <code>getAvailableTypes</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>previewRendererChanged</code></li>
	 * </ul>
	 *
	 * @param newRendererType
	 *        a <code>QString</code> specifying the type of the <em>preview</em> renderer
	 * @see #getPreviewRenderer
	 * @see #getAvailableRendererTypes
	 * @see #previewRendererChanged
	 */
	CORE_API void setPreviewRendererType (const QString newRendererType);

	/**
	 * Returns the <em>type</em> (not the instance) of the <em>preview</em> renderer.
	 *
	 * @return a <code>QString</code> specifying the type of the currently selected <em>preview</em> renderer
	 * @see #getPreviewRenderer
	 */
	CORE_API QString getPreviewRendererType() const;

	/**
	 * Returns a list of available renderer types.
	 *
	 * @return a <code>QStringList</code> containing <code>QString</code>s specifying
	 *         the available renderer types 
	 */
	CORE_API QStringList getAvailableRendererTypes() const;

	/**
	 * Enables or disables the preview renderer. If <code>enable</code> is set to <code>true</code>
	 * the <em>preview</em> renderer is internally informed about changes in the <code>Scene</code>
	 * and its <code>renderFrame</code> is called, else the <em>main</em> renderer is updated.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>previewRendererEnabled</code></li>
	 * </ul>
	 *
	 * @param enable
	 *        set to <code>true</code> if the <em>preview</em> renderer should be updated on
	 *        <code>Scene</code> changes; set to <code>false</code> if the <em>main</em> renderer
	 *        should be updated on <code>Scene</code> changes.
	 * @param renderFrame
	 *        set to <code>true</code> if the image should also be re-rendered; <code>false</code> else
	 * @see #previewRendererEnabled	 
	 */
	CORE_API void setPreviewRendererEnabled (const bool enable, const bool renderFrame);

	/**
	 * Returns <code>true</code> if the <em>preview</em> renderer is updated on <code>Scene</code> changes,
	 * <code>false</code> if the <em>main</em> renderer is updated on <code>Scene</code> changes.
	 *
	 * @return <code>true</code> if the <em>preview</em> renderer is updated; <code>false</code> if the
	 *         <em>main</em> renderer is updated
	 */
	CORE_API bool isPreviewRendererEnabled() const;

	/**
	 * Returns the frames per second (fps) rate of the <em>main</em> renderer.
	 *
	 * @return the frames per second (fps) rate of the <em>main</em> renderer
	 */
	CORE_API float getMainRendererFPS() const;

	/**
	 * Returns the frames per second (fps) rate of the <em>preview</em> renderer.
	 *
	 * @return the frames per second (fps) rate of the <em>preview</em> renderer
	 */
	CORE_API float getPreviewRendererFPS() const;

signals:

	/**
	 * Emitted whenever the <em>main</em> renderer has been changed. The new renderer
	 * will be returned by the next call to <code>getMainRenderer</code>.
	 */
	void mainRendererChanged();

	/**
	 * Emitted whenever the <em>auxiliary</em> renderer has been changed. The new renderer
	 * will be returned by the next call to <code>getAuxiliaryRenderer</code>.
	 */
	void auxiliaryRendererChanged();

	/**
	 * Emitted whenever the <em>preview</em> renderer has been changed. The new renderer
	 * will be returned by the next call to <code>getPreviewRenderer</code>.
	 */
	void previewRendererChanged();

	/**
	 * Emitted whenever the <em>preview</em> renderer has been enabled (<code>isEnabled</code> is
	 * set to <code>true</code>) and does the rendering of the <code>Scene</code> or disabled
	 * (<code>isEnabled</code> is set to <code>false</code>), and the <em>main</em> renderer does
	 * all the rendering.
	 *
	 * @param isEnabled
	 *        set to <code>true</code> if the <em>preview</em> renderer has been enabled; <code>false</code>
	 *        if the <em>main</em> renderer has been enabled
	 */
	void previewRendererEnabled (const bool isEnabled);

	/**
	 * Emitted whenever the frames per second (FPS) rate of the <em>main</em> renderer has changed.
	 *
	 * @param mainRendererPFS
	 *        the frames per second (FPS) rate of the <em>main</em> renderer
	 */
	void mainRendererFPSChanged (const float mainRendererFPS);

	/**
	 * Emitted whenever the frames per second (FPS) rate of the <em>preview</em> renderer has changed.
	 *
	 * @param previewRendererPFS
	 *        the frames per second (FPS) rate of the <em>preview</em> renderer
	 */
	void previewRendererFPSChanged (const float previewRendererFPS);

private:

	double            start,
	                  end;

	QStringList       availableRendererTypes;
	QString           mainRendererType,
		              auxiliaryRendererType,
					  previewRendererType;
	RendererInterface *mainRenderer,
		              *auxiliaryRenderer,
					  *previewRenderer;
	bool              hasMainRendererTypeChanged,
		              hasAuxiliaryRendererTypeChanged,
					  hasPreviewRendererTypeChanged,
		              isPreviewRendererActive;
	QWidget           *parent;					// the parent widget of all the renderer widgets
	float             mainRendererFPS,
		              previewRendererFPS;
	std::vector<float> mainRendererTimings,        // contains the last n timings [in seconds]
		              previewRendererTimings;
	int               mainRendererTimingsIndex,   // an index into the timing arrays
		              previewRendererTimingsIndex;
	
	RendererManager();
protected:  // To suppress warnings...
	virtual ~RendererManager();
private:

	// initializes the timing arrays, allocates memory, sets all elements to 0.0, sets the indices to 0
	void initializeTimings();

	void updateMainRendererFPS();
	void updatePreviewRendererFPS();
	double now();

private slots:

	void handleRendererPluginDirectoryChanged (const QString newRendererPluginDirectory);
	void handleMainRendererTypeChanged (const QString newRendererType);
	void handleAuxiliaryRendererTypeChanged (const QString newRendererType);
	void handlePreviewRendererTypeChanged (const QString newRendererType);

	// connected to Scene, signal sceneMoved: sets the proper transformation matrix in the main renderer and
	// the scene transformer
	void handleSceneMoved (const float *transformationMatrix);

	// connected to Scene, signal objectMoved: sets the proper transformation matrix in the scene transformer
	void handleObjectMoved();

	// connected to Scene, signal renderFrame: re-renders the frame with the main renderer
	void handleRenderFrame();

	// connected to Configuration, signal viewPortSizeChanged: sets the proper viewport size in the main
	// renderer and re-renders the image
	void handleViewPortSizeChanged (const QSize newViewPortSize);

	// connected to Configuration, signal viewFrustumChanged: sets the proper view frustum in the main
	// renderer and re-renders the image
	void handleViewFrustumChanged (const MyDataTypes::ViewFrustum newViewFrustum);

	void handleLightDirectionChanged (const Vector3D newLightDirection);

	void handleBackgroundColorChanged (const QRgb newBackgroundColor);

	// connected to Configuration, signal previewRendererEnabled: enables/disables the preview renderer
	// according to 'isEnabled' and re-renders the image
	void handlePreviewRendererEnabled (const bool isEnabled);

	void handleMainShadingChanged (const bool isEnabled);
	void handleAuxiliaryShadingChanged (const bool isEnabled);
	void handlePreviewShadingChanged (const bool isEnabled);

	void handleMainWidgetRepainted();
	void handlePreviewWidgetRepainted();

};


#endif  // __RENDERERMANAGER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
