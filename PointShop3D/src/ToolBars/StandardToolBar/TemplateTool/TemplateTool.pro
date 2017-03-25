include ( ../../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}
HEADERS		= src/TemplateTool.h
SOURCES		= src/TemplateTool.cpp
INTERFACES	= UI/TemplateConfigurationWidget.ui

ALL_DEPS	=

TARGET		= TemplateTool

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../../bin/linux/$${BUILDTYPE}/Tools

LIBS	       += -L../../../../bin/linux/$${BUILDTYPE}

QMAKE_POST_LINK = mkdir -p ../../../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp \
                     Icons/TemplateIcon.png \
                     Icons/TemplateToolIcon.png \
                     Icons/TemplateToolIcon_disabled.png \
                     ../../../../bin/linux/$${BUILDTYPE}/Resources
