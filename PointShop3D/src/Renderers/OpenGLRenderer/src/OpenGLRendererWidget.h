//
// Title:   OpenGLRendererWidget.h
// Created: Thu Sep 25 15:44:44 2003
// Authors: Oliver Knoll, Mark Pauly, Tim Weyrich, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003
// Computer Graphics Lab, ETH Zurich
// Mitsubishi Electric Research Laboratories (MERL), Cambridge MA
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
// IN NO EVENT SHALL MERL AND/OR ETH BE LIABLE TO ANY PARTY
// FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
// SOFTWARE AND ITS DOCUMENTATION, EVEN IF MERL OR ETH HAS BEEN
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
// 
// MERL AND ETH SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
// HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER MERL NOR ETH HAVE
// ANY OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// ENHANCEMENTS, OR MODIFICATIONS.
// 
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

#ifndef _OPENGLRENDERERWIDGET_H_
#define _OPENGLRENDERERWIDGET_H_

#include <qgl.h>
#include <qevent.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../Core/Interfaces/src/RendererInterface.h"
#include "OpenGLRendererConfiguration.h"

/**
 * This class implements an OpenGL preview renderer with dynamic point size.
 *
 * @author Oliver Knoll, Tim Weyrich
 * @version 1.2
 */
class OpenGLRendererWidget : public QGLWidget
{
	Q_OBJECT

public:

	OpenGLRendererWidget (const bool isVisible, OpenGLRendererConfiguration *rendererConfiguration, QWidget *parent = 0, const char *name = 0);
	virtual ~OpenGLRendererWidget();

	void setVisible (const bool enable);
	bool isVisible() const;

	QSize sizeHint() const;
	QSize minimumSizeHint() const;
	QSizePolicy sizePolicy() const;

	/**
	 * Enables/disables the lighting in OpenGL. The
	 * image is <em>not</em> re-rendered automatically.
	 *
	 * @param enable
	 *        set to <code>true</code> if lighting should be enabled; <code>false</code> else
	 */
	void setShadingEnabled (const bool enable);

	/**
	 * Returns the status of the lighting in OpenGL.
	 *
	 * @return <code>true</code> if lighting in OpenGL is enabled; <code>false</code> else
	 */
	bool isShadingEnabled() const;

	/**
	 * set the direction of the light
	 */
	void setLightDirection (const Vector3D newLightDirection);

	/**
	 * returns the direction of the light
	 */
	Vector3D getLightDirection() const;

	/**
	 * sets the background color
	 */
	void setBackgroundColor(QRgb newBackgroundColor);

	/**
	 * returns the actual background color
	 */
	QRgb getBackgroundColor() const;

	/**
	 * Enables/disables constant point size in OpenGL.
	 *
	 * @param enable
	 *        if set to <code>true</code> a constant point size is used for each GL_POINT;
	 *        if set to <code>false</code> the point size is calculated dynamically for each GL_POINT
	 */
	//void setConstantPointSizeEnabled (const bool enable);

	/**
	 * Returns <code>true</code> if a constant point size is used for each GL_POINT, <code>false</code>
	 * if the point size is calculated dynamically.
	 *
	 * @return <code>true</code> if a constant point size is used for each GL_POINT; <code>false</code>
	 *         if the point size is calculated dynamically
	 */
	//bool isConstantPointSizeEnabled() const;
	
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
	void setSceneView (const MyDataTypes::TransformationMatrix16f newSceneViewMatrix);

	/**
	 * Returns the projection matrix of the <code>Scene</code> which will be used for the next rendering operation.
	 *
	 * @param sceneViewMatrix
	 *        a <code>TransformationMatrix16f</code> where the projection matrix of the <code>Scene</code>
	 *        will be stored into
	 */
	void getSceneView (MyDataTypes::TransformationMatrix16f sceneViewMatrix) const;

