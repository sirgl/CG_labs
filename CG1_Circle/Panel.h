#pragma once

#include <QJsonObject>


class Panel {
    int xSize;
    int ySize;

public:

    Panel(int xSize, int ySize);

    Panel();

    void loadFromJson(QJsonObject& object);

    QJsonObject saveToJson();

    const Panel& operator=(const Panel& panel);
};
