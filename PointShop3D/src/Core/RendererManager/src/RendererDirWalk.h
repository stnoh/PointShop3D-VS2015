// Title:   RendererDirWalk.h
// Created: Thu Sep 25 14:28:29 2003
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

#ifndef __RENDERERDIRWALK_H_
#define __RENDERERDIRWALK_H_

#include "../../../Utilities/DirWalk/src/DirWalk.h"

/**
 * The user data needed for the <code>RendererDirWalk</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see RendererDirWalk
 */
class RendererUserData : public UserDataInterface {

public:

	/**
	 * Creates this <code>RendererUserData</code>.
	 *
	 * @param availableRenderers
	 *        a <code>QStringList</code> containing the available renderer types
	 * @see RendererDirWalk
	 */
	RendererUserData (QStringList *availableRenderers);
	virtual ~RendererUserData();

	// ***************************
	// interface UserDataInterface
	// ***************************

	UserDataInterface *copy();
	void copy (UserDataInterface *source);

 private:
	
	// access for...
	friend class RendererDirWalk;

	QStringList *availableRenderers;

};

/**
 * This class implements the <code>DirWalk</code>. It iterates over the given
 * directory and looks for renderer plugins.
 * 
 * @author Oliver Knoll
 * @version 1.2
 */
class RendererDirWalk : public DirWalk {

public:

	/**
	 * Creates this <code>RendererDirWalk</code>, specifying the <code>rendererPluginDirectory</code>
	 * which is to be traversed.
	 *
	 * @param rendererPluginDirectory
	 *        a <code>QString</code> specifying the path to the renderer plugins directory
	 */
	RendererDirWalk (QString rendererPluginDirectory);
	virtual ~RendererDirWalk();

	// *****************
	// interface DirWalk
	// *****************

	DirWalk *createNewInstance (const QString directoryName);

protected:

	void applyFileMethod();
	void applyDirectoryMethod();

};


#endif  // __RENDERERDIRWALK_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
