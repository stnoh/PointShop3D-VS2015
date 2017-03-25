// Title:   FileFormatPluginLoader.cpp
// Created: Thu Sep 25 14:18:00 2003
// Authors: Oliver Knoll, Mark Pauly, Matthias Zwicker
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

#include <qmap.h>
#include <qstring.h>

// ********************
// OS specific includes
// ********************

#if defined _WIN32
  #include <windows.h>
#elif defined _OS_LINUX_
  #include <dlfcn.h>
#else
  #include <dlfcn.h>
#endif

#include "../../Interfaces/src/FileFormatPluginInterface.h"
#include "FileFormatPluginLoader.h"

// ********************
// OS specific typedefs
// ********************

#if defined _WIN32
	typedef HINSTANCE LibraryHandle;
	typedef FARPROC   FunctionAddress;
#elif defined _OS_LINUX_
	typedef void *LibraryHandle;
	typedef void *FunctionAddress;
#else
  typedef void *LibraryHandle;
	typedef void *FunctionAddress;
#endif

// ********
// typedefs
// ********

typedef QMap<QString,LibraryHandle> DllRegistry;

// **************
// global private
// **************

// stores the dll handles for each loaded dll
static DllRegistry dllRegistry;

FileFormatPluginLoader::FileFormatPluginAccess FileFormatPluginLoader::loadFileFormatPlugin (const char *fileFormatPluginPath) {

	LibraryHandle          libraryHandle;
	FunctionAddress        functionAddress;
	FileFormatPluginAccess fileFormatPluginAccess;

	// OS specific implementation for opening shared libraries

	// *****
	// WIN32
	// *****

#if defined _WIN32

	libraryHandle = LoadLibrary (fileFormatPluginPath);
	if (libraryHandle == NULL) {
		fileFormatPluginAccess.isValid = false;
		return fileFormatPluginAccess;
	}

	// check for CreateFileFormatPlugin function
	functionAddress = GetProcAddress (libraryHandle, "createFileFormatPlugin");
	if (functionAddress != NULL) {
		fileFormatPluginAccess.createFileFormatPluginFunction = (CreateFileFormatPluginFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		fileFormatPluginAccess.isValid = false;
		return fileFormatPluginAccess;
	}

	// *****
	// LINUX
	// *****
		
#elif defined _OS_LINUX_

	libraryHandle = dlopen (fileFormatPluginPath, RTLD_LAZY);
	if (libraryHandle == 0) {
		fileFormatPluginAccess.isValid = false;
		return fileFormatPluginAccess;
	}

	// check for CreateFileFormatPlugin function
	functionAddress = dlsym (libraryHandle, "createFileFormatPlugin");
	if (dlerror() == 0) {
		fileFormatPluginAccess.createFileFormatPluginFunction = (CreateFileFormatPluginFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		fileFormatPluginAccess.isValid = false;
		return fileFormatPluginAccess;
	}
	
	// ************
	// GENERIC UNIX
	// ************
	
#else

	libraryHandle = dlopen (fileFormatPluginPath, RTLD_LAZY);
	if (libraryHandle == 0) {
		fileFormatPluginAccess.isValid = false;
		return fileFormatPluginAccess;
	}

	// check for CreateFileFormatPlugin function
	functionAddress = dlsym (libraryHandle, "createFileFormatPlugin");
	if (dlerror() == 0) {
		fileFormatPluginAccess.createFileFormatPluginFunction = (CreateFileFormatPluginFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		fileFormatPluginAccess.isValid = false;
		return fileFormatPluginAccess;
	}

#endif  // OS specific implementation

	// internally store the name of the file format plugin and the corresponding handle
	dllRegistry.insert (fileFormatPluginPath, libraryHandle);

	fileFormatPluginAccess.isValid = true;
	return fileFormatPluginAccess;

}

bool FileFormatPluginLoader::unloadFileFormatPlugin (const char *fileFormatPluginPath) {

	DllRegistry::Iterator it;        

	if (fileFormatPluginPath == 0) {
		return false;
	}

	it = dllRegistry.find (fileFormatPluginPath);
	if (it == dllRegistry.end()) {		
		return false;
	}
	
	// OS specific implementation for closing shared libraries

#if defined _WIN32
	FreeLibrary (it.data());
#elif defined _OS_LINUX_
	dlclose (it.data());
#else
	dlclose (it.data());
#endif

	dllRegistry.remove (it);

	return true;

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
