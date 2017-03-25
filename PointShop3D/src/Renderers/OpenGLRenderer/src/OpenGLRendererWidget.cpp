// Title:   OpenGLRendererWidget.cpp
// Created: Thu Sep 25 15:44:56 2003
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

#if defined(_WIN32)
#pragma warning(disable:4305) // truncation from const double to float
#endif

#define  POINT_SMOOTH

//#include <qgl.h>

#if defined( _WIN32 )
#  include <windows.h>
#  include <GL/gl.h>
#  include "win32/glext.h"
#  define GLH_EXT_SINGLE_FILE
#  include "win32/glh_genext.h"
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/gl.h>
#  include <GL/glext.h>
#endif

#include <qcolor.h>
#include <qevent.h>
#include <qimage.h>

#include "../../../Core/Scene/src/Scene.h"
#include "../../../Core/Scene/src/Object.h"
#include "../../../Core/Configuration/src/Configuration.h"
#include "../../../Core/src/DefaultMouseHandler.h"
#include "../../../Utilities/src/Matrix.h"
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../Core/CoreTools/src/CoreTools.h"
#include "OpenGLRendererConfiguration.h"
#include "OpenGLRendererWidget.h"

// Some defines used by Tim's varptsize-stuff -- will be outsourced soon
//
#if 0
//#define  NORMAL_NOMINATOR  0.5f // theoretically the correct value
//#define  NORMAL_NOMINATOR  0.35f // works best with the chamaleon
#define  NORMAL_NOMINATOR  0.30f // works best with the wasp
#else
//#define  NORMAL_NOMINATOR  0.5f // theoretically the correct value
//#define  NORMAL_NOMINATOR  0.16f // works quite well
//#define  NORMAL_NOMINATOR  0.15f // works quite well -- was used for SIGGRAPH '02 demo
//#define  NORMAL_NOMINATOR  0.125f  // Mark: "good compromise"
#define  NORMAL_NOMINATOR  0.1f	// Richard: corrected radius (factor sqrt(2/3)) multiplied with 0.125f
#endif

//#define  CULLING

// A macro to debug OpenGL errors -- only active in Debug mode
//
#ifndef NDEBUG
#  define  CHECK_FOR_GL_ERROR()													\
     do {																		\
       const char *str; int err;												\
       while ((err = glGetError ()) != GL_NO_ERROR) {							\
         switch (err)															\
           {																	\
           case GL_INVALID_ENUM:  str = "GL_INVALID_ENUM";  break;				\
           case GL_INVALID_VALUE:  str = "GL_INVALID_VALUE";  break;			\
           case GL_INVALID_OPERATION:  str = "GL_INVALID_OPERATION";  break;	\
           case GL_STACK_OVERFLOW:  str = "GL_STACK_OVERFLOW";  break;			\
           case GL_STACK_UNDERFLOW:  str = "GL_STACK_UNDERFLOW";  break;		\
           case GL_OUT_OF_MEMORY:  str = "GL_OUT_OF_MEMORY";  break;			\
           default:  str = "<unknown error>";									\
           }																	\
         qDebug ("OpenGL Error anywhere before %s:%d: %s\n",			   		\
     	     __FILE__, __LINE__, str); }										\
     } while(0)
#else
#  define  CHECK_FOR_GL_ERROR()  while(0)
#endif

#ifndef NDEBUG
//#  define  DEBUG_VERTEX_SHADER
#endif

// **************
// public methods
// **************

OpenGLRendererWidget::OpenGLRendererWidget (const bool isVisible, OpenGLRendererConfiguration *rendererConfiguration,
											QWidget *parent, const char *name)
  : QGLWidget (parent, name),
	displayListsPossible(false),
	initializedExtensions(false),
	vertexProgId(0),
    modelInteractState(RendererInterface::INTERACT_STATIC),
    viewpointInteractState(RendererInterface::INTERACT_STATIC)
{
  Scene         *scene;
  Configuration *configuration;

  this->rendererConfiguration = rendererConfiguration;
	
  // disable clearing the background before each paint event
  this->setBackgroundMode (NoBackground);

  updateViewPortSize = true;
  displayListsValid  = false;
  shadingEnabled     = true;
  visible            = isVisible;

  {
    configuration = Configuration::getInstance();
    viewFrustum = configuration->getViewFrustum();
  }

  MtrUnity4x4f (sceneViewMatrix);

  scene = Scene::getInstance();

  updateInternalFlags();  // set up internal flags derived from configuration

  // get informed whenever the Objects in the Scene change
  this->connect (scene, SIGNAL (objectModified(bool)),
				 this, SLOT (handleObjectModified()));

  // get informed whenever the active Object in the Scene changes
  this->connect (scene, SIGNAL (activeObjectChanged()),
				 this, SLOT (handleActiveObjectChanged()));

  // get informed whenever Objects are added to the Scene
  this->connect (scene, SIGNAL (objectAdded()),
				 this, SLOT (handleObjectAdded()));

  this->connect (CoreTools::getInstance()->getSelectionTool(), SIGNAL (selectionChanged (SurfelInterface::Flag)),
		           this, SLOT (handleSelectionChanged (SurfelInterface::Flag)));

  // get informed whenever Objects are added to the Scene
  this->connect (scene, SIGNAL (objectRemoved()),
				 this, SLOT (handleObjectRemoved()));

  this->connect (rendererConfiguration, SIGNAL (cutOffRadiusChanged (const float)),
				 this, SLOT (handleCutOffRadiusChanged (const float)));

  this->connect (rendererConfiguration, SIGNAL (constantPointSizeChanged (const bool)),
				 this, SLOT (handleConstantPointSizeChanged (const bool)));

  this->connect (rendererConfiguration, SIGNAL (smoothPointChanged (const bool)),
				 this, SLOT (handleSmoothPointChanged (const bool)));


  this->connect (rendererConfiguration, SIGNAL (useDisplayListsChanged (const bool)),
				 this, SLOT (handleUseDisplayListsChanged (const bool)));
	
  this->connect (rendererConfiguration, SIGNAL (useVertexShadersChanged (const bool)),
				 this, SLOT (handleUseVertexShadersChanged (const bool)));
	
	
  // get informed whenever all the settings in the OpenGLRendererConfiguration have been applied
  this->connect (rendererConfiguration, SIGNAL (configurationApplied()),
				 this, SLOT (handleConfigurationApplied()));

  bShowSelection = true;
}

