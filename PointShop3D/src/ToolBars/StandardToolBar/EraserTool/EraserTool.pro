include ( ../../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}

HEADERS		= src/EraserTool.h
SOURCES		= src/EraserTool.cpp

INTERFACES	= UI/EraserConfigurationWidget.ui

ALL_DEPS	= 

TARGET		= EraserTool

OBJECTS_DIR = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../../bin/linux/$${BUILDTYPE}/Tools

LIBS           += -L../../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities

QMAKE_POST_LINK = mkdir -p ../../../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp \
                     Icons/EraserToolIcon.png \
                     Icons/EraserToolIcon_disabled.png \
                     ../../../../bin/linux/$${BUILDTYPE}/Resources
