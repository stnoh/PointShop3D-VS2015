// Title:   RendererInterface.h
// Created: Thu Sep 25 14:29:05 2003
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

#ifndef __RENDERERINTERFACE_H_
#define __RENDERERINTERFACE_H_

#include <qobject.h>
#include <qsize.h>
#include <qrect.h>
#include <qimage.h>

#include <set>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "FrameBufferInterface.h"
#include "RendererConfigurationInterface.h"

// ***********
// definitions
// ***********

#if defined _WIN32

	// The following ifdef block is the standard way of creating macros which make exporting
	// from a DLL simpler. All files within this DLL are compiled with the RENDERER_EXPORTS
	// symbol defined on the command line. this symbol should not be defined on any project
	// that uses this DLL. This way any other project whose source files include this file see
	// RENDERER_API  functions as being imported from a DLL, wheras this DLL sees symbols
	// defined with this macro as being exported.
	#ifdef RENDERER_EXPORTS
		#define RENDERER_API __declspec(dllexport)
	#else
		#define RENDERER_API __declspec(dllimport)
	#endif

#else

	// for all other platforms RENDERER_API is defined to be "nothing"
	#define RENDERER_API
	
#endif

#ifdef _WIN32
#  pragma warning(disable:4786)
#endif

/**
 * This interface defines the API for renderers.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see FrameBufferInterface
 */
class RENDERER_API RendererInterface : public QObject {

	Q_OBJECT

public:

	/**
	 * Initializes this <code>RendererInterface</code>. Must be called at least once before any other methods.
	 * When <code>isVisible</code> is set to <code>true</code> this <code>RendererInterface</code> provides
	 * a <code>QWidget</code> which displays the rendered image.
	 *
	 * @param isVisible
	 *        if set to <code>true</code> this <code>RendererInterface</code> is visible; a resize of its
	 *        <code>QWidget</code> will also update the <code>Configuration</code>
	 * @param parent
	 *        the parent <code>QWidget</code> of the renderer <code>QWidget</code>; is ignored
	 *        if <code>isVisible</code> is set to <code>false</code>
	 * @see Configuration#setViewPortSize
	 * @see #getRendererWidget	 
	 */
	virtual void initialize (const bool isVisible, QWidget *parent) = 0;

	/**
	 * Sets this <code>RendererInterface</code> in/visible.
	 *
	 * @param enable
	 *        set to <code>true</code> if the <code>QWidget</code> of this <code>RendererInterface</code>
	 *        is visible and on a resize event should update the <code>Configuration</code>; <code>false</code>
	 *        else
	 * @see Configuration#setViewPortSize
	 * @see #getRendererWidget	 
	 */
	virtual void setVisible (const bool enable) = 0;

	/**
	 * Returns <code>true</code> if this <code>RendererInterface</code> is visible.
	 *
	 * @return <code>true</code> if this <code>RendererInterface</code> is visible; <code>false</code> else
	 */
	virtual bool isVisible() const = 0;


	/**
	 * Returns <code>true</code> if this renderer allows user interactions, that is if it provides
	 * a <code>FrameBufferInterface</code>.
	 *
	 * @return <code>true</code> if this renderer allows user interactions; <code>false</code> else
	 * @see #getFrameBufferInterface
	 */
	virtual bool isInteractive() const = 0;


	/**
	 * Returns <code>true</code> if this renderer provides point picking, that is if it allows per pixel
	 * depth value and normal to be queried.
	 *
	 * @return <code>true</code> if this renderer provides point picking; <code>false</code> else
	 */
	virtual bool providesPointPicking() const = 0;

	/**
	 * Returns the depth value in camera space at a point x, y in screen space
	 *
	 * @param x
	 *		  x-value in screen space
	 * @param y
	 *		  y-value in screen space
	 * @return depth value
	 */
	virtual float getDepth (unsigned int x, unsigned int y) const = 0;

	/**
	 * returns the normal vector in camera space of a point with coordinates x, y in screen space
	 *
	 * @param x
	 *		  x-value in screen space
	 * @param y
	 *		  y-value in screen space
	 * @return normal vector
	 */
	virtual Vector3D getCameraSpaceNormal (unsigned int x, unsigned int y) const = 0;