OpenGLRendererWidget::~OpenGLRendererWidget() {
	this->removeDisplayLists();
}

void OpenGLRendererWidget::setVisible (const bool enable) {
	visible = enable;
}

bool OpenGLRendererWidget::isVisible() const {
	return visible;
}

QSize OpenGLRendererWidget::sizeHint() const {
	return Configuration::getInstance()->getViewPortSize();
}

QSize OpenGLRendererWidget::minimumSizeHint() const {
	return Configuration::getInstance()->getMinimumViewPortSize();
}

QSizePolicy OpenGLRendererWidget::sizePolicy() const {
	return QSizePolicy (QSizePolicy::Preferred , QSizePolicy::Preferred);
}

void OpenGLRendererWidget::setShadingEnabled (const bool enable) {

	this->makeCurrent();

	shadingEnabled = enable;

	// only make changes if enable is different from the previous
	// settings, since we need to recompile the display lists
	if ((enable == true) && (glIsEnabled (GL_LIGHTING) == GL_FALSE)) {
		glEnable (GL_LIGHTING);
		glEnable (GL_LIGHT0);
		displayListsValid = false;
	}
	else if ((enable == false) && glIsEnabled (GL_LIGHTING) == GL_TRUE) {
		glDisable (GL_LIGHTING);
		glDisable (GL_LIGHT0);
		displayListsValid = false;
	}

}

bool OpenGLRendererWidget::isShadingEnabled() const {
	return glIsEnabled (GL_LIGHTING);
}

void OpenGLRendererWidget::setLightDirection (const Vector3D newLightDirection) {
	lightDirection = newLightDirection;
	this->updateLightDirection (true);
}

Vector3D OpenGLRendererWidget::getLightDirection() const {
	return lightDirection;
}


void OpenGLRendererWidget::setBackgroundColor(QRgb newBackgroundColor) {
	backgroundColor = QColor(newBackgroundColor);
	glClearColor(backgroundColor.red() / 255.0f, backgroundColor.green() / 255.0f, backgroundColor.blue() / 255.0f, 0.0);
}

QRgb OpenGLRendererWidget::getBackgroundColor() const {
	return backgroundColor.rgb();
}

void OpenGLRendererWidget::setSceneView (const MyDataTypes::TransformationMatrix16f newSceneViewMatrix) {
	memcpy (sceneViewMatrix, newSceneViewMatrix, sizeof (MyDataTypes::TransformationMatrix16f));
}

void OpenGLRendererWidget::getSceneView (MyDataTypes::TransformationMatrix16f sceneViewMatrix) const {
	memcpy (sceneViewMatrix, this->sceneViewMatrix, sizeof (MyDataTypes::TransformationMatrix16f));
}

void OpenGLRendererWidget::setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum) {

	viewFrustum = newViewFrustum;

	this->makeCurrent();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (viewFrustum.fieldOfView, viewFrustum.aspectRatio,
		            viewFrustum.nearPlane,   viewFrustum.farPlane);
	glMatrixMode (GL_MODELVIEW);

}

MyDataTypes::ViewFrustum OpenGLRendererWidget::getViewFrustum() const {
	return viewFrustum;
}

void OpenGLRendererWidget::setViewPortSize (const QSize newViewPortSize) {
	// we do not want to update the configuration, it has already been updated
	// see resizeGL()
	updateViewPortSize = false;
	this->resize (newViewPortSize);
	// from now on we want to update the view port size in the Configuration again,
	// that is whenever this widget is resized by the user
	updateViewPortSize = true;
}
QSize OpenGLRendererWidget::getViewPortSize() const {
	return this->size();
}

void OpenGLRendererWidget::setSelectionVisualizationEnabled(const bool showSelection) {

	if (bShowSelection != showSelection) {
		bShowSelection = showSelection;
		displayListsValid = false;
	}
}

void OpenGLRendererWidget::handleSelectionChanged (SurfelInterface::Flag) {
	displayListsValid = false;
}

bool OpenGLRendererWidget::isSelectionVisualizationEnabled() const {
	return bShowSelection;
}

// *****************
// protected methods
// *****************

