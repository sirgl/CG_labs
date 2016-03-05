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
    xGroup = new PickGroup;
    xGroup->setLabel("Position X");
    xGroup->setMin(MIN_X_CONTROLS);
    xGroup->setMax(MAX_X_CONTROLS);
    controlsLayout->addWidget(xGroup);
    yGroup = new PickGroup;
    yGroup->setLabel("Position Y");
    yGroup->setMin(MIN_Y_CONTROLS);
    yGroup->setMax(MAX_Y_CONTROLS);
    controlsLayout->addWidget(yGroup);
    rGroup = new PickGroup;
    rGroup->setLabel("R");
    rGroup->setMin(MIN_R_CONTROLS);
    rGroup->setMax(MAX_R_CONTROLS);
    controlsLayout->addWidget(rGroup);

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

void MainWindow::initCircleController(){
    Circle* circle = new Circle();
    circle->setPosition(100, 100);
    circle->setRadius(30);

    Axis* xAxis = new Axis;
    Axis* yAxis = new Axis;
    yAxis->setMode(true);
    canvasWidget->add(xAxis);
    canvasWidget->add(yAxis);

    Lemniscate* lem = new Lemniscate;
    lem->setFocuses(-200, 0, 200, 0);
    canvasWidget->add(lem);

    canvasWidget->add(circle);

    circleController = new CircleController;
    circleController->setParent(this);
    circleController->setXGroup(xGroup);
    circleController->setYGroup(yGroup);
    circleController->setRGroup(rGroup);
    circleController->setCircle(circle);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent){
    initMainBox();
    initControlsBox();
    initCircleController();
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
        circleController->setCircle(config.circle);
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
    Config config(circleController->getCircle(), panel);
    auto json = config.saveToJson();
    QFile file(savePath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream;
    stream.setDevice(&file);
    stream << QString(QJsonDocument(json).toJson());
    file.close();
}
