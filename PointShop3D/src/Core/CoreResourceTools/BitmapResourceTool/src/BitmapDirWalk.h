// Title:   BitmapDirWalk.h
// Created: Thu Sep 25 14:31:13 2003
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

#ifndef __BITMAPDIRWALK_H_
#define __BITMAPDIRWALK_H_

#include <qstring.h>
#include <vector>
#include <qpopupmenu.h>
#include <qimage.h>
#include <qobject.h>

#include "../../../../Utilities/DirWalk/src/DirWalk.h"

/**
 * The user data needed for the <code>BitmapDirWalk</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see BitmapDirWalk
 */
class BitmapUserData : public UserDataInterface {

public:

	/**
	 * Flags which specify the properties of bitmap images:
	 * <ul>
	 *   <li><b>DEPTH_1:</b> 1 bit per pixel</li>
	 *   <li><b>DEPTH_8:</b> 8 bits per pixel</li>
	 *   <li><b>DEPTH_16:</b> 16 bits per pixel</li>
	 *   <li><b>DEPTH_32:</b> 32 bits per pixel</li>
	 *   <li><b>GRAYSCALE:</b> all pixel entries are grayscales <em>and</em> for 8 bit images the <em>i</em>th color
	 *                         entry in the LUT is <code>QRgb (i,i,i)</code>, that is the LUT represents a grayscale
	 *                         ramp</li>
	 *   <li><b>COLOR</b> at least one pixel entry is color</li>
	 * </ul>
	 * The flags may be OR'ed together to specify multiple choices.
	 *
	 * <b>Note:</b> At least one of the depths <em>and</em> one or both <code>GRAYSCALE</code> or <code>COLOR</code>
	 *              have to be specified.
	 */
	typedef enum bitmapProperties {
		
		DEPTH_1   = 0x0000001,  // choose at least one of those
		DEPTH_8   = 0x0000002,
		DEPTH_16  = 0x0000004,
		DEPTH_32  = 0x0000008,

		GRAYSCALE = 0x0000010,  // AND of those
		COLOR     = 0x0000020

	} BitmapProperties;

	/**
	 * Creates this <code>BitmapUserData</code>, storing the <code>popupMenu</code> to
	 * which the entries should be added and the <code>signalReceiver</code> and its
	 * <code>signalHandler</code> which should be called whenever an entry in the
	 * <code>popupMenu</code> has been selected.
	 *
	 * @param bitmapProperties
	 *        the <code>BitmapProperties</code> flags which specify which depth and color the bitmaps must have;
	 *        the flags may be OR'ed together
	 * @param registry
	 *        a pointer to a <code>std::vector<QImage*></code> which stores the added bitmaps
	 * @param popupMenu
	 *        a pointer to a<code>QPopupMenu</code> to which the entries are added while traversing
	 *        the directory structure
	 * @param signalReceiver
	 *        a pointer to the <code>QObject</code> which is to be notified whenever an entry in the
	 *        <code>popupMenu</code> has been selected
	 * @param signalHandler
	 *        a pointer to the method of the <code>signalReceiver</code> which is called whenever an
	 *        entry in the <code>popupMenu</code> has been selected
	 * @see   BitmapDirWalk
	 */
	BitmapUserData (BitmapProperties bitmapProperties, std::vector<QImage*> *registry, QPopupMenu *popupMenu, QObject *signalReceiver, const char *signalHandler);
	virtual ~BitmapUserData();
  
	/**
	 * builds and returns a copy of this <code>BitmapUserData</code>
	 */
	UserDataInterface *copy ();
	
	/**
	 * copies the data from <code>source</code>
	 *
	 * <b>Note</b>:source needs to be of type <code>BitmapUserData</code>
	 */
	void copy (UserDataInterface *source);
  
private:
  
	// access for...
	friend class BitmapDirWalk;
  
	BitmapProperties bitmapProperties;
	QPopupMenu       *popupMenu;
	std::vector<QImage*>  *registry;
	QObject          *signalReceiver;
	const char       *signalHandler;
  
};

/**
 * This class implements the <code>DirWalk</code>. It iterates recursively over the given
 * directory and looks for images which can be used as textures. Entries are
 * then generated and added to the <code>QPopupMenu</code> which has been specified in
 * the <code>BitmapUserData</code> object.
 * 
 * @author Oliver Knoll
 * @version 1.2
 * @see BitmapUserData
 */
class BitmapDirWalk : public DirWalk {

public:

	/**
	 * Creates this <code>BitmapDirWalk</code>, specifying the <code>textureDirectory</code>
	 * which is to be traversed.
	 *
	 * @param textureDirectory
	 *        a <code>QString</code> specifying the path to the brush directory
	 */
	BitmapDirWalk (QString textureDirectory);
	virtual ~BitmapDirWalk();

	DirWalk *createNewInstance (const QString directoryName);

protected:

	void applyFileMethod();
	void applyDirectoryMethod();

};



#endif  // __BITMAPDIRWALK_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
