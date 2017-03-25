// Title:   ObjectViewerImpl.h
// Created: Thu Sep 25 14:42:10 2003
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

#ifndef __OBJECTVIEWERIMPL_H_
#define __OBJECTVIEWERIMPL_H_

#include <qtoolbutton.h>

#include "../UI/ObjectViewer.h"

/**
 * Object viewer dialog class.
 *
 * @author Mark Pauly
 * @version 1.2
 */
class ObjectViewerImpl : public ObjectViewer {

	Q_OBJECT

public:

	static ObjectViewerImpl *getInstance ();
	static void destroyInstance();

public slots:


private slots:

	void hideShow ();
	void activateDeactivate();

	void handleObjectListChanged ();
	

protected slots:

	void showEvent (QShowEvent *se);

private:

	
	// singleton pattern
	ObjectViewerImpl (QWidget* parent = 0, const char* name = 0, bool modal = false, WFlags flags = 0);




	




protected:  // To suppress warnings...
    virtual ~ObjectViewerImpl();

};

#endif  // __OBJECTVIEWERIMPL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
