// Title:   ResourceToolBar.h
// Created: Thu Sep 25 14:42:59 2003
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

#ifndef __RESOURCETOOLBAR_H_
#define __RESOURCETOOLBAR_H_

#include <qtoolbar.h>
#include <qbuttongroup.h>
#include <qstring.h>
#include <qevent.h>

#include "../Core/CoreResourceTools/BitmapManagerTool/src/BitmapManagerTool.h"
#include "../Core/CoreResourceTools/ColorChooserTool/src/ColorChooserTool.h"
#include "../Core/CoreResourceTools/BrushChooserTool/src/BrushChooserTool.h"

/**
 * This class implements a <code>QToolBar</code> which contains all
 * the resource tools, such as the color chooser tool.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ResourceToolBar : public QToolBar {

public:

	ResourceToolBar (const QString &label,
		             QMainWindow *mainWindow,
					 QMainWindow::ToolBarDock toolBarDock = QMainWindow::Top,
					 bool newLine = false,
					 const char *name = 0);
	virtual ~ResourceToolBar();

	/**
	 * Initializes this toolbar, adding all the resource tools to it.
	 */
	void initialize();

private:

	BitmapManagerTool         *bitmapManagerTool;	
	ColorChooserTool          *colorChooserTool;
	BrushChooserTool	      *brushChooserTool;

	// creates and adds the default tols
	void addDefaultTools();
	void cleanUp();

};

#endif  // __RESOURCETOOLBAR_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
