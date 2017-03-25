// Title:   SplashScreen.h
// Created: Thu Sep 25 15:03:55 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003, Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General
// Public License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330,
// Boston, MA 02111-1307 USA
// 
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

#ifndef __SPLASHSCREEN_H_
#define __SPLASHSCREEN_H_

#include <qlabel.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qimage.h>

#include "../../src/UtilitiesDLL.h"

/**
 * This class implements a nice splash screen, which pops up
 * before the application starts (I always wanted to do such
 * a thing ;)
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class SplashScreen {

public:

	/**
	 * This method creates an instance of this <code>SplashScreen</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>SplashScreen</code>
	 */
	UTILITIES_API static SplashScreen *getInstance();

	/**
	 * Destroys the instance of this <code>SplashScreen</code>, if there
	 * is one. Also <code>hide</code>s the splash screen, if not already
	 * done.
	 *
	 * @see #hide
	 */
	UTILITIES_API static void destroyInstance();

	/**
	 * Shows the splash screen.
	 *
	 * @see #hide
	 */
	UTILITIES_API void show();

	/**
	 * Hides the splash screen.
	 *
	 * @see #show
	 */
	UTILITIES_API void hide();

	/**
	 * This method shows a text while this <code>SplashScreen</code> is showing.
	 *
	 * @param text
	 *        a <code>QString</code> which contains the text to be shown; the
	 *        text must have reasonable length, else it is clipped
	 */
	UTILITIES_API void showText (const QString text);

private:

	QLabel  *splash;
	QPixmap splashPixmap;
	QImage  splashImage;

	// singleton pattern
	SplashScreen();
	virtual ~SplashScreen();

};

#endif  // __SPLASHSCREEN_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:

