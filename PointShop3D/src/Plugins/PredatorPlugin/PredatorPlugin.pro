include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}
HEADERS		= src/PredatorPlugin.h
SOURCES		= src/PredatorPlugin.cpp
INTERFACES	=

ALL_DEPS	=

TARGET		= PredatorPlugin

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/Plugins/Color

LIBS           += -L../../../bin/linux/$${BUILDTYPE}/ -lCore -lUtilities

