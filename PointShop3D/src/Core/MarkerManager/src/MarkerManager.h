// Title:   MarkerManager.h
// Created: Thu Sep 25 14:28:52 2003
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

#ifndef __MARKERLISTS_H_
#define __MARKERLISTS_H_

#include <qobject.h>
#include <qwidget.h>
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

#include "../../src/CoreDLL.h"
#include "Marker2D.h"
#include "Marker3D.h"

/**
 * This <code>MarkerManager</code> provides the fitting constraint lists for the 2D- and 3D-case.
 * Auto-deletion is enabled for all lists, so all markers are automatically <code>delete</code> as soon as they
 * are removed. This <code>MarkerManager</code> automatically clears the 3D markers as soon as new objects
 * are added or loaded to/into the <code>Scene</code>, since this might invalidate the references of the <code>Marker3D</code>s to the
 * <code>SurfelInterface</code>s.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class MarkerManager : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates an instance of this <code>MarkerManager</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>MarkerManager</code>
	 */
	CORE_API static MarkerManager *getInstance();

	/**
	 * Destroys the instance of this <code>MarkerManager</code>, if there is any.
	 */
	CORE_API static void destroyInstance();

	/**
	 * Initializes this <code>MarkerManager</code> and tells the markers into which <code>QWidget</code>s
	 * they have to be painted into. Must be called at least once before any other method.
	 *
	 * @param widget2D
	 *        a pointer to a <code>QWidget</code> into which the <code>Marker2D</code>s are painted
	 * @param widget3D
	 *        a pointer to a <code>QWidget</code> into which the <code>Marker3D</code>s are painted
	 */
	CORE_API void initialize (QWidget *widget2D, QWidget *widget3D);

	/**
	 * Sets the <code>QWidget</code> where all <code>Marker2D</code> are painted into.
	 *
	 * @param newWidget2D
	 *        a pointer to the <code>QWidget</code> where all <code>Marker2D</code> are painted into
	 */
	CORE_API void setWidget2D (QWidget *newWidget2D);

	/** 
	 * Returns the <code>QWidget</code> where all <code>Marker2D</code> are painted into.
	 *
	 * @return a pointer to the <code>QWidget</code> where all <code>Marker2D</code> are painted into
	 */
	CORE_API QWidget *getWidget2D() const;

	/**
	 * Sets the <code>QWidget</code> where all <code>Marker3D</code> are painted into.
	 *
	 * @param newWidget3D
	 *        a pointer to the <code>QWidget</code> where all <code>Marker3D</code> are painted into
	 */
	CORE_API void setWidget3D (QWidget *newWidget3D);

	/** 
	 * Returns the <code>QWidget</code> where all <code>Marker3D</code> are painted into.
	 *
	 * @return a pointer to the <code>QWidget</code> where all <code>Marker3D</code> are painted into
	 */
	CORE_API QWidget *getWidget3D() const;

	/**
	 * Returns a list containing all fitting constraint markers which mark the 2D-pixmaps.
	 *
	 * @return a pointer to a <code>QList</code> containing fitting constraint <code>Marker2D</code>s.
	 * @see Marker2D
	 */
	CORE_API const QList<Marker2D> *getFittingConstraints2D() const;

	/**
	 * Returns a list containing all fitting constraint markers which mark the 3D-<code>Model</code>.
	 *
	 * @return a pointer to a <code>QList</code> containing fitting constraint <code>Marker3D</code>s.
	 * @see Marker3D
	 */
	CORE_API const QList<Marker3D> *getFittingConstraints3D() const;

	/**
	 * A convenience method which returns the minimum number of elements in the 2D- and 3D-fitting constraint lists
	 *
	 * @return the minimum count of the fitting constraint lists
	 */
	CORE_API uint getMinimumFittingConstraintListCount() const;

	/**
	 * Adds a fitting constraint 2D marker.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints2DChanged</code></li>
	 * </ul>
	 *
	 * @param marker2D
	 *        a pointer to a <code>Marker2D</code> to be added
	 *
	 * @see #fittingConstraints2DChanged
	 */
	CORE_API void addFittingConstraint2D (const Marker2D *marker2D);

	/**
	 * Adds multiple fitting constraint 2D markers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints2DChanged</code></li>
	 * </ul>
	 *
	 * @param markers2D
	 *        a pointer to a <code>QList</code> containing the <code>Marker2D</code>s to be added
	 * @see #fittingConstraints2DChanged
	 */
	CORE_API void addFittingConstraints2D (const QList<Marker2D> *markers2D);

	/**
	 * Removes the fitting constraint <code>marker2D</code>.
	 * 
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints2DChanged</code></li>
	 * </ul>
	 *
	 * @param marker2D
	 *        a pointer to the <code>Marker2D</code> to be removed
	 * @see #fittingConstraints2DChanged
	 */
	CORE_API void removeFittingConstraint2D (Marker2D *marker2D);

	/**
	 * Removes all <em>selected</em> fitting constraint markers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints2DChanged</code></li>
	 * </ul>
	 *
	 * @see #fittingConstraints2DChanged
	 */
	CORE_API void removeSelectedFittingConstraints2D();

	/**
	 * Removes <em>all</em> fitting constraint markers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints2DChanged</code></li>
	 * </ul>
	 *
	 * @see #fittingConstraints3DChanged
	 */
	CORE_API void removeAllFittingConstraints2D();
	
	/**
	 * Adds a fitting constraint 3D marker.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints3DChanged</code></li>
	 * </ul>
	 *
	 * @param marker3D
	 *        a pointer to a <code>Marker3D</code> to be added
	 * @see #fittingConstraints3DChanged
	 */
	CORE_API void addFittingConstraint3D (const Marker3D *marker3D);

	/**
	 * Adds multiple fitting constraint 3D markers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints3DChanged</code></li>
	 * </ul>
	 *
	 * @param markers3D
	 *        a pointer to a <code>QList</code> containing the <code>Marker3D</code>s to be added
	 * @see #fittingConstraints3DChanged
	 */
	CORE_API void addFittingConstraints3D (const QList<Marker3D> *markers3D);

	/**
	 * Removes the fitting constraint <code>marker3D</code>.
	 * 
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints3DChanged</code></li>
	 * </ul>
	 *
	 * @param marker3D
	 *        a pointer to the <code>Marker3D</code> to be removed
	 * @see #fittingConstraints3DChanged
	 */
	CORE_API void removeFittingConstraint3D (Marker3D *marker3D);

	/**
	 * Removes all <em>selected</em> fitting constraint markers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints3DChanged</code></li>
	 * </ul>
	 *
	 * @see #fittingConstraints3DChanged
	 */
	CORE_API void removeSelectedFittingConstraints3D();

	/**
	 * Removes <em>all</em> fitting constraint markers.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fittingConstraints3DChanged</code></li>
	 * </ul>
	 *
	 * @param visualize
	 *        set to <code>true</code> if the visualization of the fitting constraint markers should be
	 *        updated; <code>false</code> else
	 *
	 * @see #fittingConstraints3DChanged
	 */
	CORE_API void removeAllFittingConstraints3D (const bool visualize);

	/**
	 * Given a <code>surfel</code>, this function returns the <code>Marker3D</code>
	 * that contains the <code>surfel</code>. If no <code>Marker3D</code>
	 * contains the surfel, it returns 0.
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> for which the <code>Marker3D</code>
	 *        has to be returned
	 * @return a pointer to the <code>Marker3D</code> which is associated with the <code>surfel</code>
	 */
	CORE_API Marker3D* getMarker3D(const SurfelInterface *surfel);

	/**
	 * Paints all <code>Marker2D</code>s in inverse mode.
	 */
	CORE_API void toggleAllFittingConstraint2DVisualization();

	/**
	 * Paints all <code>Marker3D</code>s in inverse mode.
	 */
	CORE_API void toggleAllFittingConstraint3DVisualization();

signals:

	/**
	 * Emitted whenever the <code>Marker2D</code> list has changed.
	 *
	 * @param fittingConstraint2DList
	 *        a pointer to the updated <code>QList</code> containing the <code>Marker2D</code>s
	 */
	void fittingConstraints2DChanged (const QList<Marker2D> *fittingConstraint2DList);

	/**
	 * Emitted whenever the <code>Marker3D</code> list has changed.
	 *
	 * @param fittingConstraint3DList
	 *        a pointer to the updated <code>QList</code> containing the <code>Marker3D</code>s
	 */
	void fittingConstraints3DChanged (const QList<Marker3D> *fittingConstraint3DList);

private:

	QWidget *widget2D,
		    *widget3D;

	// the texture *should* go through this points
	QList<Marker3D> fittingConstraints3D;

	// the texture *should* go through this points
	QList<Marker2D> fittingConstraints2D;

	MarkerManager();
protected:  // To suppress warnings...
	virtual ~MarkerManager();
private:

private slots:

	void handleMainRendererChanged();
	void handleActiveObjectAboutToChange();

};



#endif  // __MARKERLISTS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
