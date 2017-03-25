include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}
HEADERS		= src/PSSFileFormat.h
SOURCES		= src/PSSFileFormat.cpp
INTERFACES	= 

ALL_DEPS	=

TARGET		= PSSFileFormat

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/FileFormatPlugins

LIBS           += -L../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities

