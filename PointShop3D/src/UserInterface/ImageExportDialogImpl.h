// Title:   ImageExportDialogImpl.h
// Created: Thu Sep 25 14:43:08 2003
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

#ifndef __IMAGEEXPORTDIALOG_H_
#define __IMAGEEXPORTDIALOG_H_

#include "../UserInterface/UI/ImageExportDialog.h"

/**
 * This class implements an image export dialog, which exports the
 * currently visible image buffer into a file of user specified
 * format. The image is either taken from the currently active renderer
 * or the preview renderer, depending on which one is currently visible.
 *
 * @author Oliver Knoll
 * @version 1.1
 */
class ImageExportDialogImpl : public ImageExportDialogBase {
	
    Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>ImageExportDialogImpl</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>ImageExportDialogImpl</code>
	 */
	static ImageExportDialogImpl *getInstance();

	/**
	 * Destroys the instance of this <code>ImageExportDialogImpl</code>, if there
	 * is one.
	 */
	static void destroyInstance();

public slots:

	/**
	 * Called whenever the <em>Apply</em> button was clicked.
	 */
	void clickedApply();

	/**
	 * Called whenever the <em>Okay</em> button was clicked.
	 */
	void clickedOkay();

	/**
	 * Called whenever the <em>Help</em> button was clicked.
	 */
	void clickedHelp();

	/**
	 * Called whenever the <em>Browse..</em> button was clicked.
	 */
	void clickedBrowseButton();

	/**
	 * Called whenever the file format combo box has changed its value.
	 */
	void handleFileFormatChanged (const QString &newFileFormat);

	/**
	 * Called whenever <em>return</em> has been pressed within <code>fileNameLineEdit</code>.
	 */
	void handleFileNameChanged();

private:

	// singleton pattern
	ImageExportDialogImpl (QWidget* parent = 0, const char* name = 0, bool modal = false, WFlags fl = 0);
protected:  // To suppress warnings...
    ~ImageExportDialogImpl();
private:

	void initDialog();
	void applyDialogSettings();
	

};

#endif  // __IMAGEEXPORTDIALOG_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
