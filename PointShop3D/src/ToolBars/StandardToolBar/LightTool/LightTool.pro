include ( ../../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}

HEADERS		= src/LightTool.h
SOURCES		= src/LightTool.cpp

INTERFACES	= UI/LightConfigurationWidget.ui

ALL_DEPS	= 

TARGET		= LightTool

OBJECTS_DIR = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../../bin/linux/$${BUILDTYPE}/Tools

LIBS	       += -L../../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes

QMAKE_POST_LINK = mkdir -p ../../../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp \
                     Icons/LightToolCursor.png \
                     Icons/LightToolIcon.png \
                     Icons/LightToolIcon_disabled.png \
                     ../../../../bin/linux/$${BUILDTYPE}/Resources


