#-------------------------------------------------
#
# Project created by QtCreator 2014-12-07T15:26:19
#
#-------------------------------------------------

QT       += core gui

TARGET = MotionDetector
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#INCLUDEPATH += D:\Devel\opencv\opencv-mingw\install\include

#LIBS += -L"D:\Devel\opencv\opencv-mingw\install\x64\mingw\bin"
#LIBS += -lopencv_core300 -lopencv_video300 -lopencv_imgproc300
#LIBS += -lopencv_highgui300 -lopencv_features2d300 -lopencv_videoio300
#LIBS += -lopencv_calib3d300 -lopencv_imgcodecs300

LIBS += `pkg-config opencv --libs`

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    application.cpp \
    manager.cpp \
    testimageprovider.cpp \
    bgsubtractor.cpp \
    objectdetector.cpp \
    scene.cpp \
    stereovision.cpp \
    stereocamera.cpp

HEADERS += \
    application.h \
    manager.h \
    testimageprovider.h \
    bgsubtractor.h \
    helpertypes.h \
    objectdetector.h \
    scene.h \
    stereovision.h \
    stereocamera.h

RESOURCES += \
    resources.qrc
