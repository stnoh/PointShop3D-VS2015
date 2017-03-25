// Title:   OrthoMapper.h
// Created: Thu Sep 25 14:28:47 2003
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

#ifndef __ORTHOMAPPER_H_
#define __ORTHOMAPPER_H_

#include <qpen.h>
#include <qpointarray.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../../Utilities/src/Matrix.h"

#include "../../RendererManager/src/RendererManager.h"
#include "../../Transformer/src/Transformer.h"
#include "../../Brush/src/BrushChannel.h"
#include "../../Interfaces/src/FrameBufferInterface.h"
#include "../UI/ImageViewer.h" 

/**
 * This class provides the functionality for parameterizing patches by orthogonal projection. 
 *
 * @author Mark Pauly
 * @version 1.2
 */

class OrthoMapper {

public:

	/**
	 * Shapes of the tool cursor
	 */
	typedef enum _cursorShape {
		PLAIN       = 0,
		WITH_DEPTH  = 1,
		TWO_SHAPES  = 2,
	} CursorShape;

    /**
	 * Status of the tool cursor
	 */
	typedef enum _cursorStatus {
		NOT_VALID   = 0,
		NOT_ACTIVE  = 1,
		ACTIVE      = 2
	} CursorStatus;

	/*
	 * Attributes that define the curor shape and can be altered using the mouse
	 */
	typedef enum _cursor {
		GLOBAL_SIZE   = 0,
		X_SIZE        = 1,
		Y_SIZE        = 2,
		Z_SIZE        = 3,
		ORIENTATION   = 4,
		RELATIVE_SIZE = 5
	} Cursor;

	/**
	 * Constructor for OrthoMapper. 
	 *
	 * @param bufferSize
	 *        size of the resampling framebuffer
	 * @param shape
	 *        shape of the tool cursor
	 *
	 */
	CORE_API OrthoMapper (const QSize bufferSize, const CursorShape shape);
	
	/**
	 * Destuctor for OrthoMapper
	 */
	CORE_API ~OrthoMapper();
	
	/** 
	 * Resets the OrthoMapper after the scene view has been changed.
	 */
	CORE_API void reset ();

	/**
	 * Sets a new framebuffer size.
	 *
	 * @param newBufferSize
	 *        new size of the framebuffer
	 * @param redraw 
	 *        if true then cursor size is adapted to the new buffer size and the cursor is redrawn
	 */
	CORE_API void handleBufferSizeChanged (QSize newBufferSize, bool redraw = true);

	/**
	 * Must be called before the cursor shape is adjusted, e.g. scaled or rotated.
	 */
	CORE_API void initAdjustCursorShape ();

	/**
	 * Adjust the cursor size by moving the mouse.
	 *
	 * @param changeCursor
	 *        indicates what characteristic (e.g. size or angle) of the cursor should be adjusted.
 	 * @param mouseMoveDistance
	 *        distance in pixels that the mouse has been moved to measure the scale of the adjustment.
	 */
	CORE_API void adjustCursorShape (OrthoMapper::Cursor changeCursor, int mouseMoveDistance);

	/**
	 * Flips the tool  in the z-direction for 3D sculpting operations.
	 */
	CORE_API void invertDepth ();

	/**
	 * Draws the cursor. The old cursor is removed from the display and the new cursor drawn.
	 *
	 * @param newScreenPosition
	 *        pixel-coordinates of the cursor position
	 * @param tiling
	 *        indicates what fraction of the last cursor position is not valid
	 * @return true if cursor is active
	 */
	CORE_API bool drawCursor (Vector2D newScreenPosition, float tiling);

	/** 
	 * Renders the cursor. Used for re-drawing the cursor after the renderer has re-rendered the scene.
	 */
	CORE_API void renderCursor ();

	/**
	 * Enables or disables the viewer that depicts the patch covered by the brush.
	 *
	 * @param enable
	 *        true = patch is shown, false = patch is not shown
	 */
	CORE_API void setVisualizePatchEnable (bool enable);

	/**
	 * Sets the tool orientation.
	 *
	 * @param normalFromFrameBuffer
	 *        true = alignment normal is chosen from frameBuffer, false = fixed alignment normal
	 */
	CORE_API void setToolOrientation (bool normalFromFrameBuffer);

	/**
	 * Reconstructs the surface function. The patch is rendered into the framebuffer, using an orthogonal
	 * projection.
	 *
	 * @return true if reconstruction has been successfull.
	 */
	CORE_API bool reconstructSurfaceFunction (bool &allPixelsCovered);

	/**
	 * Computes all surfels that are contained in the patch.
	 *
	 * @param surfels
	 *        return value: list of visible surfels
	 * @param numOfSurfels
	 *        return value: number of visible surfels
	 */
	CORE_API void computeAllVisibleSurfels (std::vector<SurfelInterface *> **surfels, int &numOfSurfels);

	/** 
	 * returns the cursor scale in z direction
	 *
	 * @return the cursor scale in z direction
	 */
	CORE_API float getZScale ();

	/**
	 * Computes the surface position corresponding the the pixel (x,y) in the framebuffer.
	 *
	 * @param x
	 *        the x-coordinate in the framebuffer
	 * @param y
	 *        the y-coordinate in the framebuffer
	 * @return the surface position in object space.
	 */
	CORE_API Vector3D getSurfacePosition (int x, int y);

