// Title:   ToolManager.h
// Created: Thu Sep 25 14:27:53 2003
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

#ifndef __TOOLMANAGER_H_
#define __TOOLMANAGER_H_

#include <qstring.h>
#include <qevent.h>
#include <qmainwindow.h>

#include "../../Interfaces/src/ToolInterface.h"
#include "../../Interfaces/src/RendererInterface.h"
#include "../../CoreTools/NavigationTool/src/NavigationTool.h"
#include "../../CoreTools/ColorPickerTool/src/ColorPickerTool.h"
#include "../../CoreTools/SelectionTool/src/SelectionTool.h"
#include "../../src/CoreDLL.h"

#include "ToolDirWalk.h"

/**
 * This class manages the available <code>ToolInterface</code>s such as the
 * <code>NavigationTool</code>, the <code>ColorPickerTool</code>, the <code>SelectionTool</code>
 * and the dynamically available tool plugins.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see ToolInterface
 */
class ToolManager : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates an instance of this <code>ToolManager</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>ToolManager</code>
	 */
	CORE_API static ToolManager *getInstance();

	/**
	 * Destroys the instance of this <code>ToolManager</code>, if there is any.
	 */
	CORE_API static void destroyInstance();

	/**
	 * Initializes this <code>ToolManager</code>: the <code>toolPluginDirectory</code> ist scanned for
	 * valid tool plugins. This method must be called before any other tool access methods.
	 * Another call re-initializes this <code>ToolManager</code>.
	 *
	 * @param toolPluginDirectory
	 *        a <code>QString</code> specifying the relative or absolute path of the
	 *        tool plugin directory; this directory is scanned for
	 *        valid tool plugins - invalid tool plugins are ignored, but may lead to a system
	 *        error message, depending on the platform
	 * @param mainWindow
	 *        a pointer to a <code>QMainWindow</code> which will be the parent of the <code>QToolBar</code>
	 *        which is initialized by this method
	 */
	CORE_API void initialize (const QString toolPluginDirectory, QMainWindow *mainWindow);

	/**
	 * Checks the availability of the <code>RendererInterface</code>s and enables/disables their
	 * <code>QToolButton</code>s accordingly.
	 */
	CORE_API void checkToolAvailability();

	/**
	 * Returns the currently selected tool.
	 *
	 * @return a pointer to the currently selected <code>ToolInterface</code>
	 * @see ToolInterface
	 */
	CORE_API ToolInterface *getCurrentTool() const;

	/**
	 * deselects the active tool and activates the navigation tool
	 */
	CORE_API void switchToNavigationTool();

	/**
	 * Handle key press events - normally those events are routed to the current
	 * tool, but special shortcut keys activate certain tools.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>toolChanged</code></li>
	 * </ul>
	 *
	 * @param ke
	 *        a pointer to the specific <code>QKeyEvent</code>
	 * @see #toolChanged
	 */
	CORE_API void handleKeyPressEvent (QKeyEvent *ke);

	/**
	 * Handle key release events - normally those events are simply routed to the
	 * current tool.
	 *
	 * @param ke
	 *        a pointer to the specific <code>QKeyEvent</code>
	 */
	CORE_API void handleKeyReleaseEvent (QKeyEvent *ke);

signals:

	/**
	 * Emitted whenever the tool has been changed.
	 *
	 * @param tool
	 *        a pointer to the <code>ToolInterface</code> of the
	 *        newly selected tool
	 */
	void toolChanged (const ToolInterface *tool);

private:

	QButtonGroup    *buttonGroup;
	QToolBar        *toolBar;
	ToolInterface   *currentTool;
	QMainWindow     *mainWindow;

	ColorPickerTool *colorPickerTool;
	NavigationTool  *navigationTool;
	SelectionTool   *selectionTool;

	int             colorPickerToolID,
					navigationToolID,
	                selectionToolID;

	ToolManager();
protected:  // To suppress warnings...
	virtual ~ToolManager();
private:

	// creates and adds the default tools
	void addDefaultTools();

private slots:

	// connected to the button group
	void handleToolChanged (int id);

	// connected to the Configuration
	void handleToolDirectoryChanged (const QString newToolDirectory);

	// connected to the RendererManager
	void handleMainRendererChanged();

};

#endif  // __STANDARDTOOLBOX_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
