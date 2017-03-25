// Title:   Marker2D.h
// Created: Thu Sep 25 14:28:58 2003
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

#ifndef __MARKER2D_H_
#define __MARKER2D_H_

#include <qpoint.h>
#include <qwidget.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../src/CoreDLL.h"
#include "MarkerInterface.h"

/**
 * This class implements a marker which is used to mark 2D-bitmaps.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class Marker2D : public MarkerInterface {

public:

	/**
	 * Constructs this <code>Marker2D</code> with ID <code>id</code> and the <code>QWidget</code> which used for
	 * painting.
	 *
	 * @param textureCoordinate
	 *        the <code>TextureCoordinate</code> which specifies the position
	 * @param selected
	 *        the state of the selection
	 * @param id
	 *        a unique identifier for this <code>Marker2D</code>
	 * @param style
	 *        the <code>Style</code> which is to be used for painting this <code>Marker2D</code>
	 */
	CORE_API Marker2D (const MyDataTypes::TextureCoordinate textureCoordinate,
		               const bool selected, const uint id, const Style style);
	CORE_API virtual ~Marker2D();

	/**
	 * The normalized coordinate [0.0 .. 1.0] in x-direction.
	 *
	 * @return the normalized coordinate [0.0 .. 1.0] in x-direction.
	 */
	CORE_API float getNormalX() const;

	/**
	 * The normalized coordinate [0.0 .. 1.0] in y-direction, where y = 0 is
	 * at <em>bottom</em>.
	 *
	 * @return the normalized coordinate [0.0 .. 1.0] in y-direction
	 */
	CORE_API float getNormalY() const;

	// *************************
	// interface MarkerInterface
	// *************************

	CORE_API bool isClicked (const QPoint mousePosition, const QWidget *widget);
	CORE_API void setID (const uint newID);
	CORE_API uint getID() const;
	CORE_API QPoint getPosition (const QWidget *widget) const;

	CORE_API void setSelected (const bool selected);
	CORE_API bool isSelected() const;

	CORE_API void setStyle (const Style newStyle);
	CORE_API Style getStyle() const;

	CORE_API void togglePainting (QWidget *widget);
	CORE_API bool moveTo (const QPoint newPosition, QWidget *widget);

private:

	bool    selected,
		    eraseOldPosition;
	uint    id;
	Style   style;
	MyDataTypes::TextureCoordinate textureCoordinate;

	void updateNormalPosition (const QPoint position, const QWidget *widget);

};

#endif  // __MARKER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
