include ( ../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread $${BUILDTYPE}
HEADERS		= Brush/src/Brush.h \
		  Brush/src/BrushChannel.h \
		  Configuration/src/Configuration.h \
		  CoreResourceTools/BitmapManagerTool/src/BitmapManagerDialogImpl.h \
		  CoreResourceTools/BitmapManagerTool/src/BitmapManagerTool.h \
		  CoreResourceTools/BitmapManagerTool/src/BrushScrollView.h \
		  CoreResourceTools/BitmapManagerTool/src/NewBrushDialogImpl.h \
		  CoreResourceTools/BitmapManagerTool/src/SelectShapeDialogImpl.h \
		  CoreResourceTools/BitmapResourceTool/src/BitmapDirWalk.h \
		  CoreResourceTools/BitmapResourceTool/src/BitmapResourceTool.h \
		  CoreResourceTools/BrushChooserTool/src/BrushChooserTool.h \
		  CoreResourceTools/BrushChooserTool/src/BrushDirWalk.h \
		  CoreResourceTools/ColorChooserTool/src/ColorChooserButton.h \
		  CoreResourceTools/ColorChooserTool/src/ColorChooserTool.h \
		  CoreResourceTools/src/CoreResourceTools.h \
		  CoreTools/ColorPickerTool/src/Color.h \
		  CoreTools/ColorPickerTool/src/ColorPickerTool.h \
		  CoreTools/NavigationTool/src/NavigationTool.h \
		  CoreTools/SelectionTool/src/ScanConversion.h \
		  CoreTools/SelectionTool/src/SelectionTool.h \
		  CoreTools/src/CoreTools.h \
		  DataStructures/src/AdjacencyGraph.h \
		  DataStructures/src/AssociatedSurfels.h \
		  DataStructures/src/Cluster.h \
		  DataStructures/src/Clustering.h \
		  DataStructures/src/ClusterNode.h \
		  DataStructures/src/FifoQueue.h \
		  DataStructures/src/kdTree.h \
		  DataStructures/src/NeighbourHood.h \
		  DataStructures/src/ObjListDbl.h \
		  DataStructures/src/PriorityQueue.h \
		  DataStructures/src/QueryGrid.h \
		  DataStructures/src/SurfelCollection.h \
		  FileFormatPluginManager/src/FileFormatPluginDirWalk.h \
		  FileFormatPluginManager/src/FileFormatPluginLoader.h \
		  FileFormatPluginManager/src/FileFormatPluginManager.h \
		  FileFormatPluginManager/src/FileFormatPluginRegistry.h \
		  Interfaces/src/FileFormatPluginInterface.h \
		  Interfaces/src/FrameBufferInterface.h \
		  Interfaces/src/PluginInterface.h \
		  Interfaces/src/RendererConfigurationInterface.h \
		  Interfaces/src/RendererInterface.h \
		  Interfaces/src/ResourceToolInterface.h \
		  Interfaces/src/SurfelInterface.h \
		  Interfaces/src/ToolInterface.h \
		  MarkerManager/src/Marker2D.h \
		  MarkerManager/src/Marker3D.h \
		  MarkerManager/src/MarkerInterface.h \
		  MarkerManager/src/MarkerManager.h \
		  MarkerManager/src/MarkerUtils.h \
		  OrthoMapper/src/OrthoMapper.h \
		  Painter/src/Painter.h \
		  PluginManager/src/PluginDirWalk.h \
		  PluginManager/src/PluginLoader.h \
		  PluginManager/src/PluginManager.h \
		  PluginManager/src/PluginRegistry.h \
		  RendererManager/src/RendererDirWalk.h \
		  RendererManager/src/RendererLoader.h \
		  RendererManager/src/RendererManager.h \
		  RendererManager/src/RendererRegistry.h \
		  Scene/src/Object.h \
		  Scene/src/Scene.h \
		  ToolManager/src/ToolDirWalk.h \
		  ToolManager/src/ToolLoader.h \
		  ToolManager/src/ToolManager.h \
		  ToolManager/src/ToolRegistry.h \
		  Transformer/src/Transformer.h \
		  src/ClipBoard.h \
		  src/CoreDLL.h \
		  src/DefaultMouseHandler.h
SOURCES		= Brush/src/Brush.cpp \
		  Brush/src/BrushChannel.cpp \
		  Configuration/src/Configuration.cpp \
		  CoreResourceTools/BitmapManagerTool/src/BitmapManagerDialogImpl.cpp \
		  CoreResourceTools/BitmapManagerTool/src/BitmapManagerTool.cpp \
		  CoreResourceTools/BitmapManagerTool/src/BrushScrollView.cpp \
		  CoreResourceTools/BitmapManagerTool/src/NewBrushDialogImpl.cpp \
		  CoreResourceTools/BitmapManagerTool/src/SelectShapeDialogImpl.cpp \
		  CoreResourceTools/BitmapResourceTool/src/BitmapDirWalk.cpp \
		  CoreResourceTools/BitmapResourceTool/src/BitmapResourceTool.cpp \
		  CoreResourceTools/BrushChooserTool/src/BrushChooserTool.cpp \
		  CoreResourceTools/BrushChooserTool/src/BrushDirWalk.cpp \
		  CoreResourceTools/ColorChooserTool/src/ColorChooserButton.cpp \
		  CoreResourceTools/ColorChooserTool/src/ColorChooserTool.cpp \
		  CoreResourceTools/src/CoreResourceTools.cpp \
		  CoreTools/ColorPickerTool/src/ColorPickerTool.cpp \
		  CoreTools/NavigationTool/src/NavigationTool.cpp \
		  CoreTools/SelectionTool/src/ScanConversion.cpp \
		  CoreTools/SelectionTool/src/SelectionTool.cpp \
		  CoreTools/src/CoreTools.cpp \
		  DataStructures/src/AdjacencyGraph.cpp \
		  DataStructures/src/AssociatedSurfels.cpp \
		  DataStructures/src/Cluster.cpp \
		  DataStructures/src/Clustering.cpp \
		  DataStructures/src/ClusterNode.cpp \
		  DataStructures/src/FifoQueue.cpp \
		  DataStructures/src/kdTree.cpp \
		  DataStructures/src/NeighbourHood.cpp \
		  DataStructures/src/ObjListDbl.cpp \
		  DataStructures/src/PriorityQueue.cpp \
		  DataStructures/src/QueryGrid.cpp \
		  DataStructures/src/SurfelCollection.cpp \
		  FileFormatPluginManager/src/FileFormatPluginDirWalk.cpp \
		  FileFormatPluginManager/src/FileFormatPluginLoader.cpp \
		  FileFormatPluginManager/src/FileFormatPluginManager.cpp \
		  FileFormatPluginManager/src/FileFormatPluginRegistry.cpp \
		  Interfaces/src/RendererInterface.cpp \
		  Interfaces/src/SurfelInterface.cpp \
		  Interfaces/src/ToolInterface.cpp \
		  MarkerManager/src/Marker2D.cpp \
		  MarkerManager/src/Marker3D.cpp \
		  MarkerManager/src/MarkerManager.cpp \
		  MarkerManager/src/MarkerUtils.cpp \
		  OrthoMapper/src/OrthoMapper.cpp \
		  Painter/src/Painter.cpp \
		  PluginManager/src/PluginDirWalk.cpp \
		  PluginManager/src/PluginLoader.cpp \
		  PluginManager/src/PluginManager.cpp \
		  PluginManager/src/PluginRegistry.cpp \
		  RendererManager/src/RendererDirWalk.cpp \
		  RendererManager/src/RendererLoader.cpp \
		  RendererManager/src/RendererManager.cpp \
		  RendererManager/src/RendererRegistry.cpp \
		  Scene/src/Object.cpp \
		  Scene/src/Scene.cpp \
		  ToolManager/src/ToolDirWalk.cpp \
		  ToolManager/src/ToolLoader.cpp \
		  ToolManager/src/ToolManager.cpp \
		  ToolManager/src/ToolRegistry.cpp \
		  Transformer/src/Transformer.cpp \
		  src/ClipBoard.cpp \
		  src/DefaultMouseHandler.cpp

INTERFACES	= CoreResourceTools/BitmapManagerTool/UI/BitmapManagerDialog.ui \
		  CoreResourceTools/BitmapManagerTool/UI/NewBrushDialog.ui \
		  CoreResourceTools/BitmapManagerTool/UI/SelectShapeDialog.ui \
		  CoreTools/NavigationTool/UI/NavigationConfigurationWidget.ui \
		  CoreTools/SelectionTool/UI/SelectionConfigurationWidget.ui \
		  OrthoMapper/UI/ImageViewer.ui

TARGET		= Core

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../bin/linux/$${BUILDTYPE}

LIBS	       += -L../../bin/linux/$${BUILDTYPE} -lDataTypes -lUtilities -ldl

QMAKE_POST_LINK = mkdir -p ../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp CoreTools/SelectionTool/Icons/BrushIcon.png \
                     CoreTools/SelectionTool/Icons/BrushIcon_disabled.png \
                     CoreTools/SelectionTool/Icons/EllipseIcon.png \
                     CoreTools/SelectionTool/Icons/EllipseIcon_disabled.png \
                     CoreTools/SelectionTool/Icons/FloodFillIcon.png \
                     CoreTools/SelectionTool/Icons/FloodFillIcon_disabled.png \
                     CoreTools/SelectionTool/Icons/LassoIcon.png \
                     CoreTools/SelectionTool/Icons/LassoIcon_disabled.png \
                     CoreTools/SelectionTool/Icons/PolygonIcon.png \
                     CoreTools/SelectionTool/Icons/PolygonIcon_disabled.png \
                     CoreTools/SelectionTool/Icons/RectangleIcon.png \
                     CoreTools/SelectionTool/Icons/RectangleIcon_disabled.png \
                     CoreTools/NavigationTool/Icons/BuilderIcon.png \
                     CoreTools/NavigationTool/Icons/BuilderIcon_disabled.png \
                     CoreTools/NavigationTool/Icons/NavigationIcon.png \
                     CoreTools/NavigationTool/Icons/NavigationIcon_disabled.png \
                     CoreTools/ColorPickerTool/Icons/ColorPickerToolIcon.png \
                     CoreTools/ColorPickerTool/Icons/ColorPickerToolIcon_disabled.png \
                     CoreResourceTools/ColorChooserTool/Icons/ColorChooserIcon.png \
                     CoreResourceTools/BrushChooserTool/Icons/BrushChooserToolIcon.png \
                     CoreResourceTools/BrushChooserTool/Icons/BrushChooserToolIcon_disabled.png \
                     CoreResourceTools/BitmapManagerTool/Icons/BitmapManagerToolIcon.png \
                     CoreResourceTools/BitmapManagerTool/Icons/BitmapManagerToolIcon_disabled.png \
                     ../../bin/linux/$${BUILDTYPE}/Resources
