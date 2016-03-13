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
//    auto x3 = (y2 - y1) * 2;
//    auto y3 = (x1 - x2) * 2;
//    auto x4 = (y2 - y1);
//    auto y4 = (x1 - x2);
//    auto width = image->width();
//    auto height = image->height();
//    for(int y = - height - 1; y < height / 2; ++y) {
//        for(int x = -width - 1; x < width / 2; ++x) {
//            auto r = ((x1 - x) * (y2 - y) - (y1 - y) * (x2 - x))/*((x3 - x) * (y4 - y) - (y3 - y) * (x4 - x))*/;
//            if(r > 0) {
//                DrawingTools::drawPixel(image, x, y, new QColor(50, 0, 0));
//            } else {
//                DrawingTools::drawPixel(image, x, y, new QColor(0 , 50, 0));
//            }
//        }
//    }
//    DrawingTools::drawPixel(image, x1 , y1, new QColor(255,255,255));
//    DrawingTools::drawPixel(image, x2 , y2, new QColor(255,255,255));
//    auto pair = getStartingPoints();

//    auto seconds = computeSecondPoints(pair.first);
//    DrawingTools::drawPixel(image, pair.first, new QColor(0,0,255));
//    DrawingTools::drawPixel(image, seconds.first.first, new QColor(255,0,255));
//    DrawingTools::drawPixel(image, seconds.second.first, new QColor(255,0,255));

    auto traits = computeQuartersTraits();
    auto s = getStartingPoints();
    for(auto trait : traits) {
        drawQuarter(trait, image, s.first, s.second);
    }
    auto f = 0;

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

//    auto sides = getNeighborSides(startingPoint);
//    auto count = std::count_if(sides.begin(), sides.end(), [](QPair<Point, bool> p){return p.second == true;});
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

Point Lemniscate::getNextPoint(Point currentPoint, Point previousPoint, bool isPositive, Point firstPoint, Point secondPoint){
    auto previousPointNeighborPosition = getNeighborPosition(currentPoint, previousPoint);
    QVector<Point> candidates;
    for (int i = 3; i < 6; ++i) {
        Point point = currentPoint + getNeighbor(previousPointNeighborPosition + i);
        if(centralPoints.contains(point)) {
            return point;
        }
        if(isPositiveSided(point, firstPoint, secondPoint) != isPositive) {
            throw NoAvailablePointsException();
        }
        candidates.append(point);
    }
    if(candidates.empty()) {
        throw NoAvailablePointsException();
    }
//    QVector<long> params; // debug
//    for(auto p : candidates) {
//        params.push_back(abs(getR(p) - param));
//    }
    int position = std::min_element(candidates.begin(), candidates.end(), [&](Point p1, Point p2){
        return abs(getR(p1) - param) < abs(getR(p2) - param);
    }) - candidates.begin();

//    QVector<long> parameters;
//    QVector<Point> points; // debug
//        for (int i = 0; i < 8; ++i) {
//            Point p = getNeighbor(i) + previousPoint;
//            points.push_back(p);
//            parameters.push_back(getR(p) - param);
//        }
//    qDebug() << "picked point with " << getR(candidates[position]) - param; // debug
    return candidates[position];
}

//QVector<QPair<Point, long> > Lemniscate::getNeighborParams(Point point){
//    QVector<QPair<Point, long> > vector;
//    for(int i = 0; i < 8; ++i) {
//        Point p = point + getNeighbor(i);
//        vector.push_back(QPair<Point, long>(p, getR(p) - param));
//    }
//    return vector;
//}

//QVector<QPair<Point, bool> > Lemniscate::getNeighborSides(Point point){
//    QVector<QPair<Point, bool> >  vector;
//    for(int i = 0; i < 8; ++i) {
//        Point p = point + getNeighbor(i);
//        vector.push_back(QPair<Point, bool>(p, isPositiveSided(p)));
//    }
//    return vector;
//}

Point Lemniscate::getNeighbor(int i){
    return neighbors[(i + 16) % 8];
}

Point Lemniscate::getNeighborReversed(int i){
    return neighbors[(4096 - i) % 8];
}

QVector<QuarterTraits> Lemniscate::computeQuartersTraits(){
    auto startingPoints = getStartingPoints();
    auto firstNextPointPairs = computeSecondPoints(startingPoints.first, startingPoints.first, startingPoints.second);
    auto secondNextPointPairs = computeSecondPoints(startingPoints.second, startingPoints.first, startingPoints.second);
    QVector<QuarterTraits> traits;
    traits.push_back(QuarterTraits(startingPoints.second, secondNextPointPairs.second.first, secondNextPointPairs.second.second));
    traits.push_back(QuarterTraits(startingPoints.second, secondNextPointPairs.first.first, secondNextPointPairs.first.second));
    traits.push_back(QuarterTraits(startingPoints.first, firstNextPointPairs.second.first, firstNextPointPairs.second.second));
    traits.push_back(QuarterTraits(startingPoints.first, firstNextPointPairs.first.first, firstNextPointPairs.first.second));
    return traits;
}

QPair<QPair<Point, bool>, QPair<Point, bool>> Lemniscate::computeSecondPoints(Point startingPoint, Point firstPoint, Point secondPoint){
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
//    auto deb = getR(startingPoint) - param; // debug

//    auto s = getNeighborParams(startingPoint); // debug

//    auto pos = isPositiveSided(Point(-11, -123));
//    auto sides = getNeighborSides(Point(-10, -123));

    QPair<Point, bool> firstPair = isPositiveSided(first, firstPoint, secondPoint)
            ? QPair<Point, bool>(first, true)
            : QPair<Point, bool>(first, false);
    QPair<Point, bool> secondPair = isPositiveSided(second, firstPoint, secondPoint)
            ? QPair<Point, bool>(second, true)
            : QPair<Point, bool>(second, false);
    return QPair<QPair<Point, bool>, QPair<Point, bool>>(firstPair, secondPair);
}

bool Lemniscate::isPositiveSided(Point point, Point firstPoint, Point secondPoint){
//    return ((x1 - point.x) * (y2 - point.y) - (y1 - point.y) * (x2 - point.x)) > 0 ? true : false;
    return ((firstPoint.x - point.x) * (secondPoint.y - point.y) - (firstPoint.y - point.y) * (secondPoint.x - point.x)) > 0;
}

void Lemniscate::drawQuarter(QuarterTraits traits, QImage *image, Point firstPoint, Point secondPoint){
    Point previousPoint = traits.firstPoint;
    Point currentPoint = traits.secondPoint;
    DrawingTools::drawPixel(image, previousPoint);
    DrawingTools::drawPixel(image, currentPoint);
    qDebug() << "starting points R : " << getR(previousPoint) - param << getR(currentPoint) - param;
    /*for(int i = 0; i < 300; ++i)*/while(true) {
        Point nextPoint;
        try{
            nextPoint = getNextPoint(currentPoint, previousPoint, traits.isPositive, firstPoint, secondPoint);
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

int Lemniscate::getNeighborPosition(Point originPoint, Point neighborPoint){
    Point canonicPoint = neighborPoint - originPoint;
    return neighbors.indexOf(canonicPoint);
}
