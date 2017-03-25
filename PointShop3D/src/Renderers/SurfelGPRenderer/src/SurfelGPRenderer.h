// Title:   SurfelGPRenderer.h
// Created: Thu Sep 25 14:51:32 2003
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

#ifndef __SURFELGPRENDERER_H_
#define __SURFELGPRENDERER_H_

#include <qsize.h>
#include <qrect.h>
#include <qstring.h>
#include <qwidget.h>
#include <qevent.h>

#include "../../../Core/Scene/src/Scene.h"
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../Core/Interfaces/src/RendererConfigurationInterface.h"
#include "../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "SurfelPipeline_SurfSplat/Srf.h"
#include "SurfelGPRendererWidget.h"
#include "SurfelGPRendererConfiguration.h"

#define NIL		0
#define LEFT	1
#define	RIGHT	2
#define BOTTOM	4
#define TOP		8

/**
 * This class implements the <code>RendererInterface</code> and acts as a wrapper class for the
 * <em>SurfelGP Renderer</em> as written by Matthias Zwicker.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class SurfelGPRenderer : public RendererInterface {

	Q_OBJECT

public:

	SurfelGPRenderer();
	virtual ~SurfelGPRenderer();	

	// ***************************
	// interface RendererInterface
	// ***************************

	RENDERER_API bool isInteractive() const;
	RENDERER_API bool providesPointPicking() const;
	RENDERER_API float getDepth (unsigned int x, unsigned int y) const ;
	RENDERER_API Vector3D getCameraSpaceNormal (unsigned int x, unsigned int y) const;

	RENDERER_API void initialize (const bool isVisible, QWidget *parent);

	RENDERER_API void setVisible (const bool enable);
	RENDERER_API bool isVisible() const;

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
	RENDERER_API QRgb SurfelGPRenderer::getBackgroundColor() const;

	RENDERER_API void setTwoSidedNormalsEnabled (const bool enable);
	RENDERER_API bool isTwoSidedNormalsEnabled() const;
	RENDERER_API void setShadowsEnabled (const bool enable);
	RENDERER_API bool isShadowsEnabled() const;
	RENDERER_API void toggleSelectionVisualization();
	RENDERER_API void setSelectionVisualizationEnabled(const bool enable);
	RENDERER_API bool isSelectionVisualizationEnabled() const;
	RENDERER_API void setCutoffRadius (const float radius);
	RENDERER_API float getCutoffRadius() const;
	RENDERER_API void setBlendingThresholds (const float constThreshold, const float distThreshold, const float angleThreshold) ;
	RENDERER_API void getBlendingThresholds (float* constThreshold, float* distThreshold, float* angleThreshold);
	RENDERER_API void renderFrame (const int attributes = (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_OBJECTPOINTER), const bool allScene = true);

	RENDERER_API void addToFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels, const int attributes = FrameBufferInterface::PERPIXEL_C_Z_N_W, const bool updateRendererWidget = true);
	RENDERER_API void subtractFromFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels, const int attributes = FrameBufferInterface::PERPIXEL_C_Z_N_W, const bool updateRendererWidget = true);

	RENDERER_API FrameBufferInterface *getFrameBuffer() const;

	/**
	 * draws a line with z-buffer test
	 * The Bresenham algorithm is used for drawing. If a line point is visible, its color is set to the
	 * frame buffer but without changing the depth. So only the color changes, not the depth.
	 *
	 * @param start
	 *		start point
	 * @param end
	 *		end point
	 * @param color
	 *		line color
	 */
	RENDERER_API void drawLine (const Vector3D start, const Vector3D end, QRgb color);

	/**
	 * Cohen-Sutherland clipping algorithm
	 * 
	 * @param start
	 *			start point
	 * @param end
	 *			end point
	 * @param Ax
	 *			clipped x coordinate of start
	 * @param Ay
	 *			clipped y coordinate of start
	 * @param Bx
	 *			clipped x coordinate of end
	 * @param By
	 *			clipped y coordinate of end
	 * @return true if the line is inside the viewport, false otherwise
	 */
	RENDERER_API bool clipLine(Vector2D start, Vector2D end, int *Ax, int *Ay, int *Bx, int *By);

private:   

	int                                  iWidth, iHeight;
	int                                  superSampling;
	SrfContext                           *surfelContext;

	SrfHandle                            lightSourceHandle;
	Vector3D                             lightDirection;

	SurfelGPFrameBuffer                  *frameBuffer;

	float                                constThreshold,
	                                     distThreshold,
										 angleThreshold;

	bool								 shadingEnabled,
		                                 selectionVisualizationEnabled;

	SurfelGPRendererWidget               *rendererWidget;
	QWidget                              *parentWidget;          // the parent widget of the rendererWidget
	SurfelGPRendererConfiguration        rendererConfiguration;

	MyDataTypes::TransformationMatrix16f sceneViewMatrix;
	MyDataTypes::ViewFrustum             viewFrustum;

	QRgb								 backgroundColor;

	// instantiates the surfel context
	void instantiateSurfelContext();

	void renderShadowBuffer();

	// frees any used memory
	void cleanUp();

	// normalizes the 'lightDirection' and updates the light direction in the SurfelGPRenderer 
	// according to 'lightDirection' - does not re-render the image
	// PRE: the 'surfelContext' and the 'lightSourceHandle' must already be initialized
	// 
	// updateScene: true, if scene should be re-rendered
	void updateLightDirection (const bool updateScene);

	
	// help method for the lineClip method
	inline short SurfelGPRenderer::CompOutCode(float x, float y, float xmin, float xmax, float ymin, float ymax);

private slots:

	void handleObjectModified();
	void handleActiveObjectChanged();
	void handleObjectAdded();
	void handleObjectRemoved();
	void handleObjectMoved();

	// simply re-emits the signal 'widgetRepainted'
	void handleWidgetRepainted();

	void handleCutOffRadiusChanged (const float newCutOffRadius);

	void handleTwoSidedNormalsChanged (const bool enable);

	void handleConfigurationApplied();

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

#endif  // __SURFELGPRENDERER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
