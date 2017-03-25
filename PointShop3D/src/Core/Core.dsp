# Microsoft Developer Studio Project File - Name="Core" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Core - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Core.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Core.mak" CFG="Core - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Core - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Core - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Core - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(QTDIR)\include" /I "DataStructures\src\\" /I "Interfaces\src\\" /D "CORE_EXPORTS" /D "TOOL_EXPORTS" /D "RESOURCETOOL_EXPORTS" /D "PLUGIN_EXPORTS" /D "RENDERER_EXPORTS" /D "FILEFORMATPLUGIN_EXPORTS" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /FR /YX /FD /c
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
PostBuild_Desc=--- Installing module Core ---
PostBuild_Cmds=IF  NOT  EXIST  ..\..\bin  mkdir  ..\..\bin  	IF  NOT  EXIST  ..\..\bin\win32  mkdir  ..\..\bin\win32  	IF  NOT  EXIST  ..\..\bin\win32\Release  mkdir  ..\..\bin\win32\Release  	copy    bin\win32\Release\Core.dll  ..\..\bin\win32\Release  	IF  NOT  EXIST  ..\..\bin\win32\Debug\Resources\  mkdir  ..\..\bin\win32\Debug\Resources\  	xcopy  /S  /Y  CoreResourceTools\BitmapManagerTool\Icons  ..\..\bin\win32\Release\Resources\  	xcopy  /S  /Y  CoreResourceTools\BrushChooserTool\Icons    ..\..\bin\win32\Release\Resources\  	xcopy  /S  /Y  CoreResourceTools\ColorChooserTool\Icons  ..\..\bin\win32\Release\Resources\  	xcopy  /S  /Y  CoreTools\ColorPickerTool\Icons  ..\..\bin\win32\Release\Resources\  	xcopy  /S  /Y  CoreTools\NavigationTool\Icons  ..\..\bin\win32\Release\Resources\  	xcopy  /S    /Y  CoreTools\SelectionTool\Icons  ..\..\bin\win32\Release\Resources\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "$(QTDIR)\include" /I "..\..\external\include" /D "CORE_EXPORTS" /D "TOOL_EXPORTS" /D "RESOURCETOOL_EXPORTS" /D "PLUGIN_EXPORTS" /D "RENDERER_EXPORTS" /D "FILEFORMATPLUGIN_EXPORTS" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "QT_THREAD_SUPPORT" /D "QT_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(QTDIR)\lib\$(QTLIB) $(QTDIR)\lib\qtmain.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\external\lib-Win32\\"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=--- Installing module Core ---
PostBuild_Cmds=IF  NOT  EXIST  ..\..\bin\  mkdir  ..\..\bin  	IF  NOT  EXIST  ..\..\bin\win32\  mkdir  ..\..\bin\win32  	IF  NOT  EXIST  ..\..\bin\win32\Debug  mkdir  ..\..\bin\win32\Debug  	copy  bin\win32\Debug\Core.dll  ..\..\bin\win32\Debug  	IF  NOT  EXIST  ..\..\bin\win32\Debug\Resources\  mkdir  ..\..\bin\win32\Debug\Resources  	xcopy  /S    /Y  CoreResourceTools\BitmapManagerTool\Icons  ..\..\bin\win32\Debug\Resources\  	xcopy  /S  /Y  CoreResourceTools\BrushChooserTool\Icons  ..\..\bin\win32\Debug\Resources\  	xcopy  /S  /Y  CoreResourceTools\ColorChooserTool\Icons  ..\..\bin\win32\Debug\Resources\  	xcopy  /S  /Y  CoreTools\ColorPickerTool\Icons  ..\..\bin\win32\Debug\Resources\  	xcopy  /S  /Y  CoreTools\NavigationTool\Icons  ..\..\bin\win32\Debug\Resources\    	xcopy  /S  /Y  CoreTools\SelectionTool\Icons  ..\..\bin\win32\Debug\Resources\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Core - Win32 Release"
# Name "Core - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ClipBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DefaultMouseHandler.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\ClipBoard.h
# End Source File
# Begin Source File

SOURCE=.\src\CoreDLL.h
# End Source File
# Begin Source File

