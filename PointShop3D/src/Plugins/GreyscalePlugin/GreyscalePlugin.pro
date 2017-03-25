include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}
HEADERS		= src/GreyscalePlugin.h
SOURCES		= src/GreyscalePlugin.cpp
INTERFACES	=

ALL_DEPS	=

TARGET		= GreyscalePlugin

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/Plugins/Color

LIBS           += -L../../../bin/linux/$${BUILDTYPE} -lCore -lUtilities

