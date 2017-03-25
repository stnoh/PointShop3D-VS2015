// Title:   ColorChooserButton.h
// Created: Thu Sep 25 14:31:06 2003
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

#ifndef __COLORCHOOSERBUTTON_H_
#define __COLORCHOOSERBUTTON_H_

#include <qtoolbutton.h>
#include <qpixmap.h>

/**
 * This class implements a <code>QToolButton</code> which reacts on
 * left and right button mouse clicks.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ColorChooserButton : public QToolButton {

	Q_OBJECT

public:

	ColorChooserButton (const QPixmap &pixmap,
		                const QString &textLabel,
						const QString &groupText,
						QToolBar *parent,
						const char *name = 0);
	virtual ~ColorChooserButton();

protected:

	void mouseReleaseEvent (QMouseEvent *me);

signals:

	/**
	 * Emitted whenever the user clicked with the <em>left</em> mouse button on this
	 * <code>ColorChooserButton</code>.
	 */
	void chooseFGColor();

	/**
	 * Emitted whenever the user clicked with the <em>right</em> mouse button on this
	 * <code>ColorChooserButton</code>.
	 */
	void chooseBGColor();

};


#endif  // __COLORCHOOSERBUTTON_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
