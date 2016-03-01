#include "Config.h"
#include "ParserException.h"
#include <QJsonArray>

void Config::loadFromJson(QJsonObject object){
    QJsonValueRef circles = object["circles"];
    if (!circles.isArray()) {
        throw ParserException("Circles must have array type");
    }
    QJsonArray circlesArr = circles.toArray();
    if(circlesArr.size() < 1) {
        throw ParserException("Circles must contain at least one element");
    }
    QJsonValueRef circle = circlesArr[0];
    if(!circle.isObject()) {
        throw ParserException("First circle must have object type");
    }
    QJsonObject circleObj = circle.toObject();
    this->circle  = new Circle();
    this->circle->loadFromJson(circleObj);

    QJsonValueRef panel = object["panel"];
    if(!panel.isObject()) {
        throw ParserException("Panel must have object type");
    }
    QJsonObject panelObj = panel.toObject();
    this->panel = new Panel();
    this->panel->loadFromJson(panelObj);
}

QJsonObject Config::saveToJson(){
    QJsonObject object;
    QJsonArray circles;
    circles.append(circle->saveToJson());
    object["circles"] = circles;
    object["panel"] = panel->saveToJson();
    return object;
}

Config::Config(){

}

Config::Config(Circle *circle, Panel *panel){
    this->circle = circle;
    this->panel = panel;
}
