// Title:   ColorChooserTool.h
// Created: Thu Sep 25 14:31:05 2003
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

#ifndef __COLORCHOOSERTOOL_H_
#define __COLORCHOOSERTOOL_H_

#include <qwidget.h>
#include <qtoolbutton.h>
#include <qpixmap.h>
#include <qcursor.h>
#include <qobject.h>

// hack around MSVC 6.0 compiler bug (release mode only)
#if defined (_WIN32) && defined (NDEBUG)
// the include path as to be added in _every_ project which depends on this header file
#include <ResourceToolInterface.h>
#else
#include "../../../Interfaces/src/ResourceToolInterface.h"
#endif

/**
 * This class implements a tool with which allows the user to choose the
 * foreground and background color. On activation of this tool a color
 * palette pops up.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ColorChooserTool : public ResourceToolInterface {

	Q_OBJECT

public:

	RESOURCETOOL_API ColorChooserTool();
	RESOURCETOOL_API virtual ~ColorChooserTool();

	/**
	 * This method allows external tools to set the foreground (FG) color.
	 * 
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>fgColorChanged</code></li>
	 * </ul>
	 *
	 * @param newFGColor
	 *        the new <code>QRgb</code> color which specifies the foreground (FG)
	 *        color
	 * @see #fgColorChanged
	 */
	RESOURCETOOL_API void setFGColor (QRgb newFGColor);

	/**
	 * Returns the currently selected foreground (FG) color.
	 *
	 * @return the <code>QRgb</code> foreground (FG) color
	 */
	RESOURCETOOL_API QRgb getFGColor();

	/**
	 * This method allows external tools to set the background (BG) color.
	 * 
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>bgColorChanged</code></li>
	 * </ul>
	 *
	 * @param newBGColor
	 *        the new <code>QRgb</code> color which specifies the background (BG)
	 *        color
	 * @see #bgColorChanged
	 */
	RESOURCETOOL_API void setBGColor (QRgb newBGColor);

	/**
	 * Returns the currently selected background (BG) color.
	 *
	 * @return the <code>QRgb</code> background (BG) color
	 */
	RESOURCETOOL_API QRgb getBGColor();

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

signals:

	/**
	 * Emitted whenever the foreground (FG) color has been changed.
	 *
	 * @param newFGColor
	 *        the new foreground (FG) color which has been selected
	 */
	void fgColorChanged (QRgb newFGColor);

	/**
	 * Emitted whenever the background (BG) color has been changed.
	 *
	 * @param newBGColor
	 *        the new background (BG) color which has been selected
	 */
	void bgColorChanged (QRgb newBGColor);

private slots:

	void handleChooseFGColor();
	void handleChooseBGColor();

private:

	QCursor     *toolCursor;
	// foreground color
	QRgb        fgColor;
	// background color
	QRgb        bgColor;
	QPixmap     *pixmap;
	QToolButton *colorChooserButton;

	void updatePixmap();
	
};

#endif  // __COLORCHOOSERTOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
