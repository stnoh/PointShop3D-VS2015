// Title:   FileFormatPluginManager.h
// Created: Thu Sep 25 14:29:16 2003
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

#ifndef __FILEFORMATPLUGINMANAGER_H_
#define __FILEFORMATPLUGINMANAGER_H_

#include <qstring.h>
#include <qobject.h>

#include "../../Interfaces/src/FileFormatPluginInterface.h"
#include "../../src/CoreDLL.h"

#include "FileFormatPluginDirWalk.h"

/**
 * This class manages the available <code>FileFormatPluginInterface</code>s which are loaded dynamically.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see FileFormatPluginInterface
 */
class FileFormatPluginManager : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates an instance of this <code>FileFormatPluginManager</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>FileFormatPluginManager</code>
	 */
	CORE_API static FileFormatPluginManager *getInstance();

	/**
	 * Destroys the instance of this <code>FileFormatPluginManager</code>, if there is any.
	 */
	CORE_API static void destroyInstance();

	/**
	 * Initializes this <code>FileFormatPluginManager</code>: the <code>fileFormatPluginDirectory</code> ist scanned for
	 * valid plugins. This method must be called before any other file format plugin access methods.
	 * Another call re-initializes this <code>FileFormatPluginManager</code>.
	 *
	 * @param fileFormatPluginDirectory
	 *        a <code>QString</code> specifying the relative or absolute path of the
	 *        file format plugin directory; this directory is scanned for
	 *        valid file format plugins - invalid file format plugins are ignored, but may lead to a system
	 *        error message, depending on the platform
	 * @return <code>true</code>, if at least one valid file format plugin could be found; <code>false</code> else
	 */
	CORE_API bool initialize (const QString fileFormatPluginDirectory);

	/**
	 * Returns the <code>FileFormatPluginInterface</code> which provides the <code>fileFilter</code>.
	 *
	 * @param fileFilter
	 *        a <code>QString</code> which specifies the file filter, which is also returned by the
	 *        method <code>FileFormatPluginInterface::getFileFilter</code>
	 * @return a pointer to a <code>FileFormatPluginInterface</code> which provides the <code>fileFilter</code>
	 * @see FileFormatPluginInterface#getFileFilter
	 */
	CORE_API FileFormatPluginInterface *getFileFormatPlugin (const QString fileFilter);

	/**
	 * Returns a <code>QList</code> containing all registered <code>FileFormatPluginInterface</code>s. Must
	 * be <code>delete</code>d after use.
	 *
	 * @return a pointer to a <code>QList</code> containing all registered <code>FileFormatPluginInterface</code>s;
	 *         must be <code>delete</code>d after use
	 */
	CORE_API const QList<FileFormatPluginInterface> *getFileFormatPlugins() const;

	/**
	 * Probes for the <code>FileFormatPluginInterface</code> which is capable of scanning the
	 * <code>fileHeader</code> and returns it. If no suitable <code>FileFormatPluginInterface</code> is
	 * found then 0 is returned.
	 *
	 * @param fileHeader
	 *        the <code>FileHeader</code> to be parsed
	 * @return a pointer to the <code>FileFormatPluginInterface</code> which is capable of parsing the
	 *         <code>fileHeader</code>; 0 if no such <code>FileFormatPluginInterface</code> exists
	 */
	CORE_API FileFormatPluginInterface *probe (const FileFormatPluginInterface::FileHeader fileHeader);

private:

	FileFormatPluginInterface *fileFormatPlugin;  // the most recently selected file format plugin

	FileFormatPluginManager();
protected:  // To suppress warnings...
	virtual ~FileFormatPluginManager();
private:

private slots:

	void handleFileFormatPluginDirectoryChanged (const QString newFileFormatPluginDirectory);

};

#endif  // __FILEFORMATPLUGINMANAGER_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
