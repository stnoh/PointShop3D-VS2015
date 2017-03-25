include ( ../../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread plugin $${BUILDTYPE}

HEADERS		= src/OctreeRenderer_SurfSplat/ConfigHeaders/BasePlane.h \
		  src/OctreeRenderer_SurfSplat/ConfigHeaders/Shader.h \
		  src/OctreeRenderer_SurfSplat/ConfigHeaders/ZBuffer.h \
		  src/SurfelGPFrameBuffer.h \
		  src/SurfelGPRenderer.h \
		  src/SurfelGPRendererConfiguration.h \
		  src/SurfelGPRendererWidget.h \
		  src/SurfelPipeline_General/LSrList.h \
		  src/SurfelPipeline_General/LightSource.h \
		  src/SurfelPipeline_SurfSplat/Shader_SurfSplat.h \
		  src/SurfelPipeline_SurfSplat/Srf.h \
		  src/SurfelPipeline_SurfSplat/Warper.h \
		  src/SurfelPipeline_SurfSplat/ZBuffer_SurfSplat.h \
		  src/ViewTransformation/ViewingTrafo.h

SOURCES		= src/SurfelGPFrameBuffer.cpp \
		  src/SurfelGPRenderer.cpp \
		  src/SurfelGPRendererConfiguration.cpp \
		  src/SurfelGPRendererWidget.cpp \
		  src/SurfelPipeline_General/LSrList.cpp \
		  src/SurfelPipeline_General/LightSource.cpp \
		  src/SurfelPipeline_SurfSplat/Shader_SurfSplat.cpp \
		  src/SurfelPipeline_SurfSplat/Srf.cpp \
		  src/SurfelPipeline_SurfSplat/Warper.cpp \
		  src/SurfelPipeline_SurfSplat/ZBuffer_SurfSplat.cpp \
		  src/ViewTransformation/ViewingTrafo.cpp
INTERFACES	= UI/SurfelGPRendererConfigurationWidget.ui

ALL_DEPS	=

TARGET		= SurfelGPRenderer

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../../bin/linux/$${BUILDTYPE}/Renderers/default

LIBS           += -L../../../bin/linux/$${BUILDTYPE} -lCore -lDataTypes -lUtilities 
