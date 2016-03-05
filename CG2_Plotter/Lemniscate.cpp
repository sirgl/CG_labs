#include "Lemniscate.h"
#include "ParserException.h"
#include <QJsonValueRef>
#include <QDebug>
#include <DrawingTools.h>


Lemniscate::Lemniscate() :
    Drawable(),
    x1(0),
    y1(0),
    x2(0),
    y2(0),
    param(0),
    preLemCanonicPointNumber(0) {
    neighbors = {
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
        {0, -1},
        {1, -1},
        {1, 0}
    };
}

void Lemniscate::setFocuses(int x1, int y1, int x2, int y2){
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    param = ((long)(x1 - x2) * (x1 - x2)  + (y1 - y2) * (y1 - y2)) * ((x1 - x2) * (x1 - x2)  + (y1 - y2) * (y1 - y2));
}

void Lemniscate::loadFromJson(QJsonObject object){
    QJsonValueRef focus1Tmp = object["focus1"];
    if(!focus1Tmp.isObject()) {
        throw ParserException("Focus 1 must have object type");
    }
    QJsonObject focus1 = focus1Tmp.toObject();
    QJsonValueRef xTmp1 = focus1["x"];
    if(!xTmp1.isDouble()) {
        throw ParserException("x must be numeric for focus 1");
    }
    QJsonValueRef yTmp1 = focus1["y"];
    if(!yTmp1.isDouble()) {
        throw ParserException("y must be numeric for focus 1");
    }
    int x1 = xTmp1.toInt();
    int y1 = yTmp1.toInt();

    QJsonValueRef focus2Tmp = object["focus2"];
    if(!focus2Tmp.isObject()) {
        throw ParserException("Focus 2 must have object type");
    }
    QJsonObject focus2 = focus2Tmp.toObject();
    QJsonValueRef xTmp2 = focus2["x"];
    if(!xTmp2.isDouble()) {
        throw ParserException("x must be numeric for focus 2");
    }
    QJsonValueRef yTmp2 = focus2["y"];
    if(!yTmp2.isDouble()) {
        throw ParserException("y must be numeric for focus 2");
    }
    int x2 = xTmp2.toInt();
    int y2 = yTmp2.toInt();

    setFocuses(x1, y1, x2, y2);
}

QJsonObject Lemniscate::saveToJson(){
    QJsonObject object;
    QJsonObject focus1;
    focus1["x"] = x1;
    focus1["y"] = y1;
    QJsonObject focus2;
    focus2["x"] = x1;
    focus2["y"] = y2;
    object["focus1"] = focus1;
    object["focus2"] = focus2;
    return object;
}

void Lemniscate::draw(QImage *image){
    if(x1 == x2 && y1 == y2) {
        qDebug() << "Focuses are the same";
        return;
    }
    auto startingPoint = getStartingPoint();
    auto r = getR(Point(14, -1));
    DrawingTools::drawPixel(image, startingPoint);
    auto currentPoint = startingPoint;
    for(int i = 0; i < 2000; ++i) {
        auto nextPoint = getNextPoint(currentPoint);
        DrawingTools::drawPixel(image, nextPoint);
        currentPoint = nextPoint;
    }
    auto x = DrawingTools::getUnique();
    qDebug() << x;
}

Point Lemniscate::getStartingPoint(){
    int currentX = x2;
    int currentY = y2;
    int nextX = (x2 - x1) + x2;
    int nextY = (y2 - y1) + y2;
    while(abs(currentX - nextX) > 1 || abs(currentY - nextY) > 1) {
        int tmpX = (currentX + nextX) >> 1;
        int tmpY = (currentY + nextY) >> 1;
        long rTmp = getR(tmpX, tmpY);
        if(rTmp > param) {
            nextX = (currentX + nextX) >> 1;
            nextY = (currentY + nextY) >> 1;
        } else {
            currentX = (currentX + nextX) >> 1;
            currentY = (currentY + nextY) >> 1;
        }
    }
    return Point(currentX, currentY);
}

long Lemniscate::getR(int x, int y){
    auto l1 = (((long)x - (long)x2) * ((long)x - (long)x2) + ((long)y - (long)y2) * ((long)y - (long)y2));
    auto l2 = (((long)x - (long)x1) * ((long)x - (long)x1) + ((long)y - (long)y1) * ((long)y - (long)y1)) ;
    return 16 * (l2 * l1);
}

long Lemniscate::getR(Point p){
    return getR(p.x, p.y);
}

Point Lemniscate::getNextPoint(Point previousLemPoint){
    Point startPoint = previousLemPoint + getNeighbor(preLemCanonicPointNumber);
    auto r = getR(startPoint);
    int i = 0;
    Point nextPoint;
    if(r > param) {
        while(r > param) {
            i++;
            nextPoint = previousLemPoint + getNeighbor(i + preLemCanonicPointNumber);
            r = getR(nextPoint);
        }
    } else if (r < param) {
        while(r < param) {
            i++;
            nextPoint = previousLemPoint + getNeighbor(i + preLemCanonicPointNumber);
            r = getR(nextPoint);
        }
    } else {
        preLemCanonicPointNumber = preLemCanonicPointNumber + 8 - 2;
        return startPoint;
    }
    qDebug() << i << "iterations";
    auto preNext = getNeighbor(i + 8 - 1 + preLemCanonicPointNumber) + previousLemPoint;
    auto preNextR = getR(preNext);
    long nextDiff = abs(param - r);
    long preNextDiff = abs(param - preNextR);
    if(nextDiff <= preNextDiff){
        auto toReturn = nextPoint;
        preLemCanonicPointNumber = (preLemCanonicPointNumber + i + 8 - 2) % 8;
        return toReturn;
    }
    auto toReturn = preNext;
    preLemCanonicPointNumber = (preLemCanonicPointNumber + i + 8 - 2) % 8;
    return toReturn;
}

Point Lemniscate::getNeighbor(int i){
    return neighbors[i % 8];
}
