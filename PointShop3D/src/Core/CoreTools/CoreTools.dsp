# Microsoft Developer Studio Project File - Name="CoreTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CoreTools - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoreTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoreTools.mak" CFG="CoreTools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoreTools - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CoreTools - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF "$(CFG)" == "CoreTools - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin\win32\Release"
# PROP Intermediate_Dir "bin\win32\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORETOOLS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(QTDIR)\include" /I "..\..\Interfaces\src" /I "..\..\Utilities\src" /D "CORETOOLS_EXPORTS" /D "TOOL_EXPORTS" /D "RESOURCETOOL_EXPORTS" /D "FUNCTION_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(QTDIR)\lib\$(QTLIB) $(QTDIR)\lib\qtmain.lib /nologo /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=--- Installing module CoreTools ---
PostBuild_Cmds=IF NOT EXIST ..\..\..\bin mkdir ..\..\..\bin	IF NOT EXIST ..\..\..\bin\win32 mkdir ..\..\..\bin\win32	IF NOT EXIST ..\..\..\bin\win32\Release mkdir ..\..\..\bin\win32\Release	copy bin\win32\Release\CoreTools.dll ..\..\..\bin\win32\Release
# End Special Build Tool

!ELSEIF "$(CFG)" == "CoreTools - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin\win32\Debug"
# PROP Intermediate_Dir "bin\win32\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORETOOLS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /D "CORETOOLS_EXPORTS" /D "TOOL_EXPORTS" /D "RESOURCETOOL_EXPORTS" /D "FUNCTION_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(QTDIR)\lib\$(QTLIB) $(QTDIR)\lib\qtmain.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=--- Installing module CoreTools ---
PostBuild_Cmds=IF NOT EXIST ..\..\bin mkdir ..\..\bin	IF NOT EXIST ..\..\bin\win32 mkdir ..\..\bin\win32	IF NOT EXIST ..\..\bin\win32\Debug mkdir ..\..\bin\win32\Debug	copy bin\win32\Debug\CoreTools.dll ..\..\bin\win32\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CoreTools - Win32 Release"
# Name "CoreTools - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\CoreTools.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\CoreTools.h
# End Source File
# Begin Source File

SOURCE=.\src\CoreToolsDLL.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "ColorPickerTool"

# PROP Default_Filter ""
# Begin Group "ColorPickerTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\ColorPickerTool\src\ColorPickerTool.h

!IF "$(CFG)" == "CoreTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ColorPickerTool\src
InputPath=.\ColorPickerTool\src\ColorPickerTool.h
InputName=ColorPickerTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ColorPickerTool\src
InputPath=.\ColorPickerTool\src\ColorPickerTool.h
InputName=ColorPickerTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "ColorPickerTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\ColorPickerTool\src\ColorPickerTool.cpp
# End Source File
# End Group
# End Group
# Begin Group "NavigationTool"

# PROP Default_Filter ""
# Begin Group "NavigationTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\NavigationTool\src\NavigationTool.h

!IF "$(CFG)" == "CoreTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\NavigationTool\src
InputPath=.\NavigationTool\src\NavigationTool.h
InputName=NavigationTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\NavigationTool\src
InputPath=.\NavigationTool\src\NavigationTool.h
InputName=NavigationTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "NavigationTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\NavigationTool\src\NavigationTool.cpp
# End Source File
# End Group
# End Group
# Begin Group "SelectionTool"

# PROP Default_Filter ""
# Begin Group "SelectionTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\SelectionTool\src\PixelSelection.h
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\src\ScanConversion.h
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\src\SelectionTool.h

!IF "$(CFG)" == "CoreTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\SelectionTool\src
InputPath=.\SelectionTool\src\SelectionTool.h
InputName=SelectionTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\SelectionTool\src
InputPath=.\SelectionTool\src\SelectionTool.h
InputName=SelectionTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "SelectionTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\SelectionTool\src\PixelSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\src\ScanConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\src\SelectionTool.cpp
# End Source File
# End Group
# End Group
# Begin Group "User Interfaces (UI)"

# PROP Default_Filter "*.ui"
# End Group
# Begin Group "Icons"

# PROP Default_Filter "*.xpm"
# Begin Source File

SOURCE=.\ColorPickerTool\Icons\ColorPickerIcon.xpm
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\Icons\EllipseIcon.xpm
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\Icons\LassoIcon.xpm
# End Source File
# Begin Source File

SOURCE=.\NavigationTool\Icons\NavigationIcon.xpm
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\Icons\PolygonIcon.xpm
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\Icons\RectangleIcon.xpm
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\Icons\SelectionIcon.xpm
# End Source File
# End Group
# Begin Group "Auto-Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorPickerTool\src\moc_ColorPickerTool.cpp
# End Source File
# Begin Source File

SOURCE=.\NavigationTool\src\moc_NavigationTool.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionTool\src\moc_SelectionTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\moc_ToolManager.cpp
# End Source File
# End Group
# Begin Group "Linux Project Files"

# PROP Default_Filter "*.pro"
# Begin Source File

SOURCE=.\CoreTools.pro
# End Source File
# End Group
# Begin Group "ToolManager"

# PROP Default_Filter ""
# Begin Group "ToolManager Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\ToolManager\src\ToolDirWalk.h
# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\ToolLoader.h
# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\ToolManager.h

!IF "$(CFG)" == "CoreTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ToolManager\src
InputPath=.\ToolManager\src\ToolManager.h
InputName=ToolManager

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ToolManager\src
InputPath=.\ToolManager\src\ToolManager.h
InputName=ToolManager

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\ToolRegistry.h
# End Source File
# End Group
# Begin Group "ToolManager Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\ToolManager\src\ToolDirWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\ToolLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\ToolManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\ToolRegistry.cpp
# End Source File
# End Group
# End Group
# End Target
# End Project