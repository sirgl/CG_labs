#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "PickGroup.h"
#include "CircleController.h"
#include "CanvasWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    PickGroup* xGroup;
    PickGroup* yGroup;
    PickGroup* rGroup;

    QHBoxLayout* mainLayout;

    CircleController* circleController;
    CanvasWidget* canvasWidget;

    static const int MIN_X_CONTROLS = -10000;
    static const int MIN_Y_CONTROLS = -10000;
    static const int MIN_R_CONTROLS = 0;
    static const int MAX_X_CONTROLS = 10000;
    static const int MAX_Y_CONTROLS = 10000;
    static const int MAX_R_CONTROLS = 10000;


    void initMenu();
    void initControlsBox();
    void initMainBox();
    void initCircleController();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void saveConfig();
    void loadConfig();
};

#endif // MAINWINDOW_H
