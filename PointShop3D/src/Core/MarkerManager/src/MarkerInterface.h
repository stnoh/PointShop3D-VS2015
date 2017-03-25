// Title:   MarkerInterface.h
// Created: Thu Sep 25 14:28:54 2003
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

#ifndef __MARKERINTERFACE_H_
#define __MARKERINTERFACE_H_

#include <qwidget.h>
#include <qpoint.h>

/**
 * This class defines an interface for <em>markers</em> which can be used to mark positions in bitmaps
 * or <code>SurfelInterface</code>s of an <code>Object</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class MarkerInterface {

public:

	/**
	 * The available styles, which specify the look of the markers.
	 */
	typedef enum style {
		BOUNDARY_CONDITION_STYLE,
	    FITTING_CONSTRAINT_STYLE
	} Style;

	/**
	 * Returns <code>true</code> if this <code>MarkerInterface</code> is close enough to the
	 * <code>mousePosition</code>.
	 *
	 * @param mousePosition
	 *        the <code>QPoint</code> which specifies the mouse position
	 * @param widget
	 *        a pointer to a <code>QWidget</code> which might be used for calculating the position
	 *        within this <code>QWidget</code>
	 * @return <code>true</code>, if the mouse position is close enough to this <code>MarkerInterface</code>;
	 *         <code>false</code> else
	 */
	virtual bool isClicked (const QPoint mousePosition, const QWidget *widget) = 0;

	/**
	 * Sets the ID, which is also visually displayed.
	 *
	 * @param newID
	 *        the new ID
	 */
	virtual void setID (const uint newID) = 0;

	/**
	 * Returns the ID.
	 *
	 * @return the ID
	 */ 
	virtual uint getID() const = 0;

	/**
	 * Returns the position in screen coordinates (origin is at left <em>upper</em> corner) of this
	 * <code>MarkerInterface</code>. Usually this is the position where the user has clicked with the
	 * mouse, but the position may depend on other factors, such as the position of the associated
	 * <code>Surfel</code> or the dimension of the scaled pixmap.
	 *
	 * @param widget
	 *        a pointer to a <code>QWidget</code> which might be used for calculating the position
	 *        within this <code>QWidget</code>
	 *
	 * @return the <code>QPoint</code> which specifies the position of this <code>MarkerInterface</code> in
	 *         screen coordinates within the <code>QWidget</code>
	 */
	virtual QPoint getPosition (const QWidget *widget) const = 0;

	/**
	 * Sets the selection state to <code>selected</code>.
	 *
	 * @param selected
	 *        the new state of the selection of this <code>MarkerInterface</code>
	 */
	virtual void setSelected (const bool selected) = 0;

	/**
	 * Returns <code>true</code> if this <code>MarkerInterface</code> is selected.
	 *
	 * @return <code>true</code> if this <code>MarkerInterface</code> is selected; <code>false</code> else
	 */
	virtual bool isSelected() const = 0;

	/**
	 * Sets the <code>style</code> for painting.
	 * 
	 * @param style
	 *        the new <code>Style</code> to be used for painting
	 * @see #Style
	 */
	virtual void setStyle (const Style newStyle) = 0;

	/**
	 * Returns the <code>Style</code> which is used for painting.
	 *
	 * @return the <code>Style</code> which is used for painting
	 */
	virtual Style getStyle() const = 0;

	/**
	 * Toggles the visual representation of this <code>MarkerInterface</code>. If called the first time the
	 * marker is painted, the next time it is erased, provided that it has not been erased by another
	 * widget update operation.
	 *
	 * @param widget
	 *        a pointer to a <code>QWidget</code> into which this <code>MarkerInterface</code> is painted
	 */
	virtual void togglePainting (QWidget *widget) = 0;

	/**
	 * Moves to the <code>newPosition</code> and updates the visual representation.
	 *
	 * @param newPosition
	 *        the <code>QPoint</code> specifying the new position
	 * @param widget
	 *        a pointer to a <code>QWidget</code> into which this <code>MarkerInterface</code> is painted
	 * @return <code>true</code> if this <code>MarkerInterface</code> could be moved to the new
	 *         position; <code>false</code> else (e.g. no splats at this position in the
	 *         <code>FrameBufferInterface</code>)       
	 */
	virtual bool moveTo (const QPoint newPosition, QWidget *widget) = 0;

};


#endif  // __MARKERINTERFACE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
