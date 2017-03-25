include ( ../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread $${BUILDTYPE}

HEADERS		= src/DataTypesDLL.h \
                  src/MyDataTypes.h \
                  src/Vector2D.h \
                  src/Vector3D.h
		  
SOURCES		= src/Vector2D.cpp \
                  src/Vector3D.cpp
                  
INTERFACES	= 

TARGET          = DataTypes

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../bin/linux/$${BUILDTYPE}

LIBS	       += -L../../bin/linux/$${BUILDTYPE}
