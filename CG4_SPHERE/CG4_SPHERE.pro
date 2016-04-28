#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T11:03:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CG4_SPHERE
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        MainWindow.cpp \
    Circle.cpp \
    Config.cpp \
    Panel.cpp \
    CanvasWidget.cpp \
    Canvas.cpp \
    Drawable.cpp \
    PickGroup.cpp \
    CircleController.cpp \
    Tools.cpp \
    CoreApplication.cpp \
    Axis.cpp \
    DrawingTools.cpp \
    BezierPoint.cpp \
    Line.cpp \
    CheckBoxGroup.cpp \
    SphereProjector.cpp \
    SphereProjectorController.cpp

HEADERS  += MainWindow.h \
    Circle.h \
    Drawable.h \
    Config.h \
    Panel.h \
    CanvasWidget.h \
    ParserException.h \
    Canvas.h \
    PickGroup.h \
    CircleController.h \
    Tools.h \
    CoreApplication.h \
    Axis.h \
    Point.h \
    DrawingTools.h \
    BezierPoint.h \
    Line.h \
    CheckBoxGroup.h \
    SphereProjector.h \
    SphereProjectorController.h \
    LoadingFileException.h

FORMS    +=

RESOURCES +=
