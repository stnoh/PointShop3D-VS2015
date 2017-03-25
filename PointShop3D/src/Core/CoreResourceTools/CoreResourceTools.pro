TEMPLATE	= app
CONFIG		= qt thread dll release
HEADERS		= ../ResourceToolBar/BitmapResourceManagerTool/src/BitmapResourceManagerDialogImpl.h \
                  ../ResourceToolBar/BitmapResourceManagerTool/src/BitmapResourceManagerTool.h \
                  ../ResourceToolBar/BitmapResourceTool/src/BitmapDirWalk.h \
                  ../ResourceToolBar/BitmapResourceTool/src/BitmapResourceTool.h \
                  ../ResourceToolBar/ColorChooserTool/src/ColorChooserButton.h \
                  ../ResourceToolBar/ColorChooserTool/src/ColorChooserTool.h \
                  src/CoreResourceTools.h \
		  src/CoreResourceToolsDLL.h
SOURCES		= ../ResourceToolBar/BitmapResourceManagerTool/src/BitmapResourceManagerDialogImpl.cpp \
                  ../ResourceToolBar/BitmapResourceManagerTool/src/BitmapResourceManagerTool.cpp \
                  ../ResourceToolBar/BitmapResourceTool/src/BitmapDirWalk.cpp \
                  ../ResourceToolBar/BitmapResourceTool/src/BitmapResourceTool.cpp \
                  ../ResourceToolBar/ColorChooserTool/src/ColorChooserButton.cpp \
                  ../ResourceToolBar/ColorChooserTool/src/ColorChooserTool.cpp \
                  src/CoreResourceTools.cpp

INTERFACES	= ../ResourceToolBar/BitmapResourceManagerTool/UI/BitmapResourceManagerDialog.ui

ALL_DEPS	= 

TARGET		= libCoreResourceTools

OBJECTS_DIR     = bin/linux/release

MOC_DIR		= bin/linux/release

DESTDIR		= ../../../bin/linux/release

LIBS           += -L../../../bin/linux/release -lInterfaces -lUtilities

