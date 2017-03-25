// Title:   ScanConversion.h
// Created: Thu Sep 25 14:30:38 2003
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

#ifndef __SCANCONVERSION_H_
#define __SCANCONVERSION_H_

#include <qpointarray.h>
#if QT_VERSION >= 0x030000
# include <qptrlist.h>
# ifndef QList
#  define QList QPtrList
# endif
# ifndef QListIterator
#  define QListIterator QPtrListIterator
# endif
#else
# include <qlist.h>
#endif

#include "../../../Interfaces/src/SurfelInterface.h"

/**
 * This namespace provides several scan conversion functions, such as for polygons and ellipses.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace ScanConversion {

	/**
	 * <code>SpanFunction</code>: A pointer to a function returning void and taking 3 <code>const int</code> arguments and
	 * a pointer to a <code>QList<SurfelInterface></code> as 4th argument.
	 *
	 * @param y
	 *		  top of clipping rectangle
	 * @param xLeft
	 *		  left of clipping rectangle
	 * @param xRight
	 *		  right of clipping rectangle
	 * @param selectionFlag
	 *		  surfel flag
	 * @param select
	 *        true for select, false for de-select
	 */
	typedef int (*SpanFunction)(const int y, const int xLeft, const int xRight, SurfelInterface::Flag selectionFlag, bool select);

	/**
	 * concavePolygon: scan convert nvert-sided concave non-simple polygon with vertices at
	 * (point[i].x, point[i].y) for i in [0..nvert-1] within the window win by
	 * calling spanproc for each visible span of pixels.
	 * Polygon can be clockwise or counterclockwise.
	 * Algorithm does uniform point sampling at pixel centers.
	 * Inside-outside test done by Jordan's rule: a point is considered inside if
	 * an emanating ray intersects the polygon an odd number of times.
	 * SpanFunction should fill in pixels from xl to xr inclusive on scanline y,
	 * e.g:
	 *      drawproc(y, xl, xr)
	 *      int y, xl, xr;
	 *      {
	 *          int x;
	 *          for (x=xl; x<=xr; x++)
	 *              pixel_write(x, y, pixelvalue);
	 *      }
	 *
	 *  Paul Heckbert       30 June 81, 18 Dec 89
	 *
	 * @param nofPoints
	 *        the number of points in the <code>polygonLine</code>
	 * @param polygonLine
	 *        a <code>QPointArray</code> storing all the points of the
	 *        polygon line specifying the polygon
	 * @param clippingRectangle
	 *        a <code>QRect</code> which specifies the clipping rectangle
	 * @param spanFunction
	 *        a pointer to a function which is called for each span
	 * @param selectionFlag
	 * @param select
	 *        true for select, false for de-select
	 * @return number of covered surfels
	*/
	int  concavePolygon (const int nofPoints,
		                 const QPointArray *polygonLine,
						 const QRect clippingRectangle,
						 const SpanFunction spanFunction,
						 SurfelInterface::Flag selectionFlag,
						 bool select);

	/**
	 * Scan-converts an ellipse, given by its axis <code>a</code> and <code>b</code>
	 * and center coordinate (code>centerX</code>, <code>centerY</code>).
	 *
	 * @param a
	 *        length of the ellipse axis in x-direction [pixels]
	 * @param b 
	 *        length of the ellipse axis in y-direction [pixels]
	 * @param centerX
	 *        x-coordinate of the ellipse center in screen coordinates
	 * @param centerY
	 *        y-coordinate of the ellipse center in screen coordinates
	 * @param clippingRectangle
	 *        a <code>QRect</code> which specifies the clipping rectangle
	 * @param spanFunction
	 *        a pointer to a function which is called for each span
	 * @param selectionFlag
	 * @param select
	 *        true for select, false for de-select
	 * @return number of covered surfels
	 */
	int  ellipse (const int a, const int b,
		          const int centerX, const int centerY,
		          const QRect clippingRectangle,
				  const SpanFunction spanFunction,
				  SurfelInterface::Flag selectionFlag,
				  bool select);

};

#endif  // __SCANCONVERSION_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
