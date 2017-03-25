# Microsoft Developer Studio Project File - Name="CoreResourceTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CoreResourceTools - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoreResourceTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoreResourceTools.mak" CFG="CoreResourceTools - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoreResourceTools - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CoreResourceTools - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF "$(CFG)" == "CoreResourceTools - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORERESOURCETOOLS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(QTDIR)\include" /I "..\..\Interfaces\src" /I "..\..\Utilities\src" /D "CORERESOURCETOOLS_EXPORTS" /D "RESOURCETOOL_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /YX /FD /c
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
PostBuild_Desc=--- Installing module CoreResourceTools ---
PostBuild_Cmds=IF NOT EXIST ..\..\..\bin mkdir ..\..\..\bin 	IF NOT EXIST ..\..\..\bin\win32 mkdir ..\..\..\bin\win32 	IF NOT EXIST ..\..\..\bin\win32\Release mkdir ..\..\..\bin\win32\Release 	copy bin\win32\Release\CoreResourceTools.dll ..\..\..\bin\win32\Release 	IF NOT EXIST ..\..\..\bin\win32\Release\Textures mkdir ..\..\..\bin\win32\Release\Textures 	xcopy /S /Y ..\ResourceToolBar\BitmapResourceTool\Textures ..\..\..\bin\win32\Release\Textures\ 	IF NOT EXIST ..\..\..\bin\win32\Release\TextureAlphas mkdir ..\..\..\bin\win32\Release\TextureAlphas 	xcopy /S /Y ..\ResourceToolBar\BitmapResourceTool\TextureAlphas ..\..\..\bin\win32\Release\TextureAlphas\ 	IF NOT EXIST ..\..\..\bin\win32\Release\DisplacementMaps mkdir ..\..\..\bin\win32\Release\DisplacementMaps 	xcopy /S /Y ..\ResourceToolBar\BitmapResourceTool\DisplacementMaps \
 ..\..\..\bin\win32\Release\DisplacementMaps\ 	IF NOT EXIST ..\..\..\bin\win32\Release\DisplacementMapAlphas mkdir ..\..\..\bin\win32\Release\DisplacementMapAlphas 	xcopy /S /Y ..\ResourceToolBar\BitmapResourceTool\DisplacementMapAlphas ..\..\..\bin\win32\Release\DisplacementMapAlphas\ 
# End Special Build Tool

!ELSEIF "$(CFG)" == "CoreResourceTools - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORERESOURCETOOLS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /D "CORERESOURCETOOLS_EXPORTS" /D "RESOURCETOOL_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /YX /FD /GZ /c
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
PostBuild_Desc=--- Installing module CoreResourceTools ---
PostBuild_Cmds=IF NOT EXIST ..\..\bin mkdir ..\..\bin 	IF NOT EXIST ..\..\bin\win32 mkdir ..\..\..\bin\win32 	IF NOT EXIST ..\..\bin\win32\Debug mkdir ..\..\bin\win32\Debug 	copy bin\win32\Debug\CoreResourceTools.dll ..\..\bin\win32\Debug 	IF NOT EXIST ..\..\bin\win32\Debug\Textures mkdir ..\..\bin\win32\Debug\Textures 	xcopy /S /Y BitmapResourceTool\Textures ..\..\bin\win32\Debug\Textures\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CoreResourceTools - Win32 Release"
# Name "CoreResourceTools - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\CoreResourceTools.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\CoreResourceTools.h
# End Source File
# Begin Source File

SOURCE=.\src\CoreResourceToolsDLL.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "BitmapResourceManagerTool"

# PROP Default_Filter ""
# Begin Group "BitmapResourceManager Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\src\BitmapResourceManagerDialogImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\src\BitmapResourceManagerTool.cpp
# End Source File
# End Group
# Begin Group "BitmapResourceManager Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\src\BitmapResourceManagerDialogImpl.h

!IF "$(CFG)" == "CoreResourceTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\BitmapResourceManagerTool\src
InputPath=.\BitmapResourceManagerTool\src\BitmapResourceManagerDialogImpl.h
InputName=BitmapResourceManagerDialogImpl

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreResourceTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\BitmapResourceManagerTool\src
InputPath=.\BitmapResourceManagerTool\src\BitmapResourceManagerDialogImpl.h
InputName=BitmapResourceManagerDialogImpl

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\src\BitmapResourceManagerTool.h

