#include "CoreApplication.h"
#include "ParserException.h"
#include "Canvas.h"
#include <QImageWriter>
#include <QColor>
#include <QDebug>
#include <iostream>
#include "Config.h"
#include "Tools.h"
#include "Axis.h"
#include <QImage>

CoreApplication::CoreApplication(){
    
}

int CoreApplication::exec(int argc, char **argv){
    try {
        QString configPath = argv[1];
        QString outputName = argv[2];
        auto json = Tools::loadJsonFromFile(configPath);
        Config config;
        config.loadFromJson(json);

        Canvas canvas;
        canvas.setSize(config.panel->getXSize(), config.panel->getYSize());
        canvas.add(config.lemniscate);
        Axis* xAxis = new Axis;
        Axis* yAxis = new Axis;
        yAxis->setMode(true);
        canvas.add(xAxis);
        canvas.add(yAxis);
        canvas.draw();
        QImage* image = canvas.getImage();

        QImageWriter writer;
        writer.setFormat("png");
        writer.setFileName(outputName);
        writer.write(*image);
        return EXIT_SUCCESS;
    } catch (ParserException& e) {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_FAILURE;
}
