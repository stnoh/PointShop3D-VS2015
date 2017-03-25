include ( ../buildcfg.pro )

TEMPLATE	= app
CONFIG		= qt thread $${BUILDTYPE}
HEADERS		= UserInterface/AnimationDialogImpl.h \
		  UserInterface/CameraPositionMenu.h \
		  UserInterface/ConfigurationDialogImpl.h \
		  UserInterface/ImageExportDialogImpl.h \
                  Core/MarkerManager/src/MarkerManager.h \
                  UserInterface/PointShopWindow.h \
		  UserInterface/ResourceToolBar.h \
		  UserInterface/StatisticsDialogImpl.h \
		  UserInterface/SurfelPropertiesDialogImpl.h \
		  UserInterface/ToolConfigurationDialog.h \
		  UserInterface/BrushViewer/src/BrushViewerImpl.h \
		  UserInterface/ObjectViewer/src/ObjectViewerImpl.h
                  
SOURCES		= UserInterface/AnimationDialogImpl.cpp \
		  UserInterface/CameraPositionMenu.cpp \
		  UserInterface/ConfigurationDialogImpl.cpp \
		  UserInterface/ImageExportDialogImpl.cpp \
                  Main.cpp \
		  UserInterface/PointShopWindow.cpp \
		  UserInterface/ResourceToolBar.cpp \
		  UserInterface/StatisticsDialogImpl.cpp \
		  UserInterface/SurfelPropertiesDialogImpl.cpp \
		  UserInterface/ToolConfigurationDialog.cpp \
		  UserInterface/BrushViewer/src/BrushViewerImpl.cpp \
		  UserInterface/ObjectViewer/src/ObjectViewerImpl.cpp
		  
INTERFACES	= UserInterface/UI/AnimationDialog.ui \
                  Core/CoreResourceTools/BitmapManagerTool/UI/BitmapManagerDialog.ui \
                  UserInterface/BrushViewer/UI/BrushViewer.ui \
                  UserInterface/UI/ConfigurationDialog.ui \
                  UserInterface/UI/ImageExportDialog.ui \
                  UserInterface/UI/NoToolConfigurationWidget.ui \
                  UserInterface/UI/StatisticsDialog.ui \
                  UserInterface/UI/SurfelPropertiesDialog.ui \
                  UserInterface/ObjectViewer/UI/ObjectViewer.ui

TARGET		= PointShop

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../bin/linux/$${BUILDTYPE}

LIBS	       += -L../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities

QMAKE_POST_LINK = mkdir -p ../bin/linux/$${BUILDTYPE}/Resources && \
                  cp UserInterface/Icons/DefaultPixmap.png \
                     UserInterface/Icons/FileOpenIcon.png \
                     UserInterface/Icons/FileSaveIcon.png \
                     UserInterface/Icons/FileSaveIcon_disabled.png \
                     UserInterface/Icons/IconBackground.png \
                     UserInterface/Icons/PointShopIcon.png \
                     UserInterface/Icons/PreviewRendererIcon.png \
                     UserInterface/Icons/ToolSettingsIcon.png \
                     UserInterface/Icons/ToolSettingsIcon_disabled.png \
                     UserInterface/Icons/WhatsThisIcon.png \
                     ../bin/linux/$${BUILDTYPE}/Resources && \
                  cp -r ../Brushes ../Bitmaps ../Models ../bin/linux/$${BUILDTYPE}
