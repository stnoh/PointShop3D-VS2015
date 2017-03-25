// Title:   SurfelGPRenderer.cpp
// Created: Thu Sep 25 14:51:34 2003
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

#include <qsize.h>
#include <qrect.h>
#include <qdatetime.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>


#include "../../../Utilities/src/Matrix.h"
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../Core/Scene/src/Scene.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../Core/Configuration/src/Configuration.h"
#include "../../../Core/Transformer/src/Transformer.h"
#include "SurfelPipeline_SurfSplat/ZBuffer_SurfSplat.h"
#include "SurfelPipeline_SurfSplat/Srf.h"
#include "SurfelGPFrameBuffer.h"
#include "SurfelGPRendererWidget.h"
#include "SurfelGPRendererConfiguration.h"
#include "SurfelGPRenderer.h"


// *********
// constants
// *********


// **************
// public methods
// **************

SurfelGPRenderer::SurfelGPRenderer() {

	QSize         viewPortSize;
	Vector3D      lightDirection;
	Configuration *configuration;
	Scene         *scene;

	configuration = Configuration::getInstance();

	viewPortSize = configuration->getViewPortSize();

	// some default values for the view frustum
	viewFrustum.aspectRatio = 1.0f;
	viewFrustum.fieldOfView = 30.0f;
	viewFrustum.nearPlane   = 3.0f;
	viewFrustum.farPlane    = 100000.0f;
	
	superSampling     = 1;
	
	// no resources used yet
	rendererWidget    = 0;
	frameBuffer       = 0;	
	lightSourceHandle = 0;
	surfelContext     = 0;

	shadingEnabled = true;
	selectionVisualizationEnabled = true;

	constThreshold = 0.4f; 
    distThreshold  = 2.0f;
	angleThreshold = 8.0f;

	scene = Scene::getInstance();

	// get informed whenever an object changes
	this->connect (scene, SIGNAL (objectModified (bool)),
		           this, SLOT (handleObjectModified()));

	// get informed whenever the active object changes
	this->connect (scene, SIGNAL (activeObjectChanged()),
		           this, SLOT (handleActiveObjectChanged()));

	// get informed whenever the object count changes
	this->connect (scene, SIGNAL (objectAdded()),
		           this, SLOT (handleObjectAdded()));

	// get informed whenever the object count changes
	this->connect (scene, SIGNAL (objectRemoved()),
		           this, SLOT (handleObjectRemoved()));

	// get informed whenever an object moves
	this->connect (scene, SIGNAL (objectMoved()),
		           this, SLOT (handleObjectMoved()));

	// get informed whenever the splat cutoff radius in the SurfelGPRendererConfiguration has changed
	this->connect (&rendererConfiguration, SIGNAL (cutOffRadiusChanged (const float)),
		           this, SLOT (handleCutOffRadiusChanged (const float)));

	// get informed whenever the two-sided-normals setting has changed
	this->connect (&rendererConfiguration, SIGNAL (twoSidedNormalsChanged (const bool)),
		           this, SLOT (handleTwoSidedNormalsChanged (const bool)));

	// get informed whenever all the settings have been applied
	this->connect (&rendererConfiguration, SIGNAL (configurationApplied()),
		           this, SLOT (handleConfigurationApplied()));

}

SurfelGPRenderer::~SurfelGPRenderer() {
	this->cleanUp();
}

void SurfelGPRenderer::initialize (const bool isVisible, QWidget *parent) {

	QSize viewPortSize;
	Configuration *configuration;

	configuration = Configuration::getInstance();

	// delete the old renderer widget, if necessary
	this->cleanUp();

	MtrUnity4x4f (sceneViewMatrix);

	// first allocate the frame buffer...
	frameBuffer = new SurfelGPFrameBuffer (viewPortSize);

	// ... then the renderer widget which uses it
	if (isVisible == true) {
		this->parentWidget = parent;
		rendererWidget = new SurfelGPRendererWidget (frameBuffer, parent);
		// get informed whenever the renderer widget repaints its content
		this->connect (rendererWidget, SIGNAL (widgetRepainted()),
			           this, SLOT (handleWidgetRepainted()));
	}
	else {
		this->parentWidget = 0;
		rendererWidget = 0;
	}

	iWidth  = viewPortSize.width()  / superSampling;
	iHeight = viewPortSize.height() / superSampling;	

	// instantiate the surfel rendering context
	this->instantiateSurfelContext();

	frameBuffer->setValid (false);
	frameBuffer->clearImage (backgroundColor);
	
}