void OpenGLRendererWidget::initializeGL() {

	lightDirection = Configuration::getInstance()->getLightDirection();
	
    this->updateLightDirection (false);
    glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);

	// we need to resize the display list ID array, since there might already
	// be Objects loaded already
	this->allocateDisplayLists();

	// this call also updates the display lists
    this->setShadingEnabled (shadingEnabled);
  
    glEnable (GL_DEPTH_TEST);

	// background color(tm) by Matthias, as used in the SurfelGPRenderer
	//glClearColor (0.522, 0.596, 0.710, 0.0);
	glClearColor(backgroundColor.red() / 255.0f, backgroundColor.green() / 255.0f, backgroundColor.blue() / 255.0f, 0.0);

	if (rendererConfiguration->isSmoothPointEnabled() == true) {
		glEnable (GL_POINT_SMOOTH);
	}
	else {
		glDisable (GL_POINT_SMOOTH);
	}

	// set the view frustum
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (viewFrustum.fieldOfView, viewFrustum.aspectRatio,
		            viewFrustum.nearPlane,   viewFrustum.farPlane);
	glMatrixMode (GL_MODELVIEW);
	
    this->initGLExtensions();

    handleCutOffRadiusChanged(rendererConfiguration->getCutOffRadius());

}

#define  SET_VARPOINTSIZE(effectivelySetPointSize, coordinate, normal, radius)	\
do																				\
{																				\
	if (effectivelySetPointSize)												\
	{																			\
		glEnd();																\
		glPointSize(computePointSize(coordinate, normal, radius));				\
		glBegin (GL_POINTS);													\
		glNormal3f (normal[0], normal[1], normal[2]);							\
	}																			\
	else																		\
	{																			\
		if (effectivelyNormalEncodePointSize)									\
			glNormal3f((varPtSizNormalNomin/radius)*normal[0],					\
					   (varPtSizNormalNomin/radius)*normal[1],					\
					   (varPtSizNormalNomin/radius)*normal[2]);					\
        else																	\
			glNormal3f(normal[0], normal[1], normal[2]);						\
	}																			\
}																				\
while(0)

