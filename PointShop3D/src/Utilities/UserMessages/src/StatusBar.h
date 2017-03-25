// Title:   StatusBar.h
// Created: Thu Sep 25 15:03:50 2003
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

#ifndef __STATUSBAR_H_
#define __STATUSBAR_H_

#include <qstatusbar.h>
#include <qprogressbar.h>
#include <qtimer.h>

#include "../../src/UtilitiesDLL.h"

/**
 * This class gives access to the <code>QStatusBar</code> of the main <code>PointShopWindow</code>
 * so <code>PluginInterface</code>s and <code>ToolInterface</code>s can write messages.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class StatusBar : QObject {

	Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>StatusBar</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>StatusBar</code>
	 */
	UTILITIES_API static StatusBar *getInstance();

	/**
	 * Destroys the instance of this <code>StatusBar</code>, if there
	 * is one.
	 */
	UTILITIES_API static void destroyInstance();

	/**
	 * This method shows a text in the <code>QStatusBar</code>.
	 *
	 * @param text
	 *        a <code>QString</code> which contains the text to be shown; the
	 *        text must have reasonable length, else it is clipped
	 * @param milliSeconds
	 *        the time in milliseconds during the <code>text</code> will be shown; any negative number
	 *        will show the <code>text</code> until the next call to <code>showMessage</code>
	 */
	UTILITIES_API void showMessage (const QString text, const int milliSeconds = 5000);

	/**
	 * Returns a <code>QProgressBar</code> which is useful for <code>ToolInterface</code>s
	 * and <code>PluginInterface</code>s which want to report their progress.
	 *
	 * @return a pointer to a <code>QProgressBar</code> which is shown in this <code>StatusBar</code>
	 */
	// UTILITIES_API QProgressBar *getProgressBar();

	/**
	 * Displays the <code>progress</code> of a certain operation in the status bar of the
	 * <code>PointShopWindow</code>. If autoReset is set to <code>true</code> the progress bar
	 * is reseted to 0% after a certain amount of time.
	 *
	 * @param progress
	 *        the progress in percent to be displayed by the progress bar; must be in [0, 100]
	 * @param autoReset
	 *        set to <code>true</code> if the progress bar should be reseted to 0% after a certain amount of time
	 */
	UTILITIES_API void setProgress (const int progress, const bool autoReset = true);

	/**
	 * Returns the progress currently shown by the progress bar in the status bar of the
	 * <code>PointShopWindow</code>.
	 *
	 * @return the progress in percent currently shown by the progress bar; is in [0, 100]
	 */
	UTILITIES_API int getProgress() const;

	/**
	 * Resets the progress bar to 0%.
	 */
	UTILITIES_API void resetProgress();

private:

	// access for...
	friend class PointShopWindow;

	QStatusBar   *statusBar;
	QProgressBar *progressBar;
	QTimer       progressBarTimer;

	// singleton pattern
	StatusBar();
	virtual ~StatusBar();

	UTILITIES_API void setStatusBar (QStatusBar *statusBar);
	UTILITIES_API QStatusBar *getStatusBar() const;

private slots:

	void handleProgressBarTimer();

};

#endif  // __STATUSBAR_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
