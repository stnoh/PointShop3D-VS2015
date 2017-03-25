// Title:   SelectionTool.h
// Created: Thu Sep 25 14:30:36 2003
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

#ifndef __SELECTIONTOOL_H_
#define __SELECTIONTOOL_H_

#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qcursor.h>
#include <qevent.h>
#include <qwidget.h>
#include <qobject.h>
#include <qpopupmenu.h>
#include <qpointarray.h>
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
#include <qpainter.h>

#include "../../../Interfaces/src/ToolInterface.h"
#include "../../../Interfaces/src/SurfelInterface.h"
#include "../../../OrthoMapper/src/OrthoMapper.h"
#include "../../../Painter/src/Painter.h"
#include "../../../Brush/src/Brush.h"
#include "../UI/SelectionConfigurationWidget.h"

/**
 * This class implements a selection tool. The selection tool provides several tools such
 * as rectangle selection, polygon selection, ellipse selection, lasso selection and brush painting selection.
 * Currently the system supports three different selections that can be activated by the user using the tool settings
 * dialog. The selections are defined by the flags <code>SELECTED1</code>, <code>SELECTED2</code> and <code>SELECTED3</code>.
 * Only one selection is active at a time and in the documentation we use the symbol <code>SELECTED*</code> to refer
 * to the currently active selection.
 *
 * @author Oliver Knoll, Mark Pauly
 * @version 1.2
 */

class SelectionTool : public ToolInterface {

	Q_OBJECT

public:
	
   /**
    * creates the selection tool
	*/
	TOOL_API SelectionTool();
   /*
    * destroys the selection tool
	*/
	TOOL_API virtual ~SelectionTool();


	/**
	 * Puts all visible <code>SurfelInterface</code>s into the currently active selection, that is, the <code>SELECTED*</code> and
	 * <code>EMPHASISE</code> flag are set to <code>true</code>. The image is re-rendered if
	 * <code>visualizeSelection</code> is set to <code>true</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>selectionChanged</code></li>
	 * </ul>
	 * @param visualizeSelection
	 *        set to <code>true</code> if the image should be re-rendered; <code>false</code> else
	 * @see #selectionChanged
	 */
	TOOL_API void selectAllVisible (const bool visualizeSelection);

	/**
	 * Selects all <code>SurfelInterface</code>s in the <em>active</em> <code>Object</code>,
	 * that is, the <code>SELECTED*</code> and
	 * <code>EMPHASISE</code> flag are set to <code>true</code>. The image is re-rendered if
	 * <code>visualizeSelection</code> is set to <code>true</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>selectionChanged</code></li>
	 * </ul>
	 * @param visualizeSelection
	 *        set to <code>true</code> if the image should be re-rendered; <code>false</code> else
	 * @see Scene#getActiveObject
	 * @see #selectionChanged
	 */
	TOOL_API void selectAllActiveObject (const bool visualizeSelection);

	/**
	 * Returns the selected <code>SurfelInterface</code>s in a <code>std::vector<SurfelInterface*></code>. If no surfels are
	 * selected, the returned vector doesn't contain any pointers to surfels.
	 * Note that in the later case <code>hasSelection</code> returns <code>false</code>.
	 * 
	 * @param selectionFlag
	 *        the flag specifying the selection
	 * @return a pointer to a vector which contains pointers to the selected 
	 *         <code>SurfelInterface</code>s
	 * @see #hasSelection
	 */
	TOOL_API std::vector<SurfelInterface*> *getSelection (SurfelInterface::Flag selectionFlag = SurfelInterface::SELECTED1);

	/**
	 * Returns the <code>SurfelInterface</code>s in a <code>std::vector<SurfelInterface*></code> that are <em>not</em> selected. If all surfels are
	 * selected, the returned vector doesn't contain any pointers to surfels.
	 * 
	 * @param allNotSelected
	 *         the vector which contains pointers to the not selected <code>SurfelInterface</code>s
	 * @param selectionFlag
	 *        the flag specifying the selection
	 */
	TOOL_API void getAllNotSelected (std::vector<SurfelInterface*> &allNotSelected, SurfelInterface::Flag selectionFlag);

	/**
	 * Returns the number of surfels in the selection
	 * 
	 * @param selectionFlag
	 *        the flag specifying the selection
	 */
	TOOL_API int getSelectionSize (SurfelInterface::Flag selectionFlag = SurfelInterface::SELECTED1);

