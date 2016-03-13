#include "Lemniscate.h"
#include "ParserException.h"
#include <QJsonValueRef>
#include <QDebug>
#include <DrawingTools.h>
#include "Tools.h"
#include <algorithm>

using Tools::sqr;

int Lemniscate::getX1() const{
    return x1;
}

int Lemniscate::getY1() const{
    return y1;
}

int Lemniscate::getX2() const{
    return x2;
}

int Lemniscate::getY2() const{
    return y2;
}

void Lemniscate::setX1(int value){
    x1 = value;
    computeCentralPoints();
    computeParam();
    emit redraw();
}

void Lemniscate::setY1(int value){
    y1 = value;
    computeCentralPoints();
    computeParam();
    emit redraw();
}

void Lemniscate::setX2(int value){
    x2 = value;
    computeCentralPoints();
    computeParam();
    emit redraw();
}

void Lemniscate::setY2(int value){
    y2 = value;
    computeCentralPoints();
    computeParam();
    emit redraw();
}

void Lemniscate::computeParam(){
    param = sqr(sqr(x1 - x2) + sqr(y1 - y2));
}

void Lemniscate::computeCentralPoints(){
    centralPoints.clear();
    int tmpX = (x1 + x2) / 2;
    int tmpY = (y1 + y2) / 2;
    bool xNonInt = (x1 + x2) % 2;
    bool yNonInt = (y1 + y2) % 2;
    centralPoints.push_back(Point(tmpX, tmpY));
    if(xNonInt) {
        centralPoints.push_back(Point(tmpX + 1, tmpY));
    }
    if(yNonInt) {
        centralPoints.push_back(Point(tmpX, tmpY + 1));
    }
    if (xNonInt && yNonInt) {
        centralPoints.push_back(Point(tmpX + 1, tmpY + 1));
    }
}

Lemniscate::Lemniscate() :
    Drawable(),
    x1(0),
    y1(0),
    x2(0),
    y2(0),
    param(0),
    preLemCanonicPointNumber(0){
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
    computeParam();
    computeCentralPoints();
    emit redraw();
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
    if(abs(x1 - x2) + abs(y1 - y2) >4) {
        auto x = getStartingPointsPairs();
        QVector<QuarterTraits> traits;
        traits.push_back(QuarterTraits(x.first.first, x.first.second, isPositiveSided(x.first.second)));
        traits.push_back(QuarterTraits(x.first.second, x.first.first, isPositiveSided(x.first.first)));
        traits.push_back(QuarterTraits(x.second.first, x.second.second, isPositiveSided(x.second.second)));
        traits.push_back(QuarterTraits(x.second.second, x.second.first, isPositiveSided(x.second.first)));
        for(auto trait : traits) {
            drawQuarter(trait, image);
        }
    }
    Circle f1;
    f1.setX(x1);
    f1.setY(-y1);
    f1.setRadius(2);
    f1.draw(image);
    Circle f2;
    f2.setX(x2);
    f2.setY(-y2);
    f2.setRadius(2);
    f2.draw(image);
}

QPair<Point, Point> Lemniscate::getStartingPoints(){
    Point first = getStartingPoint(Point((x2 - x1) + x2, (y2 - y1) + y2));
    Point second = getStartingPoint(Point((x1 - x2) + x1, (y1 - y2) + y1));
    return QPair<Point, Point>(first, second);
}

Point Lemniscate::getStartingPoint(Point nextPoint){
    int currentX = x2;
    int currentY = y2;
    int nextX = nextPoint.x;
    int nextY = nextPoint.y;
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
    Point startingPoint = abs(getR(currentX, currentY) - param) <= abs(getR(nextX, nextY) - param)
            ? Point(currentX, currentY)
            : Point(nextX, nextY);
    return startingPoint;
}

long long Lemniscate::getR(int x, int y){
    auto l1 = sqr(x - x2) + sqr(y - y2);
    auto l2 = sqr(x - x1) + sqr(y - y1);
    return 16 * (l2 * l1);
}

long long Lemniscate::getR(Point p){
    return getR(p.x, p.y);
}

