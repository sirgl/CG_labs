#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T11:03:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CG3_Font
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
    BezierCurve.cpp \
    Line.cpp \
    CurveController.cpp \
    CheckBoxGroup.cpp

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
    BezierCurve.h \
    Line.h \
    CurveController.h \
    CheckBoxGroup.h

FORMS    +=
