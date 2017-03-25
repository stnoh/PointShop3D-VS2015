// Title:   SFLFileFormat.h
// Created: Thu Sep 25 15:46:07 2003
// Author:  Tim Weyrich
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

#ifndef __SFLFILEFORMAT_H_
#define __SFLFILEFORMAT_H_

#include <sfl.h>

#include <qglobal.h> // to obtain QT_VERSION
#if QT_VERSION >= 0x030000
# include <qptrlist.h>
# ifndef QList
#  define QList QPtrList
# endif
# ifndef QListIterator
#  define QListIterator QPtrListIterator
# endif
#else
# include <qlist.h>
#endif

#include "../../../Core/Interfaces/src/FileFormatPluginInterface.h"
#include "../../../Core/Scene/src/Object.h"
#include "../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../Core/MarkerManager/src/Marker3D.h"

/**
 * This class implements the universal <em>Surfel File Format</em> as implemented by Tim Weyrich,
 * which is extensible, version and platform independent.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class SFLFileFormat : public FileFormatPluginInterface {

public:
	
	FILEFORMATPLUGIN_API SFLFileFormat();
	FILEFORMATPLUGIN_API virtual ~SFLFileFormat();

	// *****************************
	// interface FileFormatInterface
	// *****************************

	FILEFORMATPLUGIN_API bool isReadable (const FileHeader fileHeader);
	FILEFORMATPLUGIN_API bool readFromFile (const QString filePath, const bool mergeWithCurrent = false);
	FILEFORMATPLUGIN_API bool writeToFile (const QString filePath, const bool onlyActiveObject = false);
	FILEFORMATPLUGIN_API QString getFileFilter() const;
	FILEFORMATPLUGIN_API QString getFileSuffix() const;
	FILEFORMATPLUGIN_API void customize (QWidget *parent);

private:

	bool                                storeMarkers;
	int                                 normalCompression;
	uint                                currentObject,
		                                nofObjects;

	void readObjectFromStream (sfl::InStream *stream, Object *object);
	void writeObjectToStream (Object *object, sfl::OutStream *out);

	void readSurfelCollectionFromStream (sfl::InStream *in, SurfelCollection *surfelCollection);
	void writeSurfelCollectionToStream (SurfelCollection *surfelCollection, const QString name,
		                                const MyDataTypes::TransformationMatrix16f scaleTranslationMatrix,
									    const MyDataTypes::TransformationMatrix16f rotationMatrix,
		                                sfl::OutStream *out);

	// returns the marker index if the 'surfelIndex' is contained within the array 'markerIndices', which has
	// 'nofMarkerIndices' elements; -1 else
	int isSurfelMarked (const uint surfelIndex, const uint32 *markerIndices, const uint nofMarkerIndices) const;

	// Conversion of SFL file format property descriptors to the internal representation.
	static SurfelInterface::PropertyDescriptor sflPropertiesToPropertyDescriptor (const uint32 sflProps);
	
	//Conversion of a PropertyDescriptor to the property representation of the SFL file format.
	static uint32 propertyDescriptorToSFLProperties (const SurfelInterface::PropertyDescriptor propDescr);
	
};

// ****************
// access functions
// ****************

// extern "C" is needed so those functions can be accessed
// with GetProcAddress() (WIN32) and dlsym() (UNIX) respective
extern "C" {
	FILEFORMATPLUGIN_API FileFormatPluginInterface *createFileFormatPlugin();
}

#endif  // __SFLFILEFORMAT_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
