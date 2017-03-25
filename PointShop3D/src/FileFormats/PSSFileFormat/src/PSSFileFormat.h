// Title:   PSSFileFormat.h
// Created: Thu Sep 25 14:47:14 2003
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

#ifndef __PSSFILEFORMAT_H_
#define __PSSFILEFORMAT_H_

#include <qwidget.h>

#include "../../../Core/Interfaces/src/FileFormatPluginInterface.h"
#include "../../../Core/Scene/src/Object.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"

/**
 * This class implements the PointShop3D file format (*.pss) which operates on <code>QDataStream</code>s
 * and is therefore platform and endianess independent, but doesn't do any data compression.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class PSSFileFormat : public FileFormatPluginInterface {

public:
	
	FILEFORMATPLUGIN_API PSSFileFormat();
	FILEFORMATPLUGIN_API virtual ~PSSFileFormat();

	// *****************************
	// interface FileFormatInterface
	// *****************************

	FILEFORMATPLUGIN_API bool isReadable (const FileHeader fileHeader);
	FILEFORMATPLUGIN_API bool hasWriteProtection();
	FILEFORMATPLUGIN_API bool readFromFile (const QString filePath, const bool mergeWithCurrent = false);
	FILEFORMATPLUGIN_API bool writeToFile (const QString filePath, const bool onlyActiveObject = false);
	FILEFORMATPLUGIN_API QString getFileFilter() const;
	FILEFORMATPLUGIN_API QString getFileSuffix() const;
	FILEFORMATPLUGIN_API void customize (QWidget *parent);
	
private:

	SurfelInterface::PropertyDescriptor propertyDescriptor;
	uint                                currentObject,
		                                nofObjects;

	void readObjectFromStream (QDataStream &stream, Object *object);
	void writeObjectToStream (Object *object, QDataStream &stream);

	void readSurfelCollectionFromStream (QDataStream &stream, SurfelCollection *surfelCollection);
	void writeSurfelCollectionToStream (SurfelCollection *surfelCollection, QDataStream &stream);
	
};

// ****************
// access functions
// ****************

// extern "C" is needed so those functions can be accessed
// with GetProcAddress() (WIN32) and dlsym() (UNIX) respective
extern "C" {
	FILEFORMATPLUGIN_API FileFormatPluginInterface *createFileFormatPlugin();
}

#endif  // __PSSFILEFORMAT_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
