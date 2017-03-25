// Title:   ToolLoader.cpp
// Created: Thu Sep 25 14:10:26 2003
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

#include "../../Interfaces/src/ToolInterface.h"

#include "ToolLoader.h"

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

ToolLoader::ToolAccess ToolLoader::loadTool (const char *toolPath) {

	LibraryHandle   libraryHandle;
	FunctionAddress functionAddress;
	ToolAccess      toolAccess;

	// OS specific implementation for opening shared libraries

	// *****
	// WIN32
	// *****

#if defined _WIN32
	
	libraryHandle = LoadLibrary (toolPath);
	if (libraryHandle == NULL) {
		toolAccess.isValid = false;
		return toolAccess;
	}

	// check for createTool function
	functionAddress = GetProcAddress (libraryHandle, "createTool");
	if (functionAddress != NULL) {
		toolAccess.createToolFunction = (CreateToolFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		toolAccess.isValid = false;
		return toolAccess;
	}
	
	// check for getToolName function
	functionAddress = GetProcAddress (libraryHandle, "getToolName");
	if (functionAddress != NULL) {
		toolAccess.getToolNameFunction = (GetToolNameFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		toolAccess.isValid = false;
		return toolAccess;
	}

	// *****
	// LINUX
	// *****

#elif defined _OS_LINUX_
		
	libraryHandle = dlopen (toolPath, RTLD_LAZY);
	if (libraryHandle == 0) {
		toolAccess.isValid = false;
		return toolAccess;
	}

	// check for createTool function
	functionAddress = dlsym (libraryHandle, "createTool");
	if (dlerror() == 0) {
		toolAccess.createToolFunction = (CreateToolFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		toolAccess.isValid = false;
		return toolAccess;
	}

	// check for getToolName function
	functionAddress = dlsym (libraryHandle, "getToolName");
	if (dlerror() == 0) {
		toolAccess.getToolNameFunction = (GetToolNameFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		toolAccess.isValid = false;
		return toolAccess;
	}
	
	// ************
	// GENERIC UNIX
	// ************
	
#else

	libraryHandle = dlopen (toolPath, RTLD_LAZY);
	if (libraryHandle == 0) {
		toolAccess.isValid = false;
		return toolAccess;
	}

	// check for createTool function
	functionAddress = dlsym (libraryHandle, "createTool");
	if (dlerror() == 0) {
		toolAccess.createToolFunction = (CreateToolFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		toolAccess.isValid = false;
		return toolAccess;
	}

	// check for getToolName function
	functionAddress = dlsym (libraryHandle, "getToolName");
	if (dlerror() == 0) {
		toolAccess.getToolNameFunction = (GetToolNameFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		toolAccess.isValid = false;
		return toolAccess;
	}
	
#endif  // OS specific implementation

	// internally store the name of the tool and the corresponding handle
	dllRegistry.insert (toolPath, libraryHandle);

	toolAccess.isValid = true;
	return toolAccess;

}

bool ToolLoader::unloadTool (const char *toolPath) {

	DllRegistry::Iterator it;
	bool                  result;

	if (toolPath == 0) {
		return false;
	}

	it = dllRegistry.find (toolPath);
	if (it == dllRegistry.end()) {		
		return false;
	}

	// OS specific implementation for closing shared libraries

#if defined _WIN32
	result = FreeLibrary (it.data());
#elif defined _OS_LINUX_
    result = (dlclose (it.data()) == 0);
#else
	result = (dlclose (it.data()) == 0);
#endif

	dllRegistry.remove (it);
	return result;

}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