void OpenGLRendererWidget::paintGL() {

	uint                                 nofObjects,
		                                 i;
	MyDataTypes::TransformationMatrix16f objectTransformation,
		                                 finalTransformation;
	Scene                                *scene;
	Object                               *object,
		                                 *activeObject;
	
	SurfelCollection                     *surfelCollection;
	SurfelInterface                      *surfel;
	Vector3D                             coordinate,
	                                     normal;
	QRgb                                 diffuseColor;
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateInternalFlags();  // set up internal flags derived from configuration
	
	scene = Scene::getInstance();
	nofObjects = scene->getNofObjects();
	if (nofObjects == 0)
	  return;

    /////////////////////////////////////////////////////////////////////////////////

	//    qDebug("smooth: %d; Use display lists: %d; Use vertex shaders: %d", smoothPointChanged, rendererConfiguration->isUseDisplayListsEnabled());
	bool  effectivelyUseDisplayLists = displayListsPossible;
    if (getModelInteractState() != RendererInterface::INTERACT_STATIC)
	  {
		displayListsValid = false;
		effectivelyUseDisplayLists = false;
	  }

    bool  effectivelyUseVertexShaders = false;
	bool  effectivelySetPointSize = false;
	bool  effectivelyNormalEncodePointSize = false;

    if (!rendererConfiguration->isConstantPointSizeEnabled())
      {
        effectivelyUseVertexShaders = rendererConfiguration->isUseVertexShadersEnabled() && vertexProgId;
        if (effectivelyUseVertexShaders)
          effectivelyNormalEncodePointSize = true;
        else if (effectivelyUseDisplayLists)
          qDebug("%s:%d: NEVER REACHED", __FILE__, __LINE__);
        else
          effectivelySetPointSize = true;
      }
		
    bool  effectivelySendPrimitives = false;
    bool  effectivelyCallDisplayList = false;
    bool  effectivelyDefineDisplayList = false;

    bool  displayListCompileAndExecute = false;

    if (effectivelyUseDisplayLists)
      if (displayListsValid)
        effectivelyCallDisplayList = true;
      else
        {
          effectivelyDefineDisplayList = true;
          effectivelySendPrimitives = true;
        }
    else
      effectivelySendPrimitives = true;

    /////////////////////////////////////////////////////////////////////////////////

    // reset the point size and shape
	glPointSize(1.f);
    if (rendererConfiguration->isSmoothPointEnabled())
      glEnable(GL_POINT_SMOOTH);
    else
      glDisable(GL_POINT_SMOOTH);
    if (rendererConfiguration->isConstantPointSizeEnabled())
      // Only for constant sizes to prevent nvidia driver to temporarily freeze for too large points sizes:
	  glPointSize(rendererConfiguration->getCutOffRadius());
	
    //qDebug("setPS: %d; normalENC: %d", effectivelySetPointSize, effectivelyNormalEncodePointSize);

	activeObject = scene->getActiveObject();
	for (object = scene->getFirstObject(), i = 0; object != 0; object = scene->getNextObject(), i++)
	  {
		if (object->isFlagOn (Object::VISIBLE))
		  {
            //qDebug("useDL: %d; defDL: %d, callDL: %d, Send: %d",
            //       effectivelyUseDisplayLists, effectivelyDefineDisplayList, effectivelyCallDisplayList, effectivelySendPrimitives);

			object->getTransformationMatrix (objectTransformation);
			MtrMult4x4f (sceneViewMatrix, objectTransformation, finalTransformation);
			glLoadMatrixf (finalTransformation);
			
			// Set up parameters for variable point sizes (used with and
			// without display lists as well)
			//if (!constantPointSize) {
			if (rendererConfiguration->isConstantPointSizeEnabled() == false)
			  {
				MyDataTypes::TransformationMatrix16f  finTransfInv;
				Vector3D                              pov;
				MtrInverse4x4f(finalTransformation, finTransfInv);
				MtrMultVector3D(finTransfInv, Vector3D (0.0f, 0.0f, 0.0f), &pov);
				this->computePointSizeBegin(finalTransformation, viewFrustum,
											pov[0], pov[1], pov[2]);
			  }

            if (effectivelyCallDisplayList)
	          glCallList (surfelCollectionDisplayLists.at(i));

            if (effectivelyDefineDisplayList)
              {
                qDebug("defining %d...", surfelCollectionDisplayLists.at(i));
                glNewList (surfelCollectionDisplayLists.at(i), displayListCompileAndExecute ? GL_COMPILE_AND_EXECUTE : GL_COMPILE);
              }

            if (effectivelySendPrimitives)
			  {
			    surfelCollection = object->getSurfelCollection();
				  
			    // ****************************
			    // calculate point size & paint
			    // ****************************
				  
			    activeObject = object;
				  
			    if (object != activeObject) {
					  
				  // ************************
				  // render the active object
				  // ************************
					  
				  if (glIsEnabled (GL_LIGHTING) == GL_TRUE) {
						  
				    glEnable (GL_COLOR_MATERIAL);
				    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
				    glBegin (GL_POINTS);			
						  
				    for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {
							  
					  coordinate   = surfel->getPosition();
					  normal       = surfel->getNormal();
							  
					  SET_VARPOINTSIZE(effectivelySetPointSize, coordinate, normal, surfel->getRadius());

					  showSelection(surfel, 0.5f, 0.5f, 0.5f);
					  
					  glVertex3f (coordinate[0], coordinate[1], coordinate[2]);
							  
				    }
			 		  
				    glEnd();
				    glDisable (GL_COLOR_MATERIAL);
						  
				  }
				  else {
						  
				    glBegin (GL_POINTS);
						  
				    for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {
							  
					  coordinate   = surfel->getPosition();
					  normal       = surfel->getNormal();
							  
					  SET_VARPOINTSIZE(effectivelySetPointSize, coordinate, normal, surfel->getRadius());
					  // Tim: Used to be "glColor3ub (0.5f, 0.5f, 0.5f);", but I think, Oli meant:

					  glColor3f (0.5f, 0.5f, 0.5f);

					  showSelection(surfel, 0.5f, 0.5f, 0.5f);
					  
					  glVertex3f (coordinate[0], coordinate[1], coordinate[2]);
							  
				    }
						  
				    glEnd();
						  
				  }
					  
			    }
			    else {
					  
				  // ************************
				  // render the active object
				  // ************************
					  
				  if (glIsEnabled (GL_LIGHTING))
				    {
						  
					  glEnable (GL_COLOR_MATERIAL);
					  glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
						  
					  glBegin (GL_POINTS);			
						  
					  for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {
							  
					    coordinate   = surfel->getPosition();
					    normal       = surfel->getNormal();
					    diffuseColor = surfel->getDiffuseColor();
							  
					    SET_VARPOINTSIZE(effectivelySetPointSize, coordinate, normal, surfel->getRadius());

  					    showSelection(surfel, qRed (diffuseColor) / 255.0f, qGreen(diffuseColor) / 255.0f, qBlue (diffuseColor) / 255.0f);

						glVertex3f (coordinate[0], coordinate[1], coordinate[2]);
							  
					  }
						  
					  glEnd();
					  glDisable (GL_COLOR_MATERIAL);
						  
				    }
				  else {
						  
				    glBegin (GL_POINTS);
						  
				    for (surfel = surfelCollection->getFirstSurfel(); surfel != 0; surfel = surfelCollection->getNextSurfel()) {
							  
					  coordinate   = surfel->getPosition();
					  normal       = surfel->getNormal();
					  diffuseColor = surfel->getDiffuseColor();
							  
					  SET_VARPOINTSIZE(effectivelySetPointSize, coordinate, normal, surfel->getRadius());

  					  showSelection(surfel, qRed (diffuseColor) / 255.0f, qGreen(diffuseColor) / 255.0f, qBlue (diffuseColor) / 255.0f);

					  glVertex3f (coordinate[0], coordinate[1], coordinate[2]);
							  
				    }
						  
				    glEnd();
						  
				  }
					  
			    }  // active object?
				  
			  }
              
            if (effectivelyDefineDisplayList)
              {
		        glEndList();
	            displayListsValid = true;

                if (!displayListCompileAndExecute)
	              glCallList(surfelCollectionDisplayLists.at(i));
              }
		
			// if (!constantPointSize)
			if (rendererConfiguration->isConstantPointSizeEnabled() == false)
			  this->computePointSizeEnd();
		  }
	  }

	// **********************************************************
	// * Callback - Extension
	glLoadMatrixf(sceneViewMatrix);
	RendererInterface::callPostRenderCBs();

	glFinish();

	emit repainted();

}