QPair<QPair<Point, Point>, QPair<Point, Point> > Lemniscate::getStartingPointsPairs(){
    auto startingPoints = getStartingPoints();
    auto firstPair = findIntersectionFocusLinePoints(startingPoints.first);
    auto secondPair = findIntersectionFocusLinePoints(startingPoints.second);
    return QPair<QPair<Point, Point>, QPair<Point, Point> >(firstPair, secondPair);
}

QPair<Point, Point> Lemniscate::findIntersectionFocusLinePoints(Point startingPoint){
    bool startingPointIsPositiveSided = isPositiveSided(startingPoint);
    Point current = startingPoint;
    while(true) {
        auto pair = computeNextPoints(current);
        if(isPositiveSided(pair.first) != startingPointIsPositiveSided){
            return QPair<Point, Point>(pair.first, current);
        }
        if(isPositiveSided(pair.second) != startingPointIsPositiveSided){
            return QPair<Point, Point>(pair.second, current);
        }
        current = abs(computeVectorFocusesMultiply(pair.first)) < abs(computeVectorFocusesMultiply(pair.second))
                ? pair.first
                : pair.second;
    }
}

int Lemniscate::computeVectorFocusesMultiply(Point point){
    return ((x1 - point.x) * (y2 - point.y) - (y1 - point.y) * (x2 - point.x));
}

bool Lemniscate::isPositiveSided(Point point){
    return computeVectorFocusesMultiply(point) >= 0;
}

QPair<Point, Point> Lemniscate::computeNextPoints(Point startingPoint){
    Point first;
    Point second;
    bool firstFound = false;
    Point currentPoint;
    Point nextPoint = startingPoint + getNeighbor(0);
    for(int i = 1; i < 9; ++i) {
        currentPoint = nextPoint;
        nextPoint = startingPoint + getNeighbor(i);
        auto currentRDelta = getR(currentPoint) - param;
        auto nextRDelta = getR(nextPoint) - param;
        if(currentRDelta * nextRDelta <= 0) {
            if(firstFound) {
                second = abs(currentRDelta) < abs(nextRDelta) ? currentPoint : nextPoint;
                break;
            } else {
                first = abs(currentRDelta) < abs(nextRDelta) ? currentPoint : nextPoint;
                firstFound = true;
            }
        }
    }
    return QPair<Point, Point>(first, second);
}

Point Lemniscate::getNextPoint(Point currentPoint, Point previousPoint, bool positive){
    auto previousPointNeighborPosition = getNeighborPosition(currentPoint, previousPoint);
    QVector<Point> candidates;
    for (int i = 3; i < 6; ++i) {
        Point point = currentPoint + getNeighbor(previousPointNeighborPosition + i);
        if(centralPoints.contains(point)) {
            return point;
        }
        if(isPositiveSided(point) != positive) {
            throw NoAvailablePointsException();
        }
        candidates.append(point);
    }
    if(candidates.empty()) {
        throw NoAvailablePointsException();
    }
    int position = std::min_element(candidates.begin(), candidates.end(), [&](Point p1, Point p2){
        return abs(getR(p1) - param) < abs(getR(p2) - param);
    }) - candidates.begin();
    return candidates[position];
}

void Lemniscate::drawQuarter(QuarterTraits traits, QImage *image){
    Point previousPoint = traits.firstPoint;
    Point currentPoint = traits.secondPoint;
    DrawingTools::drawPixel(image, previousPoint);
    DrawingTools::drawPixel(image, currentPoint);
    while(true) {
        Point nextPoint;
        try{
            nextPoint = getNextPoint(currentPoint, previousPoint, traits.isPositive);
        } catch (NoAvailablePointsException& e) {
            break;
        }

        DrawingTools::drawPixel(image, nextPoint);
        if(centralPoints.contains(nextPoint)) {
            break;
        }
        previousPoint = currentPoint;
        currentPoint = nextPoint;
    }
}

Point Lemniscate::getNeighbor(int i){
    return neighbors[(i + 8) % 8];
}

int Lemniscate::getNeighborPosition(Point originPoint, Point neighborPoint){
    Point canonicPoint = neighborPoint - originPoint;
    return neighbors.indexOf(canonicPoint);
}
