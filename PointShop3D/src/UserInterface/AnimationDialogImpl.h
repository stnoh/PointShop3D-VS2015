// Title:   AnimationDialogImpl.h
// Created: Thu Sep 25 14:43:22 2003
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

#ifndef __ANIMATIONDIALOG_H_
#define __ANIMATIONDIALOG_H_

#include <qthread.h>
#include <qstring.h>
#include <qsize.h>
#include <qevent.h>

#include "../DataTypes/src/Vector3D.h"
#include "../DataTypes/src/MyDataTypes.h"
#include "../UserInterface/UI/AnimationDialog.h"  // must be ../UserInterface/ for linux portability

/**
 * This class implements an animation dialog which lets the user create simple animations such as
 * rotation and zoom.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class AnimationDialogImpl : public AnimationDialogBase, public QThread {
	
    Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>AnimationDialogImpl</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>AnimationDialogImpl</code>
	 */
	static AnimationDialogImpl *getInstance();

	/**
	 * Destroys the instance of this <code>AnimationDialogImpl</code>, if there
	 * is one.
	 */
	static void destroyInstance();

public slots:

	/**
	 * Called whenever the <em>Animate...</em> button was clicked.
	 */
	void clickedAnimate();

	/**
	 * Called whenever the <em>Stop!</em> button was clicked.
	 */
	void clickedStop();

	/**
	 * Called whenever the <em>Reset</em> button was clicked.
	 */
	void clickedReset();

	/**
	 * Called whenever the <em>Browse...</em> button was clicked.
	 */
	void clickedBrowse();

	void handleDegreesChanged();
	void handleStartPositionChanged();
	void handleEndPositionChanged();
	void handleStartCutOffChanged();
	void handleEndCutOffChanged();
	void handleDurationChanged();
	void handleFPSChanged (int newItem);
	void handleWidthChanged();
	void handleHeightChanged();
	void handleBaseFileNameChanged();
	void handleDirectoryChanged();
	void handleStoreAnimationChanged (bool enable);

protected:

	void run();

protected slots:

	void showEvent (QShowEvent *se);
	void hideEvent (QHideEvent *he);

private:

	bool            continueAnimation,   // true, as long as the animation thread should go on
		            storeAnimation;   
	float           degrees,
		            startPosition,
		            endPosition,
					startCutOff,
					endCutOff,
		            duration,
		            fps;
	QString         baseFileName,
		            directory;
	QSize           dimension;

	// singleton pattern
	AnimationDialogImpl (QWidget *parent = 0, const char *name = 0, bool modal = false, WFlags flags = 0);
protected:  // To suppress warnings...
    ~AnimationDialogImpl();
private:

	// initializes the dialog elements
	void initDialog();

	// initializes the format text label
	void updateFormatTextLabel();

	// starts the animation
	void applyDialogSettings();

	// if 'enable' is set to true, the controls are disabled and the 'Stop!' button is enabled,
	// the other way round else
	void setWorkMode (const bool enable);

private slots:

	void handleViewPortSizeChanged (const QSize newViewPortSizeSize);
	void handleSceneMoved (const float* newTransformationMatrix);

};

#endif  // __ANIMATIONDIALOG_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