void OpenGLRendererWidget::resizeGL (int width, int height) {

	Configuration            *configuration;

    glViewport (0, 0, width, height);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    {
	  configuration = Configuration::getInstance();
	  if (updateViewPortSize == true && visible == true) {
		  configuration->setViewPortSize (QSize (width, height));
	  }
    }
}

void OpenGLRendererWidget::mousePressEvent (QMouseEvent *me) {	
	DefaultMouseHandler::handleMousePressEvent (me);
}

void OpenGLRendererWidget::mouseMoveEvent (QMouseEvent *me) {
	DefaultMouseHandler::handleMouseMoveEvent (me);
}

void OpenGLRendererWidget::mouseReleaseEvent (QMouseEvent *me) {
	DefaultMouseHandler::handleMouseReleaseEvent (me);
}

void OpenGLRendererWidget::keyPressEvent (QKeyEvent *ke) {
	ke->ignore();
}

void OpenGLRendererWidget::wheelEvent (QWheelEvent *we) {
	DefaultMouseHandler::handleMouseWheelEvent (we);
}

// ***************
// private methods
// ***************

void OpenGLRendererWidget::updateLightDirection (const bool updateScene) {

	GLfloat glLightPosition[4];   
	
	// make it the current OpenGL rendering context
	this->makeCurrent();

    // We get a direction, so set the light source far away...
	glLightPosition[0] = -10000.f * lightDirection[0];
	glLightPosition[1] = -10000.f * lightDirection[1];
	glLightPosition[2] = -10000.f * lightDirection[2];
	glLightPosition[3] = 1.0f;

	// put OpenGL light at absolute position 'glLightPosition' (not relative to current model view matrix)
	glPushMatrix();
	glLoadIdentity();
    glLightfv (GL_LIGHT0, GL_POSITION, glLightPosition);
	glPopMatrix();

	if (updateScene == true) {
		this->update();
	}

}

void OpenGLRendererWidget::allocateDisplayLists() {

	uint   nofObjects,
		   i;
	GLuint listID;

	// important to make this the current OpenGL rendering context
	this->makeCurrent();		

	// delete the old display lists, if any
	this->removeDisplayLists();

	nofObjects = Scene::getInstance()->getNofObjects();

	// allocate/deallocate space for the new display lists
	surfelCollectionDisplayLists.resize (nofObjects);

	// allocate new list identifiers
	listID = glGenLists (nofObjects);

	// initialize identifiers for display lists, start numbering listID
	for (i = 0; i < nofObjects; i++) {
		surfelCollectionDisplayLists.at(i) = listID + i;
	}

	// allocated, but not yet created
	displayListsValid = false;

}

void OpenGLRendererWidget::removeDisplayLists()
{
  // delete the OpenGL display lists, if any
  if (surfelCollectionDisplayLists.size() > 0)
	glDeleteLists (surfelCollectionDisplayLists.at(0), surfelCollectionDisplayLists.size());
  displayListsValid = false;
}

QImage OpenGLRendererWidget::renderPixmap() {

    QImage res;
    int w = this->size().width();
    int h = this->size().height();

	res = QImage( w, h, 32 );
	glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, res.bits() );
	if ( QImage::systemByteOrder() == QImage::BigEndian ) {
	    // OpenGL gives RGBA; Qt wants ARGB
	    uint *p = (uint*)res.bits();
	    uint *end = p + w*h;
		while ( p < end )
		    *p++ >>= 8;
	}
    res = res.swapRGB();
	return res.mirror();
}

///////////////////////////////////////////////////////////////////////////////
//  Tim's stuff, that will be outsourced soon...
//

static char  *vertexProgString =
#include "varptsiz.vp.h"
;

// this includes will be removed together with other debugging code...
#include <stdio.h>   // for fopen()

#ifdef DEBUG_VERTEX_SHADER
static char  *readTextFile(const char *fileName)
{
  static char  buf[65536];
  
  FILE  *in = fopen(fileName, "r");
  if (in)
    {
      long  bytes;
      bytes = fread(buf, 1, 65536, in);
      buf[bytes] = 0;
      fclose(in);
      return buf;
    }
  else
    {
      qDebug("%s:%d: Couldn't read vertex program `%s' from disk\n",
			 __FILE__, __LINE__, fileName);
      return NULL;
    }
}
#endif

void  OpenGLRendererWidget::updateInternalFlags(void)
{
  bool  oldDisplayListsPossible = displayListsPossible;
  displayListsPossible = (rendererConfiguration->isUseDisplayListsEnabled() &&
						  (rendererConfiguration->isConstantPointSizeEnabled() ||
						   (rendererConfiguration->isUseVertexShadersEnabled() && haveNVVertexProgram)));
  //qDebug("%d --> %d", oldDisplayListsPossible, displayListsPossible);
  if (oldDisplayListsPossible && !displayListsPossible)
	{
      displayListsValid = false;
	}
}

