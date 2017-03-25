// Title:   CameraPositionMenu.h
// Created: Thu Sep 25 14:43:18 2003
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

#ifndef __CAMERAPOSITIONMENU_H_
#define __CAMERAPOSITIONMENU_H_

#include <qwidget.h>
#include <qpopupmenu.h>

#include "../DataTypes/src/MyDataTypes.h"

/**
 * This class implements a <code>QPopupMenu</code> which stores and restores user defined camera positions.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class CameraPositionMenu : public QPopupMenu {

	Q_OBJECT

public:

	/**
	 * The number of camera positions which may be stored in this <code>CameraPositionMenu</code>.
	 */
	static const int NOF_CAMERA_POSITIONS;

	/**
	 * Creates this <code>CameraPositionMenu</code> with <code>parent</code> as parent widget and
	 * <code>name</code> as identifier.
	 *
	 * @param parent
	 *        a pointer to the parent <code>QWidget</code>
	 * @param name
	 *        a pointer to a string which contains the name of this <code>QPopupMenu</code>
	 */
	CameraPositionMenu (QWidget *parent = 0, const char *name = 0);
	virtual ~CameraPositionMenu();

	/**
	 * Sets (stores) the <code>cameraPosition</code> with index <code>index</code>.
	 *
	 * @param newCameraPosition
	 *        the <code>CameraPosition</code> to be stored at <code>index</code>
	 * @param index
	 *        the index of the camera position to be modified; must be in [0, NOF_CAMERA_POSITIONS - 1]
	 * @see #NOF_CAMERA_POSITIONS
	 */
	void setCameraPosition (const MyDataTypes::CameraPosition newCameraPosition, const int index);

	/**
	 * Returns the <code>CameraPosition</code> stored at index <code>index</code>.
	 *
	 * @param index
	 *        the index of the camera position to be returned; must be in [0, NOF_CAMERA_POSITIONS - 1]
	 * @return the <code>CameraPosition</code> stored at index <code>index</code>
	 * @see #NOF_CAMERA_POSITIONS
	 */
	MyDataTypes::CameraPosition getCameraPosition (const int index) const;

	/**
	 * Restores the <code>CameraPosition</code> stored at index <code>index</code>, that is the camera
	 * position in the <code>Scene</code> is set.
	 *
	 * @param index
	 *        the index of the camera position to be restored; must be in [0, NOF_CAMERA_POSITIONS - 1]
	 * @see Scene#setCameraPosition
	 */
	void restoreCameraPosition (const int index);

private:
	
	QPopupMenu                          *storeMenu,
		                                *restoreMenu;

	void setRestoreMenuItem(int index);

private slots:

	void handleStore1();
	void handleStore2();
	void handleStore3();
	void handleStore4();
	void handleStore5();
	void handleStore6();
	void handleStore7();
	void handleStore8();
	void handleStore9();
	void handleStore10();

	void handleRestore1();
	void handleRestore2();
	void handleRestore3();
	void handleRestore4();
	void handleRestore5();
	void handleRestore6();
	void handleRestore7();
	void handleRestore8();
	void handleRestore9();
	void handleRestore10();	
	
};

#endif  // __CAMERAPOSITIONMENU_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