SOURCE=.\src\DefaultMouseHandler.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\Resources\BitmapManagerToolIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\Resources\BitmapManagerToolIcon_disabled.psd
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\Resources\BrushChooserToolIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\Resources\BrushChooserToolIcon_disabled.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\Resources\ColorPickerToolIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\Resources\ColorPickerToolIcon_disabled.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\EllipseIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\EllipseIcon_disabled.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\LassoIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\LassoIcon_disabled.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\LightTool\Resources\LightToolIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\LightTool\Resources\LightToolIcon_disabled.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\PolygonIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\PolygonIcon_disabled.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\RectangleIcon.psd
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Resources\RectangleIcon_disabled.psd
# End Source File
# End Group
# Begin Group "Auto-Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\BitmapManagerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\BitmapManagerDialog.h
# End Source File
# Begin Source File

SOURCE=.\OrthoMapper\UI\ImageViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\OrthoMapper\UI\ImageViewer.h
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\moc_BitmapManagerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\moc_BitmapManagerDialogImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\moc_BitmapManagerTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapResourceTool\src\moc_BitmapResourceTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\src\moc_BrushChooserTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\ColorChooserTool\src\moc_ColorChooserButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\ColorChooserTool\src\moc_ColorChooserTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\src\moc_ColorPickerTool.cpp
# End Source File
# Begin Source File

SOURCE=.\Configuration\src\moc_Configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\moc_FileFormatPluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\OrthoMapper\UI\moc_ImageViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\moc_MarkerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\UI\moc_NavigationConfigurationWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\src\moc_NavigationTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\moc_NewBrushDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\moc_NewBrushDialogImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\SCENE\SRC\moc_Object.cpp
# End Source File
# Begin Source File

SOURCE=.\PLUGINMANAGER\SRC\moc_PluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\moc_RendererConfigurationInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\moc_RendererInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\RendererManager\src\moc_RendererManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SCENE\SRC\moc_Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\UI\moc_SelectionConfigurationWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\src\moc_SelectionTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\moc_SelectShapeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\moc_SelectShapeDialogImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\moc_SurfelCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\moc_ToolInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolManager\src\moc_ToolManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Transformer\src\moc_Transformer.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\UI\NavigationConfigurationWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\UI\NavigationConfigurationWidget.h
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\NewBrushDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\NewBrushDialog.h
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\UI\SelectionConfigurationWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\UI\SelectionConfigurationWidget.h
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\SelectShapeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\SelectShapeDialog.h
# End Source File
# End Group
# Begin Group "Linux Project Files"

# PROP Default_Filter "*.pro"
# Begin Source File

SOURCE=.\Core.pro
# End Source File
# End Group
# Begin Group "Configuration"

# PROP Default_Filter ""
# Begin Group "Configuration Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Configuration\src\Configuration.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Configuration\src
InputPath=.\Configuration\src\Configuration.h
InputName=Configuration

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Configuration\src
InputPath=.\Configuration\src\Configuration.h
InputName=Configuration

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Configuration Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Configuration\src\Configuration.cpp
# End Source File
# End Group
# End Group
# Begin Group "RendererManager"

# PROP Default_Filter ""
# Begin Group "RendererManager Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\RendererManager\src\RendererDirWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\RendererManager\src\RendererLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\RendererManager\src\RendererManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RendererManager\src\RendererRegistry.cpp
# End Source File
# End Group
# Begin Group "RendererManager Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\RendererManager\src\RendererDirWalk.h
# End Source File
# Begin Source File

SOURCE=.\RendererManager\src\RendererLoader.h
# End Source File
# Begin Source File

SOURCE=.\RendererManager\src\RendererManager.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\RendererManager\src
InputPath=.\RendererManager\src\RendererManager.h
InputName=RendererManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\RendererManager\src
InputPath=.\RendererManager\src\RendererManager.h
InputName=RendererManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RendererManager\src\RendererRegistry.h
# End Source File
# End Group
# End Group
# Begin Group "Scene"

# PROP Default_Filter ""
# Begin Group "Scene Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Scene\src\Object.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Scene\src
InputPath=.\Scene\src\Object.h
InputName=Object

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Scene\src
InputPath=.\Scene\src\Object.h
InputName=Object

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scene\src\Scene.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Scene\src
InputPath=.\Scene\src\Scene.h
InputName=Scene

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Scene\src
InputPath=.\Scene\src\Scene.h
InputName=Scene

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Scene Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\Scene\src\Object.cpp

!IF  "$(CFG)" == "Core - Win32 Release"

# ADD CPP /I "SFLFormat\src" /I "C:\home\tknoll\PointShop3D\src\Core\SFLFormat\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# ADD CPP /I "C:\home\tknoll\PointShop3D\src\Core\SFLFormat\src" /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scene\src\Scene.cpp

