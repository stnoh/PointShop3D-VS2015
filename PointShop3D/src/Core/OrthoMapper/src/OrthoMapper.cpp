// Title:   OrthoMapper.cpp
// Created: Thu Sep 25 14:15:18 2003
// Authors: Oliver Knoll, Mark Pauly, Matthias Zwicker
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

#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qwhatsthis.h>
#include <qwidget.h>
#include <qstring.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qpen.h>

#include "../../Scene/src/Object.h"
#include "../../Scene/src/Scene.h"
#include "../../Transformer/src/Transformer.h"
#include "../../Configuration/src/Configuration.h"

#include "../../../Utilities/src/Common.h"
#include "../../../Utilities/src/MyMathTools.h"
#include "../../Interfaces/src/FrameBufferInterface.h"
#include "../../Interfaces/src/RendererInterface.h"
#include "../../DataStructures/src/SurfelCollection.h"
#include "OrthoMapper.h"

OrthoMapper::OrthoMapper (const QSize bufferSize, const OrthoMapper::CursorShape shape) {

	this->bufferSize = bufferSize;
	this->cursorShapeType = shape;
	this->innerShapeFraction = 0.5;

	useConstantDepthAndNormalMap = false;
	visualizePatch = false;

	mainRenderer = RendererManager::getInstance()->getMainRenderer();
	mainFrameBuffer = mainRenderer->getFrameBuffer();
	sceneTransformer = Transformer::getSceneTransformer();
	
	// init resampling renderer
	resamplingRenderer = RendererManager::getInstance()->getAuxiliaryRenderer();
	resamplingRenderer->setViewPortSize (bufferSize);	
	Configuration::getInstance()->setAuxiliaryShadingEnabled (false);

	resamplingTransformer = NULL;
	resamplingFrameBuffer = resamplingRenderer->getFrameBuffer();


	mainBufferSizeX = mainBufferSizeY = 0;
	frameBufferDepthValues = 0;
	frameBufferNormals = 0;
	frameBufferCopyValid = false;

	// field of view for resampling renderer. the field of view (or opening angle) is small
	// to approximate parallel projection.
	fieldOfView = 5.0f;
	tan_fieldOfView_2 = tan((fieldOfView / 2.f) * (M_PI / 180.0f));

	reset();


    resamplingRenderer->setCutoffRadius (1.4f);
    resamplingRenderer->setBlendingThresholds (5.0f, 5.0f, 12.0f);
	resamplingRenderer->setSelectionVisualizationEnabled (false);


	// init cursor
	initCursorTransform();


	cursorShape   = QPointArray (20);	

	fixedNormal = Vector3D (0, 0, 1);
	normalFromFrameBuffer = true;


	// init viewers
	rgbViewer = new ImageViewer (0, "RGB Viewer");
	zViewer   = new ImageViewer (0, "Z Viewer");

	rgbPixmap = new QPixmap (bufferSize);
	zPixmap   = new QPixmap (bufferSize);
}


OrthoMapper::~OrthoMapper() {
	
	delete rgbViewer;
	delete zViewer;

	if (resamplingTransformer) {
		delete resamplingTransformer;
	}
	
	if (frameBufferDepthValues) {
		delete[] frameBufferDepthValues;
	}

	if (frameBufferNormals) {
		delete[] frameBufferNormals;
	}

}


void OrthoMapper::reset () {

	frameBufferCopyValid = false;
	cursorVisible = false;
	localTransformValid = false;
}





void OrthoMapper::handleBufferSizeChanged (QSize newBufferSize, bool redraw) {

	bufferSize = newBufferSize;

	resamplingRenderer = RendererManager::getInstance()->getAuxiliaryRenderer();

	resamplingRenderer->setViewPortSize (bufferSize);
	if (resamplingTransformer) {
		resamplingTransformer->setViewPortSize (bufferSize);	 
	}
	
	if (redraw) {
		renderCursor();
		initCursorTransform();
		renderCursor();
	}
}






void OrthoMapper::initAdjustCursorShape () {

	scaleUold = scaleU;
	scaleVold = scaleV;
	scaleWold = scaleW;
	angleOld = angle;
	innerShapeFractionOld = innerShapeFraction;
}



