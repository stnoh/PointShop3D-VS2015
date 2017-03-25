// Title:   SurfelPropertiesDialogImpl.h
// Created: Thu Sep 25 14:42:50 2003
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

#ifndef __SURFELPROPERTIESDIALOGIMPL_H_
#define __SURFELPROPERTIESDIALOGIMPL_H_

#include "../Core/Interfaces/src/SurfelInterface.h"
#include "../UserInterface/UI/SurfelPropertiesDialog.h"

/**
 * This class implements a surfel properties dialog which allows the user to
 * specify which properties such as the position, the normal, and
 * the diffuse color are stored in the <code>SurfelInterface</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class SurfelPropertiesDialogImpl : public SurfelPropertiesDialogBase {
	
    Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>SurfelPropertiesDialogImpl</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>SurfelPropertiesDialogImpl</code>
	 */
	static SurfelPropertiesDialogImpl *getInstance();

	/**
	 * Destroys the instance of this <code>SurfelPropertiesDialogImpl</code>, if there
	 * is one.
	 */
	static void destroyInstance();

public slots:

	/**
	 * Called whenever the <em>Apply</em> button was clicked.
	 */
	void clickedApply();

	/**
	 * Called whenever the <em>Help</em> button was clicked.
	 */
	void clickedHelp();

private:

	SurfelInterface::PropertyDescriptor propertyDescriptor;

	// singleton pattern
	SurfelPropertiesDialogImpl (QWidget *parent = 0, const char *name = 0, bool modal = false, WFlags flags = 0);
protected:  // To suppress warnings...
    ~SurfelPropertiesDialogImpl();
private:

	// initializes the dialog elements
	void initDialog();

	// applies the surfel property settings
	void applyDialogSettings();

private slots:

	void handleObjectModified ();
	void handleObjectAdded();
	void handleActiveObjectChanged ();

};

#endif  // __SURFELPROPERTIESDIALOGIMPL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