bool SurfelGPRenderer::isInteractive() const {
	return true;
}

bool SurfelGPRenderer::providesPointPicking () const {
	return true;
}

float SurfelGPRenderer::getDepth (unsigned int x, unsigned int y) const {
	return frameBuffer->getDepth (x, y);
}


Vector3D SurfelGPRenderer::getCameraSpaceNormal (unsigned int x, unsigned int y) const {
	return frameBuffer->getNormal (x, y);
}




void SurfelGPRenderer::setVisible (const bool enable) {

	if (enable == true && rendererWidget == 0) {
		rendererWidget = new SurfelGPRendererWidget (frameBuffer, parentWidget);
	}
	else if (enable == false && rendererWidget != 0) {
		delete rendererWidget;
		rendererWidget = 0;
	}

}

bool SurfelGPRenderer::isVisible() const {
	return rendererWidget != 0;
}

QWidget *SurfelGPRenderer::getRendererWidget() const {
	return rendererWidget;
}

RendererConfigurationInterface *SurfelGPRenderer::getRendererConfiguration() {
	return &rendererConfiguration;
}

QImage SurfelGPRenderer::getRenderedImage() {
	return *frameBuffer->getImage();
}

void SurfelGPRenderer::setSceneView (const MyDataTypes::TransformationMatrix16f newSceneViewMatrix) {

	frameBuffer->setValid (false);
	memcpy (sceneViewMatrix, newSceneViewMatrix, sizeof (MyDataTypes::TransformationMatrix16f));

}

void SurfelGPRenderer::getSceneView (MyDataTypes::TransformationMatrix16f sceneViewMatrix) const {
	memcpy (sceneViewMatrix, this->sceneViewMatrix, sizeof (MyDataTypes::TransformationMatrix16f));
}

void SurfelGPRenderer::setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum) {

	viewFrustum = newViewFrustum;

	// eventhough we are working with a left handed coordinate system (negative z values are visible),
	// the view frustum uses clipping planes with positive values. this is for compatibility with gluPerspective()!
	SrfSetFrustum (surfelContext, newViewFrustum.fieldOfView, newViewFrustum.aspectRatio, 
				   newViewFrustum.nearPlane, newViewFrustum.farPlane);
	qDebug ("SurfelGPRenderer: setViewFrustum: fov: %f, aspect: %f", newViewFrustum.fieldOfView, newViewFrustum.aspectRatio);
	frameBuffer->setValid (false);

}

MyDataTypes::ViewFrustum SurfelGPRenderer::getViewFrustum() const {
	return viewFrustum;
}

void SurfelGPRenderer::setViewPortSize (const QSize newViewPortSize) {
	
	frameBuffer->setSize (newViewPortSize);

	if (rendererWidget != 0) {
		rendererWidget->resize (newViewPortSize);
	}

	iWidth  = frameBuffer->getSize().width()  / superSampling;
	iHeight = frameBuffer->getSize().height() / superSampling;

	// free the old light
	SrfFreeLight (surfelContext, lightSourceHandle);

	// destroy the old surfel rendering context
	SrfFree (surfelContext);	

	// instantiate the new surfel rendering context
	this->instantiateSurfelContext();

	frameBuffer->setValid (false);
	
}

QSize SurfelGPRenderer::getViewPortSize() const {
	return rendererWidget->size();
}

