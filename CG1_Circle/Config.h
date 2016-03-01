#pragma once
#include <QJsonObject>
#include "Circle.h"
#include "Panel.h"

struct Config {
    Circle* circle;
    Panel* panel;

    Config();

    Config(Circle* circle, Panel* panel);

    void loadFromJson(QJsonObject object);

    QJsonObject saveToJson();
};
