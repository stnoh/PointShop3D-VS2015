include ( ../../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}

HEADERS		= src/FilterTool.h
SOURCES		= src/FilterTool.cpp

INTERFACES	= UI/FilterConfigurationWidget.ui \
		      UI/ImageViewer.ui

ALL_DEPS	= 

TARGET		= FilterTool

OBJECTS_DIR = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../../bin/linux/$${BUILDTYPE}/Tools

LIBS	       += -L../../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities

QMAKE_POST_LINK = mkdir -p ../../../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp \
                     Icons/FilterToolIcon.png \
                     Icons/FilterToolIcon_disabled.png \
                     ../../../../bin/linux/$${BUILDTYPE}/Resources
