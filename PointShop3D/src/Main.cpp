// Title:   Main.cpp
// Created: Thu Sep 25 14:33:48 2003
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

#include <qapplication.h>
#include <qstring.h>
#include <qsize.h>
#include <qcolor.h>

#include "UserInterface/PointShopWindow.h"

// The following definition is a hack, needed when compiling with g++
// 2.96 and linking against a Qt Lib, that was /not/ compiled with g++
// 2.96.
//
#if (defined(__GNUC__) && __GNUC__ == 2 && __GNUC_MINOR__ == 96)
#  ifndef QT_H
#    include "qglobal.h"
#  endif // QT_H
QSizePolicy::QSizePolicy( SizeType hor, SizeType ver, bool hfw )
	: data( hor | (ver<<HSize) | (hfw ? (1<<2*HSize) : 0) ) {}
#endif

int main (int argc, char **argv) {

	QApplication pointShop (argc, argv);
	int          i;

    PointShopWindow *pointShopWindow = new PointShopWindow();

	// make window visible
    pointShopWindow->setCaption ("PointShop");

    pointShopWindow->show();
	for (i = 1; i < argc; i++) { 
		pointShopWindow->addDroppedScene (argv[i]);
	}	
    
	// connect signals: last window closed -> quit application
	pointShop.connect (&pointShop, SIGNAL (lastWindowClosed()),
	                   &pointShop, SLOT (quit()));
    return pointShop.exec();

}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