void SurfelGPRenderer::setShadingEnabled (const bool enable) {

	shadingEnabled = enable;
	if (shadingEnabled == true) {
		SrfEnable (surfelContext, SRF_SHD_LIGHTING);
	}
	else {
		SrfDisable (surfelContext, SRF_SHD_LIGHTING);
	}
	
}

bool SurfelGPRenderer::isShadingEnabled() const {
	return shadingEnabled;
}

void SurfelGPRenderer::setLightDirection (const Vector3D newLightDirection) {
	lightDirection = newLightDirection;
	this->updateLightDirection (false);
}

Vector3D SurfelGPRenderer::getLightDirection() const {
	return lightDirection;
}

void SurfelGPRenderer::setBackgroundColor(QRgb newBackgroundColor) {
	backgroundColor = newBackgroundColor;
}

QRgb SurfelGPRenderer::getBackgroundColor() const {
	return backgroundColor;
}


FrameBufferInterface *SurfelGPRenderer::getFrameBuffer() const {
	return frameBuffer;
}

void SurfelGPRenderer::setTwoSidedNormalsEnabled (const bool enable) {

	rendererConfiguration.setTwoSidedNormalsEnabled (enable);
}

bool SurfelGPRenderer::isTwoSidedNormalsEnabled() const {
	return rendererConfiguration.isTwoSidedNormalsEnabled();
}


void SurfelGPRenderer::setShadowsEnabled (const bool enable) {

	rendererConfiguration.setTwoSidedNormalsEnabled (enable);
}

bool SurfelGPRenderer::isShadowsEnabled() const {
	return rendererConfiguration.isShadowsEnabled();
}



