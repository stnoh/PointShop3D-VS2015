// Title:   ResourceToolInterface.h
// Created: Thu Sep 25 14:29:03 2003
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

#ifndef __RESOURCETOOLINTERFACE_H_
#define __RESOURCETOOLINTERFACE_H_

#include <qobject.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qtoolbar.h>
#include <qcursor.h>
#include <qwidget.h>
#include <qpushbutton.h>

// ***********
// definitions
// ***********

#if defined _WIN32

	// The following ifdef block is the standard way of creating macros which make exporting
	// from a DLL simpler. All files within this DLL are compiled with the RESOURCETOOL_EXPORTS
	// symbol defined on the command line. this symbol should not be defined on any project
	// that uses this DLL. This way any other project whose source files include this file see
	// RESOURCETOOL_API  functions as being imported from a DLL, wheras this DLL sees symbols
	// defined with this macro as being exported.
	#ifdef RESOURCETOOL_EXPORTS
		#define RESOURCETOOL_API __declspec(dllexport)
	#else
		#define RESOURCETOOL_API __declspec(dllimport)
	#endif

#else

	// for all other platforms RESOURCETOOL_API is defined to be "nothing"
	#define RESOURCETOOL_API
	
#endif

/**
 * This interface defines the API for resource tools, that is, modules which appear
 * in the resource toolbar. In contrast to the tools which appear in the
 * standard toolbar resource tools do not receive mouse- or key-events and mustn't
 * manipulate the image buffer. The only purpose for resource tools is to provide
 * resources such as brushes or textures for other tools.
 * 
 * Note that apart from this <code>ToolInterface</code> the tool modules
 * must also implement some special C-style functions, if they are to be
 * dynamically loaded:
 * <ul>
 *   <li><code>RESOURCETOOL_API ResourceToolInterface *createTool();</code></li>
 *   <li><code>RESOURCETOOL_API void destroyTool (ResourceToolInterface *tool);</code></li>
 *   <li><code>RESOURCETOOL_API const char *getResourceToolName();</code></li>
 * </ul>
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see ToolInterface
 */
class RESOURCETOOL_API ResourceToolInterface : public QObject {

public:

	/**
	 * Creates a tool button and adds it to the <code>toolBar</code>. The <code>QToolButton</code>
	 * may also activate <code>QPopupMenu</code>s, for example.
	 *
	 * @param toolBar
	 *        a pointer to the <code>QToolBar</code> to which this tool has to be added
	 * @return   a pointer to the <code>QToolButton</code> which was created and added
	 *           to the <code>toolBar</code>
	 */
	virtual QToolButton* addButtonToToolBar (QToolBar *toolBar) = 0;


	/**
	 * If this <code>ResourceToolInterface</code> provides a method where resources can be chosen
	 * from, for example bitmaps, this <code>QPopupMenu</code> is added to the <code>QPushButton</code>.
	 *
	 * @param pushButton
	 *        a pointer to the <code>QPushButton</code> to which the <code>QPopupMenu</code> is to be added
	 */
	virtual void addMenuToPushButton (QPushButton *pushButton) = 0;


};

#endif  // __RESOURCETOOLINTERFACE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