	/**
	 * Computes the surface position corresponding the the pixel (x,y) in the framebuffer with replaced depth value.
	 *
	 * @param x
	 *        the x-coordinate in the framebuffer
	 * @param y
	 *        the y-coordinate in the framebuffer
	 * @param depth 
	 *        the depth value in camera space.
	 * @return the surface position in object space.
	 */
	CORE_API Vector3D getFilteredSurfacePosition (int x, int y, float depth);

	/**
	 * Computes the surface normal corresponding the the pixel (x,y) in the framebuffer.
	 *
	 * @param x
	 *        the x-coordinate in the framebuffer
	 * @param y
	 *        the y-coordinate in the framebuffer
	 * @return the surface normal in object space. 
	 */
	CORE_API Vector3D getSurfaceNormal (int x, int y);

	/**
	 * Returns the diffuse color corresponding the the pixel (x,y) in the framebuffer.
	 *
	 * @param x
	 *        the x-coordinate in the framebuffer
	 * @param y
	 *        the y-coordinate in the framebuffer
	 * @return the diffuse color
	 */
	CORE_API QRgb getSurfaceDiffuseColor (int x, int y);

	/**
	 * Inserts a new surfel into the objectSurfels by sampling the framebuffer at the pixel (x,y).
	 *
	 * @param objectSurfels
	 *        the <code>SurfelCollection</code> into which the new surfels is inserted
	 * @param x
	 *        the x-coordinate in the framebuffer
	 * @param y
	 *        the y-coordinate in the framebuffer
	 * @return the inserted surfel
	 */
	CORE_API SurfelInterface* insertNewSurfel (SurfelCollection* objectSurfels, int x, int y);

	/**
	 * Sets the distances to the tool plane. The distance to the tool plane is inserted into the brush
	 * channel component for carving operations.
	 *
	 * @param dispMatrix
	 *        the <code>ChannelComponent</code> in which the displacements are stored
	 * @return the direction of the tool normal, i.e. the carve direction
	 */
	CORE_API Vector3D setDistancesToToolPlane (ChannelComponent *dispMatrix);

	/**
	 * Controls the tool alignment.
	 *
	 * @param useConstantDepthAndNormalMap
	 *        if true then the depth and normal maps are used for alignment that were created when the scene view was last
	 *        altered. If false, the current depth and normal maps are used.
	 */
	CORE_API void setTouchingPointAlignment (bool useConstantDepthAndNormalMap);

	/**
	 * Returns the fixed tool normal.
	 *
	 * @return the fixed tool normal
	 * @see #setToolOrientation
	 */
	CORE_API Vector3D getFixedToolNormal ();

	/**
	 * Sets the fixed tool normal.
	 *
	 * @param fixedNormal
	 *        the fixed tool normal
	 * @see #setToolOrientation
	 */
	CORE_API void setFixedToolNormal (Vector3D fixedNormal);

	/**
	 * Returns the tool normal in object space.
	 *
	 * @return the tool normal in object space.
	 */
	CORE_API Vector3D getToolNormalInObjectSpace ();

	/**
	 * Returns the framebuffer.
	 *
	 * @return the framebuffer
	 */
	CORE_API FrameBufferInterface* getFrameBuffer();

	/** Sets the cursor shape
	 *
	 * @param shape
	 *        shape of the tool cursor
	 *
	 */
	CORE_API void setCursorShape (const CursorShape shape);



private:

	OrthoMapper::CursorShape cursorShapeType;

	bool                     normalFromFrameBuffer;

	bool                     useConstantDepthAndNormalMap;
	bool				     visualizePatch;

	int						 mainBufferSizeX, 
		                     mainBufferSizeY;
	float*                   frameBufferDepthValues;
	Vector3D*                frameBufferNormals;
	bool                     frameBufferCopyValid;

	QWidget                  *rendererWidget;

	bool                     localTransformValid;
	Vector3D                 localCenter,
		                     localXAxis,
							 localYAxis;
	float                    localXAxisLength,
	                         localYAxisLength;

	float		             metric,
			                 scaleU,
			                 scaleV,
			                 scaleW,
			                 scaleUold,
			                 scaleVold,
			                 scaleWold,
			                 angle,
			                 angleOld,
	                         innerShapeFraction,           // relative size of inner shape
							 innerShapeFractionOld;	
 
	Transformer              *resamplingTransformer;		// transforms coordinates from the resampling frame buffer to object space
	RendererInterface        *resamplingRenderer;
	FrameBufferInterface     *resamplingFrameBuffer;
	float					 fieldOfView;					// user specified, fixed field-of-view for resampling renderer
	float					 tan_fieldOfView_2;				// auxiliaray 

	QPen                     cursorPen;
	QPointArray	             cursorShape;
	bool                     cursorVisible;
	CursorStatus             cursorStatus;

	Matrix                   transform;
	Vector3D                 translation;
	Vector2D				 screenPosition;

	Vector3D				 fixedNormal;

	QSize					 bufferSize;

	ImageViewer              *rgbViewer,
	                         *zViewer;

	QPixmap                  *rgbPixmap,
							 *zPixmap;

	RendererInterface        *mainRenderer;
	FrameBufferInterface     *mainFrameBuffer;
	Transformer				 *sceneTransformer;

	void computeLocalTransform ();
	void updateTransformer();
	bool setRenderingTransform ();
	void initCursorTransform ();
	void updateFrameBufferCopy ();

	inline Vector3D localFrameToCameraSpace (const Vector3D &point);
	inline float getDepthValue (int x, int y);
	inline Vector3D getNormal (int x, int y);

};



#endif  // __ORTHOMAPPER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
