// Title:   FileFormatPluginRegistry.h
// Created: Thu Sep 25 14:29:14 2003
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

#ifndef __FILEFORMATPLUGINREGISTRY_H_
#define __FILEFORMATPLUGINREGISTRY_H_

#include <qmap.h>
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

#include "../../Interfaces/src/FileFormatPluginInterface.h"
#include "FileFormatPluginLoader.h"

// *****************
// class definitions
// *****************

/**
 * This class registers all loaded file format plugins, so they can be unloaded when not
 * needed anymore.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class FileFormatPluginRegistry {

public:

	// ********
	// typedefs
	// ********

	/**
	 * A <code>struct</code> storing the access <code>FileFormatPluginAccess</code> functions
	 * and the <code>fileFormatPluginPath</code>.
	 */
	typedef struct {
		FileFormatPluginLoader::FileFormatPluginAccess fileFormatPluginAccess;
		QString                                        fileFormatPluginPath;
	} DynamicAccess;

	/**
	 * This method creates an instance of this <code>FileFormatPluginRegistry</code>, if necessary,
	 * and returns it. Only one instance at a time exists.
	 *
	 * @return an instance of this <code>FileFormatPluginRegistry</code>
	 */
	static FileFormatPluginRegistry *getInstance();
	
	/**
	 * Destroys the instance of this <code>FileFormatPluginRegistry</code>, if there
	 * is one.
	 */
	static void destroyInstance();

	// ****************
	// Registry methods
	// ****************

	/**
	 * Registers a file format plugin, accessible with <code>DynamicAccess</code> and identified by its
	 * <code>fileFilter</code>.
	 *
	 * @param fileFilter
	 *        an file filter which identifies the plugin to be registered
	 * @param dynamicAccess
	 *        a pointer to the <code>DynamicAccess</code>, which allows to unload dynamically loaded plugins
	 * @see FileFormatPluginInterface#getFileFilter
	 * @see #unregisterPlugin
	 */
	void registerFileFormatPlugin (const QString fileFilter, const DynamicAccess dynamicAccess);
	
	/**
	 * Unregisters and unloads the plugin given by its <code>fileFilter</code>.
	 *
	 * @param fileFilter
	 *        a <code>QString</code> specifying the file filter of the file format plugin to be unregistered and unloaded
	 * @see FileFormatPluginInterface#getFileFilter
	 * @see #registerTool
	 */ 
	void unregisterFileFormatPlugin (const QString fileFilter);

	/**
	 * Unregisters all plugins, unloading the corresponding dynamic libraries.
	 */
	void unregisterAllFileFormatPlugins();

	/**
	 * Returns a <code>FileFormatPluginAccess</code> identified by its <code>fileFilter</code>. Check the field
	 * <code>isValid</code> to see if the requested <code>FileFormatPluginAccess</code> was found.
	 * 
	 * @param fileFilter
	 *        a <code>QString</code> specifying the file filter of the file format plugin which is to be returned
	 * @return the <code>FileFormatPluginAccess</code> identified by <code>fileFilter</code>; the field <code>isValid</code>
	 *         is <code>true</code> if the <code>FileFormatPluginAccess</code> identified by <code>fileFilter</code> was
	 *         found, <code>false</code> else
	 */
	FileFormatPluginLoader::FileFormatPluginAccess getFileFormatPlugin (const QString fileFilter) const;

	/**
	 * Returns a <code>QList</code> containing references to instances of the registered file format plugins. The
	 * returned list must be <code>delete</code>d after use.
	 *
	 * @return a <code>QList</code> containing all registered <code>FileFormatPluginInterface</code>s; must
	 *         be <code>delete</code>d after use
	 */
	const QList<FileFormatPluginInterface> *getFileFormatPlugins() const;

	/**
	 * Probes for the <code>FileFormatPluginInterface</code> which is capable of scanning the
	 * <code>fileHeader</code> and returns it. If no suitable <code>FileFormatPluginInterface</code> is
	 * found then 0 is returned. The <code>FileFormatPluginInterface</code> must be <code>delete</code>d
	 * after use.
	 *
	 * @param fileHeader
	 *        the <code>FileHeader</code> to be parsed
	 * @return a pointer to the <code>FileFormatPluginInterface</code> which is capable of parsing the
	 *         <code>fileHeader</code>; 0 if no such <code>FileFormatPluginInterface</code> exists; must be
	 *         <code>delete</code>d after use
	 */
	FileFormatPluginInterface *probe (const FileFormatPluginInterface::FileHeader fileHeader) const;

	/**
	 * Returns the number of registered <code>FileFormatPluginInterface</code>s.
	 *
	 * @return the number of registered <code>FileFormatPluginInterface</code>s
	 */
	uint getNofFileFormatPlugins() const;

private:

	typedef QMap<QString,DynamicAccess> Registry;

	Registry registry;	

	FileFormatPluginRegistry();
	virtual ~FileFormatPluginRegistry();

	// unloads the corresponding library
	void deleteEntry (Registry::Iterator it);	

};




#endif  // __PLUGINREGISTRY_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
