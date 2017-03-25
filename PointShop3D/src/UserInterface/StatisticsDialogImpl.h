// Title:   StatisticsDialogImpl.h
// Created: Thu Sep 25 14:42:55 2003
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

#ifndef __STATISTICSDIALOG_H_
#define __STATISTICSDIALOG_H_

#include <qwidget.h>

#include "../Core/Interfaces/src//SurfelInterface.h"

#include "../UserInterface/UI/StatisticsDialog.h"

/**
 * This class implements a statistic dialog which shows basic statistics about the <code>Scene</code>
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class StatisticsDialogImpl : public StatisticsDialogBase {
	
    Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>StatisticsDialogImpl</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>StatisticsDialogImpl</code>
	 */
	static StatisticsDialogImpl *getInstance();

	/**
	 * Destroys the instance of this <code>StatisticsDialogImpl</code>, if there
	 * is one.
	 */
	static void destroyInstance();
	
private:

	StatisticsDialogImpl (QWidget *parent = 0, const char *name = 0, bool modal = false, WFlags flags = 0);
protected:  // To suppress warnings...
	virtual ~StatisticsDialogImpl();
private:

	// initialises this dialog, setting all text labels, connects the signals
	void initDialog();

	// updates the text labels
	void updateLabels();

public slots:
	bool close(bool alsoDelete = false);
	void show();

private slots:

	void handleChanges();
	void handleSelectionChanged (SurfelInterface::Flag selectionFlag);
	void handleMainRendererChanged();
	void handlePreviewRendererChanged();
	void handleMainRendererFPSChanged (const float mainRendererFPS);
	void handlePreviewRendererFPSChanged (const float previewRendererFPS);

};

#endif  // __STATISTICSDIALOG_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
