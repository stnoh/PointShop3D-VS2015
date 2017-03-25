// Title:   SurfelGPFrameBuffer.h
// Created: Thu Sep 25 14:51:35 2003
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

#ifndef __SURFELGPFRAMEBUFFER_H_
#define __SURFELGPFRAMEBUFFER_H_

#include <qimage.h>
#include <qsize.h>
#include <qthread.h>

#include "../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../Core/Interfaces/src/RendererInterface.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/Scene/src/Scene.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../../DataTypes/src/Vector2D.h"

#include "SurfelPipeline_SurfSplat/Srf.h"
#include <vector>

/**
 * This class implements the <code>FrameBufferInterface</code> for the <em>SurfelGP Renderer</em>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class SurfelGPFrameBuffer : public FrameBufferInterface {

public:

	/**
	 * Creates this <code>SurfelGPFrameBuffer</code> with initial dimension <code>size</code> and
	 * associates it with the <code>model</code>, so the internal buffer sizes can be adjusted
	 * according to the number of <code>Surfel</code>s in the <code>Model</code>.
	 *
	 * @param size
	 *        the initial <code>QSize</code> of this <code>SurfelGPFrameBuffer</code>	 
	 */
	SurfelGPFrameBuffer (const QSize size = QSize (0, 0));
	virtual ~SurfelGPFrameBuffer();

	/**
	 * Sets the surfel context <code>SrfContext</code> for this <code>SurfelGPFrameBuffer</code>.
	 *
	 * @param srf
	 *        a pointer to the surfel context <code>SrfContext</code>
	 */
	void setSurfelContext(SrfContext *srf);

	// ******************************
	// interface FrameBufferInterface
	// ******************************

	RENDERER_API void setAttributes (const int attriubtes);
	RENDERER_API int getAttributes() const;
	RENDERER_API void setValid (const bool valid);
	RENDERER_API bool isValid() const;
	RENDERER_API void setSize (const QSize newBufferSize);
	RENDERER_API QSize getSize() const;

	RENDERER_API void setColor (const int x, const int y, const QRgb newPixelColor);
	RENDERER_API QRgb getColor (const int x, const int y) const;
	RENDERER_API const QImage *getImage() const;
	RENDERER_API void clearImage (const QRgb clearColor);

	RENDERER_API void addWeight (const int x, const int y, const float pixelWeightIncrement);
	RENDERER_API void setWeight (const int x, const int y, const float newPixelweight);
	RENDERER_API float getWeight (const int x, const int y) const;
	RENDERER_API void setDepth (const int x, const int y, const float z);
	RENDERER_API float getDepth (const int x, const int y) const;
	RENDERER_API void setNormal (const int x, const int y, const Vector3D newNormal);
	RENDERER_API void setNormal (const int x, const int y, const float nx, const float ny, const float nz);
	RENDERER_API Vector3D getNormal (const int x, const int y) const;
	RENDERER_API SurfelInterface* getSurfel(const int x, const int y);
	RENDERER_API void addVisibleSurfel (const int x, const int y, SurfelInterface *visibleSurfel);
	RENDERER_API void getVisibleSurfels (const int x, const int y,  const std::vector<SurfelInterface *> **visibleSurfels, uint *nofVisible) const;

	RENDERER_API void resetPosition (const int x, const int y);
	RENDERER_API bool isPixelCovered (const int x, const int y) const;
	RENDERER_API bool allPixelsCovered ();
	
	RENDERER_API Object* getObjectAtPixel (const int x, const int y) const;
	RENDERER_API void setObjectAtPixel (const int x, const int y, Object *object);

	RENDERER_API int getNofSplatInfoEntries() const;
	RENDERER_API std::vector<SurfelInterface *>* getSurfelsInViewFrustum();
	RENDERER_API std::vector<MyDataTypes::TextureCoordinate>* getTextureCoordinatesInViewFrustum();
	RENDERER_API std::vector<MyDataTypes::Splat>* getSplatsInViewFrustum();
	RENDERER_API void setSplatInfo (SurfelInterface *surfel, const float x, const float y, const float z, const float a, const float b_2, const float c, const int bbox[4]);

	RENDERER_API QImage *getZImage() const;	
	RENDERER_API float getMaxZValue() const;	
	RENDERER_API float getMinZValue() const;	

	RENDERER_API QList<SurfelInterface> *getAllVisibleSurfels ();
	RENDERER_API void markAllSurfels ();

private:

	typedef struct bufferEntry {
		uint                      nofVisible;	   // number of visible surfels
		std::vector<SurfelInterface *> *visibleSurfels; // the visible surfels
		Object                    *object;         // the object belonging to the last surfel rendered to this pixel
	} BufferEntry;

	BufferEntry             *buffer;            // the per pixel data which holds pointers to visible surfels

	QSize                   size;               // the size of the buffer in pixels
	int						height_1;			// size.height()-1
	int						height;				// size.height()
	int						width;				// size.width()

	SrfZBufItem*			srfFrameBuffer;		// direct access to internal frame buffer of the surfel renderer
												// this buffer stores pixel weights, z values, and normals

	int                     attributes;			// attributes that are written to the frame buffer during rendering
	QImage                  *image;             // contains the rendered image	
	
	bool                    valid;              // true, if the buffer is filled with valid data
	                        

	std::vector<SurfelInterface*> surfels;           // array storing the rendered surfels
	std::vector<MyDataTypes::TextureCoordinate>  texCoords;
	std::vector<MyDataTypes::Splat>              splats;	          // array storing splats rendered to the frame buffer

	uint					nSplats;			// number of splats currently stored in the splats array

	// initializes all buffer entries
	void initBuffer();

	// resets all buffer entries - buffer must already be initialized
	// with 'initBuffer'
	void resetBuffer();

	// removes the current 'buffer', releasing all resources
	void removeBuffer();



};

#endif  // __SURFELGPFRAMEBUFFER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
