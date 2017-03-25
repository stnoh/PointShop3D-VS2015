// Title:   Transformer.h
// Created: Thu Sep 25 14:27:42 2003
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

#ifndef __TRANSFORMER_H_
#define __TRANSFORMER_H_

#include <qobject.h>
#include <qsize.h>

#include "../../../DataTypes/src/Vector3D.h"
#include "../../../DataTypes/src/Vector2D.h"
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../src/CoreDLL.h"

/**
 * This class provides methods for transforming points and vectors between the different
 * coordinate systems used in Pointshop3D.
 *
 * <ul>
 *   <li>The viewport coordinate system represents pixel coordinates, where the origin is in
 *   the upper left corner of the rendering widget.</li>
 *   <li>Camera coordinates have their origin at the eye, i.e., the projection center. The camera
 *   coordinate system is a right handed system, the x-axis is the horizontal axis, the y-axis is 
 *   the vertical axis, and the z-axis has its negative direction pointing away from the viewer.</li>
 *   <li> Scene coordinates describe the scene as a whole. </li>
 *   <li> Individual objects are represented in their object coordinate systems. </li>
 * </ul>
 *
 * Object and scene coordinates are related to each other via the individual object transformation
 * of each object. Scene coordinates are mapped to camera coordinates using the scene transformation
 * matrix. Camera coordinates are transformed to viewport coordinates by perspective projection, the 
 * mapping also depends on the view frustum.
 *
 * Note that for a transformer object to work properly, the transformation matrices that relate the 
 * different spaces to each other have to be set appropriately by the user. The <code>Transformer</code>
 * does not automatically update them.
 *
 * @author Matthias Zwicker
 * @version 1.2
 */
class CORE_API Transformer : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates this <code>Transformer</code>.
	 *
	 * @param viewPortSize
	 * @param viewFrustum
	 * @param sceneTransformationMatrix
	 * @param objectTransformationMatrix
	 */
	Transformer (const QSize viewPortSize, const MyDataTypes::ViewFrustum viewFrustum,
		         const MyDataTypes::TransformationMatrix16f sceneTransformationMatrix,
				 const MyDataTypes::TransformationMatrix16f objectTransformationMatrix);
	virtual ~Transformer();

	/**
	 * Returns a special, global instance of this <code>Transformer</code> which is updated 
	 * by the <code>RendererManager</code> before the image is re-rendered whenever the 
	 * scene transformation matrix, the view frustum or the view port size change.
	 *
	 * @return a pointer to an instance of this <code>Transformer</code> which is connected to the
	 *         <code>Scene</code>
	 */
	static Transformer *getSceneTransformer();

	/**
	 * Destroys the instance of the scene transformer, if there is any.
	 *
	 * * @see #getSceneTransformer
	 */
	static void destroySceneTransformer();

	/**
	 * Returns the view port size which is to be used for all point transformation calculations.
	 *
	 * @return the <code>QSize</code> specifying the width and height of the view port in pixels
	 */ 
	QSize getViewPortSize() const;

	/**
	 * Returns the <code>ViewFrustum</code> which is used for all point transformation calculations.
	 *
	 * @return the <code>ViewFrustum</code> which is used for all point transformation calculations
	 */
	MyDataTypes::ViewFrustum getViewFrustum() const;

	/**
	 * Returns the current scene transformation matrix, which relates scene and camera coordinates.
	 */
	void getSceneTransformationMatrix (MyDataTypes::TransformationMatrix16f transformationMatrix) const;

	/**
	 * Returns the current object transformation matrix, which relates object and scene coordinates. Note
	 * that this matrix is different for every object. This method just returns the matrix that has
	 * been set by the user before (by calling <code>setObjectTransformationMatrix</code>).
	 */
	void getObjectTransformationMatrix (MyDataTypes::TransformationMatrix16f transformationMatrix) const;

	/**
	 * This method maps a point from object to viewport coordinates.
	 */
	Vector2D objectToViewPort (const Vector3D &point);

	/**
	 * This method maps a point from object to camera space.
	 */
	Vector3D objectToCamera (const Vector3D &point);

	/**
	 * This method maps a point from object to scene space.
	 */
	Vector3D objectToScene (const Vector3D &point);

	/**
	 * This method maps a point from viewport to object space.
	 */	
	Vector3D viewPortToObject (const Vector2D &point, const float depth);

	/**
	 * This method maps a point from viewport to camera space. This mapping depends on the geometry of the
	 * view frustum.
	 */	
	Vector3D viewPortToCamera (const Vector2D& point, const float depth);

	/**
	 * This method maps a point from viewport to scene space.
	 */
	Vector3D viewPortToScene (const Vector2D& point, const float depth);

	/**
	 * This method maps a point from scene to camera space.
	 */
	Vector3D sceneToCamera (const Vector3D& point);

	/** 
	 * This method maps a point from scene to object space.
	 */
	Vector3D sceneToObject (const Vector3D& point);

	/**
	 * This method maps a point from scene to viewport coordinates.
	 */
	Vector2D sceneToViewPort (const Vector3D& point);

	/**
	 * This method maps a point from camera to object space.
	 */
	Vector3D cameraToObject (const Vector3D &point);

	/**
	 * This method maps a point from camera to viewport space.
	 */
	Vector2D cameraToViewPort (const Vector3D& point);	

	/**
	 * This method maps a point from camera to scene space.
	 */
	Vector3D cameraToScene (const Vector3D& point);

	/**
	 * This method maps a vector from scene to camera space.
	 */
	Vector3D sceneToCameraVector(const Vector3D& vector);

	/**
	 * This method maps a vector from scene to object space.
	 */
	Vector3D sceneToObjectVector(const Vector3D& vector);

	/**
	 * This method maps a vector from camera to object space.
	 */
	Vector3D objectToCameraVector(const Vector3D& vector);

	/**
	 * This method maps a point from object to scene space.
	 */
	Vector3D objectToSceneVector(const Vector3D& vector);

	/**
	 * This method maps a vector from camera to object space.
	 */
	Vector3D cameraToObjectVector(const Vector3D& vector);

	/**
	 * This method maps a vector from camera to scene space.
	 */
	Vector3D cameraToSceneVector(const Vector3D& vector);

