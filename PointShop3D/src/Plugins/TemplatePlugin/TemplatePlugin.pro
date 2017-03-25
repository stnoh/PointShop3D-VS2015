include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}
HEADERS		= src/TemplatePlugin.h
SOURCES		= src/TemplatePlugin.cpp
INTERFACES	=

ALL_DEPS	=

TARGET		= TemplatePlugin

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/Plugins/Template

LIBS           += -L../../../bin/linux/$${BUILDTYPE}

