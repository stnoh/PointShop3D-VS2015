// Title:   PointShopWindow.h
// Created: Thu Sep 25 14:43:04 2003
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

#ifndef __POINTSHOPWINDOW_H_
#define __POINTSHOPWINDOW_H_

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qscrollview.h>
#include <qaction.h>
#include <qwidgetstack.h>
#include <qcombobox.h>

#include "../Core/Configuration/src/Configuration.h"
#include "../Core/Scene/src/Object.h"
#include "../Core/DataStructures/src/SurfelCollection.h"
#include "../Core/Interfaces/src//ToolInterface.h"
#include "CameraPositionMenu.h"
#include "ResourceToolBar.h"

/**
 * This class implements the main application single-document window, which
 * contains several toolbars, a menu, status bar and the rendered image.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class PointShopWindow: public QMainWindow {

    Q_OBJECT

public:

    PointShopWindow();
    virtual ~PointShopWindow();	

	/**
	 * Tries to add the file specified by <code>filePath</code> which was dragged onto this <code>PointShopWindow</code>.
	 *
	 * @param filePath
	 *        the <code>QString</code> specifying the path to a file
	 */
	void addDroppedScene (const QString filePath);
    
protected:

    void closeEvent (QCloseEvent *event);
	void keyPressEvent (QKeyEvent *event);
	void keyReleaseEvent (QKeyEvent *event);
	void resizeEvent (QResizeEvent *event);

	void dragEnterEvent (QDragEnterEvent *event);
	void dropEvent (QDropEvent *event);

private:

	QWidgetStack    *rendererWidgetStack;
    QToolBar        *fileToolBar,
		            *miscToolBar;
	        
	ResourceToolBar *resourceToolBar;

	QPopupMenu      *selectionMenu,
		            *editMenu,
					*viewMenu,
	                *cameraMenu,
	                *helpMenu,
					*fileMenu;

	CameraPositionMenu *cameraPositionMenu;

	bool            isModelModified,
					isModelLoaded,
					renderersFound;  // this flag is set to true as soon as renderers have been found
	                                 // necessary to prevent a resize event from getting the main
	                                 // or preview renderer (we get a resize event even if no renderers
	                                 // have been found and before we can exit()!)
    QString         filePath,
		            fileFilter;   // the file filter with which the Scene was opened last

	int             pluginMenuID,
		            editMenuID,
					viewMenuID,
		            selectMenuID,
					cameraMenuID;
					
	QWidget         *noToolConfigurationWidget;
	
	QAction         *exportImageAction,
					*saveAction,
					*saveAsAction,
					*openAction,
					*addAction,
					*closeAction,
					*quitAction,
					*cutAction,
					*cutObjectAction,
					*copyAction,
					*copyObjectAction,
					*pasteAction,
					*deleteObjectAction,
					*configurationAction,
					*toolSettingsAction,
					*animateAction,
					*statisticsAction,
					*surfelPropertiesAction,
					*enablePreviewRendererAction,
					*toggleSelectionVisualizationAction,
					*viewBrushAction,
					*viewObjectListAction;

	QComboBox       *objectComboBox;

	Object          *copiedObject;

	// enables/disables certain menues and/or buttons according to 'enable'
	// called whenever a file is loaded or unloaded
	void setFileLoadedState (const bool enable);

	// enables/disables certain menues and/or buttons according to 'enable'
	// called whenever the model is modified in some way
	void setModelModifiedState (const bool enable);

	// updates the tool configuration dialog, showing the configuration widget
	// of 'tool', if there is any
	void updateToolConfigurationDialog (const ToolInterface *tool);

	// connects external signals to this PointShopWindow
	void frenchConnections();

	// initializes all the GUI stuff, among them:
	// - initToolBars
	// - initMenus
	// - initActions
	void initGUI();

	// initializes the toolbars - must be called BEFORE initActions
	void initToolBars();

	// initializes the menus - must be called BEFORE initACTIONS
	void initMenus();

	// initializes the actions the user can make - must be called AFTER initToolBars and initMenus
	void initActions();

	void activateMainRenderer();
	void activatePreviewRenderer();

	// updates the object combo box
	void updateObjectComboBox();	
	
private slots:

	void handleToolChanged (const ToolInterface *tool);
	void handleMainRendererChanged();
	void handlePreviewRendererChanged();

    void load();
	void load (const QString filePath);
	void add();
	void add (const QString filePath);
    void save();
    void saveAs();
	void close();

	void handleSelectAllVisible();
	void handleSelectAllActiveObject();
	void handleSelectClearCurrent();
	void handleSelectClearAll();
	void handleSelectInvert();
	void handleSelectShuffle();
	void handleConfiguration();
	void handleToolSettings();
	void handleImageExport();
	void handleAnimation();
	void handleStatistics();
	void handleSurfelProperties();
	void handleViewBrush();
	void handleViewObjectList();
	
	void handleCut();
	void handleCutObject();
	void handleCopy();
	void handleCopyObject();
	void handlePaste();
	void handleDeleteObject();

	void handleEnablePreviewRenderer();

    void about();
    void aboutQt();

	void handleObjectModified ();
	void handleObjectRenamed ();
	void handleObjectAdded();
	void handleObjectRemoved();
	void handleObjectMoved();
	void handleObjectChosen (const QString &objectName);
	void handleSelectionChanged (SurfelInterface::Flag selectionFlag);

	void handlePreviewRendererEnabled (const bool isEnabled);

	void handleToggleSelectionVisualization ();
};


#endif  // __POINTSHOPWINDOW_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