void SurfelGPRenderer::renderFrame (const int attributes, const bool allScene) {

	uint				                 i,
						                 nofObjects;
	SurfelCollection                 	 *surfelCollection;
	MyDataTypes::TransformationMatrix16f objectTransformation,
		                                 finalTransformation,
										 convertedTransformation;   // this is the row-major, right-handed
	                                                                // finalTransformation
	Scene                                *scene;
	Object                               *object;
	int					                 bbox[4];
	QPainter                             painter;
	bool                                 visualizeSelection;
	int                                  notActiveObjectAttributes;
	
	
	// PRE: the frame buffer either contains valid data or no data
	//      at all (and is therefore ready to be filled with new valid data)

	// check to see if we need to fill the framebuffer: we only want to re-render the image if:
	// - the level is set to NORMAL
	// - the level is > NORMAL and the framebuffer does not already contain valid extended data
	
	if ((attributes != (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_OBJECTPOINTER)) &&
		(attributes == frameBuffer->getAttributes()) &&
		(frameBuffer->isValid() == true)) {
		// the frame buffer is already updated, so
		// we do nothing
		return;
	}


	if (isShadowsEnabled()) {
     
		SrfEnableZBufferOption(surfelContext, SRF_SHADOWS);

		// render shadow buffer.
		// for all surfels that are visible the SurfelInterface::COVERED flag will be set
		// and cleared if tye surfel is not visible
		renderShadowBuffer();
	}
	else {
		SrfDisableZBufferOption(surfelContext, SRF_SHADOWS);
	}





	
	SrfSetProgressiveSize (surfelContext, frameBuffer->getSize().width(), frameBuffer->getSize().height());
	
	
	
	scene = Scene::getInstance();
	
	
	nofObjects = scene->getNofObjects();
	if (nofObjects == 0) {
		frameBuffer->clearImage (backgroundColor);
		if (rendererWidget != 0) {
			rendererWidget->repaint (false);
		}
		return;
	}
	
	
	
#if 0
	QTime timer;
	timer.start();
#endif
	
	SrfBeginFrame (surfelContext);
	
	
	notActiveObjectAttributes = attributes & (~FrameBufferInterface::PERPIXEL_SURFELLISTS);
	visualizeSelection = isSelectionVisualizationEnabled();
	
	for (object = scene->getFirstObject(), i = 0; object != 0; object = scene->getNextObject(), i++) {
		
		if (object->isFlagOn (Object::VISIBLE)) {
			if (allScene || object == scene->getActiveObject()) {
				
				// per pixel surfel lists only for active object
				// visualize selection only for active object
				
				if (object == scene->getActiveObject()) {
					frameBuffer->setAttributes (attributes);
					setSelectionVisualizationEnabled (visualizeSelection);
				}
				else {
					frameBuffer->setAttributes (notActiveObjectAttributes);
					setSelectionVisualizationEnabled (false);
				}
				
				surfelCollection = object->getSurfelCollection();
				
				// the renderer expects right-handed row major
				object->getTransformationMatrix (objectTransformation);
				MtrMult4x4f (sceneViewMatrix, objectTransformation, finalTransformation);
				
				// make it a row major matrix
				MtrTranspose4x4f (finalTransformation, convertedTransformation);
				
				// convert left-handed to right-handed (change sign of z-coordinates)
				convertedTransformation[8]  *= -1.0f;
				convertedTransformation[9]  *= -1.0f;
				convertedTransformation[10] *= -1.0f;
				convertedTransformation[11] *= -1.0f;
				
				SrfSetTrafo (surfelContext, convertedTransformation);
				
				if (surfelCollection->getPropertyDescriptor() & SurfelInterface::TANGENT_AXES) {
					SrfSetSplatFunction (surfelContext, SRF_ELLIPTICALSURFEL);
				}
				else {
					SrfSetSplatFunction (surfelContext, SRF_SURFACESPLAT);
				}
				
				// bbox returns a bounding box containing all pixels that were hit during warping
				SrfWarp (surfelContext, object, surfelCollection, bbox);
			}
		}
	}
	
	// retain selection visualization status
	setSelectionVisualizationEnabled (visualizeSelection);
	
	// always shade the whole framebuffer
	// NOTE: the bounding box includes the border
	bbox[0] = bbox[1] = 0;
	bbox[2] = iWidth - 1;
	bbox[3] = iHeight - 1;
	SrfShadeFrame (surfelContext, bbox, backgroundColor);
	
	
	if (rendererWidget != 0) {
		rendererWidget->repaint (false);
		qDebug ("SurfelGPRenderer: renderCurrentResolution: widget %d REPAINTED", (int)rendererWidget);
	}
				
	
	if (attributes != (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::PERPIXEL_OBJECTPOINTER)) {
		frameBuffer->setValid (true);
	}
	
}

void SurfelGPRenderer::addToFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels, const int attributes,
								   const bool updateRendererWidget) {
	
	// NOTE: it is assumed that the surfels being rendered belong to the active object. As a consequence,
	// the object transformation of the active object is applied.

	MyDataTypes::TransformationMatrix16f objectTransformation;
	MyDataTypes::TransformationMatrix16f finalTransformation;
	MyDataTypes::TransformationMatrix16f convertedTransformation;

	int bbox[4];

	frameBuffer->setAttributes (attributes);

	// indicate that the current frame is modified
	SrfContinueFrame(surfelContext);

	Object* activeObject = Scene::getInstance()->getActiveObject();
	
	activeObject->getTransformationMatrix (objectTransformation);
	MtrMult4x4f (sceneViewMatrix, objectTransformation, finalTransformation);
		
	// convert trafo matrix to row major, left handed for compatibility with surfel renderer
	// 1. make it a row major matrix
	MtrTranspose4x4f (finalTransformation, convertedTransformation);
	// 2. convert left-handed to right-handed (change sign of z-coordinates)
	convertedTransformation[8]  *= -1.0f;
	convertedTransformation[9]  *= -1.0f;
	convertedTransformation[10] *= -1.0f;
	convertedTransformation[11] *= -1.0f;

	SrfSetTrafo (surfelContext, convertedTransformation);
	
	// init bounding box
	bbox[0] = iWidth - 1;
	bbox[1] = iHeight - 1;
	bbox[2] = bbox[3] = 0;

	// bbox returns a bounding box containing all pixels that were hit during warping
	SrfWarp (surfelContext, activeObject, surfels, nSurfels, bbox);

	// shades the modified area (specified by bbox) in the frame buffer and copies
	// the data to the image that is displayed
	SrfShadeFrame (surfelContext, bbox, backgroundColor);

	if ((rendererWidget != 0) && (updateRendererWidget)) {
		rendererWidget->repaint (false);
		qDebug ("SurfelGPRenderer: renderCurrentResolution: widget %d REPAINTED", (int)rendererWidget);
	}

}

