#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "PickGroup.h"
#include "CircleController.h"
#include "CanvasWidget.h"
#include "LemniscateController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    PickGroup* x1Group;
    PickGroup* y1Group;
    PickGroup* x2Group;
    PickGroup* y2Group;

    QHBoxLayout* mainLayout;

//    CircleController* circleController;
    LemniscateController* lemniscateController;

    CanvasWidget* canvasWidget;

    static const int MIN_X_CONTROLS = -100;
    static const int MIN_Y_CONTROLS = -100;
    static const int MAX_X_CONTROLS = 100;
    static const int MAX_Y_CONTROLS = 100;


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