!IF "$(CFG)" == "CoreResourceTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\BitmapResourceManagerTool\src
InputPath=.\BitmapResourceManagerTool\src\BitmapResourceManagerTool.h
InputName=BitmapResourceManagerTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreResourceTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\BitmapResourceManagerTool\src
InputPath=.\BitmapResourceManagerTool\src\BitmapResourceManagerTool.h
InputName=BitmapResourceManagerTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "BitmapResourceTool"

# PROP Default_Filter ""
# Begin Group "BitmapResourceTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\BitmapResourceTool\src\BitmapDirWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceTool\src\BitmapResourceTool.cpp
# End Source File
# End Group
# Begin Group "BitmapResourceTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\BitmapResourceTool\src\BitmapDirWalk.h
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceTool\src\BitmapResourceTool.h

!IF "$(CFG)" == "CoreResourceTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\BitmapResourceTool\src
InputPath=.\BitmapResourceTool\src\BitmapResourceTool.h
InputName=BitmapResourceTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreResourceTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\BitmapResourceTool\src
InputPath=.\BitmapResourceTool\src\BitmapResourceTool.h
InputName=BitmapResourceTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "ColorChooserTool"

# PROP Default_Filter ""
# Begin Group "ColorChooserTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\ColorChooserTool\src\ColorChooserButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorChooserTool\src\ColorChooserTool.cpp
# End Source File
# End Group
# Begin Group "ColorChooserTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\ColorChooserTool\src\ColorChooserButton.h

!IF "$(CFG)" == "CoreResourceTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ColorChooserTool\src
InputPath=.\ColorChooserTool\src\ColorChooserButton.h
InputName=ColorChooserButton

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreResourceTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ColorChooserTool\src
InputPath=.\ColorChooserTool\src\ColorChooserButton.h
InputName=ColorChooserButton

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ColorChooserTool\src\ColorChooserTool.h

!IF "$(CFG)" == "CoreResourceTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ColorChooserTool\src
InputPath=.\ColorChooserTool\src\ColorChooserTool.h
InputName=ColorChooserTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "CoreResourceTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ColorChooserTool\src
InputPath=.\ColorChooserTool\src\ColorChooserTool.h
InputName=ColorChooserTool

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "User Interfaces (UI)"

# PROP Default_Filter "*.ui"
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\UI\BitmapResourceManagerDialog.ui

!IF "$(CFG)" == "CoreResourceTools - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\BitmapResourceManagerTool\UI
InputPath=.\BitmapResourceManagerTool\UI\BitmapResourceManagerDialog.ui
InputName=BitmapResourceManagerDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

""$(InputDir)\$(InputName).h"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
 $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
 $(BuildCmds)

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
 $(BuildCmds)
# End Custom Build

!ELSEIF "$(CFG)" == "CoreResourceTools - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\BitmapResourceManagerTool\UI
InputPath=.\BitmapResourceManagerTool\UI\BitmapResourceManagerDialog.ui
InputName=BitmapResourceManagerDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

""$(InputDir)\$(InputName).h"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
 $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
 $(BuildCmds)

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
 $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Icons"

# PROP Default_Filter "*.xpm"
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\Icons\BitmapResourceManagerIcon.xpm
# End Source File
# End Group
# Begin Group "Auto-Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\UI\BitmapResourceManagerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\UI\BitmapResourceManagerDialog.h
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\UI\moc_BitmapResourceManagerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\src\moc_BitmapResourceManagerDialogImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceManagerTool\src\moc_BitmapResourceManagerTool.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapResourceTool\src\moc_BitmapResourceTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorChooserTool\src\moc_ColorChooserButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorChooserTool\src\moc_ColorChooserTool.cpp
# End Source File
# End Group
# Begin Group "Linux Project Files"

# PROP Default_Filter "*.pro"
# Begin Source File

SOURCE=.\CoreResourceTools.pro
# End Source File
# End Group
# End Target
# End Project
