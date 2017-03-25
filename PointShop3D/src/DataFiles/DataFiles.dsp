# Microsoft Developer Studio Project File - Name="DataFiles" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=DataFiles - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DataFiles.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DataFiles.mak" CFG="DataFiles - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DataFiles - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "DataFiles - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
MTL=midl.exe

!IF "$(CFG)" == "DataFiles - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF "$(CFG)" == "DataFiles - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "DataFiles - Win32 Release"
# Name "DataFiles - Win32 Debug"
# Begin Source File

SOURCE=.\Bitmaps.txt

!IF "$(CFG)" == "DataFiles - Win32 Release"

# PROP Intermediate_Dir "bin\win32\Release"
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Installing Bitmaps $(InputPath)
InputPath=.\Bitmaps.txt

"..\..\bin\win32\Release\Bitmaps" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST ..\..\bin\win32\Release\Bitmaps\ mkdir ..\..\bin\win32\Release\Bitmaps 
	xcopy /S /Y ..\..\Bitmaps ..\..\bin\win32\Release\Bitmaps\ 
	
# End Custom Build

!ELSEIF "$(CFG)" == "DataFiles - Win32 Debug"

# PROP Intermediate_Dir "bin\win32\Debug"
# Begin Custom Build - Installing Bitmaps $(InputPath)
InputPath=.\Bitmaps.txt

"..\..\bin\win32\Debug\Bitmaps" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST ..\..\bin\win32\Debug\Bitmaps\ mkdir ..\..\bin\win32\Debug\Bitmaps 
	xcopy /S /Y ..\..\Bitmaps ..\..\bin\win32\Debug\Bitmaps\ 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Brushes.txt

!IF "$(CFG)" == "DataFiles - Win32 Release"

# PROP Intermediate_Dir "bin\win32\Release"
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Installing Brushes $(InputPath)
InputPath=.\Brushes.txt

"..\..\bin\win32\Release\Brushes" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST ..\..\bin\win32\Release\Brushes\ mkdir ..\..\bin\win32\Release\Brushes 
	xcopy /S /Y ..\..\Brushes ..\..\bin\win32\Release\Brushes\ 
	
# End Custom Build

!ELSEIF "$(CFG)" == "DataFiles - Win32 Debug"

# PROP Intermediate_Dir "bin\win32\Debug"
# Begin Custom Build - Installing Brushes $(InputPath)
InputPath=.\Brushes.txt

"..\..\bin\win32\Debug\Brushes" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST ..\..\bin\win32\Debug\Brushes\ mkdir ..\..\bin\win32\Debug\Brushes 
	xcopy /S /Y ..\..\Brushes ..\..\bin\win32\Debug\Brushes\ 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Models.txt

!IF "$(CFG)" == "DataFiles - Win32 Release"

# PROP Intermediate_Dir "bin\win32\Release"
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Installing Models $(InputPath)
InputPath=.\Models.txt

"..\..\bin\win32\Release\Models" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST ..\..\bin\win32\Release\Models\ mkdir ..\..\bin\win32\Release\Models 
	xcopy /S /Y ..\..\Models ..\..\bin\win32\Release\Models\ 
	
# End Custom Build

!ELSEIF "$(CFG)" == "DataFiles - Win32 Debug"

# PROP Intermediate_Dir "bin\win32\Debug"
# Begin Custom Build - Installing Models $(InputPath)
InputPath=.\Models.txt

"..\..\bin\win32\Debug\Models" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	IF NOT EXIST ..\..\bin\win32\Debug\Models\ mkdir ..\..\bin\win32\Debug\Models 
	xcopy /S /Y ..\..\Models ..\..\bin\win32\Debug\Models\ 
	
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
