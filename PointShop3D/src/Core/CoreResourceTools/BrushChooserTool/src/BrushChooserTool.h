// Title:   BrushChooserTool.h
// Created: Thu Sep 25 14:31:10 2003
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

#ifndef __BRUSHCHOOSERTOOL_H_
#define __BRUSHCHOOSERTOOL_H_

#include <qtoolbutton.h>
#include <qimage.h>
#include <qpopupmenu.h>

#include "../../../Interfaces/src/ResourceToolInterface.h"
#include "BrushDirWalk.h"

/**
 * This class manages a list of <code>Brush</code>es.
 *
 * @author Matthias Zwicker
 * @version 1.2
 */
class BrushChooserTool : public ResourceToolInterface {

	Q_OBJECT

public:

	// *********
	// constants
	// *********

	static const uint NO_BRUSHES;

	// ****************
	// class definition
	// ****************

	/**
	 * Construct a <code>BrushChooserTool</code>. This is a Pointshop3D tool that has its button in
	 * the tool bar. The tool is instantiated by the <code>ResourceToolBar</code> class. A pointer 
	 * to this instance is managed by the <code>CoreResourceTools</code> class.
	 * 
	 * @param brushDirectory
	 *        The data directory to be scanned for brushes.
	 * 
	 * @see #getBrushDirectory
	 * @see ResourceToolBar#addDefaultTools
	 * @see CoreResourceTools#getBrushChooserTool
	 */
	RESOURCETOOL_API BrushChooserTool (const QString brushDirectory);
	RESOURCETOOL_API virtual ~BrushChooserTool();

	/**
	 * Returns the <code>Brush</code> data directory of the available <code>Brush</code>es.
	 *
	 * @return a <code>QString</code> specifying the absolute or relative path to the <code>Brush</code> data directory
	 * @see #handleBrushDirectoryChanged
	 */
	RESOURCETOOL_API QString getBrushDirectory() const;

	/**
	 * Returns the number of brushes this <code>BrushChooserTool</code> provides.
	 *
	 * @return the number of brushes this <code>BrushChooserTool</code> provides; may be 0 in case there
	 *         are no brushes available
	 */
	RESOURCETOOL_API uint getNofBrushes() const;

	/**
	 * Makes the brush with ID <code>id</code> the currently selected one. The <code>id</code> is clipped to
	 * 0 <= id < n, where <em>n</em> is the number of brushes this <code>BrushChooserTool</code>
	 * provides.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>brushChanged</code></li>
	 * </ul>
	 *
	 * @param id
	 *        the identifier specifying the brush to be selected; is clipped to 0 <= ID < n
	 * @see #getNofBitmaps
	 * @see #brushChanged
	 */
	RESOURCETOOL_API void setBrushSelected (uint id);

	/**
	 * Returns the currently selected brush. The returned <code>Brush</code>
	 * may be 0 in case the aren't any brushes available.
	 *
	 * @return a pointer to a <code>Brush</code> representing the currently
	 *         selected brush; may be 0
	 */
	RESOURCETOOL_API Brush *getSelectedBrush();

	/**
	 * Add a brush and select it.
     *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>brushChanged</code></li>
	 * </ul>
	 */
	RESOURCETOOL_API void addAndSelectBrush(Brush *brush);
	
	/**
	 * This method updates the brush preview bitmap of a brush, which is shown in the brush chooser
	 * menu.
	 *
	 * @param brush
	 *        The brush whose preview image should be updated.
	 */
	RESOURCETOOL_API void updateBrushBitmap (Brush* brush);

	/**
	 * Returns the currently selected brush ID. A valid ID is in 0 <= ID < n, where <em>n</em> is the number
	 * of brushes this <code>BrushChooserTool</code> provides. A return value of NO_BRUSHES signals
	 * that there are no brushes available.
	 *
	 * @return the ID of the currently selected brush; valid if in [0, n[; NO_BRUSHES if there are no brushes
	 *         available
	 */
	RESOURCETOOL_API uint getSelectedBrushID() const;

	/**
	 * Sets the display parameters in the menu.
	 *
	 * @param bitmapWidth
	 *        width of single bitmap in pixels
	 * @param bitmapHeight
	 *        height of single bitmap in pixels
	 * @param showBitmaps
	 *        determines if bitmaps are displayed
	 * @param showNames
	 *        determines if names are displayed
	 * @param useAlphaMasks
	 *        determines if alpha masks are incorporated into bitmap
	 */
	RESOURCETOOL_API void setDisplayParameters (unsigned int bitmapWidth, unsigned int bitmapHeight,
		bool showBitmaps, bool showNames, bool useAlphaMasks);

	/**
	 * Gets the display parameters in the menu.
	 *
	 * @param bitmapWidth
	 *        returns the width of single bitmap in pixels
	 * @param bitmapHeight
	 *        returns the height of single bitmap in pixels
	 * @param showBitmaps
	 *        returns true if bitmaps are displayed
	 * @param showNames
	 *        returns true if names are displayed
	 * @param useAlphaMasks
	 *        returns true if alpha masks are incorporated into bitmap
	 */
	RESOURCETOOL_API void getDisplayParameters (unsigned int &bitmapWidth, unsigned int &bitmapHeight,
		bool &showBitmaps, bool &showNames, bool &useAlphaMasks);



	/**
	 * Emit the <code>brushChanged</code> signal.
	 *
	 * @see #brushChanged
	 */
	RESOURCETOOL_API void emitBrushChanged();

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
	 * adds a popup menu to the push button
	 */
	RESOURCETOOL_API void addMenuToPushButton (QPushButton *pushButton);

public slots:

	/**
	 * Sets the <code>newBrushDirectory</code> and updates the available <code>Brush</coode>es.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>brushChanged</code></li>
	 * </ul>
	 *
	 * @param newBrushDirectory
	 *        a <code>QString</code> specifying the absolute or relative path to the directory
	 *        where the <code>Brush</code> data is expected to be
	 * @see #getBrushDirectory
	 * @see #brushChanged
	 */
	void handleBrushDirectoryChanged (const QString newBrushDirectory);

signals:

	/**
	 * Emitted whenever a new brush has been selected. The <code>newBrush</code> may be
	 * 0 in case there are no valid brushes available in the given directory.
	 *
	 * @param newBrush
	 *        a pointer to a <code>Brush</code> which represents the
	 *        new brush which has been selected; may be 0 in case there
	 *        is no brush available
	 */
	void brushChanged (const Brush *newBrush);
	
private slots:
	
	void handleBrushSelected (int id);

private:

	QToolButton    *brushButton;
	QString        brushDirectory;

	Brush          *currentBrush;			
	uint           currentBrushID;

	unsigned int   bitmapWidth,
		           bitmapHeight;
	bool           showBitmaps,
		           showNames,
				   useAlphaMasks;

	// the brush registry
	std::vector<Brush*> registry;

	// the brush popup menu
	QPopupMenu     popupMenu;

	// re-scans the current 'brushDirectory' and updates the available brushes
	// emits the 'handleBrushSelected' signal
	void updateBrushes();
	void clearRegistry();

};

#endif  // __BRUSHCHOOSERTOOL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