	/**
	 * Returns a <code>QWidget</code> where the image is rendered into. The parent widget of this
	 * <code>QWidget</code> is given as parameter in the method <code>initialize</code>, but this
	 * renderer has to <code>delete</code> this <code>QWidget</code> as soon as the renderer itself
	 * is <code>delete</code>d.
	 * The returned widget must implement (overwrite) at least the following methods:
	 * <ul>
	 *   <li><code>paintEvent</code> - paint the rendered image</li>
	 *   <li><code>resizeEvent</code> - update the viewport size in the Configuration</li>
	 *   <li><code>mouseClickEvent</code>, <code>mouseMoveEvent</code>, <code>mouseReleaseEvent</code> - handle
	          the mouse events</li>
	 * </ul>
	 *
	 * @return a pointer to a <code>QWidget</code> where the image is rendered into
	 */
	virtual QWidget *getRendererWidget() const = 0;

	/**
	 * Returns the <code>RendererConfigurationInterface</code> which provides methods for configuring
	 * this <code>RendererInterface</code>.
	 *
	 * @return a pointer to the <code>RendererConfigurationInterface</code>
	 * @see RendererConfigurationInterface
	 */
	virtual RendererConfigurationInterface *getRendererConfiguration() = 0;

	/**
	 * Returns a <code>QImage</code> which contains the currently rendered image.
	 *
	 * <code>Note:</b> This method is not meant to be used for displaying or updating the rendered image, the
	 *                 <code>RendererInterface</code> updates its <code>QWidget</code> itself. This method
	 *                 is useful for screenshots though, since OpenGL renderers need to re-render the image
	 *                 into a special <code>QPixmap</code>.
	 * @return a <code>QImage</code> which contains the currently rendered image
	 * @see #getRendererWidget
	 */
	virtual QImage getRenderedImage() = 0;

	// ********************
	// rendering parameters
	// ********************

	/**
	 * Sets the final projection matrix of the <code>Scene</code> which will be used for the next rendering operation.
	 * This is normally the matrix as returned by <code>Scene::getTransformationMatrix</code>, but any transformation
	 * matrix can be used here.
	 * 
	 * @param newSceneViewMatrix
	 *        the <code>TransformationMatrix16f</code> which specifies the projection matrix of the
	 *        <code>Scene</code>
	 * @see Scene#getTransformationMatrix
	 * @see #renderFrame
	 */
	virtual void setSceneView (const MyDataTypes::TransformationMatrix16f newSceneViewMatrix) = 0;

	/**
	 * Returns the projection matrix of the <code>Scene</code> which will be used for the next rendering operation.
	 *
	 * @param sceneViewMatrix
	 *        a <code>TransformationMatrix16f</code> where the projection matrix of the <code>Scene</code>
	 *        will be stored into
	 */
	virtual void getSceneView (MyDataTypes::TransformationMatrix16f sceneViewMatrix) const = 0;