!IF  "$(CFG)" == "Core - Win32 Release"

# ADD CPP /I "SFLFormat\src" /I "C:\home\tknoll\PointShop3D\src\Core\SFLFormat\src"
# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# ADD CPP /I "C:\home\tknoll\PointShop3D\src\Core\SFLFormat\src" /YX

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Transformer"

# PROP Default_Filter ""
# Begin Group "Transformer Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Transformer\src\Transformer.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Transformer\src
InputPath=.\Transformer\src\Transformer.h
InputName=Transformer

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Transformer\src
InputPath=.\Transformer\src\Transformer.h
InputName=Transformer

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Transformer Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\Transformer\src\Transformer.cpp
# End Source File
# End Group
# End Group
# Begin Group "CoreTools"

# PROP Default_Filter ""
# Begin Group "CoreTools Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreTools\src\CoreTools.h
# End Source File
# End Group
# Begin Group "CoreTools Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreTools\src\CoreTools.cpp
# End Source File
# End Group
# Begin Group "NavigationTool"

# PROP Default_Filter ""
# Begin Group "NavigationTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\src\NavigationTool.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreTools\NavigationTool\src
InputPath=.\CoreTools\NavigationTool\src\NavigationTool.h
InputName=NavigationTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreTools\NavigationTool\src
InputPath=.\CoreTools\NavigationTool\src\NavigationTool.h
InputName=NavigationTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "NavigationTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\src\NavigationTool.cpp
# End Source File
# End Group
# End Group
# Begin Group "ColorPickerTool"

# PROP Default_Filter ""
# Begin Group "ColorPickerTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\src\Color.h
# End Source File
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\src\ColorPickerTool.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreTools\ColorPickerTool\src
InputPath=.\CoreTools\ColorPickerTool\src\ColorPickerTool.h
InputName=ColorPickerTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreTools\ColorPickerTool\src
InputPath=.\CoreTools\ColorPickerTool\src\ColorPickerTool.h
InputName=ColorPickerTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "ColorPickerTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\src\ColorPickerTool.cpp
# End Source File
# End Group
# End Group
# Begin Group "SelectionTool"

# PROP Default_Filter ""
# Begin Group "SelectionTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\src\ScanConversion.h
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\src\SelectionTool.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreTools\SelectionTool\src
InputPath=.\CoreTools\SelectionTool\src\SelectionTool.h
InputName=SelectionTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreTools\SelectionTool\src
InputPath=.\CoreTools\SelectionTool\src\SelectionTool.h
InputName=SelectionTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "SelectionTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\src\ScanConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\src\SelectionTool.cpp
# End Source File
# End Group
# Begin Group "UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\UI\SelectionConfigurationWidget.ui

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreTools\SelectionTool\UI
InputPath=.\CoreTools\SelectionTool\UI\SelectionConfigurationWidget.ui
InputName=SelectionConfigurationWidget

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreTools\SelectionTool\UI
InputPath=.\CoreTools\SelectionTool\UI\SelectionConfigurationWidget.ui
InputName=SelectionConfigurationWidget

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# End Group
# Begin Group "Icons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\Icons\BitmapManagerToolIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\Icons\BitmapManagerToolIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\Icons\BrushChooserToolIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\Icons\BrushChooserToolIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\BrushIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\BrushIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\Icons\BuilderIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\Icons\BuilderIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\Icons\ColorPickerToolIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\ColorPickerTool\Icons\ColorPickerToolIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\EllipseIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\EllipseIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\LassoIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\LassoIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\Icons\NavigationIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\Icons\NavigationIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\PolygonIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\PolygonIcon_disabled.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\RectangleIcon.png
# End Source File
# Begin Source File

SOURCE=.\CoreTools\SelectionTool\Icons\RectangleIcon_disabled.png
# End Source File
# End Group
# Begin Group "CoreResourceTools"

# PROP Default_Filter ""
# Begin Group "CoreResourceTools Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreResourceTools\src\CoreResourceTools.h
# End Source File
# End Group
# Begin Group "CoreResourceTools Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreResourceTools\src\CoreResourceTools.cpp
# End Source File
# End Group
# Begin Group "ColorChooserTool"

