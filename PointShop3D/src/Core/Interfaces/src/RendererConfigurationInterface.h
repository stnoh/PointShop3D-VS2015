// Title:   RendererConfigurationInterface.h
// Created: Thu Sep 25 14:29:07 2003
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

#ifndef __RENDERERCONFIGURATIONINTERFACE_H_
#define __RENDERERCONFIGURATIONINTERFACE_H_

#include <qobject.h>
#include <qwidget.h>

#include "../../src/CoreDLL.h"

/**
 * This interface defines the API for configuring the specific renderer.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class CORE_API RendererConfigurationInterface : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates and returns a configuration <code>QWidget</code> which lets the user configure the
	 * current <code>RendererInterface</code>, for example renderer quality settings. It is added dynamically
	 * to the <code>ConfigurationDialogImpl</code>ementation dialog box, the <code>parent</code>.
	 *
	 * <b>Note:</b> It is important that the implementations of this <code>RendererConfigurationInterface</code>
	 *              <code>delete</code> this <code>QWidget</code> as soon as they are <code>delete</code>d themselves
	 *              so that this <code>QWidget</code> is removed from the configuration dialog again.
	 *
	 * @param parent
	 *        a pointer to the parent <code>QWidget</code> of the created <code>QWidget</code>
	 * @return a pointer to the <code>new</code>ly created configuration <code>QWidget</code>;
	 *         must be <code>delete</code>d by this <code>RendererConfigurationInterface</code>
	 */
	virtual QWidget *createConfigurationWidget (QWidget *parent) = 0;

	/**
	 * Applies the configuration according to the user settings in the configuration <code>QWidget</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>configurationApplied</code></li>
	 * </ul>
	 *
	 * @see #createConfigurationWidget
	 * @see #configurationApplied
	 */
	virtual void apply() = 0;

signals:

	/**
	 * Emitted whenever the <code>apply</code> method has been executed and the settings have taken effect.
	 * Useful for the renderer so it knows when to re-render the image with the changed settings.
	 */
	void configurationApplied();

};


#endif  // __RENDERERCONFIGURATIONINTERFACE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
