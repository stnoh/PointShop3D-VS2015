// Title:   Marker3D.h
// Created: Thu Sep 25 14:28:56 2003
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

#ifndef __MARKER3D_H_
#define __MARKER3D_H_

#include <qpoint.h>
#include <qwidget.h>

#include "../../Interfaces/src/SurfelInterface.h"
#include "../../src/CoreDLL.h"
#include "MarkerInterface.h"

/**
 * This class implements a marker which is used to mark 3D-<code>Model</code>s.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class Marker3D : public MarkerInterface {

public:

	/**
	 * Constructs this <code>Marker3D</code> with ID <code>id</code> and the <code>QWidget</code> which used for
	 * painting. The <code>surfel</code> is associated with this <code>Marker3D</code>, which has initial
	 * selection state according to <code>selected</code> and is drawn according to <code>style</code>.
	 *
	 * @param surfel
	 *        a pointer to the <code>Surfel</code> associated with this <code>Marker3D</code>
	 * @param selected
	 *        the state of the selection
	 * @param id
	 *        a unique identifier for this <code>Marker3D</code>
	 * @param style
	 *        the <code>Style</code> which is to be used for painting this <code>Marker3D</code>
	 */
	CORE_API Marker3D (SurfelInterface *surfel, const bool selected, const uint id, const Style style);
	CORE_API virtual ~Marker3D();

	/**
	 * Returns the <code>SurfelInterface</code> associated with this <code>MarkerInterface</code>.
	 *
	 * @return a pointer to the <code>SurfelInterface</code> associated with this <code>MarkerInterface</code>
	 */
	CORE_API SurfelInterface *getSurfel() const;

	/**
	 * Sets the <code>SurfelInterface</code> associated with this <code>MarkerInterface</code>.
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> associated with this <code>Marker3D</code>
	 */
	CORE_API void setSurfel (SurfelInterface *surfel);

	/**
	 * While this <code>Marker3D</code> is being dragged, a call to <code>togglePainting</code> has no effect.
	 *
	 * @param dragged
	 *        set to <code>true</code> if this <code>Marker3D</code> should ignore calls to
	 *        <code>togglePainting</code>; <code>false</code> else
	 * @see #togglePainting
	 */
	CORE_API void setDragged (const bool dragged);

	/**
	 * Returns <code>true</code> if this <code>Marker3D</code> is being dragged.
	 *
	 * @return <code>true</code> if this <code>Marker3D</code> is being dragged
	 */
	CORE_API bool isDragged() const;

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

	SurfelInterface *surfel;
	QPoint          position;
	bool            selected,
		            eraseOldPosition,
				    dragged;
	QWidget         *widget;
	uint            id;
	Style           style;
	float           markerZ;

	// updates the screen position according to the current 'surfel'
	void updatePosition();
	// implements the actual painting (toggle)
	void paint (QWidget *widget);

};

#endif  // __MARKER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
