// Title:   MarkerUtils.h
// Created: Thu Sep 25 14:28:50 2003
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

#ifndef __MARKERUTILS_H_
#define __MARKERUTILS_H_

#include <qpoint.h>
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

#include "../../Interfaces/src/SurfelInterface.h"
#include "../../src/CoreDLL.h"
#include "MarkerInterface.h"

/**
 * This namespace provides some utility functions for <code>MarkerInterface</code>s such as <code>pickMarker</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace MarkerUtils {

	/**	 
	 * Picks a marker at the <code>mousePosition</code> and returns the picked <code>MarkerInterface</code>, or
	 * 0 if no marker was close enough to the <code>mousePosition</code>.
	 *
	 * @param mousePosition
	 *        the <code>QPoint</code> denoting the mouse position in screen coordinates
	 * @param markerList
	 *        a pointer to the <code>QList</code> containing the <code>MarkerInterface</code>s, of which at most one
	 *        is picked
	 * @param widget
	 *        a pointer to a <code>QWidget</code> which might be used for calculating the position of the
	 *        <code>MarkerInterface</code> within this <code>QWidget</code>
	 *
	 * @return a pointer to the picked <code>MarkerInterface</code>; 0 if none has been picked
	 */
	CORE_API MarkerInterface *pickMarker (const QPoint mousePosition, QList<MarkerInterface> *markerList, const QWidget *widget);

	/**
	 * Returns the closest <code>SurfelInterface</code> to the <code>mousePosition</code>. Returns 0 if no
	 * <code>SurfelInterface</code> is close enough.
	 *
	 * @param mousePosition
	 *        the <code>QPoint</code> denoting the mouse position in screen coordinates
	 * @return the closest <code>SurfelInterface</code>; may be 0
	 */
	CORE_API SurfelInterface *getClosestSurfel (const QPoint mousePosition);

	/**
	 * Selects all <code>MarkerInterface</code>s in the <code>markerList</code> and visualizes them as selected.
	 *
	 * @param markerList
	 *        a pointer to a <code>QList</code> containing the <code>MarkerInterface</code>s to be selected
	 * @param widget
	 *        a <code>QWidget</code> which is used for painting the <code>MarkerInterface</code>s
	 */
	CORE_API void selectAll (QList<MarkerInterface> *markerList, QWidget *widget);

	/**
	 * Deselects all <code>MarkerInterface</code>s in the <code>markerList</code> and visualizes them as deselected.
	 *
	 * @param markerList
	 *        a pointer to a <code>QList</code> containing the <code>MarkerInterface</code>s to be deselected
	 * @param widget
	 *        a <code>QWidget</code> which is used for painting the <code>MarkerInterface</code>s
	 */
	CORE_API void deselectAll (QList<MarkerInterface> *markerList, QWidget *widget);

	/**
	 * Renumbers the IDs of the <code>MarkerInterface</code> in the <code>markerList</code> and 
	 * updates their visualization.
	 *
	 * @param markerList
	 *        a pointer to a <code>QList</code> containing the <code>MarkerInterface</code>s to be renumbered
	 * @param widget
	 *        a <code>QWidget</code> which is used for painting the <code>MarkerInterface</code>s
	 */
	CORE_API void reallocateIDs (QList<MarkerInterface> *markerList, QWidget *widget);

	/**
	 * Selects all deselected <code>MarkerInterface</code>s in the <code>markerList</code> and vice versa
	 * and visualizes them as selected.
	 *
	 * @param markerList
	 *        a pointer to a <code>QList</code> containing the <code>MarkerInterface</code>s to be de-/selected
	 * @param widget
	 *        a <code>QWidget</code> which is used for painting the <code>MarkerInterface</code>s
	 */
	CORE_API void toggleSelection (MarkerInterface *marker, QWidget *widget);

};

#endif  // __MARKERUTILS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
