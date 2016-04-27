#pragma once
#include <QJsonObject>
#include "Lemniscate.h"
#include "Panel.h"

struct Config {
    Lemniscate* lemniscate;
    Panel* panel;

    Config();

    Config(Lemniscate* lemniscate, Panel* panel);

    void loadFromJson(QJsonObject object);

    QJsonObject saveToJson();
};
