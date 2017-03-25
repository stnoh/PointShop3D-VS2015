// Title:   FileFormatPluginInterface.h
// Created: Thu Sep 25 14:29:12 2003
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

#ifndef __FILEFORMATPLUGININTERFACE_H_
#define __FILEFORMATPLUGININTERFACE_H_

#include <qstring.h>
#include <qdialog.h>

// ***********
// definitions
// ***********

#if defined _WIN32

	// The following ifdef block is the standard way of creating macros which make exporting
	// from a DLL simpler. All files within this DLL are compiled with the FILEFORMATPLUGIN_EXPORTS
	// symbol defined on the command line. this symbol should not be defined on any project
	// that uses this DLL. This way any other project whose source files include this file see
	// FILEFORMATPLUGIN_API  functions as being imported from a DLL, wheras this DLL sees symbols
	// defined with this macro as being exported.
	#ifdef FILEFORMATPLUGIN_EXPORTS
		#define FILEFORMATPLUGIN_API __declspec(dllexport)
	#else
		#define FILEFORMATPLUGIN_API __declspec(dllimport)
	#endif

#else

	// for all other platforms FILEFORMATPLUGIN_API is defined to be "nothing"
	#define FILEFORMATPLUGIN_API
	
#endif

/**
 * This interface defines the API for file format plugins. It defines methods for reading and writing data
 * from/to a file and for testing whether a given file can be read by a plugin.
 *
 * Note that apart from this <code>FileFormatPluginInterface</code> the file format plugins
 * must also implement some special C-style functions:
 * <ul>
 *   <li><code>TOOL_API FileFormatPluginInterface *createFileFormatPlugin()</code></li>
 * </ul>
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class FILEFORMATPLUGIN_API FileFormatPluginInterface {

public:

	/**
	 * The file header containing the first 64 chars of the file to be read.
	 */
	typedef char FileHeader[64];

	/**
	 * Returns <code>true</code> if this <code>FileFormatPluginInterface</code> can parse the <code>fileHeader</code>.
	 *
	 * @param fileHeader
	 *        the <code>FileHeader</code> to be tested; if the file to be read is smaller than the size of the
	 *        <code>FileHeader</code> then the <code>FileHeader</code> is filled up with undefined values
	 * @return <code>true</code> if the <code>fileHeader</code> can be parsed by this
	 *         <code>FileFormatPluginInterface</code>
	 */
	virtual bool isReadable (const FileHeader fileHeader) = 0;

	/**
	 * Returns <code>true</code> if this <code>FileFormatPluginInterface</code> is not allowed to write
	 *
	 *
	 * @return <code>true</code> if writting with this plugin is not allowed
	 */
	virtual bool hasWriteProtection () {
		return false;
	}

	/**
	 * Returns <code>true</code> if this <code>FileFormatPluginInterface</code> is not allowed to read
	 *
	 *
	 * @return <code>true</code> if reading with this plugin is not allowed
	 */
	virtual bool hasReadProtection () {
		return false;
	}

	/**
	 * Reads the surfel data from the file given by its <code>filePath</code> and builds up
	 * the <code>Scene</code> and its <code>Object</code>s. Returns <code>true</code> on success.
	 *
	 * @param filePath
	 *        a pointer to a string containing the absolute or relative file path of the file to be read
	 * @param mergeWithCurrent
	 *        set to <code>true</code> if the <code>Scene</code> to be loaded should be merged with the
	 *        current one; set to <code>false</code> if the <code>Scene</code> to be loaded should replace
	 *        the current one
	 * @return <code>true</code> on success; <code>false</code> if the file could not be read in which
	 *         case the <code>Scene</code> is left in an undefined state and should be <code>reset</code>ed.
	 * @see Scene#reset
	 */
	virtual bool readFromFile (const QString filePath, const bool mergeWithCurrent = false) = 0;

	/** 
	 * Writes the surfel data from the <code>Scene</code> into the file given by its <code>filePath</code>.
	 * Returns <code>true</code> on success.
	 *
	 * @param filePath
	 *        a pointer to a string containing the file path of the file to be read
	 * @param onlyActiveObject
	 *        set to <code>true</code> if only the <em>active</em> <code>Object</code> should be written;
	 *        <code>false</code> if all <code>Object</code>s should be written
	 * @return <code>true</code> on success; <code>false</code> if the file could not be written
	 * @see Scene#reset
	 * @see Scene#getActiveObject
	 */
	virtual bool writeToFile (const QString filePath, const bool onlyActiveObject = false) = 0;

	/**
	 * Returns the file filter (a combination of a short file format description and the file suffix) of
	 * the file format supported by this <code>FileFormatPluginInterface</code>, for example:
	 * <code>Point Shop Scene File (*.pss)</code>
	 *
	 * @return the file filter of the file format supported by this <code>FileFormatPluginInterface</code>
	 */
	virtual QString getFileFilter() const = 0;

	/**
	 * Returns the file suffix, for example <code>pss</code>.
	 *
	 * @return a <code>QString</code> containing the file suffix
	 */
	virtual QString getFileSuffix() const = 0;

	/**
	 * Shows a <em>modal</em> <code>QDialog</code> with <code>parent</code> which allows the user to
	 * customize the format specific settings, for example if the current selection from
	 * the <code>SelectionTool</code> should be stored as well, if the format supports this.
	 * This method does nothing if no such settings exist.
	 *
	 * @param parent
	 *        a pointer to the parent <code>QWidget</code> of the <em>modal</em> <code>QDialog</code>
	 */
	virtual void customize (QWidget *parent) = 0;

};

#endif  // __FILEFORMATPLUGININTERFACE_H_


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
