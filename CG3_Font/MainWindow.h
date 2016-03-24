#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "PickGroup.h"
#include "CircleController.h"
#include "CanvasWidget.h"
#include "CurveController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    PickGroup* xGroup;
    PickGroup* yGroup;
    PickGroup* scaleGroup;

    QHBoxLayout* mainLayout;

//    CircleController* circleController;
    CurveController* controller;

    CanvasWidget* canvasWidget;

    static const int MIN_X_CONTROLS = -1000;
    static const int MAX_X_CONTROLS = 1000;

    static const int MIN_Y_CONTROLS = -1000;
    static const int MAX_Y_CONTROLS = 1000;

    static const int MIN_SCALE_CONTROLS = -1000;
    static const int MAX_SCALE_CONTROLS = 1000;

    void initMenu();
    void initControlsBox();
    void initMainBox();
    void initLemniscateController();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void saveConfig();
    void loadConfig();
};

#endif // MAINWINDOW_H
