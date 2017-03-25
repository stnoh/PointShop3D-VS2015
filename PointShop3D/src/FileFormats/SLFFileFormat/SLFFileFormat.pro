include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}
HEADERS		= src/SLFFileFormat.h \
                  src/NormalMap.h
SOURCES		= src/SLFFileFormat.cpp \
                  src/NormalMap.cpp
INTERFACES	= 

ALL_DEPS	=

TARGET		= SLFFileFormat

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/FileFormatPlugins

LIBS           += -L../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities
