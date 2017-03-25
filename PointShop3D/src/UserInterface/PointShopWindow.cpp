// Title:   PointShopWindow.cpp
// Created: Thu Sep 25 14:43:06 2003
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

#include <memory.h>
#include <stdlib.h>  // for exit()

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qkeycode.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qwhatsthis.h>
#include <qstring.h>
#include <qaction.h>
#include <qevent.h>
#include <qcombobox.h>
#include <qdragobject.h>
#include <qstrlist.h>


#include "../Core/Scene/src/Scene.h"
#include "../Core/Scene/src/Object.h"
#include "../Core/RendererManager/src/RendererManager.h"
#include "../Core/FileFormatPluginManager/src/FileFormatPluginManager.h"
#include "../Core/Configuration/src/Configuration.h"
#include "../Core/src/ClipBoard.h"
#include "../Core/MarkerManager/src/MarkerManager.h"
#include "../Core/Interfaces/src/RendererInterface.h"
#include "../Core/Interfaces/src/FileFormatPluginInterface.h"
#include "../Core/Transformer/src/Transformer.h"
#include "../Core/ToolManager/src/ToolManager.h"
#include "../Core/PluginManager/src/PluginManager.h"
#include "../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "../Core/CoreTools/src/CoreTools.h"
#include "../Core/Interfaces/src/ToolInterface.h"
#include "../Utilities/src/Matrix.h"
#include "../Utilities/UserMessages/src/SplashScreen.h"
#include "../Utilities/UserMessages/src/StatusBar.h"
#include "../Utilities/src/Version.h"
#include "../Utilities/src/FileTools.h"
#include "../Utilities/src/Common.h"
#include "../UserInterface/UI/NoToolConfigurationWidget.h"
#include "BrushViewer/src/BrushViewerImpl.h"
#include "ObjectViewer/src/ObjectViewerImpl.h"
#include "ResourceToolBar.h"
#include "ConfigurationDialogImpl.h"
#include "AnimationDialogImpl.h"
#include "StatisticsDialogImpl.h"
#include "SurfelPropertiesDialogImpl.h"
#include "ImageExportDialogImpl.h"
#include "ToolConfigurationDialog.h"
#include "CameraPositionMenu.h"
#include "PointShopWindow.h"

// *********
// constants
// *********

static const char *ALL_FILES = "All supported files (*.*)";

// ****************
// What's This text
// ****************

const char *fileOpenText = "<img source=\"fileopen\"> "
"Click this button to open a <em>new file</em>. <br><br>"
"You can also select the <b>Open command</b> from the File menu.";

const char *fileSaveText = "<img source=\"filesave\"> "
"Click this button to save the file you are "
"editing. <br><br>You will be prompted for a file name.\n\n"
"You can also select the <b>Save command</b> from the File menu.\n\n";

const char *configurationText     = "Configure Pointshop3D.";
const char *toolSettingsText      = "Activate the current tool settings, if available.";
const char *surfelPropertiesText  = "Choose the supported properties for the Surfels.";
const char *viewBrushesText  = "View the available brushes.";
const char *viewObjectListText  = "View the available objects.";

// **************
// public methods
// **************

PointShopWindow::PointShopWindow()
    : QMainWindow (0, "Point Shop", WDestructiveClose) {

	QPixmap                 openIcon,
	                        saveIcon,
				            programIcon;

	Configuration           *configuration;
	RendererManager         *rendererManager;
	FileFormatPluginManager *fileFormatPluginManager;

	isModelModified = false;
	isModelLoaded   = false;
	renderersFound  = false;
	copiedObject    = 0;

	this->move(QPoint(0,0));

	SplashScreen::getInstance()->show();
	SplashScreen::getInstance()->showText ("Loading Pointshop3D...");

	// initialize the global access to the status bar
	StatusBar::getInstance()->setStatusBar (this->statusBar());

	
	// ***********************
	// order is important here
	// ***********************	

	// *******************
	// initialize renderer
	// *******************

	rendererManager = RendererManager::getInstance();
	// here we access the configuration for
	// the first time, so it is created
	configuration = Configuration::getInstance();

	rendererWidgetStack = new QWidgetStack (this);
	this->setCentralWidget (rendererWidgetStack);
	if (rendererManager->initialize (configuration->getRendererPluginDirectory(), rendererWidgetStack) == false) {
		QMessageBox::critical (this, "No renderer found!", QString ("No renderer found! Install a renderer plugin in the %1 subdirectory first and restart PointShop3D.").arg (configuration->getRendererPluginDirectory()));
		// a crude way of quitting... ;)
		exit (-1);		
	}
	renderersFound = true;
	rendererWidgetStack->raiseWidget (rendererManager->getMainRenderer()->getRendererWidget());

	// *******************
	// file format plugins
	// *******************

	fileFormatPluginManager = FileFormatPluginManager::getInstance();

	if (fileFormatPluginManager->initialize (configuration->getFileFormatPluginDirectory()) == false) {
		QMessageBox::critical (this, "No file format plugins found!", "No file format plugins found! Install a file format plugin in the 'FileFormatPlugins' subdirectory first and restart Pointshop3D.");
		// another crude way of quitting... ;)
		exit (-1);		
	}

	filePath = configuration->getDataDirectory();

	// ***
	// GUI
	// ***	

	this->setAcceptDrops (true);
	this->initGUI();
	this->updateToolConfigurationDialog (ToolManager::getInstance()->getCurrentTool());
	SplashScreen::getInstance()->showText ("Graphical user interface initialized.");

	// make the connections from signals to slots
	// senders/receivers must already be created at this point
	this->frenchConnections();	

	SplashScreen::getInstance()->showText ("Activating renderer...");
	this->activateMainRenderer();
	SplashScreen::getInstance()->showText ("Renderer activated.");

	programIcon = QPixmap ("Resources/PointShopIcon.png");
	this->setIcon (programIcon);

	// no file loaded yet, so disable certain menues/buttons
	this->setFileLoadedState (false);
	// no file modified yet, so disable certain menues/buttons
	this->setModelModifiedState (false);	
	
    this->statusBar()->message ("Ready", 2000);	

	// *************
	// miscellaneous
	// *************

	// we want to get key events
	this->setFocusPolicy (QWidget::StrongFocus);

	// update the tool cursor in the renderer widget
	RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setCursor (*(ToolManager::getInstance()->getCurrentTool()->getToolCursor()));
	
	SplashScreen::destroyInstance();

}

PointShopWindow::~PointShopWindow() {

	// remove the global objects

	// IMPORTANT: destroy the animation dialog BEFORE the renderers, since it might still
	//            have an animation thread running which accesses the current renderer
	AnimationDialogImpl::destroyInstance();
	StatisticsDialogImpl::destroyInstance();
	ConfigurationDialogImpl::destroyInstance();
	ImageExportDialogImpl::destroyInstance();
	SurfelPropertiesDialogImpl::destroyInstance();

	//when destroying the ToolManager, also the tools are destroyed
	ToolManager::destroyInstance();

	// after these renderers have been destroyed no secondary thread is running
	// anymore - when destroying the RendererManager, also the renderers are destroyed
	RendererManager::destroyInstance();

	MarkerManager::destroyInstance();

	delete noToolConfigurationWidget;	

	CoreTools::destroyInstance();
	CoreResourceTools::destroyInstance();
	ToolConfigurationDialog::destroyInstance();
	Configuration::destroyInstance();
	ClipBoard::destroyInstance();
	Scene::destroyInstance();
	Transformer::destroySceneTransformer();
	StatusBar::destroyInstance();
	BrushViewerImpl::destroyInstance();
	ObjectViewerImpl::destroyInstance();

	exit(0);

}

