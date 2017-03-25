// Title:   FileTools.h
// Created: Thu Sep 25 15:01:51 2003
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

#ifndef __FILETOOLS_H_
#define __FILETOOLS_H_

#include <qstring.h>

#include "UtilitiesDLL.h"

/**
 * This namespace provides utility functions for filenames/files.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace FileTools {

	/**
	 * Makes sure that the <code>fileName</code> ends with the <code>suffix</code>, separated by
	 * a dot ".", if this suffix isn't already there.
	 *
	 * <pre>
	 * Examples: fileName   suffix    result 
	 *
	 *           foo        bar       foo.bar
	 *           foobar     bar       foobar.bar
	 *           foo.bar    bar       foo.bar
	 *           foo.bar    chi       foo.bar.chi
	 * </pre>
	 *
	 * @param fileName
	 *        a <code>QString</code> storing the file name to be auto-completed
	 * @param suffix
	 *        a <code>QString</code> storing the suffix to be appended, if necessary, separated by a dot "."
	 * @return a <code>QString</code> storing the auto-completed <code>fileName</code>
	 */
	UTILITIES_API QString autoCompleteSuffix (const QString fileName, const QString suffix);

	/**
	 * Replaces the existing suffix in the <code>fileName</code> with the <code>newSuffix</code>.
	 * A suffix is considered to be all characters behind the last point in the <code>fileName</code>.
	 * If no such suffix exists, the <code>newSuffix</code> is simply appended.
	 *
	 * <pre>
	 * Examples: fileName   newSuffix      result
	 *
	 *           foo.bar    tra            foo.tra
	 *           foo        bar            foo.bar
	 *           foo.bar    <empty string> foo
	 * </pre>
	 *
	 * @param fileName
	 *        a <code>QString</code> storing the file name where the suffix has to be replaced
	 * @param newSuffix
	 *        a <code>QString</code> storing the new suffix; if it is a null string the existing
	 *        suffix is replaced
	 * @return a <code>QString</code> storing the new <code>fileName</code>
	 */
	UTILITIES_API QString replaceSuffix (const QString fileName, const QString newSuffix);

	/**
	 * Returns the suffix of the <code>fileName</code>. The suffix is separated by the last point
	 *
	 * <pre>
	 * Examples: fileName    result
	 *
	 *           foo.bar     bar
	 *           foo.        <empty string>
	 *           boo.bar.bla bla
	 * </pre>
	 *
	 * @param fileName
	 *        a <code>QString</code> storing the file name where the suffix has to be returned
	 * @return a <code>QString</code> containing the suffix
	 */
	UTILITIES_API QString getSuffix (const QString fileName);

	/**
	 * Returns the file name of the <code>filePath</code>, that is the part behind the last directory
	 * separator ('/').
	 *
	 * <b>Note:</b> Be careful on platforms such as <code>Win32</code>, since they use '\' as separator,
	 *              which wouldn't work here. <em>Qt</em> uses '/' platform-independently, so as long
	 *              as file paths returned from e.g. <code>QFileDialog</code> are used this is fine.
	 *
	 * <pre>
	 * Examples: fileName    result
	 *
	 *           foo/bar     bar
	 *           foo         foo
	 * </pre>
	 *
	 * @param filePath
	 *        a <code>QString</code> containing the file path
	 * @return the a <code>QString</code> containing the file name of the <code>filePath</code>
	 */
	UTILITIES_API QString getFileName (const QString filePath);

	/**
	 * Returns the file name of the <code>filePath</code>, that is the part behind the last directory
	 * separator ('/') and removes the suffix.
	 *
	 * <b>Note:</b> Be careful on platforms such as <code>Win32</code>, since they use '\' as separator,
	 *              which wouldn't work here. <em>Qt</em> uses '/' platform-independently, so as long
	 *              as file paths returned from e.g. <code>QFileDialog</code> are used this is fine.
	 *
	 * @param filePath
	 *        a <code>QString</code> containing the file path
	 * @return the a <code>QString</code> containing the file name without suffix of the <code>filePath</code>
	 */
	UTILITIES_API QString getFileNameWithoutSuffix(const QString filePath);
	

};

#endif  // __FILETOOLS_H_


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
