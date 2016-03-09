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
#include "Lemniscate.h"

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
    x1Group = new PickGroup;
    x1Group->setLabel("X 1");
    x1Group->setMin(MIN_X_CONTROLS);
    x1Group->setMax(MAX_X_CONTROLS);
    controlsLayout->addWidget(x1Group);

    y1Group = new PickGroup;
    y1Group->setLabel("Y 1");
    y1Group->setMin(MIN_Y_CONTROLS);
    y1Group->setMax(MAX_Y_CONTROLS);
    controlsLayout->addWidget(y1Group);

    x2Group = new PickGroup;
    x2Group->setLabel("X 2");
    x2Group->setMin(MIN_X_CONTROLS);
    x2Group->setMax(MAX_X_CONTROLS);
    controlsLayout->addWidget(x2Group);

    y2Group = new PickGroup;
    y2Group->setLabel("Y 2");
    y2Group->setMin(MIN_X_CONTROLS);
    y2Group->setMax(MAX_X_CONTROLS);
    controlsLayout->addWidget(y2Group);

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
    Circle* circle = new Circle();
    circle->setPosition(100, 100);
    circle->setRadius(30);

    Axis* xAxis = new Axis;
    Axis* yAxis = new Axis;
    yAxis->setMode(true);
    canvasWidget->add(xAxis);
    canvasWidget->add(yAxis);

    Lemniscate* lem = new Lemniscate;
    lem->setFocuses(0, -10, 0, 10);
    canvasWidget->add(lem);

    lemniscateController = new LemniscateController;
    lemniscateController->setX1Group(x1Group);
    lemniscateController->setY1Group(y1Group);
    lemniscateController->setX2Group(x2Group);
    lemniscateController->setY2Group(y2Group);
    lemniscateController->setLemniscate(lem);

//    canvasWidget->add(circle);

//    circleController = new CircleController;
//    circleController->setParent(this);
//    circleController->setXGroup(xGroup);
//    circleController->setYGroup(yGroup);
//    circleController->setRGroup(rGroup);
//    circleController->setCircle(circle);
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
        auto json = Tools::loadJsonFromFile(path);
        Config config;
        config.loadFromJson(json);
//        circleController->setCircle(config.circle);
        //TODO
        canvasWidget->removeAll();
        canvasWidget->add(config.circle);
    }
    catch (ParserException e) {
        QMessageBox::critical(this, "Parsing error", e.what());
    }
}

void MainWindow::saveConfig() {
    auto savePath = QFileDialog::getSaveFileName();
    auto panel = new Panel(canvasWidget->getCanvasWidth(), canvasWidget->getCanvasHeight());
    //TODO
//    Config config(circleController->getCircle(), panel);
//    auto json = config.saveToJson();
//    QFile file(savePath);
//    file.open(QIODevice::WriteOnly);
//    QTextStream stream;
//    stream.setDevice(&file);
//    stream << QString(QJsonDocument(json).toJson());
//    file.close();
}