void SurfelGPRenderer::subtractFromFrame (const std::vector<SurfelInterface*>* surfels, int nSurfels,
										  const int attributes, const bool updateRendererWidget) {
	
	SrfSetSplatFunction(surfelContext, SRF_SURFACEUNSPLAT);

	addToFrame(surfels, nSurfels, attributes, updateRendererWidget);

	SrfSetSplatFunction(surfelContext, SRF_SURFACESPLAT);
}

void SurfelGPRenderer::setCutoffRadius (const float newRadius) {
	rendererConfiguration.setCutOffRadius (newRadius);
	//SrfSetCutoffRadius (surfelContext, newRadius);	
}

float SurfelGPRenderer::getCutoffRadius() const { 
	return SrfGetCutoffRadius (surfelContext);
}

void SurfelGPRenderer::setBlendingThresholds(const float constThreshold, const float distThreshold, const float angleThreshold) {
	
	this->constThreshold = constThreshold;
	this->distThreshold  = distThreshold;
	this->angleThreshold = angleThreshold;
	
	SrfSetBlendingThresholds(surfelContext, constThreshold, distThreshold, angleThreshold);
}

void SurfelGPRenderer::getBlendingThresholds (float *constThreshold, float *distThreshold, float *angleThreshold) {
	SrfGetBlendingThresholds(surfelContext, constThreshold, distThreshold, angleThreshold);
}

short SurfelGPRenderer::CompOutCode(float x, float y, float xmin, float xmax, float ymin, float ymax) {
	short outcode;

	outcode = NIL;	 
	if (y > ymax) outcode = TOP;
	else if (y < ymin) outcode = BOTTOM;
	if (x > xmax) outcode |= RIGHT;		
	else if (x < xmin) outcode |= LEFT;	 

	return outcode;
}

bool SurfelGPRenderer::clipLine(Vector2D start, Vector2D end, int *Ax, int *Ay, int *Bx, int *By) {
	QSize viewPortSize = Configuration::getInstance()->getViewPortSize();
	bool accept = false;
	bool done = false;
	float x,y;
	float slope = (end[1] - start[1]) / (end[0] - start[0]);
	float invSlope = 1/slope;
	short outcode0, outcode1;
	short outcodeOut;
	float x0 = start[0];
	float y0 = start[1];
	float x1 = end[0];
	float y1 = end[1];

	/* Clipping-rectangle coordinates */
	float xmin = 0;
	float ymin = 0;
	float xmax = viewPortSize.width()-1;
	float ymax = viewPortSize.height()-1;

	outcode0 = CompOutCode(start[0], start[1], xmin, xmax, ymin, ymax);
	outcode1 = CompOutCode(end[0], end[1], xmin, xmax, ymin, ymax);

	do {
		if (!(outcode0 | outcode1)) {	// trivial inside
			accept = true;
			done = true;
		}
		else if ((outcode0 & outcode1)) {	// trivial outside
			return false;
		}
		else {
			if (outcode0) 
				outcodeOut = outcode0;
			else
				outcodeOut = outcode1;
			// find intercept point: y = y0 + slope * (x-x0)
			//						 x = x0 + 1/slope * (y-y0)
			if (outcodeOut & TOP) {
				x = x0 + invSlope * (ymax - y0);
				y = ymax;
			}
			else if (outcodeOut & BOTTOM) {
				x = x0 + invSlope * (ymin - y0);
				y = ymin;
			}
			else if (outcodeOut & RIGHT) {
				y = y0 + slope * (xmax - x0);
				x = xmax;
			}
			else if (outcodeOut & LEFT) {
				y = y0 + slope * (xmin - x0);
				x = xmin;
			}

			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = CompOutCode(x0, y0, xmin, xmax, ymin, ymax);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = CompOutCode(x1, y1, xmin, xmax, ymin, ymax);
			}
		}
	} while (!done);

	*Ax = (int)x0;
	*Ay = (int)y0;
	*Bx = (int)x1;
	*By = (int)y1;

	return true;
}

