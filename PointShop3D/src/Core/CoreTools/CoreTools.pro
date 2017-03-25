TEMPLATE	= app
CONFIG		= qt thread dll release
HEADERS		= ../FunctionToolBar/PaintFunction/src/CarveFunction.h \
		  ../FunctionToolBar/PaintFunction/src/Edge.h \
		  ../FunctionToolBar/PaintFunction/src/EdgeList.h \
		  ../FunctionToolBar/PaintFunction/src/EraserFunction.h \
		  ../FunctionToolBar/PaintFunction/src/FilterFunction.h \
		  ../FunctionToolBar/PaintFunction/src/Heap.h \
		  ../FunctionToolBar/PaintFunction/src/MaterialFunction.h \
		  ../FunctionToolBar/PaintFunction/src/MaterialFunctionConfigurationWidgetImpl.h \
		  ../FunctionToolBar/PaintFunction/src/ResamplingFunction.h \
		  ../FunctionToolBar/PaintFunction/src/SurfelCluster.h \
		  ../FunctionToolBar/PaintFunction/src/PaintFunction.h \
                  ../StandardToolBar/ColorPickerTool/src/ColorPickerTool.h \
                  ../StandardToolBar/NavigationTool/src/NavigationTool.h \
                  ../StandardToolBar/SelectionTool/src/PixelSelection.h \
                  ../StandardToolBar/SelectionTool/src/ScanConversion.h \
                  ../StandardToolBar/SelectionTool/src/SelectionTool.h \
                  src/CoreTools.h \
		  src/CoreToolsDLL.h
SOURCES		= ../FunctionToolBar/PaintFunction/src/CarveFunction.cpp \
		  ../FunctionToolBar/PaintFunction/src/Edge.cpp \
		  ../FunctionToolBar/PaintFunction/src/EdgeList.cpp \
		  ../FunctionToolBar/PaintFunction/src/EraserFunction.cpp \
		  ../FunctionToolBar/PaintFunction/src/FilterFunction.cpp \
		  ../FunctionToolBar/PaintFunction/src/MaterialFunction.cpp \
		  ../FunctionToolBar/PaintFunction/src/MaterialFunctionConfigurationWidgetImpl.cpp \
		  ../FunctionToolBar/PaintFunction/src/ResamplingFunction.cpp \
		  ../FunctionToolBar/PaintFunction/src/SurfelCluster.cpp \
		  ../FunctionToolBar/PaintFunction/src/PaintFunction.cpp \
                  ../StandardToolBar/ColorPickerTool/src/ColorPickerTool.cpp \
                  ../StandardToolBar/NavigationTool/src/NavigationTool.cpp \
                  ../StandardToolBar/SelectionTool/src/PixelSelection.cpp \
                  ../StandardToolBar/SelectionTool/src/ScanConversion.cpp \
                  ../StandardToolBar/SelectionTool/src/SelectionTool.cpp \
                  src/CoreTools.cpp
INTERFACES	= ../FunctionToolBar/PaintFunction/UI/CarveFunctionConfigurationWidget.ui \
                  ../FunctionToolBar/PaintFunction/UI/EraserFunctionConfigurationWidget.ui \
                  ../FunctionToolBar/PaintFunction/UI/FilterFunctionConfigurationWidget.ui \
                  ../FunctionToolBar/PaintFunction/UI/ImageViewer.ui \
                  ../FunctionToolBar/PaintFunction/UI/MaterialFunctionConfigurationWidget.ui \
				  ../FunctionToolBar/PaintFunction/UI/PaintFunctionConfigurationWidget.ui \
                  ../FunctionToolBar/PaintFunction/UI/ResamplingFunctionConfigurationWidget.ui

ALL_DEPS	= 

TARGET		= libCoreTools

OBJECTS_DIR     = bin/linux/release

MOC_DIR		= bin/linux/release

DESTDIR		= ../../../bin/linux/release

LIBS           += -L../../../bin/linux/release -lCore -lUtilities -lInterfaces -lCoreResourceTools -lRenderer