	/**
	 * Deselects all selected <code>SurfelInterface</code>s in the active selection, that is, the <code>SELECTED*</code> and <code>EMPHASISE</code>
	 * flag are set to <code>false</code>.
	 * The image is re-rendered if <code>visualizeSelection</code> is set to <code>true</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>selectionChanged</code></li>
	 * </ul>
	 *
	 * @param visualizeSelection
	 *        set to <code>true</code> if the image should be re-rendered; <code>false</code> else
	 * @see #selectionChanged
	 */
	TOOL_API void clearSelection (const bool visualizeSelection, const bool emitSignal = true);

	/**
	 * Inverts the active selection, that is, the <code>SELECTED*</code> flag is inverted.
	 * The image is re-rendered if <code>visualizeSelection</code> is set to <code>true</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>selectionChanged</code></li>
	 * </ul>
	 *
	 * @param visualizeSelection
	 *        set to <code>true</code> if the image should be re-rendered; <code>false</code> else
	 * @see #selectionChanged
	 */
	TOOL_API void invertSelection (const bool visualizeSelection);

	/**
	 * Shuffles the selections.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>selectionChanged</code></li>
	 * </ul>
	 *
	 * @param visualizeSelection
	 *        set to <code>true</code> if the image should be re-rendered; <code>false</code> else
	 * @see #selectionChanged
	 */
	TOOL_API void shuffleSelection (const bool visualizeSelection);

	/**
	 * Returns <code>true</code>, if the active selection is not empty. The selected surfels can be received with
	 * <code>getSelection</code>.
	 *
	 * @return <code>true</code>, if there are selected surfels
	 * @see #getSelection
	 */
	TOOL_API bool hasSelection();

	/**
	 * Sets the active selection
	 * 
	 * @param activeSelection
	 *        index into the list of selections
	 *
	 */
	TOOL_API void setActiveSelection (int activeSelection);

	/**
	 * Gets the index of the active selection
	 * 
	 * @return  the index of the active selection
	 *
	 */
	TOOL_API int getActiveSelectionIndex () const;
	

	/**
	 * counts the number of surfels for each selection
	 */
	TOOL_API void updateSelectionCounters ();

	/**
	 * Emits the signal <code>selectionChanged</code> for all selections.
	 *
	 * @see #selectionChanged
	 */
	TOOL_API void emitAllSelectionsChanged();

	// ***********************
	// interface ToolInterface
	// ***********************

	/**
	 * Creates a tool button and adds it to the <code>toolBar</code>. The <code>QToolButton</code>
	 * may also activate <code>QPopupMenu</code>s, for example.
	 *
	 * @param toolBar
	 *        a pointer to the <code>QToolBar</code> to which this tool has to be added
	 * @return   a pointer to the <code>QToolButton</code> which was created and added
	 *           to the <code>toolBar</code>
	 */
	TOOL_API QToolButton *addButtonToToolBar (QToolBar *toolBar);

	/**
	 * Returns the <code>QToolButton</code> of this <code>ToolInterface</code>, after it has been created
	 * with <code>addButtonToToolBar</code>. Useful for enabling/disabling a tool by enabling/disabling
	 * its tool button.
	 *
	 * @return a pointer to a <code>QToolButton</code> which has been created by <code>addButtonToToolBar</code>
	 * @see #addButtonToToolBar
	 */
	TOOL_API QToolButton *getToolButton();
	
	/**
	 * Returns the tool cursor which is to be displayed as soon as the mouse is over
	 * the renderer area.
	 *
	 * @return the <code>QCursor</code> of this <code>ToolInterface</code>
	 */
	TOOL_API QCursor *getToolCursor() const;
	
	/**
	 * Returns a <code>QWidget</code> with control elements which control the settins of this
	 * <code>ToolInterface</code>. This widget is then added to a toplevel tool
	 * configuration dialog. The <code>QWidget</code> is <code>delete</code> as soon as
	 * this <code>ToolInterface</code> is <code>delete</code>d, that is, there is no need
	 * to <code>delete</code> the <code>QWidget</code> after use.
	 *
	 * @return a <code>QWidget</code> with control elements; may be 0
	 */ 
	TOOL_API QWidget *getToolConfigurationWidget() const;