void SurfelGPRenderer::drawLine (const Vector3D start, const Vector3D end, QRgb color) {
	float depth;
	int Ax, Ay, Bx, By;

	Transformer *transformer = Transformer::getSceneTransformer();
	// transform the start vector into camera coordinates
	Vector3D cameraCoordA = transformer->objectToCamera(start);
	Vector2D screenA = transformer->cameraToViewPort(cameraCoordA);
	// transform the end vector into camera coordinates
	Vector3D cameraCoordB = transformer->objectToCamera(end);
	Vector2D screenB = transformer->cameraToViewPort(cameraCoordB);

	if (!clipLine(screenA, screenB, &Ax, &Ay, &Bx, &By)) {
		return;
	}
	
	// z-component of line vector in camera coordinates
	float cameraLineVectorZ = cameraCoordB[2] - cameraCoordA[2];
	float screenLineVectorLength = (Vector2D(screenB[0] - screenA[0], screenB[1] - screenA[1])).getLength();

	//*** use bresenham algorithm to incrementally draw the line ***
	
	// initialize the components of the algorithm that are not affected by the
	// slope or direction of the line
	int dX = abs(Bx - Ax);	// store the change in X and Y of the line endpoints
	int dY = abs(By - Ay);
	
	// determine "directions" to increment x and y (regardless of decision)
	int Xincr, Yincr;
	if (Ax > Bx) { Xincr = -1; } else { Xincr = 1; }	// which direction in X?
	if (Ay > By) { Yincr = -1; } else { Yincr = 1; }	// which direction in Y?
	
	// determine independent variable (one that always increments by 1 (or -1) )
	// and initiate appropriate line drawing routine (based on first octant
	// always). the x and y's may be flipped if y is the independent variable.
	if (dX >= dY)	// if X is the independent variable
	{           
		int dPr 	= dY<<1;           // amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dX<<1);   // amount to increment decision if up is chosen
		int P 		= dPr - dX;		   // decision variable start value

		for (; dX>=0; dX--)            // process each point in the line one at a time (just use dX)
		{
			// do not interpolate the z-coordinate directly because of projection
			// the new line point is reprojected into camera coordinates to get the z-value of it
			depth = cameraCoordA[2] + ((Vector2D(Ax-screenA[0], Ay-screenA[1])).getLength() / screenLineVectorLength) * cameraLineVectorZ;
			if (depth >= frameBuffer->getDepth(Ax, Ay)) {
				frameBuffer->setColor(Ax, Ay, color); // plot the pixel
			}

			if (P > 0)               // is the pixel going right AND up?
			{ 
				Ax += Xincr;	       // increment independent variable
				Ay += Yincr;         // increment dependent variable
				P  += dPru;           // increment decision (for up)
			}
			else                    // is the pixel just going right?
			{
				Ax += Xincr;         // increment independent variable
				P  += dPr;            // increment decision (for right)
			}
		}		
	}
	else              // if Y is the independent variable
	{
		int dPr 	= dX<<1;           // amount to increment decision if right is chosen (always)
		int dPru 	= dPr - (dY<<1);   // amount to increment decision if up is chosen
		int P 		= dPr - dY;  // decision variable start value

		for (; dY>=0; dY--)            // process each point in the line one at a time (just use dY)
		{
			// do not interpolate the z-coordinate directly because of projection
			// the new line point is reprojected into camera coordinates to get the z-value of it
			depth = cameraCoordA[2] + ((Vector2D(Ax-screenA[0], Ay-screenA[1])).getLength() / screenLineVectorLength) * cameraLineVectorZ;
			if (depth >= frameBuffer->getDepth(Ax, Ay)) {
				frameBuffer->setColor(Ax, Ay, color); // plot the pixel
			}

			if (P > 0)               // is the pixel going up AND right?
			{ 
				Ax += Xincr;         // increment dependent variable
				Ay += Yincr;         // increment independent variable
				P  += dPru;           // increment decision (for up)
			}
			else                     // is the pixel just going up?
			{
				Ay += Yincr;         // increment independent variable
				P  += dPr;            // increment decision (for right)
			}
		}		
	}		

}

