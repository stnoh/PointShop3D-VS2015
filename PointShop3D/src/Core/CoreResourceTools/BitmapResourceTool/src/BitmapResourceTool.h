// Title:   BitmapResourceTool.h
// Created: Thu Sep 25 14:31:11 2003
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

#ifndef __BITMAPRESOURCETOOL_H_
#define __BITMAPRESOURCETOOL_H_

#include <qtoolbutton.h>
#include <qimage.h>
#include <qpopupmenu.h>

#include "../../../Interfaces/src/ResourceToolInterface.h"
#include "BitmapDirWalk.h"

/**
 * This class implements a resource tool which offers 8bit, 24bit and/or 32bit bitmaps to other tools. Any
 * bitmap which is supported by <em>Qt</em> can be read, such as <code>BMP</code>, <code>PNG</code> or <code>JPG</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class BitmapResourceTool : public ResourceToolInterface {

	Q_OBJECT

public:

	// *********
	// constants
	// *********

	static const uint NO_BITMAPS;

	// ****************
	// class definition
	// ****************

	/**
	 * Creates this <code>BitmapResourceTool</code> which looks in <code>bitmapDirectory</code> for available
	 * bitmaps which have the specified <code>bitmapProperties</code>.
	 *
	 * @param bitmapDirectory
	 *        a <code>QString</code> specifying the absolute or relative path to the directory where the bitmaps
	 *        are expected to be
	 * @param bitmapProperties
	 *        the expected <code>BitmapProperties</code>
	 * @see BitmapUserData#BitmapProperties
	 */
	RESOURCETOOL_API BitmapResourceTool (const QString bitmapDirectory, const BitmapUserData::BitmapProperties bitmapProperties);
	RESOURCETOOL_API virtual ~BitmapResourceTool();

	/**
	 * Returns the bitmap directory of the available bitmaps.
	 *
	 * @return a <code>QString</code> specifying the absolute or relative path to the bitmap directory
	 * @see #handleBitmapDirectoryChanged
	 */
	RESOURCETOOL_API QString getBitmapDirectory() const;

	/**
	 * Specifies the new <code>BitmapProperties</code> and updates the available bitmaps.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>bitmapChanged</code></li>
	 * </ul>
	 *
	 * @param newBitmapProperties
	 *        the <code>BitmapProperties</code> of the available bitmaps
	 * @see BitmapUserData#BitmapProperties
	 * @see bitmapChanged
	 */
	RESOURCETOOL_API void setBitmapProperties (const BitmapUserData::BitmapProperties newBitmapProperties);

	/**
	 * Returns the <code>BitmapProperties</code> of the available bitmaps.
	 * 
	 * @return the <code>BitmapProperties</code> of the available bitmaps
	 */
	RESOURCETOOL_API BitmapUserData::BitmapProperties getBitmapProperties() const;

	/**
	 * Returns the number of bitmaps this <code>BitmapResourceTool</code> provides.
	 *
	 * @return the number of bitmaps this <code>BitmapResourceTool</code> provides; may be 0 in case there
	 *         are no bitmaps available
	 */
	RESOURCETOOL_API uint getNofBitmaps() const;

	/**
	 * Makes the bitmap with ID <code>id</code> the currently selected one. The <code>id</code> is clipped to
	 * 0 <= id < n, where <em>n</em> is the number of bitmaps this <code>BitmapResourceTool</code>
	 * provides.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>bitmapChanged</code></li>
	 * </ul>
	 *
	 * @param id
	 *        the identifier specifying the bitmap to be selected; is clipped to 0 <= ID < n
	 * @see #getNofBitmaps
	 * @see #bitmapChanged
	 */
	RESOURCETOOL_API void setBitmapSelected (uint id);

	/**
	 * Returns the currently selected bitmap. The returned <code>QImage</code>
	 * may be 0 in case the aren't any bitmaps available.
	 *
	 * @return a pointer to a <code>QImage</code> representing the currently
	 *         selected bitmap; may be 0
	 */
	RESOURCETOOL_API const QImage *getSelectedBitmap() const;

	/**
	 * Returns the currently selected bitmap ID. A valid ID is in 0 <= ID < n, where <em>n</em> is the number
	 * of bitmaps this <code>BitmapResourceTool</code> provides.. A return value of NO_BITMAPS signals
	 * that there are no bitmaps available.
	 *
	 * @return the ID of the currently selected bitmap; valid if in [0, n[; NO_BITMAPS if there are no bitmaps
	 *         available
	 */
	RESOURCETOOL_API uint getSelectedBitmapID() const;

	// *******************************
	// interface ResourceToolInterface
	// *******************************

	/**
	 * Creates a tool button and adds it to the <code>toolBar</code>. The <code>QToolButton</code>
	 * may also activate <code>QPopupMenu</code>s, for example.
	 *
	 * @param toolBar
	 *        a pointer to the <code>QToolBar</code> to which this tool has to be added
	 * @return   a pointer to the <code>QToolButton</code> which was created and added
	 *           to the <code>toolBar</code>
	 */
	RESOURCETOOL_API QToolButton *addButtonToToolBar (QToolBar *toolBar);
	
	/**
	 * empty method, for implementing <code>ResourceToolInterface</code>
	 */
	RESOURCETOOL_API void addMenuToPushButton (QPushButton *pushButton);

public slots:

	/**
	 * Sets the <code>newBitmapDirectory</code> and updates the available bitmaps.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>bitmapChanged</code></li>
	 * </ul>
	 *
	 * @param newBitmapDirectory
	 *        a <code>QString</code> specifying the absolute or relative path to the directory
	 *        where the bitmaps are expected to be
	 * @see #getBitmapDirectory
	 * @see #bitmapChanged
	 */
	void handleBitmapDirectoryChanged (const QString newBitmapDirectory);
	
private slots:
	
	void handleBitmapSelected (int id);

signals:

	/**
	 * Emitted whenever a new bitmap has been selected. The <code>newBitmapImage</code> may be
	 * 0 in case there are no valid bitmaps available in the given directory.
	 *
	 * @param newBitmapImage
	 *        a pointer to a <code>QImage</code> which represents the
	 *        new bitmap which has been selected; may be 0 in case there
	 *        is no texture image available
	 */
	void bitmapChanged (const QImage *newBitmapImage);

private:

	QToolButton                      *bitmapButton;
	QString                          bitmapDirectory;
	BitmapUserData::BitmapProperties bitmapProperties;

	const QImage                     *currentBitmap;
	uint                             currentBitmapID;

	// the bitmap registry
	std::vector<QImage*>                  registry;

	// the bitmap popup menu
	QPopupMenu                       popupMenu;

	// re-scans the current 'bitmapDirectory' and updates the available bitmaps
	// emits the 'handleBitmapSelected' signal
	void updateBitmaps();
	void clearRegistry();

};

#endif  // __BITMAPRESOURCETOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
