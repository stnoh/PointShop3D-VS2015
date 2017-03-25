// Title:   RendererLoader.cpp
// Created: Thu Sep 25 14:12:08 2003
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

#include "../../Interfaces/src/RendererInterface.h"
#include "RendererLoader.h"

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

RendererLoader::RendererAccess RendererLoader::loadRendererPlugin (const char *rendererPluginPath) {

	LibraryHandle   libraryHandle;
	FunctionAddress functionAddress;
	RendererAccess  rendererAccess;

	// OS specific implementation for opening shared libraries

	// *****
	// WIN32
	// *****

#if defined _WIN32

	libraryHandle = LoadLibrary (rendererPluginPath);
	if (libraryHandle == NULL) {
		rendererAccess.isValid = false;
		return rendererAccess;
	}

	// check for CreateRenderer function
	functionAddress = GetProcAddress (libraryHandle, "createRenderer");
	if (functionAddress != NULL) {
		rendererAccess.createRendererFunction = (CreateRendererFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		rendererAccess.isValid = false;
		return rendererAccess;
	}


	// check for GetRendererType function
	functionAddress = GetProcAddress (libraryHandle, "getRendererType");
	if (functionAddress != NULL) {
		rendererAccess.getRendererTypeFunction = (GetRendererTypeFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		rendererAccess.isValid = false;
		return rendererAccess;
	}

	// *****
	// LINUX
	// *****
		
#elif defined _OS_LINUX_

	libraryHandle = dlopen (rendererPluginPath, RTLD_LAZY);
	if (libraryHandle == 0) {
	  qDebug("dlopen: %s", dlerror());
		rendererAccess.isValid = false;
		return rendererAccess;
	}

	// check for CreateRenderer function
	functionAddress = dlsym (libraryHandle, "createRenderer");
	if (dlerror() == 0) {
		rendererAccess.createRendererFunction = (CreateRendererFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		qDebug("Could resolve symbol `%s'\n", "createRenderer");
		rendererAccess.isValid = false;
		return rendererAccess;
	}

	// check for GetRendererType function
	functionAddress = dlsym (libraryHandle, "getRendererType");
	if (dlerror() == 0) {
		rendererAccess.getRendererTypeFunction = (GetRendererTypeFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		qDebug("Could resolve symbol `%s'\n", "getRendererType");
		rendererAccess.isValid = false;
		return rendererAccess;
	}
	
	// ************
	// GENERIC UNIX
	// ************
	
#else

	libraryHandle = dlopen (rendererPluginPath, RTLD_LAZY);
	if (libraryHandle == 0) {
		rendererAccess.isValid = false;
		return rendererAccess;
	}

	// check for CreatePlugin function
	functionAddress = dlsym (libraryHandle, "createRenderer");
	if (dlerror() == 0) {
		rendererAccess.createRendererFunction = (CreateRendererFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		rendererAccess.isValid = false;
		return rendererAccess;
	}

	// check for GetRendererType function
	functionAddress = dlsym (libraryHandle, "getRendererType");
	if (dlerror() == 0) {
		rendererAccess.getRendererTypeFunction = (GetRendererTypeFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		rendererAccess.isValid = false;
		return rendererAccess;
	}

#endif  // OS specific implementation

	// internally store the name of the renderer plugin and the corresponding handle
	dllRegistry.insert (rendererPluginPath, libraryHandle);

	rendererAccess.isValid = true;
	return rendererAccess;

}

bool RendererLoader::unloadRendererPlugin (const char *rendererPluginPath) {

	DllRegistry::Iterator it;
        

	if (rendererPluginPath == 0) {
		return false;
	}

	it = dllRegistry.find (rendererPluginPath);
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

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