// ***************
// private methods
// ***************

void SurfelGPRenderer::instantiateSurfelContext() {

	Configuration *configuration = Configuration::getInstance();

	// instantiate the surfel rendering context
	surfelContext = SrfNew (frameBuffer, superSampling);
	frameBuffer->setSurfelContext(surfelContext);

	// light sources
	lightSourceHandle = SrfNewLight (surfelContext);
	SrfEnableLight (surfelContext, lightSourceHandle);
	lightDirection = configuration->getLightDirection();
	this->updateLightDirection (false);

	// enable/disable two-sided normals
	this->setTwoSidedNormalsEnabled (rendererConfiguration.isTwoSidedNormalsEnabled());

	// enable/disable selection visualization
	this->setSelectionVisualizationEnabled(selectionVisualizationEnabled);

	// enable/disable lighting
	this->setShadingEnabled (shadingEnabled);

	this->setCutoffRadius (rendererConfiguration.getCutOffRadius());
	this->setBlendingThresholds (constThreshold, distThreshold, angleThreshold);

	// set the view frustum as well
	// NOTE: eventhough we are working with a left handed coordinate system (negative z values are visible),
	// the view frustum uses clipping planes with positive values. this is for compatibility with gluPerspective()!
	SrfSetFrustum (surfelContext, viewFrustum.fieldOfView, viewFrustum.aspectRatio, 
                   viewFrustum.nearPlane, viewFrustum.farPlane);

}


	

void SurfelGPRenderer::renderShadowBuffer () {
		
	uint				                 i,
						                 nofObjects;
	SurfelCollection                 	 *surfelCollection;
	MyDataTypes::TransformationMatrix16f objectTransformation,
		                                 lightViewMatrix,
		                                 restoreSceneViewMatrix,
		                                 finalTransformation,
										 convertedTransformation;   // this is the row-major, right-handed
	                                                                // finalTransformation
	Scene                                *scene;
	Object                               *object;
	int					                 bbox[4];
	QPainter                             painter;
	SurfelInterface                      *surfel;

	SrfSetProgressiveSize (surfelContext, frameBuffer->getSize().width(), frameBuffer->getSize().height());
	
	scene = Scene::getInstance();
	
	nofObjects = scene->getNofObjects();
	if (nofObjects == 0) {
		return;
	}
	
	SrfBeginFrame (surfelContext);
	
	frameBuffer->setAttributes (FrameBufferInterface::PERPIXEL_SURFELLISTS);
	
	MtrCopy4x4f (sceneViewMatrix, restoreSceneViewMatrix);

	MtrUnity4x4f (lightViewMatrix);

	setSceneView (lightViewMatrix);

	for (object = scene->getFirstObject(), i = 0; object != 0; object = scene->getNextObject(), i++) {
		
		surfelCollection = object->getSurfelCollection();
		
		// clear covered flag
		for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {
			scene->setSurfelFlagOn (surfel, SurfelInterface::COVERED, false, false);
		}
		
		// the renderer expects right-handed row major
		object->getTransformationMatrix (objectTransformation);
		MtrMult4x4f (sceneViewMatrix, objectTransformation, finalTransformation);
		
		// make it a row major matrix
		MtrTranspose4x4f (finalTransformation, convertedTransformation);
		
		// convert left-handed to right-handed (change sign of z-coordinates)
		convertedTransformation[8]  *= -1.0f;
		convertedTransformation[9]  *= -1.0f;
		convertedTransformation[10] *= -1.0f;
		convertedTransformation[11] *= -1.0f;
		
		SrfSetTrafo (surfelContext, convertedTransformation);
		
		// bbox returns a bounding box containing all pixels that were hit during warping
		SrfWarp (surfelContext, object, surfelCollection, bbox);
	}
	
	frameBuffer->markAllSurfels();		

	setSceneView (restoreSceneViewMatrix);


}




