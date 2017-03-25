// Title:   OpenGLRenderer.cpp
// Created: Thu Sep 25 15:45:06 2003
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

#include <qcolor.h>
#include <qevent.h>
#include <qimage.h>

#include <vector>
#include "../../../Core/Configuration/src/Configuration.h"
#include "../../../Utilities/src/Matrix.h"
#include "../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "OpenGLRendererConfiguration.h"
#include "OpenGLRenderer.h"

// **************
// public methods
// **************

OpenGLRenderer::OpenGLRenderer()
  : rendererWidget(0)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
  if (rendererWidget != 0) {
    delete rendererWidget;
  }
}

void OpenGLRenderer::initialize (const bool isVisible, QWidget *parent)
{
  // delete the old widget, if necessary
  if (rendererWidget != 0) {
    delete rendererWidget;
  }

  // we allocate the renderer widget in any case, since the OpenGL rendering is implemented there
  rendererWidget = new OpenGLRendererWidget (isVisible, &rendererConfiguration, parent);
  // get informed whenever the rendererWidget is repainted
  this->connect (rendererWidget, SIGNAL (repainted()),
		 this, SLOT (handleRepainted()));
}

void OpenGLRenderer::setVisible (const bool enable)
{
  rendererWidget->setVisible (enable);
}

bool OpenGLRenderer::isVisible() const
{
  return rendererWidget->isVisible();
}

bool OpenGLRenderer::isInteractive() const
{
  return false;
}


bool OpenGLRenderer::providesPointPicking () const
{
  return false;
}

float OpenGLRenderer::getDepth (unsigned int x, unsigned int y) const
{
  	// depth is taken from depth buffer
	GLint viewPort[4];
	glGetIntegerv(GL_VIEWPORT, viewPort);
	y = viewPort[3] - y - 1;
	GLfloat depth;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	return depth;
}


Vector3D OpenGLRenderer::getCameraSpaceNormal (unsigned int x, unsigned int y) const
{
  (void)x; (void)y;
  return Vector3D (0, 0, 1);
}



QWidget *OpenGLRenderer::getRendererWidget() const
{
  return rendererWidget;
}

RendererConfigurationInterface *OpenGLRenderer::getRendererConfiguration()
{
  return &rendererConfiguration;
}

QImage OpenGLRenderer::getRenderedImage()
{
  return rendererWidget->renderPixmap();
}

void OpenGLRenderer::setSceneView (const MyDataTypes::TransformationMatrix16f newSceneViewMatrix)
{
  rendererWidget->setSceneView (newSceneViewMatrix);
}

void OpenGLRenderer::getSceneView (MyDataTypes::TransformationMatrix16f sceneViewMatrix) const
{
  rendererWidget->getSceneView (sceneViewMatrix);
}

void OpenGLRenderer::setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum)
{
  rendererWidget->setViewFrustum (newViewFrustum);
}

MyDataTypes::ViewFrustum OpenGLRenderer::getViewFrustum() const
{
  return rendererWidget->getViewFrustum();
}

void OpenGLRenderer::setViewPortSize (const QSize newViewPortSize)
{
  rendererWidget->setViewPortSize (newViewPortSize);
}

QSize OpenGLRenderer::getViewPortSize() const
{
  return rendererWidget->getViewPortSize();
}

void OpenGLRenderer::setShadingEnabled (const bool enable) {
  rendererWidget->setShadingEnabled (enable);
}

bool OpenGLRenderer::isShadingEnabled() const
{
  return rendererWidget->isShadingEnabled();
}

void OpenGLRenderer::setLightDirection (const Vector3D newLightDirection)
{
  rendererWidget->setLightDirection (newLightDirection);
}

Vector3D OpenGLRenderer::getLightDirection() const
{
  return rendererWidget->getLightDirection();
}

void OpenGLRenderer::setBackgroundColor(QRgb newBackgroundColor) {
	rendererWidget->setBackgroundColor(newBackgroundColor);
}

QRgb OpenGLRenderer::getBackgroundColor() const {
	return rendererWidget->getBackgroundColor();
}

void OpenGLRenderer::setTwoSidedNormalsEnabled(const bool enable)
{
  (void)enable;
}

bool OpenGLRenderer::isTwoSidedNormalsEnabled() const
{
  return true;
}

void OpenGLRenderer::setShadowsEnabled(const bool enable)
{
  (void)enable;
}

bool OpenGLRenderer::isShadowsEnabled() const
{
  return false;
}


void OpenGLRenderer::toggleSelectionVisualization() {
	rendererWidget->setSelectionVisualizationEnabled(!rendererWidget->isSelectionVisualizationEnabled());
}

void OpenGLRenderer::setSelectionVisualizationEnabled (const bool enable)
{
  rendererWidget->setSelectionVisualizationEnabled(enable);
}

bool OpenGLRenderer::isSelectionVisualizationEnabled() const
{
  return rendererWidget->isSelectionVisualizationEnabled();
}

void OpenGLRenderer::setCutoffRadius (const float radius)
{
  rendererConfiguration.setCutOffRadius (radius);
}

float OpenGLRenderer::getCutoffRadius() const
{
  return rendererConfiguration.getCutOffRadius();
}

void OpenGLRenderer::setBlendingThresholds (const float constThreshold,
					    const float distThreshold,
					    const float angleThreshold)
{
  (void)constThreshold; (void)distThreshold; (void)angleThreshold;
}

void OpenGLRenderer::getBlendingThresholds (float *constThreshold, float *distThreshold, float *angleThreshold)
{
  // return default values
  *constThreshold = 1.0f;
  *distThreshold  = 1.0f;
  *angleThreshold = 1.0f;
}

void OpenGLRenderer::renderFrame (const int, const bool)
{
	rendererWidget->repaint();
}

void OpenGLRenderer::addToFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels,
				 const int attributes, const bool updateRendererWidget)
{
  (void)surfels; (void)nSurfels; (void)attributes; (void)updateRendererWidget;
}

void OpenGLRenderer::subtractFromFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels,
					const int attributes, const bool updateRendererWidget)
{
  (void)surfels; (void)nSurfels; (void)attributes; (void)updateRendererWidget;
}

FrameBufferInterface *OpenGLRenderer::getFrameBuffer() const
{
  // return dummy value (not supported)
  return 0;
}

// interaction stuff //////////////////////////////////////////////////////////

RendererInterface::InteractStateEnum  OpenGLRenderer::getModelInteractState(void) const
{
  return rendererWidget->getModelInteractState();
}

RendererInterface::InteractStateEnum  OpenGLRenderer::getViewpointInteractState(void) const
{
  return rendererWidget->getViewpointInteractState();
}

void  OpenGLRenderer::setInteractState(RendererInterface::InteractStateEnum state,
				                       RendererInterface::InteractStateAspectEnum target)
{
  rendererWidget->setInteractState(state, target);
}
  

///////////////////////////////////////////////////////////////////////////////
    
// *************
// private slots
// *************

void OpenGLRenderer::handleRepainted()
{
  emit rendererWidgetRepainted();
}

// ****************
// access functions
// ****************

RendererInterface *createRenderer()
{
  return new OpenGLRenderer();
}

const char *getRendererType()
{
  return "OpenGL (Preview)";
}

	
// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