# PROP Default_Filter ""
# Begin Group "ColorChooserTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreResourceTools\ColorChooserTool\src\ColorChooserButton.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\ColorChooserTool\src
InputPath=.\CoreResourceTools\ColorChooserTool\src\ColorChooserButton.h
InputName=ColorChooserButton

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\ColorChooserTool\src
InputPath=.\CoreResourceTools\ColorChooserTool\src\ColorChooserButton.h
InputName=ColorChooserButton

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\ColorChooserTool\src\ColorChooserTool.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\ColorChooserTool\src
InputPath=.\CoreResourceTools\ColorChooserTool\src\ColorChooserTool.h
InputName=ColorChooserTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\ColorChooserTool\src
InputPath=.\CoreResourceTools\ColorChooserTool\src\ColorChooserTool.h
InputName=ColorChooserTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "ColorChooserTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreResourceTools\ColorChooserTool\src\ColorChooserButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\ColorChooserTool\src\ColorChooserTool.cpp
# End Source File
# End Group
# End Group
# Begin Group "BrushChooserTool"

# PROP Default_Filter ""
# Begin Group "BrushChooserTool Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\src\BrushChooserTool.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BrushChooserTool\src
InputPath=.\CoreResourceTools\BrushChooserTool\src\BrushChooserTool.h
InputName=BrushChooserTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BrushChooserTool\src
InputPath=.\CoreResourceTools\BrushChooserTool\src\BrushChooserTool.h
InputName=BrushChooserTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\src\BrushDirWalk.h
# End Source File
# End Group
# Begin Group "BrushChooserTool Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\src\BrushChooserTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BrushChooserTool\src\BrushDirWalk.cpp
# End Source File
# End Group
# End Group
# Begin Group "BitmapManagerTool"

# PROP Default_Filter ""
# Begin Group "BitmapManagerTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerDialogImpl.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerDialogImpl.h
InputName=BitmapManagerDialogImpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerDialogImpl.h
InputName=BitmapManagerDialogImpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerTool.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerTool.h
InputName=BitmapManagerTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerTool.h
InputName=BitmapManagerTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\BrushScrollView.h
# PROP Ignore_Default_Tool 1
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\NewBrushDialogImpl.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\NewBrushDialogImpl.h
InputName=NewBrushDialogImpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\NewBrushDialogImpl.h
InputName=NewBrushDialogImpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\SelectShapeDialogImpl.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\SelectShapeDialogImpl.h
InputName=SelectShapeDialogImpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapManagerTool\src
InputPath=.\CoreResourceTools\BitmapManagerTool\src\SelectShapeDialogImpl.h
InputName=SelectShapeDialogImpl

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "BitmapManagerTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerDialogImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\BitmapManagerTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\BrushScrollView.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\NewBrushDialogImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\src\SelectShapeDialogImpl.cpp
# End Source File
# End Group
# Begin Group "BitmapManagerTool UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\BitmapManagerDialog.ui

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreResourceTools\BitmapManagerTool\UI
InputPath=.\CoreResourceTools\BitmapManagerTool\UI\BitmapManagerDialog.ui
InputName=BitmapManagerDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreResourceTools\BitmapManagerTool\UI
InputPath=.\CoreResourceTools\BitmapManagerTool\UI\BitmapManagerDialog.ui
InputName=BitmapManagerDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\NewBrushDialog.ui

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreResourceTools\BitmapManagerTool\UI
InputPath=.\CoreResourceTools\BitmapManagerTool\UI\NewBrushDialog.ui
InputName=NewBrushDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreResourceTools\BitmapManagerTool\UI
InputPath=.\CoreResourceTools\BitmapManagerTool\UI\NewBrushDialog.ui
InputName=NewBrushDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapManagerTool\UI\SelectShapeDialog.ui

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreResourceTools\BitmapManagerTool\UI
InputPath=.\CoreResourceTools\BitmapManagerTool\UI\SelectShapeDialog.ui
InputName=SelectShapeDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreResourceTools\BitmapManagerTool\UI
InputPath=.\CoreResourceTools\BitmapManagerTool\UI\SelectShapeDialog.ui
InputName=SelectShapeDialog

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "BitmapResourceTool"