public slots:

	/**
	 * Sets the view port size which is to be used for all point transformation calculations.
	 *
	 * @param newViewPortSize
	 *        the <code>QSize</code> specifying the width and height of the view port in pixels
	 */
	 void setViewPortSize (const QSize newViewPortSize);

	/**
	 * Sets the <code>ViewFrustum</code> which is to be used for all point transformation calculations.
	 *
	 * @param newViewFrustum
	 *        the <code>ViewFrustum</code> which is to be used for all point transformation calculations
	 */
	void setViewFrustum (const MyDataTypes::ViewFrustum newViewFrustum);

	/**
	 * This method sets the scene transformation matrix which relates scene and camera space. It is used
	 * in all methods that involve a transformation between these two spaces.
	 */
	void setSceneTransformationMatrix (const float* newSceneTransformationMatrix);

	/**
	 * This method sets the object transformation matrix which relates object and camera space. It is used
	 * in all methods that involve a transformation between these two spaces. Note that each object has 
	 * its own object space, and therefore a different object transformation matrix.
	 */
	void setObjectTransformationMatrix (const float* newObjectTransformationMatrix);
	
private:

	QSize                                viewPortSize;
	MyDataTypes::ViewFrustum             viewFrustum;
	MyDataTypes::TransformationMatrix16f sceneTransformationMatrix,
		                                 objectTransformationMatrix,
										 transposedSceneTransformationMatrix,
										 transposedObjectTransformationMatrix,
										 inverseSceneTransformationMatrix,
										 inverseObjectTransformationMatrix,
										 transposedInverseSceneTransformationMatrix,
										 transposedInverseObjectTransformationMatrix;

	float                                viewPortScaling[2],
	                                     viewPortTranslation[2];

	// Update the viewport transformation using the current view frustum
	// and viewport.
	void updateViewPortTrafo();

};


#endif  // __TRANSFORMER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
