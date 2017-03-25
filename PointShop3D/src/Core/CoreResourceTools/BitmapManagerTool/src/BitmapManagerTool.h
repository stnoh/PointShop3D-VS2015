// Title:   BitmapManagerTool.h
// Created: Thu Sep 25 14:31:20 2003
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


#ifndef __BITMAPMANAGERTOOL_H_
#define __BITMAPMANAGERTOOL_H_

#include <qtoolbutton.h>
#include <qimage.h>

#include "../../../Interfaces/src/ResourceToolInterface.h"

/**
 * This class implements a bitmap manager.
 *
 * @author Mark Pauly
 * @version 1.2
 */
class BitmapManagerTool : public ResourceToolInterface {

	Q_OBJECT

public:


	typedef enum {
		CUSTOM_SIZE = 0
	} Sizetype;


	/**
	 * creates the bitmap manager
	 */
	RESOURCETOOL_API BitmapManagerTool();
	/**
	 * destroys the bitmap manager
	 */
	RESOURCETOOL_API virtual ~BitmapManagerTool();

	/**
	 * reads all the brush bitmaps if this has not been done yet
	 */
	RESOURCETOOL_API void readBrushBitmaps();
	
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
	


private:

	// direct access for...
	friend class BitmapManagerDialogImpl;


	QToolButton    *bitmapButton;
	QSize          commonSize;


private slots:

	void handleBitmapManagerDialog();

};

#endif  // __BITMAPMANAGERTOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