# PROP Default_Filter ""
# Begin Group "BitmapResourceTool Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapResourceTool\src\BitmapDirWalk.h
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapResourceTool\src\BitmapResourceTool.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapResourceTool\src
InputPath=.\CoreResourceTools\BitmapResourceTool\src\BitmapResourceTool.h
InputName=BitmapResourceTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\CoreResourceTools\BitmapResourceTool\src
InputPath=.\CoreResourceTools\BitmapResourceTool\src\BitmapResourceTool.h
InputName=BitmapResourceTool

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "BitmapResourceTool Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapResourceTool\src\BitmapDirWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\CoreResourceTools\BitmapResourceTool\src\BitmapResourceTool.cpp
# End Source File
# End Group
# End Group
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

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ToolManager\src
InputPath=.\ToolManager\src\ToolManager.h
InputName=ToolManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\ToolManager\src
InputPath=.\ToolManager\src\ToolManager.h
InputName=ToolManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
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
# Begin Group "DataStructures"

# PROP Default_Filter ""
# Begin Group "DataStructures Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\DataStructures\src\AdjacencyGraph.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\AssociatedSurfels.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\Cluster.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\FifoQueue.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\InsideOutsideTest.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\kdTree.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\NeighbourHood.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\ObjListDbl.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\PriorityQueue.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\QueryGrid.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\SurfelCollection.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\DataStructures\src
InputPath=.\DataStructures\src\SurfelCollection.h
InputName=SurfelCollection

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\DataStructures\src
InputPath=.\DataStructures\src\SurfelCollection.h
InputName=SurfelCollection

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "DataStructures Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\DataStructures\src\AdjacencyGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\AssociatedSurfels.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\Cluster.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\FifoQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\InsideOutsideTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\kdTree.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\NeighbourHood.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\ObjListDbl.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\PriorityQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\QueryGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\SurfelCollection.cpp
# End Source File
# End Group
# Begin Group "Miniball"

# PROP Default_Filter ""
# Begin Group "Miniball Headers"

# PROP Default_Filter "*.h; *.hpp"
# Begin Source File

SOURCE=.\DataStructures\src\Miniball\Miniball.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\Miniball\miniball_config.H
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\Miniball\wrapped_array.H
# End Source File
# End Group
# Begin Group "Miniball Source"

# PROP Default_Filter "*.c; *.cpp"
# Begin Source File

SOURCE=.\DataStructures\src\Miniball\miniball.cpp
# End Source File
# End Group
# End Group
# Begin Group "ClusterTree"

# PROP Default_Filter ""
# Begin Group "ClusterTree Headers"

# PROP Default_Filter "h;hpp"
# Begin Source File

SOURCE=.\DataStructures\src\Clustering.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\ClusterNode.h
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\ClusterTree.h
# End Source File
# End Group
# Begin Group "ClusterTree Source"

# PROP Default_Filter "cpp;c"
# Begin Source File

SOURCE=.\DataStructures\src\Clustering.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\ClusterNode.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructures\src\ClusterTree.cpp
# End Source File
# End Group
# End Group
# End Group
# Begin Group "PluginManager"

# PROP Default_Filter ""
# Begin Group "PluginManager Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\PluginManager\src\PluginDirWalk.h
# End Source File
# Begin Source File

SOURCE=.\PluginManager\src\PluginLoader.h
# End Source File
# Begin Source File

SOURCE=.\PluginManager\src\PluginManager.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\PluginManager\src
InputPath=.\PluginManager\src\PluginManager.h
InputName=PluginManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\PluginManager\src
InputPath=.\PluginManager\src\PluginManager.h
InputName=PluginManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PluginManager\src\PluginRegistry.h
# End Source File
# End Group
# Begin Group "PluginManager Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\PluginManager\src\PluginDirWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginManager\src\PluginLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginManager\src\PluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginManager\src\PluginRegistry.cpp
# End Source File
# End Group
# End Group
# Begin Group "FileFormatPluginManager"

# PROP Default_Filter ""
# Begin Group "FileFormatPluginManager Header"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginDirWalk.h
# End Source File
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginLoader.h
# End Source File
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginManager.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\FileFormatPluginManager\src
InputPath=.\FileFormatPluginManager\src\FileFormatPluginManager.h
InputName=FileFormatPluginManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\FileFormatPluginManager\src
InputPath=.\FileFormatPluginManager\src\FileFormatPluginManager.h
InputName=FileFormatPluginManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginRegistry.h
# End Source File
# End Group
# Begin Group "FileFormatPluginManager Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginDirWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFormatPluginManager\src\FileFormatPluginRegistry.cpp
# End Source File
# End Group
# End Group
# Begin Group "Brush"

# PROP Default_Filter ""
# Begin Group "Brush Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brush\src\Brush.h
# End Source File
# Begin Source File