void  OpenGLRendererWidget::initGLExtensions(void)
{
#ifdef GL_NV_vertex_program
  if (!initializedExtensions)
    {
      haveNVVertexProgram = false;
	  
#  if defined( _WIN32 )
      // Initialize vertex shader extension
      if (glh_init_extension("GL_NV_vertex_program"))
		{
		  // GL_NV_vertex_program extension found
#    if !defined( NDEBUG )
		  qDebug("Library supports GL_NV_vertex_program.\n");
#    endif
		  haveNVVertexProgram = true;
		}
#  else
      {
#    ifdef GL_NV_vertex_program
		// We just assume, GL_NV_vertex_program exists, because the header knew it ;-)
		//   (which actually is a bad idea on remote connections...)
		haveNVVertexProgram = false/*commented out (we had some problems with NVGL): true*/;
#    else
		haveNVVertexProgram = false;
#    endif
      }
#  endif
      
      initializedExtensions = true;

      //
      // And now, already bind vertex program
      //
      if (haveNVVertexProgram)
		{
		  glGenProgramsNV( 1, &vertexProgId );
		  if (!vertexProgId)
			qDebug("%s:%d: Warning: Couldn't allocate vertex program\n",
					__FILE__, __LINE__);
		  else
			{
			  qDebug("Got vertex program id %d\n", vertexProgId);

			  glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgId);
	  
			  qDebug("-- Loaded Vertex Program ------------------------------------------------------\n");
			  qDebug("%s", vertexProgString);
			  qDebug("-------------------------------------------------------------------------------\n");
	  
			  glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgId,
							  strlen(vertexProgString), (GLubyte *)vertexProgString);

			  // Paranoia:
			  glDisable(GL_VERTEX_PROGRAM_NV);
			  glDisable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
			}
		}
      else
		qDebug("No GL_NV_vertex_program extension found.\n");
	  
	  this->handleConstantPointSizeChanged (rendererConfiguration->isConstantPointSizeEnabled()); // because we might have changed some booleans :-(
    }
#endif  // GL_NV_vertex_program
}

void  OpenGLRendererWidget::computePointSizeBegin(const MyDataTypes::TransformationMatrix16f finalTransformation,
												  const MyDataTypes::ViewFrustum viewFrustum,
												  GLfloat lightX, GLfloat lightY, GLfloat lightZ,
												  const GLfloat *lightAmbient,
												  const GLfloat *lightDiffuse,
												  const GLfloat *lightSpecular)
{
  (void)lightX; (void)lightY; (void)lightZ;

  GLint vp[4];

  // **************************
  // init point size parameters
  // **************************

  // init vertex program variables:
  glGetIntegerv (GL_VIEWPORT, &vp[0]);

  //const Transform  I = camIntr;
  //const MatrixP3  &M = I.getMatrix();
  //const float     fx = fabs(M.v[0][0]) * vp[2];  // fy * aspectRatio
  //const float     fy = fabs(M.v[1][1]) * vp[3];  // tan (phi / 2)
  float fy = (float)tan(viewFrustum.fieldOfView / 2.0);
  float fx = fy / viewFrustum.aspectRatio;
  fx = (float)fabs (fx) * 0.5f*vp[2];
  fy = (float)fabs (fy) * 0.5f*vp[3];
#if 0
  // if you've got paranoia:
  const float f  = fx < fy ? fx : fy;
  varPtSizF2     = f * f;
#else
  // theoretically the best:
  varPtSizF2 = fx * fy;
#endif

  //varPtSizExtrTransf    = camExtr.getMatrix();
  //varPtSizExtrTransfInv = camExtr.getInvMatrix();
  MtrCopy4x4f (finalTransformation, varPtSizExtrTransf);
  MtrInverse4x4f (varPtSizExtrTransf, varPtSizExtrTransfInv);

  // 1st row
  varPtSizExtrTransfR[0][0] = varPtSizExtrTransf[0];
  varPtSizExtrTransfR[0][1] = varPtSizExtrTransf[4];
  varPtSizExtrTransfR[0][2] = varPtSizExtrTransf[8];

  // 2nd row
  varPtSizExtrTransfR[1][0] = varPtSizExtrTransf[1];
  varPtSizExtrTransfR[1][1] = varPtSizExtrTransf[5];
  varPtSizExtrTransfR[1][2] = varPtSizExtrTransf[9];

  // 3th row
  varPtSizExtrTransfR[2][0] = varPtSizExtrTransf[2];
  varPtSizExtrTransfR[2][1] = varPtSizExtrTransf[6];
  varPtSizExtrTransfR[2][2] = varPtSizExtrTransf[10];

  // 3th column
  varPtSizExtrTransfT[0]    = varPtSizExtrTransf[12];
  varPtSizExtrTransfT[1]    = varPtSizExtrTransf[13];
  varPtSizExtrTransfT[2]    = varPtSizExtrTransf[14];

  //MtrMultVector3D (varPtSizExtrTransfInv, Vector3D (0.0f, 0.0f, 0.0f), varPtSizExtrTransfPOV);
  varPtSizExtrTransfPOV[0] = varPtSizExtrTransfInv[12]/varPtSizExtrTransfInv[15];
  varPtSizExtrTransfPOV[1] = varPtSizExtrTransfInv[13]/varPtSizExtrTransfInv[15];
  varPtSizExtrTransfPOV[2] = varPtSizExtrTransfInv[14]/varPtSizExtrTransfInv[15];
  //qDebug("POV: [%g, %g, %g]", varPtSizExtrTransfPOV[0], varPtSizExtrTransfPOV[1], varPtSizExtrTransfPOV[2]);
  
  CHECK_FOR_GL_ERROR();
#ifdef GL_NV_vertex_program
  if (rendererConfiguration->isConstantPointSizeEnabled() == false && rendererConfiguration->isUseVertexShadersEnabled() && vertexProgId) 
    {
      const GLfloat  defaultLightAmbient[4]  = {0.2f, 0.2f, 0.2f, 1.0f};
      const GLfloat  defaultLightDiffuse[4]  = {0.6f, 0.6f, 0.6f, 1.0f};
      const GLfloat  defaultLightSpecular[4] = {0.0f, 0.0f, 0.0f, 0.0f};

      const GLfloat  *lightAmb  = lightAmbient  ? lightAmbient : defaultLightAmbient;
      const GLfloat  *lightDiff = lightDiffuse  ? lightDiffuse : defaultLightDiffuse;
      const GLfloat  *lightSpec = lightSpecular ? lightSpecular : defaultLightSpecular;

	  CHECK_FOR_GL_ERROR();
#ifdef DEBUG_VERTEX_SHADER
      glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgId);
#  if 0
      vertexProgString = readTextFile("c:\\SourceSafe\\PointShop3D\\src\\Renderers\\OpenGLRenderer\\src\\varptsiz.vp");
#  elif 1
      vertexProgString = readTextFile("c:\\home\\tknoll\\PointShop3D\\src\\Renderers\\OpenGLRenderer\\src\\varptsiz.vp");
#  else
      vertexProgString = readTextFile("/home/weyrich/src/cvs/pcrenderer/pcrenderer/varptsiz.vp");
#  endif
      if (vertexProgString)
		glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgId,
						strlen(vertexProgString), (GLubyte *)vertexProgString);
