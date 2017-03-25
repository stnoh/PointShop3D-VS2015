include ( ../../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}

HEADERS		= src/BrushTool.h
SOURCES		= src/BrushTool.cpp

INTERFACES	= UI/BrushConfigurationWidget.ui \
		      UI/ImageViewer.ui

ALL_DEPS	= 

TARGET		= BrushTool

OBJECTS_DIR = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../../bin/linux/$${BUILDTYPE}/Tools

LIBS	       += -L../../../../bin/linux/$${BUILDTYPE} -lCore -lUtilities -lDataTypes

QMAKE_POST_LINK = mkdir -p ../../../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp \
                     Icons/BrushToolIcon.png \
                     Icons/BrushToolIcon_disabled.png \
                     ../../../../bin/linux/$${BUILDTYPE}/Resources
