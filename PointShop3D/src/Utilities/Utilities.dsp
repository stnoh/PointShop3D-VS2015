# Microsoft Developer Studio Project File - Name="Utilities" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Utilities - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Utilities.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Utilities.mak" CFG="Utilities - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Utilities - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Utilities - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF "$(CFG)" == "Utilities - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UTILITIES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(QTDIR)\include" /I "..\Interfaces\src" /I "src" /I ".\sparselib_1_5d\include" /I ".\sparselib_1_5d\mv\include" /D "UTILITIES_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /FR /YX /FD /c
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
PostBuild_Desc=--- Installing module Utilities ---
PostBuild_Cmds=IF NOT EXIST ..\..\bin mkdir ..\..\bin 	IF NOT EXIST ..\..\bin\win32 mkdir ..\..\bin\win32 	IF NOT EXIST ..\..\bin\win32\Release mkdir ..\..\bin\win32\Release 	copy bin\win32\Release\Utilities.dll ..\..\bin\win32\Release 	IF NOT EXIST ..\..\bin\win32\Release\Resources mkdir ..\..\bin\win32\Release\Resources\ 	xcopy /S /Y UserMessages\Splash ..\..\bin\win32\Release\Resources\ 
# End Special Build Tool

!ELSEIF "$(CFG)" == "Utilities - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UTILITIES_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /I ".\sparselib_1_5d\include" /I ".\sparselib_1_5d\mv\include" /D "UTILITIES_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /FR /YX /FD /GZ /c
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
PostBuild_Desc=--- Installing module Utilities ---
PostBuild_Cmds=IF NOT EXIST ..\..\bin mkdir ..\..\bin 	IF NOT EXIST ..\..\bin\win32 mkdir ..\..\bin\win32 	IF NOT EXIST ..\..\bin\win32\Debug mkdir ..\..\bin\win32\Debug 	copy bin\win32\Debug\Utilities.dll ..\..\bin\win32\Debug 	IF NOT EXIST ..\..\bin\win32\Debug\Resources\ mkdir ..\..\bin\win32\Debug\Resources 	xcopy /S /Y UserMessages\Splash ..\..\bin\win32\Debug\Resources\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Utilities - Win32 Release"
# Name "Utilities - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ColorConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FileTools.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ImageOps.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MyMathTools.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StringTools.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\TrackBall.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Version.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\ColorConversion.h
# End Source File
# Begin Source File

SOURCE=.\src\Common.h
# End Source File
# Begin Source File

SOURCE=.\src\FileTools.h
# End Source File
# Begin Source File

SOURCE=.\src\ImageOps.h
# End Source File
# Begin Source File

SOURCE=.\src\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\src\MyMathTools.h
# End Source File
# Begin Source File

SOURCE=.\src\StringTools.h
# End Source File
# Begin Source File

SOURCE=.\Src\TrackBall.h
# End Source File
# Begin Source File

SOURCE=.\src\UtilitiesDLL.h
# End Source File
# Begin Source File

SOURCE=.\src\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\UserMessages\Splash\Splash.png
# End Source File
# Begin Source File

SOURCE=.\UserMessages\resource\Splash.psd
# End Source File
# End Group
# Begin Group "DirWalk"

# PROP Default_Filter ""
# Begin Group "DirWalk Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\DirWalk\src\DirWalk.h
# End Source File
# End Group
# Begin Group "DirWalk Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\DirWalk\src\DirWalk.cpp
# End Source File
# End Group
# End Group
# Begin Group "Auto-Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UserMessages\src\moc_StatusBar.cpp
# End Source File
# End Group
# Begin Group "Linux Project Files"

# PROP Default_Filter "*.pro"
# Begin Source File

SOURCE=.\Utilities.pro
# End Source File
# End Group
# Begin Group "ANN"

# PROP Default_Filter ""
# Begin Group "ANN Source Files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\ANN\ANN.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\bd_pr_search.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\bd_search.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\bd_tree.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\brute.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_pr_search.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_search.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_split.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_tree.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_util.cpp
# End Source File
# Begin Source File

SOURCE=.\ANN\perf.cpp
# End Source File
# End Group
# Begin Group "ANN Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\ANN\ANN.h
# End Source File
# Begin Source File

SOURCE=.\ANN\ANNperf.h
# End Source File
# Begin Source File

SOURCE=.\ANN\ANNx.h
# End Source File
# Begin Source File

SOURCE=.\ANN\bd_tree.h
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_pr_search.h
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_search.h
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_split.h
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_tree.h
# End Source File
# Begin Source File

SOURCE=.\ANN\kd_util.h
# End Source File
# Begin Source File

SOURCE=.\ANN\pr_queue.h
# End Source File
# Begin Source File

SOURCE=.\ANN\pr_queue_k.h
# End Source File
# Begin Source File

SOURCE=.\ANN\win32_values.h
# End Source File
# End Group
# End Group
# Begin Group "User Messages"

# PROP Default_Filter ""
# Begin Group "User Messages Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\UserMessages\src\SplashScreen.h
# End Source File
# Begin Source File

SOURCE=.\UserMessages\src\StatusBar.h

!IF "$(CFG)" == "Utilities - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\UserMessages\src
InputPath=.\UserMessages\src\StatusBar.h
InputName=StatusBar

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "Utilities - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\UserMessages\src
InputPath=.\UserMessages\src\StatusBar.h
InputName=StatusBar

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "UserMessages Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\UserMessages\src\SplashScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMessages\src\StatusBar.cpp
# End Source File
# End Group
# End Group
# Begin Group "MagicSoft"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MagicSoft\MagicFMLibType.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcAppr3DPlaneFit.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcAppr3DPlaneFit.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcBox2.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcBox2.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcBox2.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcCont2DMinBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcCont2DMinBox.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcConvexHull2D.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcConvexHull2D.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcConvexHull2D.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcDelaunay2D.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcDelaunay2D.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcDelaunay2D.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcEigen.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcEigen.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcEigen.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcLinearSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcLinearSystem.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcLinearSystem.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMath.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMath.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMath.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMatrix2.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMatrix2.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMatrix2.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMatrix3.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMatrix3.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcMatrix3.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcQuaternion.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcQuaternion.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcRTLib.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcVector2.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcVector2.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcVector2.inl
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcVector3.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcVector3.h
# End Source File
# Begin Source File

SOURCE=.\MagicSoft\MgcVector3.inl
# End Source File
# End Group
# End Target
# End Project