void OrthoMapper::adjustCursorShape (OrthoMapper::Cursor changeCursor, int mouseMoveDistance) {
	
	float scale;
	switch (changeCursor) {
		
	case OrthoMapper::GLOBAL_SIZE:
			
		// scale u, v and w
		scale = 1.0f + (float) mouseMoveDistance / 100.0f;
		if ((scaleUold * scale > metric) && (scaleVold * scale > metric) && (fabs(scaleWold * scale) > metric)) {		
			scaleU = scaleUold * scale;
			scaleV = scaleVold * scale;		
			scaleW = scaleWold * scale;
		}
		
		break;
		
	case OrthoMapper::X_SIZE:
		// scale u
		scaleU = scaleUold + (float) mouseMoveDistance * metric;
		if (scaleU < 0) {
			scaleU = scaleUold = 0;
		}
		break;
		
	case OrthoMapper::Y_SIZE:
		// scale v
		scaleV = scaleVold + (float) mouseMoveDistance * metric;
		if (scaleV < 0) {
			scaleV = scaleVold = 0;
		}
		break;
		
	case OrthoMapper::Z_SIZE:
		// scale w
		scaleW = scaleWold + (float) mouseMoveDistance * metric;
		break;
		
	case OrthoMapper::ORIENTATION:
		
		angle = angleOld + (float) mouseMoveDistance / 100.0f;
		break;

	case OrthoMapper::RELATIVE_SIZE:
			
		innerShapeFraction = innerShapeFractionOld + (float) mouseMoveDistance / 100.0f;
		break;

	
	}
}



void OrthoMapper::invertDepth () {

	scaleW = -scaleW;
}





void OrthoMapper::setToolOrientation (bool normalFromFrameBuffer) {
	
	this->normalFromFrameBuffer = normalFromFrameBuffer;
	
}





bool OrthoMapper::drawCursor (Vector2D newScreenPosition, float tiling) {
		
	// remove old cursor (XOR mode)
	if (cursorVisible) {	
		renderCursor();
		cursorVisible = false;
	}

    cursorStatus = NOT_VALID;		
	screenPosition = newScreenPosition;

	if (mainFrameBuffer->isPixelCovered ((int)screenPosition[0], (int)screenPosition[1])) {
				
		if (mainFrameBuffer->getObjectAtPixel ((int)screenPosition[0], (int)screenPosition[1])
		    != Scene::getInstance()->getActiveObject()) {
			cursorStatus = NOT_ACTIVE;
		}
		else {
			
			if ((tiling == 0.0f) || (!localTransformValid)) {
				cursorStatus = ACTIVE;
			}
			else {				
				
				// all local variables (localCenter, etc., are computed in reconstructSurfaceFunction (..)
				
				Vector3D newOrigin = sceneTransformer->viewPortToCamera (screenPosition, 
					getDepthValue ((int)screenPosition[0], (int)screenPosition[1]));
				
				
				float distanceX = fabs(Vector3D::dotProduct (newOrigin - localCenter, localXAxis));
				float distanceY = fabs(Vector3D::dotProduct (newOrigin - localCenter, localYAxis));
				
				if ((distanceX > localXAxisLength * tiling * 2.0f) ||
					(distanceY > localYAxisLength * tiling * 2.0f)) {
					cursorStatus = ACTIVE;
				}
				else {
					cursorStatus = NOT_ACTIVE;
				}
			}
		}
	}
	
			
	if (cursorStatus != NOT_VALID) {
		
		this->computeLocalTransform ();
		Vector2D pointViewSpace;
		
		// cursor shape
		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (-1, -1, 0)));	
		cursorShape.setPoint (0, (int)pointViewSpace[0], (int)pointViewSpace[1] );
		
		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (-1, 1, 0)));	
		cursorShape.setPoint (1, (int)pointViewSpace[0], (int)pointViewSpace[1]);
		
		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (1, 1, 0)));	
		cursorShape.setPoint (2, (int)pointViewSpace[0], (int)pointViewSpace[1]);
		
		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (1, -1, 0)));	
		cursorShape.setPoint (3, (int)pointViewSpace[0], (int)pointViewSpace[1]);

		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (0, 0, scaleW)));	
		cursorShape.setPoint (4, (int)pointViewSpace[0], (int)pointViewSpace[1]);

		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (0, 0, 0)));	
		cursorShape.setPoint (5, (int)pointViewSpace[0], (int)pointViewSpace[1]);
		
		// cursor orientation
		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (-1.1f, -1, 0)));	
		cursorShape.setPoint (8, (int)pointViewSpace[0], (int)pointViewSpace[1]);
		
		pointViewSpace = sceneTransformer->cameraToViewPort (localFrameToCameraSpace (Vector3D (-1.1f, 1, 0)));	
		cursorShape.setPoint (9, (int)pointViewSpace[0], (int)pointViewSpace[1]);
		
				
		if (cursorStatus == ACTIVE) {
			cursorPen = QPen (Qt::white, 1, Qt::SolidLine);
		}
		else {
			cursorPen = QPen (Qt::white, 1, Qt::DotLine);
		}
		
		// draw new cursor
		renderCursor();				
		cursorVisible = true;
		
	}

	return (cursorStatus == ACTIVE);
}


