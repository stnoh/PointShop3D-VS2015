include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}
HEADERS		= src/SFLFileFormat.h
SOURCES		= src/SFLFileFormat.cpp
INTERFACES	= UI/SFLCustomizeDialog.ui

INCLUDEPATH = include

ALL_DEPS	=

TARGET		= SFLFileFormat

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/FileFormatPlugins

LIBS	       += -L../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities -lpi -lsfl

QMAKE_PRE_LINK = mkdir -p ../../../bin/linux/$${BUILDTYPE} && \
                  cp lib-Linux-$${BUILDTYPE}/libpi.so \
                     lib-Linux-$${BUILDTYPE}/libsfl.so \
                     ../../../bin/linux/$${BUILDTYPE}
