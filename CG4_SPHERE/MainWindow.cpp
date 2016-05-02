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
#include "Line.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include "CheckBoxGroup.h"
#include "SphereProjector.h"
#include "SphereProjectorController.h"
#include "LoadingFileException.h"
#include "ComboBoxGroup.h"
#include <QStringList>

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

    filtrationGroup =  new ComboBoxGroup;
    filtrationGroup->setLabel("Filtration");
    QStringList list;
    list.push_back("bilinear");
    list.push_back("nearest");
    filtrationGroup->setList(list);
    controlsLayout->addWidget(filtrationGroup);

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

    SphereProjector* projector = new  SphereProjector();
    projector->setScale(0);
    projector->setX(0);
    projector->setY(0);
    projector->setFiltration(FiltrationType::bilinear);
    projector->setTexutureFile("earth.jpg");

    canvasWidget->add(projector);

    control = new SphereProjectorController;
    control->setXGroup(xGroup);
    control->setYGroup(yGroup);
    control->setScaleGroup(scaleGroup);
    control->setFiltrationGroup(filtrationGroup);
    control->setSphereProjector(projector);

    connect(canvasWidget, SIGNAL(centerPositionChanged(int,int)), control, SLOT(xyOffsetChanged(int,int)));
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
        SphereProjector* projector = new SphereProjector;
        projector->loadFromJson(json);
        control->setSphereProjector(projector);
        canvasWidget->removeAll();
        canvasWidget->add(projector);
    }
    catch (ParserException e) {
        QMessageBox::critical(this, "Parsing error", e.what());
    }
}

void MainWindow::saveConfig() {
    auto savePath = QFileDialog::getSaveFileName();
    auto json = control->getSphereProjector()->saveToJson();
    QFile file(savePath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream;
    stream.setDevice(&file);
    stream << QString(QJsonDocument(json).toJson());
    file.close();
}
