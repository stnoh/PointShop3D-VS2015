include ( ../../buildcfg.pro )

TEMPLATE	= lib
CONFIG		= qt thread $${BUILDTYPE}
HEADERS		= ANN/ANN.h \
		  ANN/ANNperf.h \
		  ANN/ANNx.h \
		  ANN/bd_tree.h \
		  ANN/kd_pr_search.h \
		  ANN/kd_search.h \
		  ANN/kd_split.h \
		  ANN/kd_tree.h \
		  ANN/kd_util.h \
		  ANN/pr_queue.h \
		  ANN/pr_queue_k.h \
		  ANN/win32_values.h \
		  DirWalk/src/DirWalk.h \
		  UserMessages/src/SplashScreen.h \
		  UserMessages/src/StatusBar.h \
		  src/ColorConversion.h \
		  src/Common.h \
		  src/FileTools.h \
		  src/ImageOps.h \
		  src/Matrix.h \
		  src/MyMathTools.h \
		  src/StringTools.h \
		  src/TrackBall.h \
		  src/UtilitiesDLL.h \
		  src/Version.h \
                  MagicSoft/MagicFMLibType.h \
                  MagicSoft/MgcAppr3DPlaneFit.h \
                  MagicSoft/MgcBox2.h \
                  MagicSoft/MgcCont2DMinBox.h \
                  MagicSoft/MgcConvexHull2D.h \
                  MagicSoft/MgcDelaunay2D.h \
                  MagicSoft/MgcEigen.h \
                  MagicSoft/MgcLinearSystem.h \
                  MagicSoft/MgcMath.h \
                  MagicSoft/MgcMatrix2.h \
                  MagicSoft/MgcMatrix3.h \
                  MagicSoft/MgcQuaternion.h \
                  MagicSoft/MgcRTLib.h \
                  MagicSoft/MgcVector2.h \
                  MagicSoft/MgcVector3.h \
                  MagicSoft/MgcBox2.inl \
                  MagicSoft/MgcConvexHull2D.inl \
                  MagicSoft/MgcDelaunay2D.inl \
                  MagicSoft/MgcEigen.inl \
                  MagicSoft/MgcLinearSystem.inl \
                  MagicSoft/MgcMath.inl \
                  MagicSoft/MgcMatrix2.inl \
                  MagicSoft/MgcMatrix3.inl \
                  MagicSoft/MgcVector2.inl \
                  MagicSoft/MgcVector3.inl
SOURCES		= ANN/ANN.cpp \
		  ANN/bd_pr_search.cpp \
		  ANN/bd_search.cpp \
		  ANN/bd_tree.cpp \
		  ANN/brute.cpp \
		  ANN/kd_pr_search.cpp \
		  ANN/kd_search.cpp \
		  ANN/kd_split.cpp \
		  ANN/kd_tree.cpp \
		  ANN/kd_util.cpp \
		  ANN/perf.cpp \
		  DirWalk/src/DirWalk.cpp \
		  UserMessages/src/SplashScreen.cpp \
		  UserMessages/src/StatusBar.cpp \
		  src/ColorConversion.cpp \
		  src/FileTools.cpp \
		  src/ImageOps.cpp \
		  src/Matrix.cpp \
		  src/MyMathTools.cpp \
		  src/StringTools.cpp \
		  src/TrackBall.cpp \
		  src/Version.cpp \
                  MagicSoft/MgcAppr3DPlaneFit.cpp \
                  MagicSoft/MgcBox2.cpp \
                  MagicSoft/MgcCont2DMinBox.cpp \
                  MagicSoft/MgcConvexHull2D.cpp \
                  MagicSoft/MgcDelaunay2D.cpp \
                  MagicSoft/MgcEigen.cpp \
                  MagicSoft/MgcLinearSystem.cpp \
                  MagicSoft/MgcMath.cpp \
                  MagicSoft/MgcMatrix2.cpp \
                  MagicSoft/MgcMatrix3.cpp \
                  MagicSoft/MgcQuaternion.cpp \
                  MagicSoft/MgcVector2.cpp \
                  MagicSoft/MgcVector3.cpp

INTERFACES	=

TARGET		= Utilities

OBJECTS_DIR     = bin/linux/$${BUILDTYPE}

MOC_DIR		= bin/linux/$${BUILDTYPE}

DESTDIR		= ../../bin/linux/$${BUILDTYPE}

LIBS	       += -L../../bin/linux/$${BUILDTYPE}

QMAKE_POST_LINK = mkdir -p ../../bin/linux/$${BUILDTYPE}/Resources && \
                  cp UserMessages/Splash/Splash.png ../../bin/linux/$${BUILDTYPE}/Resources