#endif
	  CHECK_FOR_GL_ERROR();
      
      glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertexProgId);
      
	  CHECK_FOR_GL_ERROR();

      glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV,		GL_IDENTITY_NV);
      glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4,               GL_MODELVIEW,		GL_INVERSE_TRANSPOSE_NV);
#if 1
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV,  8,
							 finalTransformation[0+4*0],
							 finalTransformation[0+4*1],
							 finalTransformation[0+4*2],
							 finalTransformation[0+4*3]);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV,  9,
							 finalTransformation[1+4*0],
							 finalTransformation[1+4*1],
							 finalTransformation[1+4*2],
							 finalTransformation[1+4*3]);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 10,
							 finalTransformation[2+4*0],
							 finalTransformation[2+4*1],
							 finalTransformation[2+4*2],
							 finalTransformation[2+4*3]);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 11,
							 finalTransformation[3+4*0],
							 finalTransformation[3+4*1],
							 finalTransformation[3+4*2],
							 finalTransformation[3+4*3]);
      
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 12,
							 finalTransformation[0+4*0],
							 finalTransformation[0+4*1],
							 finalTransformation[0+4*2],
							 finalTransformation[0+4*3]);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 13,
							 finalTransformation[1+4*0],
							 finalTransformation[1+4*1],
							 finalTransformation[1+4*2],
							 finalTransformation[1+4*3]);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 14,
							 finalTransformation[2+4*0],
							 finalTransformation[2+4*1],
							 finalTransformation[2+4*2],
							 finalTransformation[2+4*3]);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 15,
							 finalTransformation[3+4*0],
							 finalTransformation[3+4*1],
							 finalTransformation[3+4*2],
							 finalTransformation[3+4*3]);
#else
      glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 8,               GL_MODELVIEW,		GL_IDENTITY_NV);
      glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 12,             GL_PROJECTION,		GL_IDENTITY_NV);
#endif

	  // Directional Light: Set light vector
	  {
		Vector3D   p = Configuration::getInstance()->getLightDirection();

		MyDataTypes::TransformationMatrix16f  extrTransfT;
		MtrTranspose4x4f (varPtSizExtrTransf, extrTransfT);
		
#if 1
		p = -p;  // negate light-direction to achieve appropriate light-vector
#endif
		
        Vector3D  q;
      	q[0] = (varPtSizExtrTransf[0]*p[0] + varPtSizExtrTransf[1]*p[1] + varPtSizExtrTransf[2]*p[2]
      			+ varPtSizExtrTransf[3]);
      	q[1] = (varPtSizExtrTransf[4]*p[0] + varPtSizExtrTransf[5]*p[1] + varPtSizExtrTransf[6]*p[2]
      			+ varPtSizExtrTransf[7]);
      	q[2] = (varPtSizExtrTransf[8]*p[0] + varPtSizExtrTransf[9]*p[1] + varPtSizExtrTransf[10]*p[2]
      			+ varPtSizExtrTransf[11]);
		q.normalize();
		
		glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 32,
							   q[0], q[1], q[2], 0.0f);	// light \e direction in object coordinates

		glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 34, lightAmb[0], lightAmb[1], lightAmb[2],
							   lightAmb[3]);	// ambient color
		glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 35, lightDiff[0], lightDiff[1], lightDiff[2], lightDiff[3]);	// diffuse color
		glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 36, lightSpec[0], lightSpec[1], lightSpec[2], lightSpec[3]);	// specular color
	  }


      // Variable Point-Size Stuff
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 64,
							 varPtSizExtrTransfT[0], varPtSizExtrTransfT[1], varPtSizExtrTransfT[2], 0.0f);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 65,
							 varPtSizExtrTransfPOV[0], varPtSizExtrTransfPOV[1], varPtSizExtrTransfPOV[2], 1.0f);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 68, varPtSizF2, varPtSizF2, varPtSizF2, varPtSizF2);

      // some useful constants
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 50, -1.0f, 0.0f, 1.0f, 2.0f);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 51, 1.0f/2, 1.0f/3, 1.0f/4, 3.0f/4);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 52, 3.0f, 4.0f, 5.0f, 6.0f);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 53, 7.0f, 8.0f, 9.0f, 10.0f);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 54, 15.f, 21.f, 27.f, 33.f);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 55, 66.f, 100.f, 150.f, 200.f);
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 56, .15f, .1f, .05f, .025f);
	
      // testing-constants
      glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 57, 100.0f, 0.0f, 0.0f, 0.0f);

	  CHECK_FOR_GL_ERROR();
      // Yeah!
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
      glEnable(GL_VERTEX_PROGRAM_NV);
      
	  CHECK_FOR_GL_ERROR();
    }
