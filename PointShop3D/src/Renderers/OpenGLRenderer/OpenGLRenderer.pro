include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt opengl thread plugin $${BUILDTYPE}
HEADERS		= src/OpenGLRenderer.h \
	          src/OpenGLRendererConfiguration.h \
			  src/OpenGLRendererWidget.h
SOURCES		= src/OpenGLRenderer.cpp \
	          src/OpenGLRendererConfiguration.cpp \
			  src/OpenGLRendererWidget.cpp
INTERFACES	= UI/OpenGLRendererConfigurationWidget.ui

ALL_DEPS	=

TARGET		= OpenGLRenderer

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/Renderers/preview

LIBS           += -L../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities
