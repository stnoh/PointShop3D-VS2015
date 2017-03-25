include ( ../../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}

HEADERS		= src/BitmapMarker.h \
		  src/MultiGridLevel.h \
		  src/Parameterization.h \
		  src/ParameterizationDialogImpl.h \
		  src/ParameterizationTool.h \
		  src/SparseLeastSquares.h \
		  src/SplatBuffer.h
SOURCES		= src/BitmapMarker.cpp \
		  src/MultiGridLevel.cpp \
		  src/Parameterization.cpp \
		  src/ParameterizationDialogImpl.cpp \
		  src/ParameterizationTool.cpp \
		  src/SparseLeastSquares.cpp \
		  src/SplatBuffer.cpp
INTERFACES	= UI/ExtendedSettings.ui \
	          UI/ParameterizationConfigurationWidget.ui \
			  UI/ParameterizationDialog.ui

ALL_DEPS	= 

TARGET		= ParameterizationTool

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../../bin/linux/$${BUILDTYPE}/Tools

LIBS	       += -L../../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities

QMAKE_POST_LINK = mkdir -p ../../../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp \
                     Icons/ParameterizationIcon.png \
                     Icons/ParameterizationToolIcon.png \
                     Icons/ParameterizationToolIcon_disabled.png \
                     ../../../../bin/linux/$${BUILDTYPE}/Resources