#endif  // GL_NV_vertex_program
}

void  OpenGLRendererWidget::computePointSizeEnd(void)
{
  CHECK_FOR_GL_ERROR();
#ifdef GL_NV_vertex_program
  if (rendererConfiguration->isConstantPointSizeEnabled() == false && rendererConfiguration->isUseVertexShadersEnabled() && vertexProgId)
    {
      glDisable(GL_VERTEX_PROGRAM_NV);
  CHECK_FOR_GL_ERROR();
      glDisable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
    }
#endif
  CHECK_FOR_GL_ERROR();
}

float  OpenGLRendererWidget::computePointSize(const Vector3D point, const Vector3D &normal, float radius)
{
  (void)normal;  // because it's only used if back-face culling is enabled

#ifdef CULLING
  if (dotProduct ((varPtSizExtrTransfPOV - point), normal) < 1e-6) {
	return 0.0f;
  }
#endif

  // Compute optimized (we know, it's rigid):  pViewLocal = varPtSizExtrTransf * p;
  const Vector3D  pViewLocal (Vector3D::dotProduct (varPtSizExtrTransfR[0], point) + varPtSizExtrTransfT[0],
							  Vector3D::dotProduct (varPtSizExtrTransfR[1], point) + varPtSizExtrTransfT[1],
							  Vector3D::dotProduct (varPtSizExtrTransfR[2], point) + varPtSizExtrTransfT[2]);
  const float             z = pViewLocal[2];
  const Vector3D      viewD = ((varPtSizF2 / (z * z * z)) * pViewLocal);

  float          size = sqrt(viewD.getLength()) * 2.0f * radius;

  size = size / (2 * varPtSizNormalNomin);
  return size < 1.0f ? 1.0f : size;

}

//
//  End of Tim's stuff, that will be outsourced soon...
///////////////////////////////////////////////////////////////////////////////


// *************
// private slots
// *************

void OpenGLRendererWidget::handleObjectModified () {
	displayListsValid = false;
}

void OpenGLRendererWidget::handleActiveObjectChanged() {
	displayListsValid = false;
}

void OpenGLRendererWidget::handleObjectAdded() {
	this->allocateDisplayLists();
}

void OpenGLRendererWidget::handleObjectRemoved() {
	this->allocateDisplayLists();
}

void OpenGLRendererWidget::handleCutOffRadiusChanged (const float newCutOffRadius) {
  
  (void)newCutOffRadius;
  
#if 0
	varPtSizNormalNomin = NORMAL_NOMINATOR;
#else
    //qDebug("================================== Cut off radius: %g", rendererConfiguration->getCutOffRadius());
	varPtSizNormalNomin = NORMAL_NOMINATOR / rendererConfiguration->getCutOffRadius();
#endif

  if (!rendererConfiguration->isConstantPointSizeEnabled() &&
      rendererConfiguration->isUseVertexShadersEnabled() && haveNVVertexProgram && displayListsPossible)
    displayListsValid = false;
}

void OpenGLRendererWidget::handleConstantPointSizeChanged (const bool enabled)
{
  displayListsValid = false;
  updateInternalFlags();
  this->update();
}

void OpenGLRendererWidget::handleSmoothPointChanged (const bool enabled)
{
  updateInternalFlags();
  this->update();
}


void OpenGLRendererWidget::handleUseDisplayListsChanged (const bool enabled)
{
  updateInternalFlags();
  this->update();
}

void OpenGLRendererWidget::handleUseVertexShadersChanged (const bool enabled)
{
  displayListsValid = false;
  updateInternalFlags();
  this->update();
}

void OpenGLRendererWidget::handleConfigurationApplied()
{
  displayListsValid = false;
  updateInternalFlags();
  this->update();
}

// interaction stuff //////////////////////////////////////////////////////////

RendererInterface::InteractStateEnum  OpenGLRendererWidget::getModelInteractState(void) const
{
  return modelInteractState;
}

RendererInterface::InteractStateEnum  OpenGLRendererWidget::getViewpointInteractState(void) const
{
  return viewpointInteractState;
}

void  OpenGLRendererWidget::setInteractState(RendererInterface::InteractStateEnum state,
											 RendererInterface::InteractStateAspectEnum target)
{
  if (target & RendererInterface::INTERACT_MODEL)      modelInteractState = state;
  if (target & RendererInterface::INTERACT_VIEWPOINT)  viewpointInteractState = state;
}

///////////////////////////////////////////////////////////////////////////////


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