void OrthoMapper::renderCursor () {
	
	if (cursorStatus != NOT_VALID) {
		
		QPainter painter;
		
		
		painter.begin (mainRenderer->getRendererWidget());	
		painter.setPen (cursorPen);
		// XOR mode
		painter.setRasterOp (Qt::XorROP);
		
		painter.drawPolygon (cursorShape, false, 0, 4);
		
		if (cursorShapeType == OrthoMapper::PLAIN) {
			painter.drawLine (cursorShape.at(5), cursorShape.at(4));
		}
		else if (cursorShapeType == OrthoMapper::WITH_DEPTH) {
			painter.drawLine (cursorShape.at(0), cursorShape.at(4));
			painter.drawLine (cursorShape.at(1), cursorShape.at(4));
			painter.drawLine (cursorShape.at(2), cursorShape.at(4));
			painter.drawLine (cursorShape.at(3), cursorShape.at(4));
			painter.drawLine (cursorShape.at(8), cursorShape.at(9));
		}
		else if (cursorShapeType == OrthoMapper::TWO_SHAPES) {
			cursorShape.setPoint (10, (1 - innerShapeFraction) * cursorShape.at(5) + cursorShape.at(0) * innerShapeFraction);
			cursorShape.setPoint (11, (1 - innerShapeFraction) * cursorShape.at(5) + cursorShape.at(1) * innerShapeFraction);
			cursorShape.setPoint (12, (1 - innerShapeFraction) * cursorShape.at(5) + cursorShape.at(2) * innerShapeFraction);
			cursorShape.setPoint (13, (1 - innerShapeFraction) * cursorShape.at(5) + cursorShape.at(3) * innerShapeFraction);
			painter.drawPolygon (cursorShape, false, 10, 4);
		}
		

		painter.end();	
	}
	
}



void OrthoMapper::setVisualizePatchEnable (bool enable) {
	visualizePatch = enable;
}



bool OrthoMapper::reconstructSurfaceFunction(bool &allPixelsCovered) {
	
	if(setRenderingTransform() == true) {

		resamplingRenderer->renderFrame (FrameBufferInterface::PERPIXEL_C_Z_N_W | FrameBufferInterface::ALL_SPLATS |
			FrameBufferInterface::PERPIXEL_SURFEL, false);

		if (visualizePatch) {
			rgbPixmap->convertFromImage (*resamplingFrameBuffer->getImage());
			rgbViewer->PixmapLabel->setPixmap (*rgbPixmap);
			rgbViewer->show();
			
			zPixmap->convertFromImage (*resamplingFrameBuffer->getZImage());
			zViewer->PixmapLabel->setPixmap (*zPixmap);
			zViewer->show();
		}

		allPixelsCovered = resamplingFrameBuffer->allPixelsCovered();

		return true;
	} 
	else {
		return false;
	}
}


void OrthoMapper::computeAllVisibleSurfels (std::vector<SurfelInterface *> **surfels, int &numOfSurfels) {

	setRenderingTransform();

	resamplingRenderer->renderFrame (FrameBufferInterface::ALL_SPLATS, false);

	*surfels = resamplingFrameBuffer->getSurfelsInViewFrustum();
	numOfSurfels = resamplingFrameBuffer->getNofSplatInfoEntries();

}


float OrthoMapper::getZScale() {
	return 0.5f * scaleW / metric;
}


Vector3D OrthoMapper::getFilteredSurfacePosition (int x, int y, float depth) {	

	return resamplingTransformer->viewPortToObject (Vector2D (x + 0.5f, y + 0.5f), depth);
}


Vector3D OrthoMapper::getSurfacePosition (int x, int y) {	

	return resamplingTransformer->viewPortToObject (Vector2D (x + 0.5f, y + 0.5f), resamplingFrameBuffer->getDepth (x, y));
}

Vector3D OrthoMapper::getSurfaceNormal (int x, int y) {

	Vector3D normal = resamplingTransformer->cameraToObjectVector (resamplingFrameBuffer->getNormal (x, y));
	normal.normalize();
	return normal;
}

