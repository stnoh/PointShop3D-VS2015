// Title:   BitmapMarker.h
// Created: Thu Sep 25 14:58:34 2003
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

#ifndef __BitmapMarker_H_
#define __BitmapMarker_H_

#include <qimage.h>
#include <qlabel.h>
#include <qevent.h>

#include "../../../../Core/MarkerManager/src/MarkerInterface.h"
#include "../../../../Core/MarkerManager/src/Marker2D.h"
#include "../../../../Core/MarkerManager/src/MarkerManager.h"

/**
 * This class implements a widget which shows the currently selected bitmap to be marked and
 * updates the <code>MarkerTool</code> accordingly.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see MarkerTool
 */
class BitmapMarker : public QLabel {

public:

	/**
	 * This method creates an instance of this <code>BitmapMarker</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return a pointer to an instance of this <code>BitmapMarker</code>
	 */
	static BitmapMarker *getInstance();

	/**
	 * Destroys the instance of this <code>BitmapMarker</code>, if there
	 * is one.
	 */
	static void destroyInstance();

	/**
	 * Decides whether boundary conditions or fitting constraints should be inserted.
	 *
	 * @param enable
	 *        set to <code>true</code> if boundary conditions should be inserted; set to <code>false</code>
	 *        if fitting constraints should be inserted
	 */
	void setBoundaryConditionEnabled (const bool enable);

	/**
	 * returns true if boundary conditions are enabled
	 *
	 * @see #setBoundaryConditionEnabled
	 */
	bool isBoundaryConditionEnabled() const;

	/**
	 * Displays the <code>pixmap</code> and resizes this <code>BitmapMarker</code> to fit
	 * its size.
	 *
	 * @param pixmap
	 *        a pointer to the <code>QPixmap</code> to be displayed
	 */
	void displayPixmap (const QPixmap *pixmap);

protected:

	// **************
	// event handlers
	// **************

	void mousePressEvent (QMouseEvent *me);
	void mouseMoveEvent (QMouseEvent *me);
	void mouseReleaseEvent (QMouseEvent *me);
	void paintEvent (QPaintEvent *pe);

private:

	Marker2D    *selectedMarker;          // the marker which was clicked

	bool        insertBoundaryCondition,  // true, if we should insert boundary conditions, false if we should
		                                  // insert fitting constraints
		        isDragging,               // true, if we are dragging a marker with the mouse
	            keepMarker;               // false, if the marker is dragged out of the pixmap

	// singleton
	BitmapMarker (QWidget *parent = 0, const char *name = 0, WFlags flags = 0);
	virtual ~BitmapMarker();

};


#endif  // __BitmapMarker_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