void PointShopWindow::addDroppedScene (const QString filePath) {
	fileFilter = QString (ALL_FILES);
	if (!isModelLoaded) {
		this->load(filePath);
	}
	else {
		this->add (filePath);
	}
}

// *****************
// protected methods
// *****************

void PointShopWindow::closeEvent (QCloseEvent *event) {

    if (isModelModified == false) {
		event->accept();
		return;
    }

    switch (QMessageBox::information (this, "Model modified!",
				                      "The document has been changed since "
				                      "the last save.",
				                      "Save Now", "Cancel", "Leave Anyway",
				                      0, 1 )) {
		case 0:
			this->save();
			event->accept();
			break;
		case 1:
			event->ignore();
			break;
		case 2:
			event->accept();
			break;
		default: // just for sanity
			break;
	
    }

}

void PointShopWindow::keyPressEvent (QKeyEvent *event) {

	Configuration     *configuration;
	Scene             *scene;
	ToolManager       *toolManager;
	RendererInterface *mainRenderer,
	                  *previewRenderer;
	float             cutoffRadius;
	RendererManager   *rendererManager;

	configuration = Configuration::getInstance();
	scene = Scene::getInstance();

	switch (event->key()) {

	case Qt::Key_Left:
		{
			if (event->state() == Qt::ControlButton) {
				scene->translate (-10.0f, 0.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ControlButton | Qt::ShiftButton)) {
				scene->translate (-50.0f, 0.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ShiftButton)) {
				scene->rotate (-90.0f * PI_180, 0.0f, 1.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == Qt::NoButton) {
				scene->rotate (-10.0f * PI_180, 0.0f, 1.0f, 0.0f);
				event->accept();
			}
		}
		break;

	case Qt::Key_Right:
		{
			if (event->state() == Qt::ControlButton) {
				scene->translate (10.0f, 0.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ControlButton | Qt::ShiftButton)) {
				scene->translate (50.0f, 0.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ShiftButton)) {
				scene->rotate (90.0f * PI_180, 0.0f, 1.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == Qt::NoButton) {
				scene->rotate (10.0f * PI_180, 0.0f, 1.0f, 0.0f);
				event->accept();
			}
		}
		break;

	case Qt::Key_Up:
		{
			if (event->state() == Qt::ControlButton) {
				scene->translate (0.0f, 10.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ControlButton | Qt::ShiftButton)) {
				scene->translate (0.0f, 50.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == Qt::AltButton) {
				scene->translate (0.0f, 0.0f, 10.0f);
				event->accept();
			}
			else if (event->state() == (Qt::AltButton | Qt::ShiftButton)) {
				scene->translate (0.0f, 0.0f, 50.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ShiftButton)) {
				scene->rotate (-90.0f * PI_180, 1.0f, 0.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == Qt::NoButton) {
				scene->rotate (-10.0f * PI_180, 1.0f, 0.0f, 0.0f);
				event->accept();
			}
		}
		break;

	case Qt::Key_Down:
		{
			if (event->state() == Qt::ControlButton) {
				scene->translate (0.0f, -10.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ControlButton | Qt::ShiftButton)) {
				scene->translate (0.0f, -50.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == Qt::AltButton) {
				scene->translate (0.0f, 00.0f, -10.0f);
				event->accept();
			}
			else if (event->state() == (Qt::AltButton | Qt::ShiftButton)) {
				scene->translate (0.0f, 00.0f, -50.0f);
				event->accept();
			}
			else if (event->state() == (Qt::ShiftButton)) {
				scene->rotate (90.0f * PI_180, 1.0f, 0.0f, 0.0f);
				event->accept();
			}
			else if (event->state() == Qt::NoButton) {
				scene->rotate (10.0f * PI_180, 1.0f, 0.0f, 0.0f);
				event->accept();
			}
		}
		break;

	case Qt::Key_PageUp:
		{
			if (event->state() == Qt::ShiftButton) {
				scene->translate (0.0f, 0.0f, 50.0f);
				event->accept();
			}
			else if (event->state() == Qt::NoButton) {
				scene->translate (0.0f, 0.0f, 10.0f);
				event->accept();
			}
		}
		break;

	case Qt::Key_PageDown:
		{
			if (event->state() == Qt::ShiftButton) {
				scene->translate (0.0f, 0.0f, -50.0f);
				event->accept();
			}
			else if (event->state() == Qt::NoButton) {
				scene->translate (0.0f, 0.0f, -10.0f);
				event->accept();
			}
		}
		break;

	case Qt::Key_Home:
		{
			scene->setCameraPosition (configuration->getHomePosition());
			event->accept();
		}
		break;

	case Qt::Key_Plus:
		{
			rendererManager = RendererManager::getInstance();
			mainRenderer    = rendererManager->getMainRenderer();
			previewRenderer = rendererManager->getPreviewRenderer();
			
			if (! (event->state() & Qt::AltButton)) {
				cutoffRadius = mainRenderer->getCutoffRadius();
				mainRenderer->setCutoffRadius (cutoffRadius * 2.0f);
			}

			if (! (event->state() & Qt::ControlButton) ) {
				cutoffRadius = previewRenderer->getCutoffRadius();
				previewRenderer->setCutoffRadius (cutoffRadius * 2.0f);
			}

			if (rendererManager->isPreviewRendererEnabled() == false) {
				mainRenderer->renderFrame();
			}
			else {
				previewRenderer->renderFrame();
			}
			event->accept();
		}
		break;

	case Qt::Key_Minus:
		{
			rendererManager = RendererManager::getInstance();
			mainRenderer = rendererManager->getMainRenderer();
			previewRenderer = rendererManager->getPreviewRenderer();
			
			if (! (event->state() & Qt::AltButton)) {
				cutoffRadius = mainRenderer->getCutoffRadius();
				mainRenderer->setCutoffRadius (cutoffRadius / 2.0f);
			}

			if (! (event->state() & Qt::ControlButton) ) {
				cutoffRadius = previewRenderer->getCutoffRadius();
				previewRenderer->setCutoffRadius (cutoffRadius / 2.0f);
			}

			if (rendererManager->isPreviewRendererEnabled() == false) {
				mainRenderer->renderFrame();
			}
			else {
				previewRenderer->renderFrame();
			}
			event->accept();
		}
		break;

	default:
		{
			toolManager = ToolManager::getInstance();
			toolManager->handleKeyPressEvent (event);
		}
		break;

	}  // switch (event->key())

}

void PointShopWindow::keyReleaseEvent (QKeyEvent *event) {

	ToolManager *toolManager;

	toolManager = ToolManager::getInstance();
	toolManager->handleKeyReleaseEvent (event);

}

void PointShopWindow::resizeEvent (QResizeEvent *) {

	QWidget           *rendererWidget;

	// this test is only necessary for the case that we haven't found a renderer plugin
	// since then a QMessageBox pops up, triggering this resize event!
	if (renderersFound == true) {

		if (Configuration::getInstance()->isPreviewRendererEnabled() == false) {
			rendererWidget = RendererManager::getInstance()->getMainRenderer()->getRendererWidget();
		}
		else {
			rendererWidget = RendererManager::getInstance()->getPreviewRenderer()->getRendererWidget();
		}

		this->statusBar()->message (QString ("(%1, %2)").arg (rendererWidget->size().width()).arg (rendererWidget->size().height()), 2000);

	}

}

void PointShopWindow::dragEnterEvent(QDragEnterEvent* event) {    
	event->accept (QUriDrag::canDecode (event));
}

void PointShopWindow::dropEvent (QDropEvent *event) {
    
    QStringList           filePaths;
	QStringList::Iterator it;

	if (QUriDrag::decodeLocalFiles (event, filePaths) == true) {

		// check if local files where given
		for (it = filePaths.begin(); it != filePaths.end(); ++it) {			
			this->addDroppedScene ((*it).latin1());
		}

	}

}

// ***************
// private methods
// ***************

void PointShopWindow::setFileLoadedState (const bool enable) {

	ToolManager   *toolManager;
	PluginManager *pluginManager;

	toolManager = ToolManager::getInstance();
	pluginManager = PluginManager::getInstance();

	// select menu
	this->menuBar()->setItemEnabled (selectMenuID, enable);

	// tools
	toolManager->checkToolAvailability();
	
	// plugins
	pluginManager->checkPluginAvailability();

	// save button
	saveAsAction->setEnabled (enable);

	// close button
	closeAction->setEnabled (enable);

	// camera menu
	this->menuBar()->setItemEnabled (cameraMenuID, enable);

	isModelLoaded = enable;

}

void PointShopWindow::setModelModifiedState (const bool enable) {
	saveAction->setEnabled (enable);
	isModelModified = enable;
}

void PointShopWindow::updateToolConfigurationDialog (const ToolInterface *tool) {

	QWidget *toolConfigurationWidget;
	ToolConfigurationDialog *toolConfigurationDialog;

	toolConfigurationDialog = ToolConfigurationDialog::getInstance();

	// remove the old tool configuration widget
	toolConfigurationDialog->removeWidget (toolConfigurationDialog->visibleWidget());

	// add the new tool configuration widget, if any
	toolConfigurationWidget = tool->getToolConfigurationWidget();
	if (toolConfigurationWidget != 0) {
		toolConfigurationDialog->addWidget (toolConfigurationWidget, 1);
		toolConfigurationDialog->resize (toolConfigurationWidget->size());
		toolConfigurationDialog->setMinimumSize (toolConfigurationWidget->size());
		toolConfigurationDialog->setMaximumSize (toolConfigurationWidget->size());
	}
	else {
		toolConfigurationDialog->addWidget (noToolConfigurationWidget, 1);
		toolConfigurationDialog->resize (noToolConfigurationWidget->size());
		toolConfigurationDialog->setMinimumSize (noToolConfigurationWidget->size());
		toolConfigurationDialog->setMaximumSize (noToolConfigurationWidget->size());
	}

	
	toolConfigurationDialog->raiseWidget(1);

}

void PointShopWindow::frenchConnections() {

	Scene           *scene;
	RendererManager *rendererManager;

	this->connect (ToolManager::getInstance(), SIGNAL (toolChanged (const ToolInterface *)),
	  	           this, SLOT (handleToolChanged (const ToolInterface *)));

	rendererManager = RendererManager::getInstance();

	// get informed whenever the main renderer changes, as to disable/enable certain tools/menues
	this->connect (rendererManager, SIGNAL (mainRendererChanged()),
		           this, SLOT (handleMainRendererChanged()));

	// get informed whenever the preview renderer changes, as to disable/enable certain tools/menues
	this->connect (rendererManager, SIGNAL (previewRendererChanged()),
		           this, SLOT (handlePreviewRendererChanged()));

	this->connect (rendererManager, SIGNAL (previewRendererEnabled (const bool)),
		           this, SLOT (handlePreviewRendererEnabled (const bool)));

	scene = Scene::getInstance();
	// get informed whenever the objects in the Scene change, as to update the object drop down list
	// and to enable the save action
	this->connect (scene, SIGNAL (objectModified (bool)),
		           this, SLOT (handleObjectModified ()));
	this->connect (scene, SIGNAL (objectRenamed ()),
		           this, SLOT (handleObjectRenamed ()));

	// get informed whenever an object has been added to the Scene, as to update the object drop down list
	this->connect (scene, SIGNAL (objectAdded()),
		           this, SLOT (handleObjectAdded()));

	// get informed whenever an object has been removed from the Scene, as to update the object drop down list
	this->connect (scene, SIGNAL (objectRemoved()),
		           this, SLOT (handleObjectRemoved()));

	// get informed whenever an object in the Scene moves, as to enable the save action
	this->connect (scene, SIGNAL (objectMoved()),
		           this, SLOT (handleObjectMoved()));

	this->connect (objectComboBox, SIGNAL (activated (const QString &)),
		           this, SLOT (handleObjectChosen (const QString &)));

	// get informed whenever the selection has changed
	this->connect (CoreTools::getInstance()->getSelectionTool(), SIGNAL (selectionChanged (SurfelInterface::Flag)),
		           this, SLOT (handleSelectionChanged (SurfelInterface::Flag)));

	

}

void PointShopWindow::initGUI() {
	
	this->initToolBars();
	this->initMenus();
	this->initActions();

	objectComboBox = new QComboBox (miscToolBar);
	objectComboBox->setEditable (false);
	objectComboBox->setMinimumWidth (120);

}

void PointShopWindow::initToolBars() {

	ToolManager *toolManager;

	noToolConfigurationWidget = new NoToolConfigurationWidget(0);

	fileToolBar = new QToolBar (this, "file operations");
	fileToolBar->setLabel (tr ("File Operations"));
	fileToolBar->setPalette (QPalette (Constants::TOOLBAR_BACKGROUND));

	// it is important to initialize the resource toolbar first...
	resourceToolBar = new ResourceToolBar ("Resource Tool Bar", this);
	resourceToolBar->initialize();
	resourceToolBar->setPalette (QPalette (Constants::TOOLBAR_BACKGROUND));

	// ... then the toolmanager, since certain tools might need resource tools!
	toolManager = ToolManager::getInstance();
	toolManager->initialize (Configuration::getInstance()->getToolDirectory(), this);
	
	miscToolBar = new QToolBar (this, "Miscellaneous Stuff");
	miscToolBar->setPalette (QPalette (Constants::TOOLBAR_BACKGROUND));

	SplashScreen::getInstance()->showText ("Toolbars initialized");

}

void PointShopWindow::initMenus() {

	PluginManager *pluginManager;

	fileMenu = new QPopupMenu (this);
	this->menuBar()->insertItem ("&File", fileMenu);

	editMenu = new QPopupMenu (this);
	editMenuID = this->menuBar()->insertItem ("&Edit", editMenu);

	cameraMenu = new QPopupMenu (this);
	cameraMenuID = this->menuBar()->insertItem ("&Camera", cameraMenu);

	cameraPositionMenu = new CameraPositionMenu (this);
	cameraMenu->insertItem ("Camera Positions", cameraPositionMenu);

	selectionMenu = new QPopupMenu (this);
	selectMenuID = this->menuBar()->insertItem ("&Select", selectionMenu);	
	selectionMenu->insertItem ("&All Visible", this, SLOT (handleSelectAllVisible()), CTRL + Key_A);
	selectionMenu->insertItem ("&Active Object", this, SLOT (handleSelectAllActiveObject()), ALT + Key_A);
	selectionMenu->insertItem ("&Clear Current", this, SLOT (handleSelectClearCurrent()), SHIFT + CTRL + Key_A);
	selectionMenu->insertItem ("Clear All", this, SLOT (handleSelectClearAll()), SHIFT + ALT + Key_A);
	selectionMenu->insertItem ("&Invert", this, SLOT (handleSelectInvert()), SHIFT + CTRL + Key_I);
	selectionMenu->insertItem ("&Shuffle", this, SLOT (handleSelectShuffle()));

	pluginManager = PluginManager::getInstance();
	pluginManager->initialize (Configuration::getInstance()->getPluginDirectory(), this->menuBar(), &pluginMenuID);

	viewMenu = new QPopupMenu (this);
	viewMenuID = this->menuBar()->insertItem ("&View", viewMenu);
	
	helpMenu = new QPopupMenu (this);
	this->menuBar()->insertSeparator();
    this->menuBar()->insertItem ("&Help", helpMenu);

}

void PointShopWindow::initActions() {

	Configuration *configuration;
	QPixmap       openIcon;
	QToolButton   *whatsThisButton;
	QIconSet      saveIconSet,
		          toolSettingsIconSet;
	QPixmap       normalSaveIcon ("Resources/FileSaveIcon.png"),
		          disabledSaveIcon ("Resources/FileSaveIcon_disabled.png"),
	              normalToolSettingsIcon ("Resources/ToolSettingsIcon.png"),
				  disabledToolSettingsIcon ("Resources/ToolSettingsIcon_disabled.png");

	configuration = Configuration::getInstance();

	openIcon = QPixmap ("Resources/FileOpenIcon.png");
	saveIconSet.setPixmap (normalSaveIcon, QIconSet::Automatic, QIconSet::Normal);
	saveIconSet.setPixmap (disabledSaveIcon, QIconSet::Automatic, QIconSet::Disabled);
	toolSettingsIconSet.setPixmap (normalToolSettingsIcon, QIconSet::Automatic, QIconSet::Normal);
	toolSettingsIconSet.setPixmap (disabledToolSettingsIcon, QIconSet::Automatic, QIconSet::Disabled);

	exportImageAction = new QAction ("Export image buffer into image file", "Export image...", 0, this);
	exportImageAction->setWhatsThis ("Export the image buffer into an image file of user specified format");
	this->connect (exportImageAction, SIGNAL (activated()),
		           this, SLOT (handleImageExport()));

	openAction = new QAction ("Open Scene", openIcon, "&Open Scene", CTRL + Key_O, this, "Open Scene", false);
	this->connect (openAction, SIGNAL (activated()),
		           this, SLOT (load()));
	
	openAction->setWhatsThis (fileOpenText);
	QMimeSourceFactory::defaultFactory()->setPixmap ("fileopen", openIcon);

	addAction = new QAction ("Add Scene", openIcon, "&Add Scene", 0, this, "Add Scene", false);
	this->connect (addAction, SIGNAL (activated()),
		           this, SLOT (add()));

	closeAction = new QAction ("&Close", "Close", 0, this, "&Close", false);
	this->connect (closeAction, SIGNAL (activated()),
		           this, SLOT (close()));
	
	saveAction = new QAction ("Save File", normalSaveIcon, "&Save", CTRL + Key_S, this, "Save", false);
	saveAction->setIconSet (saveIconSet);
	this->connect (saveAction, SIGNAL (activated()),
		           this, SLOT (save()));	
	saveAction->setWhatsThis (fileSaveText);	

	saveAsAction = new QAction ("Save File As...", normalSaveIcon, "Save &as...", 0, this, "Save As", false);
	saveAsAction->setIconSet (saveIconSet);
	this->connect (saveAsAction, SIGNAL (activated()),
		           this, SLOT (saveAs()));	
	saveAsAction->setWhatsThis (fileSaveText);

	QMimeSourceFactory::defaultFactory()->setPixmap ("filesave", normalSaveIcon);

	quitAction = new QAction ("&Quit", "Quit", CTRL + Key_Q, this, "Quit", false);
	this->connect (quitAction, SIGNAL (activated()),
		           qApp, SLOT (closeAllWindows()));
	
	configurationAction = new QAction ("Settings...", "S&ettings...", 0, this, "Settings", false);
	this->connect (configurationAction, SIGNAL (activated()),
		           this, SLOT (handleConfiguration()));	
	configurationAction->setWhatsThis (configurationText);

	toolSettingsAction = new QAction ("Tool Settings", normalToolSettingsIcon, "&Tool Settings", 0, this, "Tool Settings", false);
	toolSettingsAction->setIconSet (toolSettingsIconSet);
	this->connect (toolSettingsAction, SIGNAL (activated()),
		           this, SLOT (handleToolSettings()));	
	toolSettingsAction->setWhatsThis (toolSettingsText);

	surfelPropertiesAction = new QAction ("Surfel Properties...", "&Surfel Properties...", 0, this, "Surfel Properties...", false);
	this->connect (surfelPropertiesAction, SIGNAL (activated()),
		           this, SLOT (handleSurfelProperties()));	
	surfelPropertiesAction->setWhatsThis (surfelPropertiesText);

	viewBrushAction = new QAction ("Show Available Brushes", "&Brush...", 0, this, "Brush...", false);
	this->connect (viewBrushAction, SIGNAL (activated()),
		           this, SLOT (handleViewBrush()));	
	viewBrushAction->setWhatsThis (viewBrushesText);


	viewObjectListAction = new QAction ("Show Available Objects", "&Object List...", 0, this, "Object List...", false);
	this->connect (viewObjectListAction, SIGNAL (activated()),
		           this, SLOT (handleViewObjectList()));	
	viewObjectListAction->setWhatsThis (viewObjectListText);
	
	
	
	
	animateAction = new QAction ("Animate...", "&Animate...", 0, this, "Animate", false);
	this->connect (animateAction, SIGNAL (activated()),
		           this, SLOT (handleAnimation()));	

	statisticsAction = new QAction ("Statistics...", "&Statistics...", 0, this, "Statistics", false);
	this->connect (statisticsAction, SIGNAL (activated()),
		           this, SLOT (handleStatistics()));	
	 
	cutAction = new QAction ("Cut", "&Cut", CTRL + Key_X, this, "Cut", false);
	this->connect (cutAction, SIGNAL (activated()),
		           this, SLOT (handleCut()));
	cutAction->setEnabled (false);

	cutObjectAction = new QAction ("Cut Object", "Cut O&bject", SHIFT + CTRL + Key_X, this, "Cut Object", false);
	this->connect (cutObjectAction, SIGNAL (activated()),
		           this, SLOT (handleCutObject()));
	cutObjectAction->setEnabled (false);

	copyAction = new QAction ("Copy", "C&opy", CTRL + Key_C, this, "Copy", false);
	this->connect (copyAction, SIGNAL (activated()),
		           this, SLOT (handleCopy()));
	copyAction->setEnabled (false);

	copyObjectAction = new QAction ("Copy Object", "Co&py Object", SHIFT + CTRL + Key_C, this, "Copy Object", false);
	this->connect (copyObjectAction, SIGNAL (activated()),
		           this, SLOT (handleCopyObject()));
	copyObjectAction->setEnabled (false);

	pasteAction = new QAction ("Paste", "&Paste", CTRL + Key_V, this, "Paste", false);
	this->connect (pasteAction, SIGNAL (activated()),
		           this, SLOT (handlePaste()));
	pasteAction->setEnabled (false);

	deleteObjectAction = new QAction ("Delete Object", "&Delete Object", CTRL + Key_Delete, this, "Delete Object", false);
	this->connect (deleteObjectAction, SIGNAL (activated()),
		           this, SLOT (handleDeleteObject()));
	deleteObjectAction->setEnabled (false);
 
	enablePreviewRendererAction = new QAction ("Enable Preview Renderer", QIconSet (QPixmap ("Resources/PreviewRendererIcon.png")), "Preview Renderer", Key_Space, this, "Enable Preview Renderer", false);
	this->connect (enablePreviewRendererAction, SIGNAL (activated()),
		           this, SLOT (handleEnablePreviewRenderer()));
	enablePreviewRendererAction->setToggleAction (true);
	enablePreviewRendererAction->setOn (configuration->isPreviewRendererEnabled());

	toggleSelectionVisualizationAction = new QAction ("Toggle Selection Visualization", "Toggle Selection Visualization", CTRL + Key_B, this, "Toggle Selection Visualization", false);
	this->connect (toggleSelectionVisualizationAction, SIGNAL (activated()),
					this, SLOT (handleToggleSelectionVisualization()));


	// add the actions to the places they belong to

	// misc toolbar

	toolSettingsAction->addTo (miscToolBar);
	enablePreviewRendererAction->addTo (miscToolBar);	

	// file menu

	openAction->addTo (fileMenu);
	openAction->addTo (fileToolBar);
	addAction->addTo (fileMenu);
	closeAction->addTo (fileMenu);	
 
	saveAction->addTo (fileMenu);
	saveAction->addTo (fileToolBar);
	whatsThisButton = QWhatsThis::whatsThisButton (fileToolBar);
	whatsThisButton->setPixmap (QPixmap ("Resources/WhatsThisIcon.png"));

	saveAsAction->addTo (fileMenu);

    fileMenu->insertSeparator();	
	exportImageAction->addTo (fileMenu);
	fileMenu->insertSeparator();
    
	quitAction->addTo (fileMenu);

	// edit menu

	cutAction->addTo (editMenu);
	cutObjectAction->addTo (editMenu);
	copyAction->addTo (editMenu);
	copyObjectAction->addTo (editMenu);
	pasteAction->addTo (editMenu);
	editMenu->insertSeparator();
	deleteObjectAction->addTo (editMenu);
	editMenu->insertSeparator();
	surfelPropertiesAction->addTo (editMenu);
	configurationAction->addTo (editMenu);		

	// camera menu

	animateAction->addTo (cameraMenu);

	// view menu

	statisticsAction->addTo (viewMenu);
	viewBrushAction->addTo (viewMenu);
	viewObjectListAction->addTo (viewMenu);
	viewMenu->insertSeparator();
	enablePreviewRendererAction->addTo (viewMenu);
	toolSettingsAction->addTo (viewMenu);

	// help menu

	helpMenu->insertItem ("&About", this, SLOT (about()), Key_F1);
    helpMenu->insertItem ("About &Qt", this, SLOT (aboutQt()));
    helpMenu->insertSeparator();
    helpMenu->insertItem ("What's &This", this, SLOT (whatsThis()), SHIFT+Key_F1);

}

void PointShopWindow::activateMainRenderer() {

	RendererInterface *renderer;
	ToolManager       *toolManager;
	PluginManager     *pluginManager;
	RendererManager   *rendererManager;

	rendererManager = RendererManager::getInstance();
	renderer        = rendererManager->getMainRenderer();
	toolManager     = ToolManager::getInstance();
	pluginManager   = PluginManager::getInstance();
	
	toolManager->checkToolAvailability();
	pluginManager->checkPluginAvailability();

	if (renderer->isInteractive() == true) {
	
		
		// select menu
		this->menuBar()->setItemEnabled (selectMenuID, isModelLoaded);


	}
	else {
		// select menu
		this->menuBar()->setItemEnabled (selectMenuID, false);
	}

	if (rendererManager->isPreviewRendererEnabled() == false) {

		// re-render image
		renderer->renderFrame();
		rendererWidgetStack->raiseWidget (renderer->getRendererWidget());
		// update the tool cursor in the renderer widget
		rendererManager->getMainRenderer()->getRendererWidget()->setCursor (*(ToolManager::getInstance()->getCurrentTool()->getToolCursor()));

	}	

}

void PointShopWindow::activatePreviewRenderer() {

	RendererInterface *renderer;
	ToolManager       *toolManager;
	PluginManager     *pluginManager;
	RendererManager   *rendererManager;

	rendererManager = RendererManager::getInstance();
	renderer        = rendererManager->getMainRenderer();
	toolManager     = ToolManager::getInstance();
	pluginManager   = PluginManager::getInstance();
	
	toolManager->checkToolAvailability();
	pluginManager->checkPluginAvailability();

	if (renderer->isInteractive() == true) {
		// select menu
		this->menuBar()->setItemEnabled (selectMenuID, isModelLoaded);
	}
	else {
		// select menu
		this->menuBar()->setItemEnabled (selectMenuID, false);
	}

	if (rendererManager->isPreviewRendererEnabled() == true) {

		// re-render image
		renderer->renderFrame();
		rendererWidgetStack->raiseWidget (renderer->getRendererWidget());
		// update the tool cursor in the renderer widget
		rendererManager->getPreviewRenderer()->getRendererWidget()->setCursor (*(ToolManager::getInstance()->getCurrentTool()->getToolCursor()));

	}	

}

void PointShopWindow::updateObjectComboBox() {

	Scene  *scene;
	Object *object;

	scene = Scene::getInstance();

	// remove old entries
	objectComboBox->clear();

	for (object = scene->getFirstObject(); object != 0; object = scene->getNextObject()) {
		objectComboBox->insertItem (object->getName());
	}
	
	objectComboBox->setCurrentItem (scene->getActiveObjectID());

}

// *************
// private slots
// *************

void PointShopWindow::handleToolChanged (const ToolInterface *tool) {
	
	if (Configuration::getInstance()->isPreviewRendererEnabled() == false) {
		RendererManager::getInstance()->getMainRenderer()->getRendererWidget()->setCursor (*(tool->getToolCursor()));
	}
	else {
		RendererManager::getInstance()->getPreviewRenderer()->getRendererWidget()->setCursor (*(tool->getToolCursor()));
	}
	this->updateToolConfigurationDialog (tool);

}

void PointShopWindow::handleMainRendererChanged() {
	this->activateMainRenderer();
}

void PointShopWindow::handlePreviewRendererChanged() {
	this->activatePreviewRenderer();
}

void PointShopWindow::handleSelectAllVisible() {
	CoreTools::getInstance()->getSelectionTool()->selectAllVisible (true);
}

void PointShopWindow::handleSelectAllActiveObject() {
	CoreTools::getInstance()->getSelectionTool()->selectAllActiveObject (true);
}

void PointShopWindow::handleSelectClearCurrent() {
	CoreTools::getInstance()->getSelectionTool()->clearSelection (true);
}

void PointShopWindow::handleSelectClearAll() {
	int selection = CoreTools::getInstance()->getSelectionTool()->getActiveSelectionIndex();

	CoreTools::getInstance()->getSelectionTool()->setActiveSelection (0);
	CoreTools::getInstance()->getSelectionTool()->clearSelection (false);
	CoreTools::getInstance()->getSelectionTool()->setActiveSelection (1);
	CoreTools::getInstance()->getSelectionTool()->clearSelection (false);
	CoreTools::getInstance()->getSelectionTool()->setActiveSelection (2);
	CoreTools::getInstance()->getSelectionTool()->clearSelection (true);
	CoreTools::getInstance()->getSelectionTool()->setActiveSelection (selection);
}


void PointShopWindow::handleSelectInvert() {
	CoreTools::getInstance()->getSelectionTool()->invertSelection (true);
}

void PointShopWindow::handleSelectShuffle() {
	CoreTools::getInstance()->getSelectionTool()->shuffleSelection (true);
}



void PointShopWindow::load() {

	QString                                fileDialogFileFilter;
	const QList<FileFormatPluginInterface> *fileFormatPlugins;
	FileFormatPluginInterface              *fileFormatPlugin;
	QFileDialog                            *fileDialog;
	QFile									file;
	int                                    result;

	if (isModelModified == true) {
		
		switch (QMessageBox::information (this, "Model modified!",
										  "The document has been changed since "
										  "the last save.",
										  "Save Old Scene First", "Cancel", "Open New Scene Anyway",
										  0, 1 )) {
			case 0:
				this->save();
				break;
			case 1:
				return;
				break;
			case 2:
				break;
			default: // just for sanity
				return;
				break;

		}
	
    }

	// first all supported files...
	fileDialogFileFilter += ALL_FILES;
	fileDialogFileFilter += ";;";

	fileFormatPlugins = FileFormatPluginManager::getInstance()->getFileFormatPlugins();
	QListIterator<FileFormatPluginInterface> it (*fileFormatPlugins);
	for ( ; (fileFormatPlugin = it.current()) != 0; ++it) {
		if (!fileFormatPlugin->hasReadProtection()) {
			fileDialogFileFilter += fileFormatPlugin->getFileFilter();
			fileDialogFileFilter += ";;";
		}
	}

	fileDialog = new QFileDialog (filePath, fileDialogFileFilter, this, "Open", true);
	fileDialog->setMode(QFileDialog::ExistingFile);
	// repeat showing the file dialog until it is aborted or an existing file is chosen	
	do {
		result = fileDialog->exec();
		filePath = fileDialog->selectedFile();
		file.setName (filePath);
		if (file.open (IO_ReadOnly) == true) {
			file.close();
			break;
		}
	} while (result > 0);
	
	if (result > 0) {
		// update the file filter
		fileFilter = fileDialog->selectedFilter();
		this->load (filePath);
	}
    else {
		statusBar()->message ("Loading aborted", 2000);
	}

	// remove unused resources
	delete fileDialog;
	delete fileFormatPlugins;

}

void PointShopWindow::add() {

	QString                                fileDialogFileFilter;
	const QList<FileFormatPluginInterface> *fileFormatPlugins;
	FileFormatPluginInterface              *fileFormatPlugin;
	QFileDialog                            *fileDialog;
	int                                    result;

	// all supported files first...
	fileDialogFileFilter += ALL_FILES;
	fileDialogFileFilter += ";;";

	fileFormatPlugins = FileFormatPluginManager::getInstance()->getFileFormatPlugins();
	QListIterator<FileFormatPluginInterface> it (*fileFormatPlugins);
	for ( ; (fileFormatPlugin = it.current()) != 0; ++it) {		
		if (!fileFormatPlugin->hasReadProtection()) {
			fileDialogFileFilter += fileFormatPlugin->getFileFilter();
			fileDialogFileFilter += ";;";
		}
	}	

	fileDialog = new QFileDialog (filePath, fileDialogFileFilter, this, "Open", true);
	fileDialog->setMode(QFileDialog::ExistingFile);		
	// repeat showing the file dialog until it is aborted or an existing file is chosen
	do {
		result = fileDialog->exec();
		filePath = fileDialog->selectedFile();
	} while (result > 0 && !QFile::exists(filePath));
	
	if (result > 0) {
		// update the file filter
		fileFilter = fileDialog->selectedFilter();
		this->add (filePath);
	}
    else {
		statusBar()->message ("Loading aborted", 2000);
	}

	// remove unused resources
	delete fileDialog;
	delete fileFormatPlugins;

}

void PointShopWindow::load (const QString filePath) {

	RendererInterface                     *renderer;
	Scene                                 *scene;
	Configuration                         *configuration;
	QString                               message,
		                                  messageFilePath;
	FileFormatPluginInterface             *fileFormatPlugin;
	FileFormatPluginInterface::FileHeader fileHeader;
	QFile                                 file;

	renderer      = RendererManager::getInstance()->getMainRenderer();
	scene         = Scene::getInstance();
	configuration = Configuration::getInstance();

	// this may take a while...
	QApplication::setOverrideCursor (Qt::waitCursor);

	// truncate the file name for the message
	if (filePath.length() > 16) {
		messageFilePath = "..\"" + filePath.right(14) + "\"";
	}
	else {
		messageFilePath = "\"" + filePath.right(16) + "\"";
	}

	fileFormatPlugin = 0;
	if (fileFilter != QString (ALL_FILES)) {
		fileFormatPlugin = FileFormatPluginManager::getInstance()->getFileFormatPlugin (fileFilter);
	}
	else {

		// probe for a suitable file format plugin
		file.setName (filePath);
		if (file.open (IO_ReadOnly) == true) {

			QDataStream dataStream (&file);

			dataStream.readRawBytes (fileHeader, sizeof (fileHeader));
			file.close();

			fileFormatPlugin = FileFormatPluginManager::getInstance()->probe (fileHeader);
		}
		else {
			message.sprintf ("Could not open the file %s", messageFilePath.latin1());
		}

	}

	// did we find a file format plugin?
	if (fileFormatPlugin != 0) {

		fileFilter = fileFormatPlugin->getFileFilter();	// set new filter

		// switch to the navigation tool before loading new scene
		ToolManager::getInstance()->switchToNavigationTool();

		if (fileFormatPlugin->readFromFile (filePath) == true) {

			this->setCaption (filePath);
			message.sprintf ("Loaded Scene %s", messageFilePath.latin1());
			this->setFileLoadedState (true);
			this->setModelModifiedState (false);

			// the loaded object may contain surfels that are flagged as "selected".
			// update the selection tool accordingly.
			CoreTools::getInstance()->getSelectionTool()->updateSelectionCounters();
		}
		else {

			message.sprintf ("Loading of Scene %s failed! Corrupt file format?", messageFilePath.latin1());
			this->setCaption ("PointShop");
			this->setFileLoadedState (false);
			// finished, but no success :(
			scene->reset();
			this->setModelModifiedState (false);

		}

	}
	else {
		message.sprintf ("Format in %s not supported.", messageFilePath.latin1());
	}

	this->statusBar()->message (message, 2000);

	// ah.. finally!
	QApplication::restoreOverrideCursor();

}

void PointShopWindow::add (const QString filePath) {

	RendererInterface                     *renderer;
	Scene                                 *scene;
	Configuration                         *configuration;
	QString                               message,
		                                  messageFilePath;
	MyDataTypes::TransformationMatrix16f  sceneViewMatrix;
	FileFormatPluginInterface             *fileFormatPlugin;
	FileFormatPluginInterface::FileHeader fileHeader;
	QFile                                 file;

	renderer      = RendererManager::getInstance()->getMainRenderer();
	scene         = Scene::getInstance();
	configuration = Configuration::getInstance();

	// this may take a while...
	QApplication::setOverrideCursor (Qt::waitCursor);

	// truncate the file name for the message
	if (filePath.length() > 16) {
		messageFilePath = "..\"" + filePath.right(14) + "\"";
	}
	else {
		messageFilePath = "\"" + filePath.right(16) + "\"";
	}

	fileFormatPlugin = 0;
	if (fileFilter != QString (ALL_FILES)) {
		fileFormatPlugin = FileFormatPluginManager::getInstance()->getFileFormatPlugin (fileFilter);
	}
	else {

		// probe for a suitable file format plugin
		file.setName (filePath);
		if (file.open (IO_ReadOnly) == true) {

			QDataStream dataStream (&file);

			dataStream.readRawBytes (fileHeader, sizeof (fileHeader));
			file.close();

			fileFormatPlugin = FileFormatPluginManager::getInstance()->probe (fileHeader);
		}
		else {
			message.sprintf ("Could not open the file %s", messageFilePath.latin1());
		}

	}

	// did we find a file format plugin?
	if (fileFormatPlugin != 0) {

		fileFilter = fileFormatPlugin->getFileFilter();	// set new filter

		if (fileFormatPlugin->readFromFile (filePath, true) == true) {

			this->setCaption (filePath);
			message.sprintf ("Added Scene %s", messageFilePath.latin1());
			this->setFileLoadedState (true);
			// we have to initialize the renderer with the scene view of the new scene
			scene->getTransformationMatrix (sceneViewMatrix);
			renderer->setSceneView (sceneViewMatrix);
			this->setModelModifiedState (false);

			// the loaded object may contain surfels that are flagged as "selected".
			// update the selection tool accordingly.
			CoreTools::getInstance()->getSelectionTool()->updateSelectionCounters();
		}
		else {

			message.sprintf ("Adding of Scene %s failed! Corrupt file format?", messageFilePath.latin1());
			this->setCaption ("PointShop");
			this->setFileLoadedState (false);
			// finished, but no success :(
			scene->reset();
			this->setModelModifiedState (false);

		}

	}
	else {
		message.sprintf ("Format in %s not supported.", messageFilePath.latin1());
	}

	this->statusBar()->message (message, 2000);

	// ah.. finally!
	QApplication::restoreOverrideCursor();

}

void PointShopWindow::handleConfiguration() {

  ConfigurationDialogImpl *configurationDialog;

  configurationDialog = ConfigurationDialogImpl::getInstance();
  configurationDialog->show();
  configurationDialog->raise();
}

void PointShopWindow::handleToolSettings() {

  ToolConfigurationDialog *toolConfigurationDialog;

  toolConfigurationDialog = ToolConfigurationDialog::getInstance(); 
  toolConfigurationDialog->show();
  toolConfigurationDialog->raise();

}

void PointShopWindow::handleImageExport() {
	
	ImageExportDialogImpl *imageExportDialog;
	
	imageExportDialog = ImageExportDialogImpl::getInstance();
	imageExportDialog->show();
	imageExportDialog->raise();

}

void PointShopWindow::handleAnimation() {

	AnimationDialogImpl *animationDialog;

	animationDialog = AnimationDialogImpl::getInstance();
	animationDialog->show();
	animationDialog->raise();

}

void PointShopWindow::handleStatistics() {

	StatisticsDialogImpl *statisticsDialog;

	statisticsDialog = StatisticsDialogImpl::getInstance();
	statisticsDialog->show();
	statisticsDialog->raise();

}

void PointShopWindow::handleSurfelProperties() {

	SurfelPropertiesDialogImpl *surfelPropertiesDialog;

	surfelPropertiesDialog = SurfelPropertiesDialogImpl::getInstance();
	surfelPropertiesDialog->show();
	surfelPropertiesDialog->raise();

}

void PointShopWindow::handleViewBrush() {
	
	BrushViewerImpl *brushViewer;

	brushViewer = BrushViewerImpl::getInstance();
	brushViewer->show();
	brushViewer->raise();

}


void PointShopWindow::handleViewObjectList() {
	
	ObjectViewerImpl *objectViewer;

	objectViewer = ObjectViewerImpl::getInstance();
	objectViewer->show();
	objectViewer->raise();

}



void PointShopWindow::save() {

	FileFormatPluginInterface *fileFormatPlugin;
	QString                   messageFilePath;

    if (filePath.isEmpty()) {
		this->saveAs();
		return;
    }
	
	fileFormatPlugin = FileFormatPluginManager::getInstance()->getFileFormatPlugin (fileFilter);

	if (!fileFormatPlugin || fileFormatPlugin->hasWriteProtection()) {
		this->saveAs();
		return;
	}

	// truncate the file name for the message
	if (filePath.length() > 16) {
		messageFilePath = "..\"" + filePath.right(14) + "\"";
	}
	else {
		messageFilePath = "\"" + filePath.right(16) + "\"";
	}

	fileFormatPlugin->customize (this);
	// wait for an instant...
	QApplication::setOverrideCursor (Qt::waitCursor);
	if (fileFormatPlugin->writeToFile (filePath) == false) {
		this->statusBar()->message (QString("Could not write to %1").arg (messageFilePath), 2000);
		// oh no...
		QApplication::restoreOverrideCursor();
		return;
    }

    this->setCaption (filePath);
	this->setModelModifiedState (false);
    this->statusBar()->message (QString ("File %1 saved").arg (messageFilePath), 2000);

	// yesssss, success, waiting is over...
	QApplication::restoreOverrideCursor();

}

void PointShopWindow::saveAs() {

	const QList<FileFormatPluginInterface> *fileFormatPlugins;
	FileFormatPluginInterface              *fileFormatPlugin;
	QFileDialog                            *fileDialog;
	QString                                fileDialogFileFilter;
	int                                    result;
	QFile                                  file;

	fileFormatPlugins = FileFormatPluginManager::getInstance()->getFileFormatPlugins();
	QListIterator<FileFormatPluginInterface> it (*fileFormatPlugins);
	for ( ; (fileFormatPlugin = it.current()) != 0; ++it) {		
		if (!fileFormatPlugin->hasWriteProtection()) {
			fileDialogFileFilter += fileFormatPlugin->getFileFilter();
			fileDialogFileFilter += ";;";
		}
	}

	fileDialog = new QFileDialog (filePath, fileDialogFileFilter, this, "Save", true);
	fileDialog->setMode(QFileDialog::AnyFile);
	result = fileDialog->exec();
	if (result > 0) {

		filePath = fileDialog->selectedFile();
		// update the file filter
		fileFilter = fileDialog->selectedFilter();
		filePath = FileTools::autoCompleteSuffix (filePath, FileFormatPluginManager::getInstance()->getFileFormatPlugin (fileFilter)->getFileSuffix());
		
		// check if the file already exists;
		file.setName (filePath);

		if (file.exists() == true) {
			result = QMessageBox::warning (this, "File already exists", QString ("The file %1 already exists.\nDo you want to overwrite it?").arg (filePath), "Yes", "No");
			if (result == 0) {
				// "yes"
				this->save();
			}
			else {
				// "no"
				this->statusBar()->message ("Saving aborted", 2000);
			}
		}
		else {
			this->save();
		}
		
	}
    else {
		this->statusBar()->message ("Saving aborted", 2000);
	}	
	
	// remove unused resources
	delete fileDialog;
	delete fileFormatPlugins;

}

void PointShopWindow::close() {

	if (isModelModified == false) {
		Scene::getInstance()->reset();
		this->setFileLoadedState (false);
		return;
    }

    switch (QMessageBox::information (this, "Model modified!",
				                      "The document has been changed since "
				                      "the last save.",
				                      "Save Now and Close", "Cancel", "Close Anyway",
				                      0, 1 )) {
		case 0:
			this->save();
			Scene::getInstance()->reset();
			this->setFileLoadedState (false);
			this->setModelModifiedState (false);
			break;

		case 1:
			break;

		case 2:
			Scene::getInstance()->reset();
			this->setFileLoadedState (false);
			this->setModelModifiedState (false);
			break;

		default: // just for sanity
			break;
	
    }
	
}

void PointShopWindow::about() {
    QMessageBox::about (this, "Pointshop3D",
			"PointShop3D by:\n\n"
			"         Richard Keiser\n"
			"         Oliver Knoll,\n"
			"         Mark Pauly,\n"
			"         Tim Weyrich, and\n"
			"         Matthias Zwicker\n"			
			"\n"
			"www.pointshop3d.com\n\n"
			"(c) CGL, ETHZ 2001 - 2004\n\n"
			"Version " + Version::toString());
}

void PointShopWindow::aboutQt() {
    QMessageBox::aboutQt (this, "Pointshop3D");
}

void PointShopWindow::handleCut() {
	ClipBoard::getInstance()->cut();
	pasteAction->setEnabled (true);	
}

void PointShopWindow::handleCutObject() {	
	ClipBoard::getInstance()->cutObject();
	pasteAction->setEnabled (true);
}

void PointShopWindow::handleCopy() {	
	ClipBoard::getInstance()->copy();
	pasteAction->setEnabled (true);
}
 
void PointShopWindow::handleCopyObject() {	
	ClipBoard::getInstance()->copyObject();
	pasteAction->setEnabled (true);
}

void PointShopWindow::handlePaste() {
	ClipBoard::getInstance()->paste();
}

void PointShopWindow::handleDeleteObject() {

	Scene *scene;

	scene = Scene::getInstance();
	scene->removeObject (scene->getActiveObject());

}

void PointShopWindow::handleEnablePreviewRenderer() {
	Configuration::getInstance()->setPreviewRendererEnabled (enablePreviewRendererAction->isOn());
}

void PointShopWindow::handleToggleSelectionVisualization() {

	RendererManager::getInstance()->getPreviewRenderer()->toggleSelectionVisualization();
	RendererManager::getInstance()->getMainRenderer()->toggleSelectionVisualization();
	RendererManager::getInstance()->getMainRenderer()->renderFrame();

}

void PointShopWindow::handleObjectRenamed () {
	this->updateObjectComboBox();
}

void PointShopWindow::handleObjectModified() {
	this->setModelModifiedState (true);
}

void PointShopWindow::handleObjectAdded() {
	
	this->updateObjectComboBox();
    this->setFileLoadedState(true);
	cutObjectAction->setEnabled (true);
	copyObjectAction->setEnabled (true);
	deleteObjectAction->setEnabled (true);

}

void PointShopWindow::handleObjectRemoved() {
	
	this->updateObjectComboBox();
	if (Scene::getInstance()->getNofObjects() == 0) {
		cutObjectAction->setEnabled (false);
		copyObjectAction->setEnabled (false);
		deleteObjectAction->setEnabled (false);
	}

}

void PointShopWindow::handleObjectMoved() {
	this->setModelModifiedState (true);
}

void PointShopWindow::handleObjectChosen (const QString &objectName) {
	Scene::getInstance()->setActiveObject (objectName);
}

void PointShopWindow::handleSelectionChanged (SurfelInterface::Flag selectionFlag) {
	
	if (selectionFlag == SurfelInterface::SELECTED1) {
		if (CoreTools::getInstance()->getSelectionTool()->getSelectionSize (selectionFlag) > 0) {
			cutAction->setEnabled (true);
			copyAction->setEnabled (true);
		}
		else {
			cutAction->setEnabled (false);
			copyAction->setEnabled (false);
		}
	}
}

void PointShopWindow::handlePreviewRendererEnabled (const bool isEnabled) {

	RendererManager *rendererManager;
	QWidget         *rendererWidget;

	rendererManager = RendererManager::getInstance();
	if (isEnabled == true) {

		rendererWidget = rendererManager->getPreviewRenderer()->getRendererWidget();
		rendererWidgetStack->raiseWidget (rendererWidget);		
		rendererWidget->setCursor (*(ToolManager::getInstance()->getCurrentTool()->getToolCursor()));
		
	}
	else {

		rendererWidget = rendererManager->getMainRenderer()->getRendererWidget();
		rendererWidgetStack->raiseWidget (rendererWidget);
		rendererWidget->setCursor (*(ToolManager::getInstance()->getCurrentTool()->getToolCursor()));
		
	}

}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
