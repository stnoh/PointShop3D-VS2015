// Title:   OpenGLRenderer.h
// Created: Thu Sep 25 15:45:04 2003
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

#ifndef _OPENGLRENDERER_H_
#define _OPENGLRENDERER_H_

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../Core/Interfaces/src/RendererConfigurationInterface.h"
#include "../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "OpenGLRendererWidget.h"
#include "OpenGLRendererConfiguration.h"

/**
 * This class implements the <code>RendererInterface</code> and acts as a wrapper class
 * for the OpenGL renderer in the <code>OpenGLRendererWidget</code>
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see OpenGLRendererWidget
 */
class OpenGLRenderer : public RendererInterface {

	Q_OBJECT

public:

	OpenGLRenderer();
	virtual ~OpenGLRenderer();

	// ***************************
	// interface RendererInterface
	// ***************************

	RENDERER_API void initialize (const bool isVisible, QWidget *parent);
	RENDERER_API void setVisible (const bool enable);
	RENDERER_API bool isVisible() const;
	RENDERER_API bool isInteractive() const;
	RENDERER_API bool providesPointPicking() const;
	RENDERER_API float getDepth (unsigned int x, unsigned int y) const ;
	RENDERER_API Vector3D getCameraSpaceNormal (unsigned int x, unsigned int y) const;

	RENDERER_API QWidget *getRendererWidget() const;
	RENDERER_API QImage getRenderedImage();
	RENDERER_API RendererConfigurationInterface *getRendererConfiguration();

	RENDERER_API void setSceneView (const MyDataTypes::TransformationMatrix16f newSceneViewMatrix);
	RENDERER_API void getSceneView (MyDataTypes::TransformationMatrix16f sceneViewMatrix) const;
	RENDERER_API void setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum);
	RENDERER_API MyDataTypes::ViewFrustum getViewFrustum() const;
	RENDERER_API void setViewPortSize (const QSize newViewPortSize);
	RENDERER_API QSize getViewPortSize() const;
	RENDERER_API void setShadingEnabled (const bool enable);
	RENDERER_API bool isShadingEnabled() const;
	RENDERER_API void setLightDirection (const Vector3D newLightDirection);
	RENDERER_API Vector3D getLightDirection() const;
	RENDERER_API void setBackgroundColor(QRgb newBackgroundColor);
	RENDERER_API QRgb getBackgroundColor() const;
	RENDERER_API void setTwoSidedNormalsEnabled(const bool enable);
	RENDERER_API bool isTwoSidedNormalsEnabled() const;
	RENDERER_API void setShadowsEnabled (const bool enable);
	RENDERER_API bool isShadowsEnabled() const;
	RENDERER_API void toggleSelectionVisualization();
	RENDERER_API void setSelectionVisualizationEnabled (const bool enable);
	RENDERER_API bool isSelectionVisualizationEnabled() const;
	RENDERER_API void setCutoffRadius (const float radius);
	RENDERER_API float getCutoffRadius() const;
	RENDERER_API void setBlendingThresholds (const float constThreshold, const float distThreshold, const float angleThreshold);
	RENDERER_API void getBlendingThresholds (float *constThreshold, float *distThreshold, float *angleThreshold) ;

	
	RENDERER_API void renderFrame (const int = (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_OBJECTPOINTER), const bool allScene = true);	
	RENDERER_API void addToFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels, const int attributes = FrameBufferInterface::PERPIXEL_C_Z_N_W, const bool updateRendererWidget = true);
	RENDERER_API void subtractFromFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels, const int attributes = FrameBufferInterface::PERPIXEL_C_Z_N_W , const bool updateRendererWidget = true);

	// *******************************************
	// not supported (not an interactive renderer)
	// *******************************************

	RENDERER_API FrameBufferInterface *getFrameBuffer() const;
	
private:

	OpenGLRendererWidget        *rendererWidget;
	OpenGLRendererConfiguration rendererConfiguration;

public:
    // interact mode stuff...
    virtual InteractStateEnum  getModelInteractState(void) const;
    virtual InteractStateEnum  getViewpointInteractState(void) const;
    virtual void               setInteractState(InteractStateEnum state,
												InteractStateAspectEnum target=INTERACT_MODEL_AND_VIEWPOINT);
  
private slots:

	// connected to the OpenGLRendererWidget signal repainted: emits the signal rendererWidgetRepainted
	void handleRepainted();

public:
	// **********************************************************
	// * Callback - Extension
	RENDERER_API void registerCallBack(void (*extCallBack)(void));	
	RENDERER_API void unregisterCallBack(void);

};

// ****************
// access functions
// ****************

// extern "C" is needed so those functions can be accessed
// with GetProcAddress() (WIN32) and dlsym() (UNIX) respective
extern "C" {
	RENDERER_API RendererInterface *createRenderer();
	RENDERER_API const char *getRendererType();
}

#endif  // _OPENGLRENDERER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
