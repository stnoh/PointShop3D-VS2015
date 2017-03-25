// Title:   BrushDirWalk.h
// Created: Thu Sep 25 14:31:08 2003
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

#ifndef __BRUSHDIRWALK_H_
#define __BRUSHDIRWALK_H_

#include <qstring.h>
#include <qpopupmenu.h>
#include <qimage.h>
#include <qobject.h>
#include <vector>
#include "../../../../Utilities/DirWalk/src/DirWalk.h"
#include "../../../Brush/src/Brush.h"

/**
 * The user data needed for the <code>BrushDirWalk</code>.
 *
 * @author Matthias Zwicker
 * @version 1.2
 * @see BrushDirWalk
 */
class BrushUserData : public UserDataInterface {

public:

	/**
	 * Creates this <code>BrushUserData</code>, storing the <code>popupMenu</code> to
	 * which the entries should be added and the <code>signalReceiver</code> and its
	 * <code>signalHandler</code> which should be called whenever an entry in the
	 * <code>popupMenu</code> has been selected.
	 *
	 * @param registry
	 *        a pointer to a <code>std::vector<Brush*></code> which stores the added brushes
	 * @param popupMenu
	 *        a pointer to a<code>QPopupMenu</code> to which the entries are added while traversing
	 *        the directory structure
	 * @param signalReceiver
	 *        a pointer to the <code>QObject</code> which is to be notified whenever an entry in the
	 *        <code>popupMenu</code> has been selected
	 * @param signalHandler
	 *        a pointer to the method of the <code>signalReceiver</code> which is called whenever an
	 *        entry in the <code>popupMenu</code> has been selected
	 * @param bitmapWidth
	 *        width of single bitmap in pixels in menu entry
	 * @param bitmapHeight
	 *        height of single bitmap in pixels in menu entry
	 * @param showBitmaps
	 *        determines if brush bitmaps are displayed
	 * @param showNames
	 *        determines if brush names are displayed
	 * @param useAlphaMasks
	 *        determines if alpha masks are incorporated into bitmap
	 * @see   BrushDirWalk
	 */
	BrushUserData (std::vector<Brush*> *registry, QPopupMenu *popupMenu, QObject *signalReceiver, const char *signalHandler,
		           unsigned int bitmapWidth, unsigned int bitmapHeight, bool showBitmaps, bool showNames, bool useAlphaMasks);
	virtual ~BrushUserData();
  
	UserDataInterface *copy ();
	void copy (UserDataInterface *source);
  
private:
  
	// access for...
	friend class BrushDirWalk;
  
	QPopupMenu     *popupMenu;
	std::vector<Brush*> *registry;
	QObject        *signalReceiver;
	const char     *signalHandler;
	unsigned int   bitmapWidth,
                   bitmapHeight;
	bool           showBitmaps,
		           showNames,
				   useAlphaMasks;
};

/**
 * This class implements the <code>DirWalk</code>. It iterates recursively over the given
 * directory and looks for brushes. Entries are then generated and added to the 
 * <code>QPopupMenu</code> which has been specified in the <code>BrushUserData</code> object.
 *
 * @author Matthias Zwicker
 * @version 1.2
 * @see BrushUserData
 */
class BrushDirWalk : public DirWalk {

public:

	static const uint MORE_REGISTRY_ELEMENTS;

	/**
	 * Creates this <code>BrushDirWalk</code>, specifying the <code>brushDirectory</code>
	 * which is to be traversed.
	 *
	 * @param brushDirectory
	 *        a <code>QString</code> specifying the path to the brush directory
	 */
	BrushDirWalk (QString brushDirectory);
	virtual ~BrushDirWalk();

	DirWalk *createNewInstance (const QString directoryName);

protected:

	void applyFileMethod();
	void applyDirectoryMethod();

};



#endif  // __BRUSHDIRWALK_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
