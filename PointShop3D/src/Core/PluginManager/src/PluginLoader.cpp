// Title:   PluginLoader.cpp
// Created: Thu Sep 25 14:13:01 2003
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

#include "../../Interfaces/src/PluginInterface.h"

#include "PluginLoader.h"

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

PluginLoader::PluginAccess PluginLoader::loadPlugin (const char *pluginPath) {

	LibraryHandle   libraryHandle;
	FunctionAddress functionAddress;
	PluginAccess    pluginAccess;

	// OS specific implementation for opening shared libraries

	// *****
	// WIN32
	// *****

#if defined _WIN32

	libraryHandle = LoadLibrary (pluginPath);
	if (libraryHandle == NULL) {
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for CreatePlugin function
	functionAddress = GetProcAddress (libraryHandle, "createPlugin");
	if (functionAddress != NULL) {
		pluginAccess.createPluginFunction = (CreatePluginFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for GetPluginName function
	functionAddress = GetProcAddress (libraryHandle, "getPluginName");
	if (functionAddress != NULL) {
		pluginAccess.getPluginNameFunction = (GetPluginNameFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for GetPluginDescription function
	functionAddress = GetProcAddress (libraryHandle, "getPluginDescription");
	if (functionAddress != NULL) {
		pluginAccess.getPluginDescriptionFunction = (GetPluginDescriptionFunction)functionAddress;
	}
	else {
		FreeLibrary (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// *****
	// LINUX
	// *****
		
#elif defined _OS_LINUX_

	libraryHandle = dlopen (pluginPath, RTLD_LAZY);
	if (libraryHandle == 0) {
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for CreatePlugin function
	functionAddress = dlsym (libraryHandle, "createPlugin");
	if (dlerror() == 0) {
		pluginAccess.createPluginFunction = (CreatePluginFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for GetPluginName function
	functionAddress = dlsym (libraryHandle, "getPluginName");
	if (dlerror() == 0) {
		pluginAccess.getPluginNameFunction = (GetPluginNameFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for GetPluginDescription function
	functionAddress = dlsym (libraryHandle, "getPluginDescription");
	if (dlerror() == 0) {
		pluginAccess.getPluginDescriptionFunction = (GetPluginDescriptionFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}
	
	// ************
	// GENERIC UNIX
	// ************
	
#else

	libraryHandle = dlopen (pluginPath, RTLD_LAZY);
	if (libraryHandle == 0) {
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for CreatePlugin function
	functionAddress = dlsym (libraryHandle, "createPlugin");
	if (dlerror() == 0) {
		pluginAccess.createPluginFunction = (CreatePluginFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for GetPluginName function
	functionAddress = dlsym (libraryHandle, "getPluginName");
	if (dlerror() == 0) {
		pluginAccess.getPluginNameFunction = (GetPluginNameFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

	// check for GetPluginDescription function
	functionAddress = dlsym (libraryHandle, "getPluginDescription");
	if (dlerror() == 0) {
		pluginAccess.getPluginDescriptionFunction = (GetPluginDescriptionFunction)functionAddress;
	}
	else {
		dlclose (libraryHandle);
		pluginAccess.isValid = false;
		return pluginAccess;
	}

#endif  // OS specific implementation

	// internally store the name of the plugin and the corresponding handle
	dllRegistry.insert (pluginPath, libraryHandle);

	pluginAccess.isValid = true;
	return pluginAccess;

}

bool PluginLoader::unloadPlugin (const char *pluginPath) {

	DllRegistry::Iterator it;
        

	if (pluginPath == 0) {
		return false;
	}

	it = dllRegistry.find (pluginPath);
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