QRgb OrthoMapper::getSurfaceDiffuseColor (int x, int y) {
	return resamplingFrameBuffer->getColor (x, y); 
}

SurfelInterface* OrthoMapper::insertNewSurfel (SurfelCollection* objectSurfels, int x, int y) {

	// create new surfel with same attributes as surfel in framebuffer
	SurfelInterface *surfel = objectSurfels->addSurfel (resamplingFrameBuffer->getSurfel (x, y), false);

	// resample some attributes from framebuffer			
	// surfel Position					
	Scene::getInstance()->setSurfelPosition (surfel, getSurfacePosition (x, y), false);
	
	// surfel normal
	Scene::getInstance()->setSurfelNormal (surfel, getSurfaceNormal (x, y), false);
	
	// diffuse color
	Scene::getInstance()->setSurfelDiffuseColor (surfel, getSurfaceDiffuseColor (x, y), false);		

	return surfel;
}


Vector3D OrthoMapper::setDistancesToToolPlane (ChannelComponent *dispMatrix) {

	int x, y;
	float toolZValue;

	if (scaleW > 0.0f) {
		toolZValue = resamplingFrameBuffer->getMaxZValue();
	} 
	else {
		toolZValue = resamplingFrameBuffer->getMinZValue();
	}

	for (x = 0; x < bufferSize.width(); x++) {
		for (y = 0; y < bufferSize.height(); y++) {
			dispMatrix->setValue (x, y, resamplingFrameBuffer->getDepth(x, y) + toolZValue);
		}
	}
	return resamplingTransformer->cameraToObjectVector (Vector3D (0, 0, 1));
}

		

void OrthoMapper::setTouchingPointAlignment (bool useConstantDepthAndNormalMap) {

	this->useConstantDepthAndNormalMap = useConstantDepthAndNormalMap;

	if (!frameBufferDepthValues && useConstantDepthAndNormalMap) {
		reset();
	}
}



Vector3D OrthoMapper::getFixedToolNormal () {
	return fixedNormal;
}

void OrthoMapper::setFixedToolNormal (Vector3D fixedNormal) {
	this->fixedNormal = fixedNormal;
}


Vector3D OrthoMapper::getToolNormalInObjectSpace () {

	Vector3D normal = Transformer::getSceneTransformer()->cameraToObjectVector (mainFrameBuffer->getNormal ((int)screenPosition[0],
                                                                                                            (int)screenPosition[1]));
	normal.normalize();
	return normal;
}


FrameBufferInterface* OrthoMapper::getFrameBuffer () {
	return resamplingFrameBuffer;
}


void OrthoMapper::setCursorShape (const CursorShape shape) {
	this->renderCursor();
	this->cursorShapeType = shape;
	this->renderCursor();
}




// ************
// public slots
// ************



// ***************
// private methods
// ***************


void OrthoMapper::computeLocalTransform () {
	
	Vector3D normal,
		basePoint,
		uAxis,
		vAxis;
	
	
	float cosPhi;
	float sinPhi;
	
	
	// get normal
	if (normalFromFrameBuffer) {	
		normal = getNormal ((int)screenPosition[0], (int)screenPosition[1]);
	}
	else {
		normal = fixedNormal;
	}
	

	// get base point
	basePoint = sceneTransformer->viewPortToCamera (screenPosition, getDepthValue ((int)screenPosition[0], (int)screenPosition[1]));
	
	// project uAxis into tangent plane
	uAxis = Vector3D (1.0f, 0.0f, 0.0f);
	uAxis = uAxis - normal * Vector3D::dotProduct (normal, uAxis);
	uAxis.normalize();
	vAxis = Vector3D::crossProduct (normal, uAxis);
	
	cosPhi = cos (angle);
    sinPhi = sin (angle);
	
	transform.setEntry (0, 0, (cosPhi * uAxis[0] + sinPhi * vAxis[0]) * (scaleU / metric)); 
	transform.setEntry (1, 0, (cosPhi * uAxis[1] + sinPhi * vAxis[1]) * (scaleU / metric));
	transform.setEntry (2, 0, (cosPhi * uAxis[2] + sinPhi * vAxis[2]) * (scaleU / metric));
	
	transform.setEntry (0, 1, (-sinPhi * uAxis[0] + cosPhi * vAxis[0]) * (scaleV / metric)); 
	transform.setEntry (1, 1, (-sinPhi * uAxis[1] + cosPhi * vAxis[1]) * (scaleV / metric)); 
	transform.setEntry (2, 1, (-sinPhi * uAxis[2] + cosPhi * vAxis[2]) * (scaleV / metric)); 
	
	transform.setEntry (0, 2, normal[0] * (1.0f / metric)); 
	transform.setEntry (1, 2, normal[1] * (1.0f / metric));
	transform.setEntry (2, 2, normal[2] * (1.0f / metric));
	
	translation = basePoint;

	localTransformValid = true;
	
}


