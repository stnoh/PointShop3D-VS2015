# Microsoft Developer Studio Project File - Name="SurfelGPRenderer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SurfelGPRenderer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SurfelGPRenderer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SurfelGPRenderer.mak" CFG="SurfelGPRenderer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SurfelGPRenderer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SurfelGPRenderer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF "$(CFG)" == "SurfelGPRenderer - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SURFELGPRENDERER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(QTDIR)\include" /I "..\..\Core\DataStructures\src\\" /I "src\SurfelPipeline_SurfSplat" /I "..\..\Core\Interfaces\src" /D "RENDERER_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(QTDIR)\lib\$(QTLIB) $(QTDIR)\lib\qtmain.lib opengl32.lib glu32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=-- Installing Renderer SurfelGPRenderer --
PostBuild_Cmds=IF NOT EXIST ..\..\..\bin mkdir ..\..\..\bin	IF NOT EXIST ..\..\..\bin\win32 mkdir ..\..\..\bin\win32	IF NOT EXIST ..\..\..\bin\win32\Release mkdir ..\..\..\bin\win32\Release	IF NOT EXIST ..\..\..\bin\win32\Release\Renderers mkdir ..\..\..\bin\win32\Release\Renderers	IF NOT EXIST ..\..\..\bin\win32\Release\Renderers\default mkdir ..\..\..\bin\win32\Release\Renderers\default	copy bin\win32\Release\SurfelGPRenderer.dll ..\..\..\bin\win32\Release\Renderers\default
# End Special Build Tool

!ELSEIF "$(CFG)" == "SurfelGPRenderer - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SURFELGPRENDERER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /D "RENDERER_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(QTDIR)\lib\$(QTLIB) $(QTDIR)\lib\qtmain.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../external/lib-Win32"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=-- Installing Renderer SurfelGPRenderer --
PostBuild_Cmds=IF NOT EXIST ..\..\..\bin mkdir ..\..\..\bin	IF NOT EXIST ..\..\..\bin\win32 mkdir ..\..\..\bin\win32	IF NOT EXIST ..\..\..\bin\win32\Debug mkdir ..\..\..\bin\win32\Debug	IF NOT EXIST ..\..\..\bin\win32\Debug\Renderers mkdir ..\..\..\bin\win32\Debug\Renderers	IF NOT EXIST ..\..\..\bin\win32\Debug\Renderers\default mkdir ..\..\..\bin\win32\Debug\Renderers\default	copy bin\win32\Debug\SurfelGPRenderer.dll ..\..\..\bin\win32\Debug\Renderers\default
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SurfelGPRenderer - Win32 Release"
# Name "SurfelGPRenderer - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "SurfelPipeline_SurfSplat Heade"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\Shader_SurfSplat.h
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\Srf.h
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\Warper.h
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\ZBuffer_SurfSplat.h
# End Source File
# End Group
# Begin Group "SurfelPipeline_SurfSplat Sourc"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\Shader_SurfSplat.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\Srf.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\Warper.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\Warper_IncludeCode.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_SurfSplat\ZBuffer_SurfSplat.cpp
# End Source File
# End Group
# Begin Group "OctreeRenderer_SurfSplat Heade"

# PROP Default_Filter "*.h"
# Begin Group "Config Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\src\OctreeRenderer_SurfSplat\ConfigHeaders\BasePlane.h
# End Source File
# Begin Source File

SOURCE=.\src\OctreeRenderer_SurfSplat\ConfigHeaders\Shader.h
# End Source File
# Begin Source File

SOURCE=.\src\OctreeRenderer_SurfSplat\ConfigHeaders\ZBuffer.h
# End Source File
# End Group
# End Group
# Begin Group "SurfelPipeline_General Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\src\SurfelPipeline_General\LightSource.h
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_General\LSrList.h
# End Source File
# End Group
# Begin Group "SurfelPipeline_General Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\src\SurfelPipeline_General\LightSource.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SurfelPipeline_General\LSrList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\SurfelGPFrameBuffer.h
# End Source File
# Begin Source File

SOURCE=.\src\SurfelGPRenderer.h

!IF "$(CFG)" == "SurfelGPRenderer - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src
InputPath=.\src\SurfelGPRenderer.h
InputName=SurfelGPRenderer

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "SurfelGPRenderer - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src
InputPath=.\src\SurfelGPRenderer.h
InputName=SurfelGPRenderer

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\SurfelGPRendererConfiguration.h

!IF "$(CFG)" == "SurfelGPRenderer - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src
InputPath=.\src\SurfelGPRendererConfiguration.h
InputName=SurfelGPRendererConfiguration

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "SurfelGPRenderer - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src
InputPath=.\src\SurfelGPRendererConfiguration.h
InputName=SurfelGPRendererConfiguration

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\SurfelGPRendererWidget.h

!IF "$(CFG)" == "SurfelGPRenderer - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src
InputPath=.\src\SurfelGPRendererWidget.h
InputName=SurfelGPRendererWidget

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF "$(CFG)" == "SurfelGPRenderer - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\src
InputPath=.\src\SurfelGPRendererWidget.h
InputName=SurfelGPRendererWidget

""$(InputDir)\moc_$(InputName).cpp"" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\SurfelGPFrameBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SurfelGPRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SurfelGPRendererConfiguration.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SurfelGPRendererWidget.cpp
# End Source File
# End Group
# Begin Group "Auto-Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\moc_SurfelGPRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\moc_SurfelGPRendererConfiguration.cpp
# End Source File
# Begin Source File

SOURCE=.\UI\moc_SurfelGPRendererConfigurationWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\moc_SurfelGPRendererWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\UI\SurfelGPRendererConfigurationWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\UI\SurfelGPRendererConfigurationWidget.h
# End Source File
# End Group
# Begin Group "User Interfaces (UI)"

# PROP Default_Filter "*.ui"
# Begin Source File

SOURCE=.\UI\SurfelGPRendererConfigurationWidget.ui

!IF "$(CFG)" == "SurfelGPRenderer - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\UI
InputPath=.\UI\SurfelGPRendererConfigurationWidget.ui
InputName=SurfelGPRendererConfigurationWidget

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

!ELSEIF "$(CFG)" == "SurfelGPRenderer - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\UI
InputPath=.\UI\SurfelGPRendererConfigurationWidget.ui
InputName=SurfelGPRendererConfigurationWidget

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
# Begin Group "Linux Project Files"

# PROP Default_Filter "*.pro"
# Begin Source File

SOURCE=.\SurfelGPRenderer.pro
# End Source File
# End Group
# End Target
# End Project
