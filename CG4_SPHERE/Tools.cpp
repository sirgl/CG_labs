#include "Tools.h"
#include <QFile>
#include <QJsonDocument>
#include "ParserException.h"

QJsonObject Tools::loadJsonFromFile(QString path){
    QFile configFile(path);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        throw ParserException("Failure opening file");
    }
    QString rawData = configFile.readAll();
    configFile.close();

    QJsonDocument document = QJsonDocument::fromJson(rawData.toUtf8());
    if (!document.isObject()) {
        throw ParserException("Config must be json object");
    }
    return document.object();
}

long long Tools::sqr(long long number){
    return number * number;
}

float Tools::sqr(float number){
    return number * number;
}

double Tools::sqr(double number) {
    return number * number;
}
