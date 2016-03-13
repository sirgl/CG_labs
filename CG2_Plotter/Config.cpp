#include "Config.h"
#include "ParserException.h"
#include <QJsonArray>

void Config::loadFromJson(QJsonObject object){
    QJsonValueRef lemniscate = object["lemniscate"];
    if (!lemniscate.isObject()) {
        throw ParserException("Lemniscate must have object type");
    }
    QJsonObject lemniscateObj = lemniscate.toObject();
    Lemniscate* lem = new Lemniscate;
    lem->loadFromJson(lemniscateObj);
    this->lemniscate = lem;
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
    object["lemniscate"] = lemniscate->saveToJson();
    object["panel"] = panel->saveToJson();
    return object;
}

Config::Config(){
}

Config::Config(Lemniscate *lemniscate, Panel *panel){
    this->lemniscate = lemniscate;
    this->panel = panel;
}
