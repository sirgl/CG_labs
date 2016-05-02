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
#include <QCheckBox>
#include <QHBoxLayout>
#include "CheckBoxGroup.h"

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

    fillBox = new CheckBoxGroup;
    fillBox->setLabel("Fill");
    controlsLayout->addWidget(fillBox);

    outlineBox = new CheckBoxGroup;
    outlineBox->setLabel("Outline");
    controlsLayout->addWidget(outlineBox);

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

void MainWindow::initCurveController(){

    Axis* xAxis = new Axis;
    Axis* yAxis = new Axis;
    yAxis->setMode(true);
    canvasWidget->add(xAxis);
    canvasWidget->add(yAxis);

    BezierCurve* curve = new BezierCurve;

    QVector<BezierPoint> points;
    BezierPoint pointsArr[] = {
        BezierPoint(-100, 20, true),
        BezierPoint(0, 90.5, false),
        BezierPoint(200, 20, true),
        BezierPoint(20, 70, true),
        BezierPoint(90, -90, false),
        BezierPoint(-20, 70, true),
        BezierPoint(90, -90, false)
    };
    for (int i = 0; i < 7; ++i) {
        points.push_back(pointsArr[i]);
    }

    QVector<QVector<BezierPoint>> pointsSet;
    pointsSet.push_back(points);
    curve->setPointsSets(pointsSet);
    curve->setXOffset(50);
    curve->setXOffset(-50);
    canvasWidget->add(curve);

    //DEBUG:
//    auto path = "./bez3.json";
//    auto json = Tools::loadJsonFromFile(path);
//    BezierCurve* curve = new BezierCurve;
//    curve->loadFromJson(json);
//    canvasWidget->removeAll();
//    Axis* xAxis = new Axis;
//    Axis* yAxis = new Axis;
//    yAxis->setMode(true);
//    canvasWidget->add(xAxis);
//    canvasWidget->add(yAxis);
//    canvasWidget->add(curve);
    //END

    controller = new CurveController;
    controller->setXGroup(xGroup);
    controller->setYGroup(yGroup);
    controller->setScaleGroup(scaleGroup);
    controller->setFillGroup(fillBox);
    controller->setOutlineGroup(outlineBox);
    controller->setCurve(curve);
    connect(canvasWidget, SIGNAL(centerPositionChanged(double,double)), controller, SLOT(centerPositionChanged(double,double)));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent){
    initMainBox();
    initControlsBox();
    initCurveController();
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
        auto json = Tools::loadJsonFromFile(path);
        BezierCurve* curve = new BezierCurve;
        curve->loadFromJson(json);
        controller->setCurve(curve);
        canvasWidget->removeAll();
        Axis* xAxis = new Axis;
        Axis* yAxis = new Axis;
        yAxis->setMode(true);
        canvasWidget->add(xAxis);
        canvasWidget->add(yAxis);
        canvasWidget->add(curve);
    }
    catch (ParserException e) {
        QMessageBox::critical(this, "Parsing error", e.what());
    }
}

void MainWindow::saveConfig() {
    auto savePath = QFileDialog::getSaveFileName();
    auto json = controller->getCurve()->saveToJson();
    QFile file(savePath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream;
    stream.setDevice(&file);
    stream << QString(QJsonDocument(json).toJson());
    file.close();
}