SOURCE=.\Brush\src\BrushChannel.h
# End Source File
# End Group
# Begin Group "Brush Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Brush\src\Brush.cpp
# End Source File
# Begin Source File

SOURCE=.\Brush\src\BrushChannel.cpp
# End Source File
# End Group
# End Group
# Begin Group "User Interfaces (UI)"

# PROP Default_Filter "*.ui"
# Begin Source File

SOURCE=.\CoreTools\NavigationTool\UI\NavigationConfigurationWidget.ui

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreTools\NavigationTool\UI
InputPath=.\CoreTools\NavigationTool\UI\NavigationConfigurationWidget.ui
InputName=NavigationConfigurationWidget

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\CoreTools\NavigationTool\UI
InputPath=.\CoreTools\NavigationTool\UI\NavigationConfigurationWidget.ui
InputName=NavigationConfigurationWidget

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "OrthoMapper"

# PROP Default_Filter ""
# Begin Group "OrthoMapper Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OrthoMapper\src\OrthoMapper.h
# End Source File
# End Group
# Begin Group "OrthoMapper Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OrthoMapper\src\OrthoMapper.cpp
# End Source File
# End Group
# Begin Group "OrthoMapper UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OrthoMapper\UI\ImageViewer.ui

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\OrthoMapper\UI
InputPath=.\OrthoMapper\UI\ImageViewer.ui
InputName=ImageViewer

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Uic'ing $(InputName).ui ...
InputDir=.\OrthoMapper\UI
InputPath=.\OrthoMapper\UI\ImageViewer.ui
InputName=ImageViewer

BuildCmds= \
	%qtdir%\bin\uic.exe $(InputPath) -o "$(InputDir)\$(InputName).h" \
	%qtdir%\bin\uic.exe $(InputPath) -i $(InputName).h -o $(InputDir)\$(InputName).cpp \
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp" \
	

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Painter"

# PROP Default_Filter ""
# Begin Group "Painter Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Painter\src\Painter.h
# End Source File
# End Group
# Begin Group "Painter Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Painter\src\Painter.cpp
# End Source File
# End Group
# End Group
# Begin Group "MarkerManager"

# PROP Default_Filter ""
# Begin Group "MarkerManager Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\MarkerManager\src\Marker2D.h
# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\Marker3D.h
# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\MarkerInterface.h
# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\MarkerManager.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\MarkerManager\src
InputPath=.\MarkerManager\src\MarkerManager.h
InputName=MarkerManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\MarkerManager\src
InputPath=.\MarkerManager\src\MarkerManager.h
InputName=MarkerManager

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\MarkerUtils.h
# End Source File
# End Group
# Begin Group "MarkerManager Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\MarkerManager\src\Marker2D.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\Marker3D.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\MarkerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkerManager\src\MarkerUtils.cpp
# End Source File
# End Group
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Group "Interfaces Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Interfaces\src\FileFormatPluginInterface.h
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\FrameBufferInterface.h
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\NearestNeighborInterface.h
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\PluginInterface.h
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\RendererConfigurationInterface.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Interfaces\src
InputPath=.\Interfaces\src\RendererConfigurationInterface.h
InputName=RendererConfigurationInterface

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Interfaces\src
InputPath=.\Interfaces\src\RendererConfigurationInterface.h
InputName=RendererConfigurationInterface

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\RendererInterface.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Interfaces\src
InputPath=.\Interfaces\src\RendererInterface.h
InputName=RendererInterface

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Interfaces\src
InputPath=.\Interfaces\src\RendererInterface.h
InputName=RendererInterface

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\ResourceToolInterface.h
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\SurfelInterface.h
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\ToolInterface.h

!IF  "$(CFG)" == "Core - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Interfaces\src
InputPath=.\Interfaces\src\ToolInterface.h
InputName=ToolInterface

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ELSEIF  "$(CFG)" == "Core - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Moc'ing $(InputName).h ...
InputDir=.\Interfaces\src
InputPath=.\Interfaces\src\ToolInterface.h
InputName=ToolInterface

"$(InputDir)\moc_$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%qtdir%\bin\moc.exe "$(InputDir)\$(InputName).h" -o "$(InputDir)\moc_$(InputName).cpp"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Interfaces Source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\Interfaces\src\RendererInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\SurfelInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Interfaces\src\ToolInterface.cpp
# End Source File
# End Group
# End Group
# End Target
# End Project
