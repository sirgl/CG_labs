#include "MainWindow.h"
#include "Circle.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QJsonDocument>
#include "ParserException.h"
#include "Tools.h"
#include "Config.h"
#include "Axis.h"
#include "BezierCurve.h"
#include "Line.h"

void MainWindow::initMenu(){
    auto fileMenu = menuBar()->addMenu("File");
    auto loadConfigurationAction = fileMenu->addAction("Load config");
    auto saveConfigurationAction = fileMenu->addAction("Save config");
    connect(loadConfigurationAction, SIGNAL(triggered()), this, SLOT(loadConfig()));
    connect(saveConfigurationAction, SIGNAL(triggered()), this, SLOT(saveConfig()));
}

void MainWindow::initControlsBox(){
    QGroupBox* controlsBox = new QGroupBox("Controls");
    QVBoxLayout* controlsLayout = new QVBoxLayout();
    controlsBox->setLayout(controlsLayout);
    controlsBox->setFixedWidth(200);
    xGroup = new PickGroup;
    xGroup->setLabel("X offset");
    xGroup->setMin(MIN_X_CONTROLS);
    xGroup->setMax(MAX_X_CONTROLS);
    controlsLayout->addWidget(xGroup);

    yGroup = new PickGroup;
    yGroup->setLabel("Y offset");
    yGroup->setMin(MIN_Y_CONTROLS);
    yGroup->setMax(MAX_Y_CONTROLS);
    controlsLayout->addWidget(yGroup);

    scaleGroup = new PickGroup;
    scaleGroup->setLabel("Scale");
    scaleGroup->setMin(MIN_SCALE_CONTROLS);
    scaleGroup->setMax(MAX_SCALE_CONTROLS);
    controlsLayout->addWidget(scaleGroup);

    mainLayout->addWidget(controlsBox);
}

void MainWindow::initMainBox(){
    QGroupBox* mainBox = new QGroupBox();
    mainLayout = new QHBoxLayout();
    mainBox->setLayout(mainLayout);

    canvasWidget = new CanvasWidget(this);
    mainLayout->addWidget(canvasWidget);

    setCentralWidget(mainBox);
}

void MainWindow::initLemniscateController(){

    Axis* xAxis = new Axis;
    Axis* yAxis = new Axis;
    yAxis->setMode(true);
    canvasWidget->add(xAxis);
    canvasWidget->add(yAxis);



    BezierCurve* curve = new BezierCurve;

    QVector<BezierPoint> points;
    BezierPoint pointsArr[] = {
        BezierPoint(-100, 20, true),
        BezierPoint(0, -200.5, false),
        BezierPoint(200, 30, true),
        BezierPoint(20, 70, true)
    };
    points.push_back(pointsArr[0]);
    points.push_back(pointsArr[1]);
    points.push_back(pointsArr[2]);
    points.push_back(pointsArr[3]);
    curve->setPoints(points);
    curve->setOutline(true);
//    curve->setScale(1);
    curve->setXOffset(50);
    curve->setXOffset(-50);
    canvasWidget->add(curve);


    controller = new CurveController;
    controller->setXGroup(xGroup);
    controller->setYGroup(yGroup);
    controller->setScaleGroup(scaleGroup);
    controller->setCurve(curve);

//    lemniscateController = new LemniscateController;
//    lemniscateController->setX1Group(x1Group);
//    lemniscateController->setY1Group(y1Group);
//    lemniscateController->setX2Group(x2Group);
//    lemniscateController->setY2Group(y2Group);
//    lemniscateController->setLemniscate(lem);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent){
    initMainBox();
    initControlsBox();
    initLemniscateController();
    initMenu();
    resize(800, 600);
}

MainWindow::~MainWindow(){
}


void MainWindow::loadConfig() {
    QString path = QFileDialog::getOpenFileName();
    if (path == "") {
        return;
    }
    try{
//        auto json = Tools::loadJsonFromFile(path);
//        Config config;
//        config.loadFromJson(json);
//        lemniscateController->setLemniscate(config.lemniscate);
//        canvasWidget->removeAll();
//        Axis* xAxis = new Axis;
//        Axis* yAxis = new Axis;
//        yAxis->setMode(true);
//        canvasWidget->add(xAxis);
//        canvasWidget->add(yAxis);
//        canvasWidget->add(config.lemniscate);
    }
    catch (ParserException e) {
        QMessageBox::critical(this, "Parsing error", e.what());
    }
}

void MainWindow::saveConfig() {
//    auto savePath = QFileDialog::getSaveFileName();
//    auto panel = new Panel(canvasWidget->getCanvasWidth(), canvasWidget->getCanvasHeight());
//    Config config(lemniscateController->getLemniscate(), panel);
//    auto json = config.saveToJson();
//    QFile file(savePath);
//    file.open(QIODevice::WriteOnly);
//    QTextStream stream;
//    stream.setDevice(&file);
//    stream << QString(QJsonDocument(json).toJson());
//    file.close();
}
