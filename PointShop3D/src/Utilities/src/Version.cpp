// Title:   Version.cpp
// Created: Thu Sep 25 15:00:03 2003
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

#include <qdatetime.h>
#include <qstring.h>

#include "Version.h"

// *********
// constants
// *********

static const int MAJOR    = 2;
static const int MINOR    = 0;
static const int SUBMINOR = 0;
static const QString CODE = QString ("SunshineRai");
static const QDate DATE   = QDate (2003, 9, 15);

int Version::getMajor() {
	return MAJOR;
}

int Version::getMinor() {
	return MINOR;
}

int Version::getSubMinor() {
	return SUBMINOR;
}

QDate Version::getVersionDate() {
	return DATE;
}

QString Version::toString() {
	return QString ("%1.%2.%3").arg (MAJOR).arg(MINOR).arg(SUBMINOR);
}

QString Version::getCodeName() {
	return CODE;
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
