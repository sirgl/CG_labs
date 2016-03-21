#pragma once

#include <QString>
#include <QJsonObject>

namespace Tools{
    QJsonObject loadJsonFromFile(QString path);
    long long sqr(long long number);
    float sqr(float number);
    double sqr(double number);
}