	/**
	 * Sets the <code>ViewFrustum</code> which will be used for the next rendering operation.
	 *
	 * @param newViewFrustum
	 *        the <code>ViewFrustum</code> which will be used for the next rendering operation
	 * @see #renderFrame
	 */
	void setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum);

	/**
	 * Returns the <code>viewFrustum</code> which will be used for the next rendering operation.
	 *
	 * @return the <code>ViewFrustum</code> which will be used for the next rendering operation
	 * @see #renderFrame
	 */
	MyDataTypes::ViewFrustum getViewFrustum() const;

	/**
	 * set the size of the view port
	 */
	void setViewPortSize (const QSize newViewPortSize);
	
	/**
	 * returns the size of the view port
	 */
	QSize getViewPortSize() const;

	/**
	 * Set if the selection should be rendered
	 *
	 * @param showSelection
	 *			true if the selected surfels should be rendered differently
	 */
	void setSelectionVisualizationEnabled(const bool showSelection);

	/**
	 * Returns whether the selected surfels are rendered differently
	 */
	bool isSelectionVisualizationEnabled() const;
	
signals:

	/**
	 * Emitted whenever this <code>OpenGLRendererWidget</code> has repainted its content.
	 *
	 * <b>Note:</b> Connected to the <code>OpenGLRenderer</code> which on its turn emits
	 *              the signal <code>rendererWidgetRepainted</code>.
	 */
	void repainted();

protected:

	void mousePressEvent (QMouseEvent *me);
	void mouseMoveEvent (QMouseEvent *me);
	void mouseReleaseEvent (QMouseEvent *me);
	void keyPressEvent (QKeyEvent *ke);
	void wheelEvent (QWheelEvent *we);
	
	void initializeGL();
	void resizeGL (int width, int height);
	void paintGL();

private:

	friend class OpenGLRenderer;

	OpenGLRendererConfiguration          *rendererConfiguration;
	Vector3D                             lightDirection;	
	
	std::vector<GLuint>                  surfelCollectionDisplayLists;  // each surfel collection goes into
	                                                                    // its own display list
	MyDataTypes::TransformationMatrix16f sceneViewMatrix;
	MyDataTypes::ViewFrustum             viewFrustum;
	bool                                 shadingEnabled,
		                                 updateViewPortSize,            // true whenever the view port size
	                                                                    // in the Configuration should be updated
	                                     displayListsValid,
										 visible;

	QColor								 backgroundColor;

    // Variables used by Tim's variable point-size splatter; probably subject
    // to a future class of its own:
    //

    // internal flags derived from current configuration
    bool                                 displayListsPossible;			// true, if the current 
                                                                        // configuration allows display lists

	/*static*/ bool                          initializedExtensions;			// whether GL-extensions were initialized
    bool                                 haveNVVertexProgram;           // whether the GL_NV_vertex_program
																		// extension is available
    GLuint                               vertexProgId;					// id of the current vertex program

    float								 varPtSizNormalNomin;			// normal length nominator
  	MyDataTypes::TransformationMatrix16f varPtSizExtrTransf;			// vertex program parameters 8..11
  	MyDataTypes::TransformationMatrix16f varPtSizExtrTransfInv;			// May I avoid it?
  	float                                varPtSizF2;					// vertex program parameter 68.xyzw
  	Vector3D                             varPtSizExtrTransfR[3];		// vertex program parameters 8..11
  	Vector3D                             varPtSizExtrTransfT;			// vertex program parameter 64
  	Vector3D                             varPtSizExtrTransfPOV;			// vertex program parameter 65

	bool								 bShowSelection;				// true if selected surfels should be rendered differently
    //
    // End of Tim-Stuff

	// updates the OpenGL light direction according to 'lightDirection'
	// updateScene: true, if scene should be re-rendered with the new light direction
	void updateLightDirection (const bool updateScene);
	
	// allocates GL display lists for each Object in the Scene - deletes old GL display lists, if any
	void allocateDisplayLists();

	// removes all GL display lists, if any
	void removeDisplayLists();

	// returns the framebuffer / makes a screenshot
	QImage renderPixmap();

    // Methods, that are used by Tim's point-splatter
    //

    //! Update internal flags derived from the current configuration.
    /*! Affected flags are: \c displayListsPossible.
	 */
    void  updateInternalFlags(void);
  
    void  initGLExtensions(void);
	//! Initializes the vertex program, or all variables used by computePointSize, respectively.
    void  computePointSizeBegin(const MyDataTypes::TransformationMatrix16f finalTransformation,
							  	const MyDataTypes::ViewFrustum viewFrustum,
							  	GLfloat lightX, GLfloat lightY, GLfloat lightZ,
							  	const GLfloat *lightAmbient = NULL,
							  	const GLfloat *lightDiffuse = NULL,
							  	const GLfloat *lightSpecular = NULL);
    //! Tidy up after a call of computePointSizeBegin().
    void  computePointSizeEnd(void);

	//! Computes the point size for \a point with \a normal and \a radius.
    /*! This point size can be used for glPointSize() -- this method must not be
	 *  called outside computePointSizeBegin() and computePointSizeEnd().
	 */
	float  computePointSize(const Vector3D point, const Vector3D &normal, float radius);

    //
    // End of Tim's methods

	inline void showSelection(SurfelInterface *surfel,float r_comp,float g_comp,float b_comp);
	inline void showSelection(SurfelInterface *surfel);