void SurfelGPRenderer::cleanUp() {

	if (lightSourceHandle != 0) {
		SrfFreeLight (surfelContext, lightSourceHandle);
	}

	if (surfelContext != 0) {
		SrfFree (surfelContext);
		surfelContext = 0;
	}

	if (rendererWidget != 0) {
		delete rendererWidget;
	}

	if (frameBuffer != 0) {
		delete frameBuffer;
	}

}

void SurfelGPRenderer::updateLightDirection (const bool updateScene) {
	
	Vector3D t;

	lightDirection.normalize();

	// the surfel renderer expects a light direction that is towards the light, therefore
	// flip the direction here. additionally, it uses a left handed coordinate system, therefore,
	// flip z twice.
	t[0] = -lightDirection[0];
	t[1] = -lightDirection[1];
	t[2] = lightDirection[2];

	SrfSetLightDirection (surfelContext, lightSourceHandle, t);

	if (updateScene == true) {
		this->renderFrame();
	}

}

// *************
// private slots
// *************

void SurfelGPRenderer::handleObjectModified () {
	// an object has been changed so we have to 
	// invalidate the frame buffer
	frameBuffer->setValid (false);
}

void SurfelGPRenderer::handleActiveObjectChanged() {
	frameBuffer->setValid (false);
}

void SurfelGPRenderer::handleObjectAdded() {
	frameBuffer->setValid (false);
}

void SurfelGPRenderer::handleObjectRemoved() {
	frameBuffer->setValid (false);
}

void SurfelGPRenderer::handleObjectMoved() {
	frameBuffer->setValid (false);
}

void SurfelGPRenderer::handleWidgetRepainted() {
	emit rendererWidgetRepainted();
}

void SurfelGPRenderer::handleCutOffRadiusChanged (const float newCutOffRadius) {
	SrfSetCutoffRadius (surfelContext, newCutOffRadius);
}

void SurfelGPRenderer::handleTwoSidedNormalsChanged (const bool enable) {

	if (enable == true) {
		SrfEnableZBufferOption (surfelContext, SRF_BIDINORMALS);
	} 
	else {
		SrfDisableZBufferOption (surfelContext, SRF_BIDINORMALS);
	}

}

void SurfelGPRenderer::toggleSelectionVisualization() {
	
	selectionVisualizationEnabled = !selectionVisualizationEnabled;

	setSelectionVisualizationEnabled(selectionVisualizationEnabled);
}

void SurfelGPRenderer::setSelectionVisualizationEnabled(const bool enable) {

	selectionVisualizationEnabled = enable;

	if(selectionVisualizationEnabled) {
		SrfEnableZBufferOption(surfelContext, SRF_VISUALIZESELECTION);
	}
	else {
		SrfDisableZBufferOption(surfelContext, SRF_VISUALIZESELECTION);
	}
}

bool SurfelGPRenderer::isSelectionVisualizationEnabled() const {
	return selectionVisualizationEnabled;
}

void SurfelGPRenderer::handleConfigurationApplied() {
	
	if (rendererWidget != 0) {
		this->renderFrame();
	}

}

// ****************
// access functions
// ****************

RendererInterface *createRenderer() {
	return new SurfelGPRenderer();
}

const char *getRendererType() {
	return "SurfelGPRenderer";
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