void OrthoMapper::updateTransformer () {

	MyDataTypes::TransformationMatrix16f sceneViewMatrix;
	MyDataTypes::TransformationMatrix16f objectMatrix;
	
	resamplingRenderer->getSceneView (sceneViewMatrix);
	Scene::getInstance()->getActiveObject()->getTransformationMatrix (objectMatrix);
	
	if (resamplingTransformer) {

		resamplingTransformer->setViewPortSize (bufferSize);	 
		resamplingTransformer->setViewFrustum (resamplingRenderer->getViewFrustum());
		resamplingTransformer->setSceneTransformationMatrix (sceneViewMatrix);
		resamplingTransformer->setObjectTransformationMatrix (objectMatrix);
	}
	else {
		resamplingTransformer = new Transformer (bufferSize, resamplingRenderer->getViewFrustum(), sceneViewMatrix, objectMatrix);
	}
}




bool OrthoMapper::setRenderingTransform () {

	Transformer *transformer = Transformer::getSceneTransformer();
	FrameBufferInterface *frameBuffer = resamplingRenderer->getFrameBuffer();
	(void)frameBuffer;
	
	localCenter = localFrameToCameraSpace (Vector3D (0, 0, 0));
	localXAxis = localFrameToCameraSpace (Vector3D (1, 0, 0));
	localYAxis = localFrameToCameraSpace (Vector3D (0, 1, 0));

	// compute scene coordinates of tangent space basis
	Vector3D center = transformer->cameraToScene (localCenter);
	Vector3D xAxis = transformer->cameraToScene (localXAxis) - center;
	Vector3D yAxis = transformer->cameraToScene (localYAxis) - center;
	Vector3D normal = transformer->cameraToScene (localFrameToCameraSpace (Vector3D (0, 0, 1))) - center;
	normal.normalize();


	localXAxis -= localCenter;
	localYAxis -= localCenter;

	localXAxisLength = localXAxis.normalize();
	localYAxisLength = localYAxis.normalize();


	// set up the resampling renderer

	// compute sence view matrix of resampling renderer.
	// first compute scene to tangent space matrix then tangent space to camera matrix
	float tanToScene[16];
	float sceneToTan[16];
	float tanToCam[16];

	float lengthX = xAxis.normalize();
	float lengthY = yAxis.normalize();

	// construct tangential-to-scene space transformation
	// center, x, y represent the tangent space basis in scene coordinates
	tanToScene[0] = xAxis[0];
	tanToScene[1] = xAxis[1];
	tanToScene[2] = xAxis[2];
	tanToScene[3] = 0.f;

	tanToScene[4] = yAxis[0];
	tanToScene[5] = yAxis[1];
	tanToScene[6] = yAxis[2];
	tanToScene[7] = 0.f;

	tanToScene[8] = normal[0];
	tanToScene[9] = normal[1];
	tanToScene[10] = normal[2];
	tanToScene[11] = 0.f;

	tanToScene[12] = center[0];
	tanToScene[13] = center[1];
	tanToScene[14] = center[2];
	tanToScene[15] = 1.f;

	// invert tangential-to-scene space transformation to get scene-to-tangential
	// space conversion
	if (MtrInverse4x4f(tanToScene, sceneToTan) == false) {
		return false;
	}

	// set up tangential-to-"resampling camera" space transformation (column major ordering!)
	// the resampling camera coordinates only differ by a negative translation along z!
	tanToCam[0] = 1.f;
	tanToCam[1] = 0.f;
	tanToCam[2] = 0.f;
	tanToCam[3] = 0.f;

	tanToCam[4] = 0.f;
	tanToCam[5] = 1.f;
	tanToCam[6] = 0.f;
	tanToCam[7] = 0.f;

	tanToCam[8] = 0.f;
	tanToCam[9] = 0.f;
	tanToCam[10] = 1.f;
	tanToCam[11] = 0.f;

	// translation, move the object back along the z-axis
	// NOTE: the translation of the resampling camera is given by the user specified field-of-view angle
	// of the resampling camera viewing frustum
	float resamplingZTranslation = lengthY / tan_fieldOfView_2;

	tanToCam[12] = 0.f;
	tanToCam[13] = 0.f;
	tanToCam[14] = -resamplingZTranslation;
	tanToCam[15] = 1.f;

	float sceneViewMatrix[16];

	// this means: apply first sceneToTan, then tanToCam
	MtrMult4x4f(tanToCam, sceneToTan, sceneViewMatrix);

	resamplingRenderer->setSceneView (sceneViewMatrix);
	
	MyDataTypes::ViewFrustum viewFrustum;
	
	// the resampling camera has a fixed, user specified field-of-view
	viewFrustum.aspectRatio = lengthX / lengthY;
	viewFrustum.fieldOfView = fieldOfView;
	// the near and far clipping planes are set relative to the z-translation
	// and the size of the view frustum in the resampling renderer camera space
	viewFrustum.nearPlane = resamplingZTranslation - lengthY * 0.5f;
	viewFrustum.farPlane = resamplingZTranslation + lengthY * 0.5f;

	viewFrustum.nearPlane = resamplingZTranslation - lengthY * 2.0f;
	viewFrustum.farPlane = resamplingZTranslation + lengthY * 2.0f;
	
	resamplingRenderer->setViewFrustum (viewFrustum);
		
	updateTransformer();

	return true;
}






