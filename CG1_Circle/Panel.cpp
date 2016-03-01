#include "Panel.h"
#include "ParserException.h"

int Panel::getXSize() const{
    return xSize;
}

int Panel::getYSize() const{
    return ySize;
}

Panel::Panel(int xSize, int ySize) : xSize(xSize), ySize(ySize) {}

Panel::Panel() : Panel(0, 0){}

void Panel::loadFromJson(QJsonObject &object){
    auto sizeTmp = object["size"];
    if (!sizeTmp.isObject()) {
        throw ParserException("Size must be object");
    }
    auto size = sizeTmp.toObject();
    auto xSizeTmp = size["x"];
    if (!xSizeTmp.isDouble()) {
        throw ParserException("x size must be numeric");
    }
    int x = xSizeTmp.toInt();
    auto ySizeTmp = size["y"];
    if (!ySizeTmp.isDouble()) {
        throw ParserException("y size must be numeric");
    }
    int y = ySizeTmp.toInt();
    xSize = x;
    ySize = y;
}

QJsonObject Panel::saveToJson(){
    QJsonObject panel;
    QJsonObject size;
    size["x"] = xSize;
    size["y"] = ySize;
    panel["size"] = size;
    return panel;
}

const Panel &Panel::operator=(const Panel &panel){
    xSize = panel.xSize;
    ySize = panel.ySize;
    return *this;
}