	/**
	 * Returns <code>true</code> if this <code>ToolInterface</code> is selectable, <code>false</code>
	 * if any conditions this <code>ToolInterface</code> requires are not met. Conditions for
	 * the availability of a <code>ToolInterface</code> might include:
	 * <ul>
	 *   <li>an <code>Object</code> in the <code>Scene</code> is available</li>
	 *   <li>the <em>main</em> <code>RendererInterface</code> is an <em>interactive</em> renderer</li>
	 *   <li>a <code>Brush</code> is available</li>
	 * </ul>
	 *
	 * @return <code>true</code> if this <code>ToolInterface</code> is available; <code>false</code> else
	 */
	TOOL_API bool isAvailable() const;

	/**
	 * Handles mouse press events which occured when the mouse was over the
	 * renderer area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	TOOL_API void handleMousePressEvent (QMouseEvent *event);

	/**
	 * Handles mouse release events which occured when the mouse was over the
	 * render area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	TOOL_API void handleMouseReleaseEvent (QMouseEvent *event);

	/**
	 * Handles mouse move events which occured when the mouse was over the
	 * render area.
	 *
	 * @param event
	 *        the specific <code>QMouseEvent</code>
	 */
	TOOL_API void handleMouseMoveEvent (QMouseEvent *event);
	
	/**
	 * Handles key press events which are not handled by the application
	 * itself.
	 *
	 * @param event
	 *        the specific <code>QKeyEvent</code>
	 */
	TOOL_API void handleKeyPressEvent (QKeyEvent *event);

	/**
	 * Handles key release events which are not handled by the application
	 * itself.
	 *
	 * @param event
	 *        the specific <code>QKeyEvent</code>
	 */
	TOOL_API void handleKeyReleaseEvent (QKeyEvent *event);

	/**
	 * This method is called whenever the tool is selected.
	 */
	TOOL_API void selectTool();

	/**
	 * This method is called whenever the tool is deselected.
	 */
	TOOL_API void deselectTool();
	

signals:

	/**
	 * Emitted whenever the selection has changed. The <code>selection</code> contains the selected
	 * <code>SurfelInterface</code>s.
	 *
	 * @param selectionFlag
	 *        flag of the modified selection
	 * <code>SurfelInterface</code>s
	 */
	void selectionChanged (SurfelInterface::Flag selectionFlag);

public slots:

	void handleRendererWidgetRepainted();
	
private:

	SelectionConfigurationWidget *configurationWidget;
	QCursor                *toolCursor;
	QIconSet               ellipseIconSet,
                           lassoIconSet,
					       polygonIconSet,
                           rectangleIconSet,
						   brushIconSet,
						   floodFillIconSet;
	QToolButton            *toolButton;
	QPopupMenu             selectionMenu;
	int                    ellipseID,
	                       lassoID,
					       polygonID,
	                       rectangleID,
						   brushID,
						   floodFillID,
	                       currentTool;
	QRect                  currentEllipseRect,
		                   currentRectangleRect;
	QPointArray            currentPolyLine;
	
	uint                   nofPolyPoints;   // number of points in the polyline
	bool                   isPolygonLineBeingConstructed;
	QPoint                 oldMousePosition,
		                   startMousePosition;
	QPoint                 lastMouseDragPosition;
	Vector2D			   screenPosition;

	std::vector<SurfelInterface*> *selectionLists;
	bool                   *selectionsValid;

	int                    *selectionsNumSurfels;


	bool                   currentPositionValid;
	OrthoMapper            *orthoMapper;
    Painter                *painter;
	const Brush            *currentBrush;
   
	bool                   select;  // true for select, false for de-select

	void initiateSelectOperation (bool select);
	void initiateFloodFillOperation (int x, int y, bool select);
	SurfelInterface::Flag  getCurrentSelectionFlag();
	int getSelectionIndex (SurfelInterface::Flag selectionFlag);
	SurfelInterface::Flag getSelectionFlag (int selectionIndex);

	bool checkToolAvailability() const;

private slots:


	void handleToolSelected (int id);
	void handleActiveObjectChanged();
	void handleObjectModified (bool samplingPatternAltered);
	void handleSelectedBrushChanged (const Brush *newBrush);
	void handleSceneMoved();
	void handleViewPortSizeChanged ();


};

#endif  // __SELECTION_TOOL_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