void OrthoMapper::initCursorTransform () {

	metric    = 1.0f;
	scaleU    = bufferSize.width();
	scaleV    = bufferSize.height();
	scaleW    = MyMathTools::minf (bufferSize.width(), bufferSize.height());
	angle     = 0.0f;
}




void OrthoMapper::updateFrameBufferCopy () {
	
	if (useConstantDepthAndNormalMap) {
		// make a copy of the framebuffer depth values and normals so they
		// can still be used when the image is rerendered
		// reallocate when size has changed
		if ((mainBufferSizeX != mainFrameBuffer->getSize().width()) ||
			(mainBufferSizeY != mainFrameBuffer->getSize().height())) {

			if (frameBufferDepthValues) {
				delete[] frameBufferDepthValues;
			}
			frameBufferDepthValues = 0;
		
			if (frameBufferNormals) {
				delete[] frameBufferNormals;
			}
			frameBufferNormals = 0;

			mainBufferSizeX = mainFrameBuffer->getSize().width();
			mainBufferSizeY = mainFrameBuffer->getSize().height();
		}

		if (frameBufferDepthValues == 0) {
			frameBufferDepthValues = new float [mainBufferSizeX * mainBufferSizeY];
		}
		if (frameBufferNormals == 0) {
			frameBufferNormals = new Vector3D [mainBufferSizeX * mainBufferSizeY];
		}

		int x, y;

		for (x = 0; x < mainBufferSizeX; x++) {
			for (y = 0; y < mainBufferSizeY; y++) {
				frameBufferDepthValues [y * mainBufferSizeX + x] = mainFrameBuffer->getDepth (x, y);
				frameBufferNormals [y * mainBufferSizeX + x] = mainFrameBuffer->getNormal (x, y);
			}
		}
		frameBufferCopyValid = true;
	}
	else {

		if (frameBufferDepthValues) {
			delete[] frameBufferDepthValues;
		}
		frameBufferDepthValues = 0;
		
		if (frameBufferNormals) {
			delete[] frameBufferNormals;
		}
		frameBufferNormals = 0;
		
	}
}


inline Vector3D OrthoMapper::localFrameToCameraSpace (const Vector3D &point) {
	
	return transform.transformPoint (point) + translation;
}


inline float OrthoMapper::getDepthValue (int x, int y) {

	if (useConstantDepthAndNormalMap) {
		if (frameBufferCopyValid == false) {
			this->updateFrameBufferCopy();
		}
		return frameBufferDepthValues [y * mainBufferSizeX + x];
	}
	else {
		return mainRenderer->getDepth (x, y);
	}
}


inline Vector3D OrthoMapper::getNormal(int x, int y) {

	if (useConstantDepthAndNormalMap) {
		if (frameBufferCopyValid == false) {
			this->updateFrameBufferCopy();
		}
		return frameBufferNormals [y * mainBufferSizeX + x];
	}
	else {
		return mainRenderer->getCameraSpaceNormal (x, y);
	}
}



// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