	/**
	 * Sets the <code>ViewFrustum</code> which will be used for the next rendering operation.
	 *
	 * @param newViewFrustum
	 *        the <code>ViewFrustum</code> which will be used for the next rendering operation
	 * @see #renderFrame
	 */
	virtual void setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum) = 0;

	/**
	 * Returns the <code>ViewFrustum</code> which will be used for the next rendering operation.
	 *
	 * @return the <code>ViewFrustum</code> which will be used for the next rendering operation
	 * @see #renderFrame
	 */
	virtual MyDataTypes::ViewFrustum getViewFrustum() const = 0;

	/**
	 * Sets the view port size in pixels.
	 *
	 * @param newViewPortSize
	 *        the new <code>QSize</code> of the view port [in pixels]
	 */
	virtual void setViewPortSize (const QSize newViewPortSize) = 0;

	/**
	 * Returns the view port size in pixels.
	 *
	 * @return the <code>QSize</code> of the view port [in pixels]
	 */
	virtual QSize getViewPortSize() const = 0;

	/**
	 * Enables/disables pixel shading.
	 *
	 * @param enable
	 *        set to <code>true</code> if shading should be enabled; <code>false</code> else
	 */
	virtual void setShadingEnabled (const bool enable) = 0;

	/**
	 * Returns <code>true</code> if shading is enabled.
	 *
	 * @return <code>true</code> if shading is enabled; <code>false</code> else
	 */
	virtual bool isShadingEnabled() const = 0;

	/**
	 * Sets the light direction.
	 *
	 * @param newLightDirection
	 *        a <code>Vector3D</code> which describes the light direction
	 */
	virtual void setLightDirection (const Vector3D newLightDirection) = 0;

	/**
	 * Returns the light direction.
	 *
	 * @return a <code>Vector3D</code> which describes the light direction
	 */
	virtual Vector3D getLightDirection() const = 0;
	
	/**
	 * Sets the background color
	 *
	 * @param newBackgroundColor
	 *		the background color
	 */
	virtual void setBackgroundColor(QRgb newBackgroundColor) = 0;

	/**
	 * Gets the background color
	 *
	 * @return the background color
	 */
	virtual QRgb getBackgroundColor() const = 0;

	/**
	 * Enables/disables two sided normals.
	 *
	 * @param enable
	 *        set to <code>true</code> if the image should be rendered with two sided normals;
	 *        <code>false</code> if one sided normals should be used
	 */
	virtual void setTwoSidedNormalsEnabled (const bool enable) = 0;

	/**
	 * Returns <code>true</code> if two sided normals are enabled.
	 *
	 * @return <code>true</code> if two sided normals are enabled; <code>false</code> if one sided normals are used
	 */
	virtual bool isTwoSidedNormalsEnabled() const = 0;

	/**
	 * Enables/disables shadows.
	 *
	 * @param enable
	 *        set to <code>true</code> if shadows should be enabled; <code>false</code> else
	 */
	virtual void setShadowsEnabled (const bool enable) = 0;

	/**
	 * Returns <code>true</code> if shadows are enabled.
	 *
	 * @return <code>true</code> if shadows are enabled; <code>false</code> else
	 */
	virtual bool isShadowsEnabled() const = 0;

	/**
	 * Toggles the visualisation of the selection.
	 */
	virtual void toggleSelectionVisualization() = 0;

	/**
	 * Enables/disables the visualisation of the selection.
	 *
	 * @param enable
	 *        set to <code>true</code> if the selection should be visualised; <code>false</code> else
	 */
	virtual void setSelectionVisualizationEnabled (const bool enable) = 0;

	/**
	 * Returns <code>true</code> if the selection is visualised.
	 *
	 * @return <code>true</code> if the selection is visualised; <code>false</code> else
	 */
	virtual bool isSelectionVisualizationEnabled() const = 0;

	/**
	 * Sets the cutoff radius for the splats.
	 *
	 * @param newCutOffRadius
	 *        the new cutoff radius for the splats
	 */
	virtual void setCutoffRadius (const float newCutOffRadius) = 0;

	/**
	 * Returns the cutoff radius for the splats.
	 *
	 * @return the new cutoff radius for the splats
	 */
	virtual float getCutoffRadius() const = 0;

	/**
	 * Sets the blending thresholds.
	 *
	 * @param constThreshold
	 *        the constant threshold
	 * @param distThreshold
	 *        the distance threshold
	 * @param angleThreshold
	 *        the angle threshold
	 */
	virtual void setBlendingThresholds (const float constThreshold, const float distThreshold, const float angleThreshold) = 0;

	/**
	 * Gets the blending thresholds.
	 *
	 * @param constThreshold
	 *        a pointer to a <code>float</code> where the the constant threshold will be stored
	 * @param distThreshold
	 *        a pointer to a <code>float</code> where the distance threshold will be stored
	 * @param angleThreshold
	 *        a pointer to a <code>float</code> where the angle threshold will be stored
	 */
	virtual void getBlendingThresholds (float *constThreshold, float *distThreshold, float *angleThreshold) = 0;

	
	// ********************
	// rendering operations
	// ********************

	/**
	 * Renders the frame with full detail (i.e. no progressive rendering). Optionally updates
	 * the <code>FrameBufferInterface</code> with additional data such as visible <code>Surfel</code>s.
	 * 
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>widgetRepainted</code></li>
	 * </ul>
	 *
	 * @param attributes
	 *        rendering attributes for framebuffer
	 * @param allScene
	 *        set to <code>true</code> if the whole <code>Scene</code> should be rendered; <code>false</code>
	 *        if only the <em>active</em> <code>Object</code> should be rendered
	 *
	 * @see FrameBufferInterface::Level
	 * @see Scene#getActiveObject
	 * @see #widgetRepainted
	 */
	virtual void renderFrame (const int attributes = (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_OBJECTPOINTER), const bool allScene = true) = 0;

	/**
	 * Adds the surfels in <code>std::vector<SurfelInterface*></code> to the current frame.
	 *
	 * <b>Note:</b> It is assumed that the surfels belong to the current active object. As a consequence,
	 *              the object transformation of the active object is applied.
	 */
	virtual void addToFrame (const std::vector<SurfelInterface *>* surfels, int nSurfels, const int attributes = FrameBufferInterface::PERPIXEL_C_Z_N_W, const bool updateRendererWidget = true) = 0;

	/**
	 * Subtracts the surfels in <code>std::vector<SurfelInterface*></code> from the current frame.
	 *
	 * <b>Note:</b> It is assumed that the surfels belong to the current active object. As a consequence,
	 * the object transformation of the active object is applied.
	 */
	virtual void subtractFromFrame (const std::vector<SurfelInterface *>* surfels, int nSurfels, const int attributes = FrameBufferInterface::PERPIXEL_C_Z_N_W , const bool updateRendererWidget = true) = 0;

	/**
	 * Draws a line from object space directly into the frame buffer
	 *
	 * @param start
	 *			start point of line
	 * @param end
	 *			end point of line
	 */
	virtual void drawLine (const Vector3D start, const Vector3D end, QRgb color) {
	}


	// *********************************************************
	// methods which are supported only by interactive renderers
	// *********************************************************

	// *******************
	// frame buffer access
	// *******************

	/**
	 * Returns the frame buffer which this
	 * <code>RendererInterface</code> is using.
	 *
	 * @return a pointer to the <code>FrameBufferInterface</code>
	 */ 
	virtual FrameBufferInterface *getFrameBuffer() const = 0;


	// ***********************************************************
	// methods controlling progressive behaviour and display lists
    // 
    // 	 All methods may affect rendering performance. Using the
    // 	 states INTERACT_DYNAMIC and INTERACT_HELD may lead to a
    // 	 reduce rendering quality...
	// ***********************************************************

    typedef enum {
	  INTERACT_STATIC, INTERACT_DYNAMIC, INTERACT_HELD
    } InteractStateEnum;
  
    typedef enum {
	  INTERACT_MODEL               = 0x1,
	  INTERACT_VIEWPOINT           = 0x2,
	  INTERACT_MODEL_AND_VIEWPOINT = 0x3      // equals (INTERACT_MODEL | INTERACT_VIEWPOINT)
    } InteractStateAspectEnum;
  
    virtual InteractStateEnum  getModelInteractState(void) const { return INTERACT_STATIC; }
    virtual InteractStateEnum  getViewpointInteractState(void) const { return INTERACT_STATIC; }
  
    virtual void  setInteractState(InteractStateEnum state,
								   InteractStateAspectEnum target=INTERACT_MODEL_AND_VIEWPOINT) { }

	// *****************************************************************
	// Callback - Extension 
	// 
	// For overwriting.
	// Can be used to hook-in the renderer. The renderer should call the 
	// function extCallBack after redrawing
	// *****************************************************************
private:
	static std::set<void (*)(void)>  *postRenderCBMap;

public:
	static void  registerPostRenderCB(void (*extCallBack)(void));
	static void  unregisterPostRenderCB(void (*extCallBack)(void));

	static void  callPostRenderCBs(void);
								 
	RendererInterface();
	virtual ~RendererInterface();

signals:

	/**
	 * Emitted whenever the <code>QWidget</code> of this <code>RendererInterface</code> has been repainted.
	 *
	 * @see #getRendererWidget
	 */
	void rendererWidgetRepainted();
};


#endif  // __RENDERERINTERFACE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