private slots:
	
	void handleObjectModified();
	void handleActiveObjectChanged();
	void handleObjectAdded();
	void handleObjectRemoved();	

	void handleCutOffRadiusChanged (const float newCutOffRadius);

	void handleConstantPointSizeChanged (const bool enabled);

	void handleSmoothPointChanged (const bool enabled);
	void handleUseDisplayListsChanged (const bool enabled);
	void handleUseVertexShadersChanged (const bool enabled);
	void handleSelectionChanged (SurfelInterface::Flag selectionFlag);


	// connected to the OpenGLRendererConfiguration, signal configurationApplied: re-renders the image
	void handleConfigurationApplied();


    // interact mode stuff; will be moved to class OpenGLRenderer
    // after reorganization...
private:
    RendererInterface::InteractStateEnum  modelInteractState;
    RendererInterface::InteractStateEnum  viewpointInteractState;
  
public:
    virtual RendererInterface::InteractStateEnum  getModelInteractState(void) const;
    virtual RendererInterface::InteractStateEnum  getViewpointInteractState(void) const;

    virtual void  setInteractState(RendererInterface::InteractStateEnum state,
				                   RendererInterface::InteractStateAspectEnum  target=RendererInterface::INTERACT_MODEL_AND_VIEWPOINT);
  
};

#endif  // _OPENGLRENDERERWIDGET_H_


inline void OpenGLRendererWidget::showSelection(SurfelInterface *surfel) {

	QColor tmp(surfel->getDiffuseColor());

	showSelection(surfel,	(float) tmp.red() / 255.0f,
							(float) tmp.green() / 255.0f,
							(float) tmp.blue() / 255.0f);
	

}
 

inline void OpenGLRendererWidget::showSelection(SurfelInterface *surfel,float r_comp,float g_comp,float b_comp) {
		
	// modify color if surfel is selected and visualization of the selection is enabled
	if(bShowSelection && surfel->isFlagOn (SurfelInterface::EMPHASISE) == true) {
		if (surfel->isFlagOn (SurfelInterface::SELECTED1) == true) {
			r_comp *= 0.5f;
			g_comp *= 0.5f;
			b_comp = b_comp * 0.5f + 122.0f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED2) == true) {
			r_comp = r_comp * 0.5f + 122.0f;
			g_comp *= 0.5f;
			b_comp *= 0.5f;
		}
		if (surfel->isFlagOn (SurfelInterface::SELECTED3) == true) {
			r_comp *= 0.5f;
			g_comp = g_comp * 0.5f + 122.0f;
			b_comp *= 0.5f;
		}
	}
	
	glColor3f (r_comp, g_comp, b_comp);
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
