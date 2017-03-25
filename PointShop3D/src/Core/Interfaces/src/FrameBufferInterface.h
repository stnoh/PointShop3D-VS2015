// Title:   FrameBufferInterface.h
// Created: Thu Sep 25 14:29:10 2003
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

#ifndef __FRAMEBUFFERINTERFACE_H_
#define __FRAMEBUFFERINTERFACE_H_

#include <qimage.h>
#include <qcolor.h>
#include <qsize.h>
#include <qobject.h>
#include <qrect.h>

#include "../../../DataTypes/src/Vector3D.h"
#include "../../../DataTypes/src/Vector2D.h"
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../Scene/src/Object.h"
#include "../../src/CoreDLL.h"
#include "SurfelInterface.h"

/**
 * This interface defines the API for framebuffers. Framebuffers contain additional information
 * such as visible <code>SurfelInterface</code>s at a given position in the framebuffer and the rendered image.
 * The coordinates are <em>screen</em> coordinates, that is x = y = 0 is in the <em>upper</em> left corner of the
 * buffer.
 *
 * Airliner: "Weather at our destination is 50 degrees with some broken clouds,
 *            but we'll try to have them fixed before we arrive."
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class CORE_API FrameBufferInterface  {

public:

	/**
	 * Describes the <code>Attribute</code>s with which this <code>FrameBufferInterface</code> should be 
	 * filled by the <code>RendererInterface</code>:
	 * <ul>
	 *   <li>PERPIXEL_C_Z_N_W       - rasterize color, depth, normals, and weights</li>
	 *   <li>PERPIXEL_SURFELLISTS   - store all surfels in each pixel</li>
	 *   <li>PERPIXEL_SURFEL        - stores only the pointer to the last rendered surfel
	 *   <li>PERPIXEL_OBJECTPOINTER - per pixel pointer to the object to which this splat belongs</li>
	 *   <li>ALL_SPLATS             - record all splats in the framebuffer</li>
	 * </ul>
	 *
	 * NOTE: Attributes can be ORed, default is PERPIXEL_C_Z_N_W
	 *
	 * @see #setAttributes
	 * @see #getAttributes
	 */
	typedef enum attributes {
		PERPIXEL_SURFELLISTS   = 0x00000001,	// generate per pixel surfel lists
		PERPIXEL_C_Z_N_W       = 0x00000002,	// rasterize color, depth, normals, and weights
		PERPIXEL_SURFEL        = 0x00000004,	// stores only the pointer to the last rendered surfel
		PERPIXEL_OBJECTPOINTER = 0x00000008,	// per pixel pointer to object 
		ALL_SPLATS             = 0x00000010,	// record all splats in the frame buffer
	} Attributes;

	/**
	 * Sets the <code>Attributes</code> this <code>FrameBufferInterface</code> should provide upon the
	 * next buffer refresh.
	 *
	 * @param attributes
	 *        OR'ed <code>Attributes</code> which define which values this <code>FrameBufferInterface</code>
	 *        should provide.
	 */
	virtual void setAttributes (const int attributes) = 0;

	/**
	 * Returns the <code>Attributes</code> this <code>FrameBufferInterface</code> provides.
	 *
	 * @return the OR'ed <code>Attributes</code>
	 */
	virtual int getAttributes() const = 0;

	/**
	 * Sets this <code>FrameBufferInterface</code> valid.
	 *
	 * @param valid
	 *        set to <code>true</code> if the data in this <code>FrameBufferInterface</code> is valid;
	 *        <code>false</code> else
	 */
	virtual void setValid (const bool valid) = 0;

	/**
	 * Returns <code>true</code> if the data in this <code>FrameBufferInterface</code> is valid.
	 *
	 * @return <code>true</code> if the data in this <code>FrameBufferInterface</code> is valid;
	 *         <code>false</code> else
	 */
	virtual bool isValid() const = 0;

	// ***********
	// buffer size
	// ***********

	/**
	 * Sets the buffer size in pixel.
	 *
	 * @param newBufferSize
	 *        the <code>QSize</code> specifying the width and height of this
	 *        frame buffer in pixels
	 */
	virtual void setSize (const QSize newBufferSize) = 0;

	/**
	 * Returns the buffer size in pixel.
	 *
	 * @return the <code>QSize</code> specifying the width and height of this
	 *         frame buffer in pixels
	 */
	virtual QSize getSize() const = 0;

	// **************
	// rendered image
	// **************	

	/**
	 * Sets the pixel at position (<code>x</code>, <code>y</code>) to the <code>newPixelColor</code>.
	 *
	 * @param x
	 *        the x screen coordinate
	 * @param y
	 *        the y screen coordinate
	 * @param newPixelColor
	 *        the new <code>QRgb</code> color for the pixel at position (<code>x</code>, <code>y</code>)
	 */
	virtual void setColor (const int x, const int y, const QRgb newPixelColor) = 0;

	/**
	 * Returns the pixel color at position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @param x
	 *        the x screen coordinate
	 * @param y
	 *        the y screen coordinate
	 * @return the <code>QRgb</code> color of the pixel at position (<code>x</code>, <code>y</code>)
	 */
	virtual QRgb getColor (const int x, const int y) const = 0;

	/**
	 * Returns the rendered image.
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @return a pointer to a <code>QImage</code> containing the rendered image
	 */
	virtual const QImage *getImage() const = 0;

	/**
	 * Clears the image with the <code>clearColor</code>.
	 *
	 * @param clearColor
	 *        the <code>QRgb</code> color which is used as background color
	 */
	virtual void clearImage (const QRgb clearColor) = 0;
	
	// **************
	// buffer content
	// **************

	/**
	 * Adds the <code>pixelWeightIncrement</code> to the pixel weight stored at position (<code>x</code>, <code>y</code>).
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @param pixelWeightIncrement
	 *        the increment which is added to the pixel weight stored at the specified position
	 */
	virtual void addWeight (const int x, const int y, const float pixelWeightIncrement) = 0;
	
	/**
	 * Sets the pixel weight stored at position (<code>x</code>, <code>y</code>) to <code>newPixelWeight</code>.
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @param newPixelweight
	 *        the new pixel weight to be stored at the specified position
	 */
	virtual void setWeight (const int x, const int y, const float newPixelweight) = 0;
	
	/**
	 * Gets the pixel weight stored at position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @return the pixel weight stored at the specified position
	 */
	virtual float getWeight (const int x, const int y) const = 0;

	/**
	 * Adds a <code>SurfelInterface</code> which is visible at position (<code>x</code>, <code>y</code>).
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @param visibleSurfel
	 *        a pointer to a <code>SurfelInterface</code> which is visible at the given position
	 */
	virtual void addVisibleSurfel (const int x, const int y, SurfelInterface *visibleSurfel) = 0;
	
	/**
	 * Returns an array of visible <code>SurfelInterface</code>s at the given position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_SURFELLISTS</code>.
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @param visibleSurfels
	 *        a resulting pointer which will point to an internal pointer to an <code>std::vector<SurfelInterface*></code> which stores
	 *        the visible <code>SurfelInterface</code>s at the given position
	 * @param nofVisible
	 *        the resulting number of visible <code>SurfelInterface</code>s at the given position
	 */
	virtual void getVisibleSurfels (const int x, const int y, const std::vector<SurfelInterface *> **visibleSurfels, uint *nofVisible) const = 0;

	/**
	 * Sets the depth value (<code>z</code>-value) at position (<code>x</code>, <code>y</code>).
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @param z
	 *        the depth value (<code>z</code>-value) at the given position
	 */
	virtual void setDepth (const int x, const int y, const float z) = 0;

	/**
	 * Returns the depth value (<code>z</code>-value) at position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @return the depth value (<code>z</code>-value) at the given position
	 */
	virtual float getDepth (const int x, const int y) const = 0;

	/**
	 * Sets the <code>newNormal</code> at position (<code>x</code>, <code>y</code>).
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @param newNormal
	 *        the (averaged) normal at the given position
	 */
	virtual void setNormal (const int x, const int y, const Vector3D newNormal) = 0;

	/**
	 * Sets the <code>newNormal</code> at position (<code>x</code>, <code>y</code>).
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @param nx
	 *        the x-component of the (averaged) normal at the given position
	 * @param ny
	 *        the y-component of the (averaged) normal at the given position
	 * @param nz
	 *        the z-component of the (averaged) normal at the given position
	 */
	virtual void setNormal (const int x, const int y, const float nx, const float ny, const float nz) = 0;

	/**
	 * Returns the normal at position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @return the (averaged) normal at the given position
	 */
	virtual Vector3D getNormal (const int x, const int y) const = 0;

	/**
	 * Get the <code>SurfelInterface</code> that is stored in a frame buffer pixel.
	 * Framebuffer pixels only contain the last <code>SurfelInterface</code>
	 * that was rendered and is visible in the pixel.
	 *
	 * <b>Note:</b> Render with the option <code>PERPIXEL_SURFEL</code>.
	 *
	 * @param x
	 *        the x-coordinate of the desired position
	 * @param y
	 *        the y-coordinate of the desired position
	 * @return a pointer to the last rendered <code>SurfelInterface</code> at the given position
	 */
	virtual SurfelInterface *getSurfel (const int x, const int y) = 0;

	/**
	 * Resets all values (weight, color, normal, visible surfel array) at position (<code>x</code>, <code>y</code>).
	 *
	 * @param x
	 *        the x screen coordinate
	 * @param y
	 *        the y screen coordinate
	 */
	virtual void resetPosition (const int x, const int y) = 0;
	
	/**
	 * Returns <code>true</code> if there are visible <code>SurfelInterface</code>
	 * at position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @param x
	 *        the x screen coordinate
	 * @param y
	 *        the y screen coordinate
	 * @return <code>true</code> if at least one <code>SurfelInterface</code> is visible at the
	 *         given position; <code>false</code> else
	 * @see #allPixelsCovered
	 */
	virtual bool isPixelCovered (const int x, const int y) const = 0;

	/**
	 * Returns the object pointer that is stored at position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Note 1:</b> render with the option <code>PERPIXEL_OBJECTPOINTER</code>.
	 * <b>Note 2:</b> Make sure that the pixel is covered (<code>isPixelCovered</code> returns
	 *                <code>true</code>) before accessing the <code>Object</code>, else the
	 *                returned pointer is meaningless.
	 *
	 * @param x
	 *        the x screen coordinate
	 * @param y
	 *        the y screen coordinate
	 * @return a pointer to the <code>Object</code> which is visible at the given position;
	 *         make sure that the position contains visible <code>SurfelInterface</code>, else
	 *         the pointer is meaningless
	 * @see #isPixelCovered
	 */
	virtual Object *getObjectAtPixel (const int x, const int y) const = 0;

	/**
	 * Set the <code>Object</code> pointer at position (<code>x</code>, <code>y</code>).
	 *
	 * @param x
	 *        the x screen coordinate
	 * @param y
	 *        the y screen coordinate
	 * @param object
	 *        a pointer to an <code>Object</code> which is visible at the given position 
	 */
	virtual void setObjectAtPixel (const int x, const int y, Object *object) = 0;

	/**
	 * Returns <code>true</code> if all pixels of this <code>FrameBufferInterface</code> are covered.
     *
     * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @return <code>true</code> if all pixels of this <code>FrameBufferInterface</code> are covered
	 * @see #isPixelCovered
	 */
	virtual bool allPixelsCovered() = 0;
	
	/**
	 * Creates a <code>QImage<code> that contains the z-values as a 8-bit height field. Has to be
	 * <code>delete</code>d after use. <b>Don't use anymore, deprecated</b>
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @return a pointer to the <code>QImage</code>; has to be <code>delete</code> after use
	 * @deprecated
	 */
	virtual QImage *getZImage() const = 0;	
	
	/**
	 * Returns all <code>SurfelInterface</code>s that are visible in this <code>FrameBufferInterface</code>.
	 *
	 * <b>Note:</b> Render with the option <code>PERPIXEL_SURFELLISTS</code>.
	 *
	 * @return a pointer to a <code>QList</code> containing all visible <code>SurfelInterface</code>s 
	 */
	virtual QList<SurfelInterface> *getAllVisibleSurfels() = 0;

	/**
	 * Returns the maximum z-value in this <code>FrameBufferInterface</code>.
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @return the maximum z-value in this <code>FrameBufferInterface</code>
	 */
	virtual float getMaxZValue() const = 0;

	/**
	 * Returns the minimum z-value in this <code>FrameBufferInterface</code>.
	 *
	 * <b>Note:</b> render with the option <code>PERPIXEL_C_Z_N_W</code>.
	 *
	 * @return the minimum z-value in this <code>FrameBufferInterface</code>
	 */
	virtual float getMinZValue() const = 0;

	/**
	 * Sets informations about the <code>Splat</code> at position (<code>x</code>, <code>y</code>).
	 *
	 * <b>Attention</b>: Unlike the other methods <code>x</code> and <code>y</code> are framebuffer
	 *                   coordinates which the origin in the <b>lower</b> left corner!
	 *
	 * @param surfel
	 *        a pointer to the <code>SurfelInterface</code> which is associated with this <code>Splat</code>
	 * @param x
	 *        the x framebuffer coordinate
	 * @param y
	 *        the y framebuffer coordinate
	 * @param z
	 *        the depth value
	 * @param a
	 *        an entry into the conic matrix which describes the <code>Splat</code> ellipse
	 * @param b_2
	 *        an entry into the conic matrix which describes the <code>Splat</code> ellipse
	 * @param c
	 *        an entry into the conic matrix which describes the <code>Splat</code> ellipse
	 * @param bbox
	 *        the bounding box of the <code>Splat</code> ellipse
	 */
	virtual void setSplatInfo (SurfelInterface *surfel, const float x, const float y, const float z, const float a, const float b_2, const float c, const int bbox[4]) = 0;

	/**
	 * Returns the number of splat info entries.
	 *
	 * <b>Note:</b> Render with the option <code>ALL_SPLATS</code>.
	 *
	 * @return the number of splat info entries
	 */
	virtual int getNofSplatInfoEntries() const = 0;

	/**
	 * Returns the <code>SurfelInterface</code>s in the <code>ViewFrustum</code>, including
	 * the hidden <code>SurfelInterface</code>s.
	 *
	 * <b>Note:</b> Render with the option <code>ALL_SPLATS</code>.
	 *
	 * @return a pointer to a <code>std::vector<SurfelInterface*></code> which contains all <code>SurfelInterface</code>s
	 *         in the <code>ViewFrustum</code>, including the hidden <code>SurfelInterface</code>s;
	 *         use <code>getNofSplatInfoEntries</code> to query the number of <code>SurfelInterface</code>s
	 *         in this vector
	 * @see #getNofSplatInfoEntries
	 */
	virtual std::vector<SurfelInterface *> *getSurfelsInViewFrustum() = 0;

	/**
	 * Returns the <code>TextureCoordinate</code>s in the <code>ViewFrustum</code>, including
	 * the hidden <code>SurfelInterface</code>s.
	 *
	 * <b>Note:</b> Render with the option <code>ALL_SPLATS</code>.
	 *
	 * @return a pointer to a <code>std::vector<MyDataTypes::TextureCoordinate>/code> which contains all <code>SurfelInterface</code>s
	 *         in the <code>ViewFrustum</code>, including the hidden <code>TextureCoordinate</code>s;
	 *         use <code>getNofSplatInfoEntries</code> to query the number of <code>TextureCoordinate</code>s
	 *         in this vector
	 * @see #getNofSplatInfoEntries
	 */
	virtual std::vector<MyDataTypes::TextureCoordinate> *getTextureCoordinatesInViewFrustum() = 0;
	
	/**
	 * Returns the <code>Splat</code>s in the <code>ViewFrustum</code>, including
	 * the hidden <code>Splat</code>s.
	 *
	 * <b>Note:</b> Render with the option <code>ALL_SPLATS</code>.
	 *
	 * @return a reference to a <code>std::vector<MyDataTypes::Splat></code> which contains all <code>Splat</code>s
	 *         in the <code>ViewFrustum</code>, including the hidden <code>Splat</code>s;
	 *         use <code>getNofSplatInfoEntries</code> to query the number of <code>Splat</code>s
	 *         in this vector
	 * @see #getNofSplatInfoEntries
	 */
	virtual std::vector<MyDataTypes::Splat> *getSplatsInViewFrustum() = 0;	

};

#endif  // __FRAMEBUFFERINTERFACE_H_


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
