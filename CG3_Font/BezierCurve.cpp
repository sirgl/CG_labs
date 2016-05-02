#include "BezierCurve.h"
#include "Tools.h"
#include "Line.h"
#include "Circle.h"
#include "DrawingTools.h"
#include <QDebug>
#include <QJsonArray>
#include <ParserException.h>

using Tools::sqr;
using std::abs;
using std::round;
static const double MIN_DISTANCE_TRESHOLD = 0.5;

void BezierCurve::setPointsSets(QVector<QVector<BezierPoint> > value){
    originPointsSet = value;
}


bool BezierCurve::getFill() const{
    return fill;
}

void BezierCurve::setFill(bool value){
    fill = value;
    emit redraw();
}

bool BezierCurve::getOutline() const{
    return outline;
}

void BezierCurve::setOutline(bool value){
    outline = value;
    emit redraw();
}

BezierPoint BezierCurve::scaleAndShiftPoint(BezierPoint point){
    point.x += xOffset;
    point.y += yOffset;
    double scaleK = scale <= 0 ? 1 + (scale / 1000) :  1 + (scale / 100);
    return point * scaleK;
}
double BezierCurve::getXOffset() const{
    return xOffset;
}

void BezierCurve::setXOffset(double value){
    xOffset = value;
    emit redraw();
    emit xOffsetChanged(value);
}

double BezierCurve::getYOffset() const{
    return yOffset;
}

void BezierCurve::setYOffset(double value){
    yOffset = value;
    emit redraw();
    emit yOffsetChanged(value);
}

double BezierCurve::getScale() const{
    return scale;
}

void BezierCurve::setScale(int value){
    scale = value;
    emit redraw();
    emit scaleChanged(value);
}

void BezierCurve::setOriginPointsSet(const QVector<QVector<BezierPoint> > &value){
    originPointsSet = value;
}

QJsonObject BezierCurve::saveToJson(){
    QJsonObject object;
    QJsonObject position;
    position["x"] = (int)xOffset;
    position["y"] = (int)yOffset;
    object["position"] = position;
    object["scale"] = scale;
    object["fill"] = fill;
    object["outline"] = outline;
    QJsonArray glyphs;
    for(int i = 0; i < originPointsSet.size(); ++i) {
        QJsonArray curve;
        for (BezierPoint point : originPointsSet[i]) {
            curve.push_back(point.saveToJson());
        }
        QJsonObject figure;
        figure[QString("figure") + i] = curve;
        glyphs.push_back(figure);
    }
    object["glyphs"] = glyphs;
    return object;
}

void BezierCurve::loadFromJson(QJsonObject object){
    auto positionRef = object["position"];
    if(!positionRef.isObject()) {
        throw ParserException("Bad json format");
    }
    auto positionObj = positionRef.toObject();

    auto xRef = positionObj["x"];
    if(!xRef.isDouble()) {
        throw ParserException("Bad json format");
    }
    auto xObj = xRef.toInt();

    auto yRef = positionObj["y"];
    if(!yRef.isDouble()) {
        throw ParserException("Bad json format");
    }
    auto yObj = yRef.toInt();

    auto scaleRef = object["scale"];
    if(!scaleRef.isDouble()) {
        throw ParserException("Bad json format");
    }
    auto scaleObj = scaleRef.toDouble();


    auto fillRef = object["fill"];
    if(!fillRef.isBool()) {
        throw ParserException("Bad json format");
    }
    auto fillObj = fillRef.toBool();

    auto outlineRef = object["outline"];
    if(!outlineRef.isBool()) {
        throw ParserException("Bad json format");
    }
    auto outlineObj = outlineRef.toBool();

    auto glyphsRef = object["glyphs"];
    if(!glyphsRef.isArray()) {
        throw ParserException("Bad json format");
    }
    auto glyphsObj = glyphsRef.toArray();
    QVector<QVector<BezierPoint>> curveArr;
    for(auto figure : glyphsObj) {
        if(!figure.isObject()) {
            throw ParserException("Bad json format");
        }
        auto figureObj = figure.toObject();
        for(auto figurePointArrRef : figureObj) {
            if(!figurePointArrRef.isArray()) {
                throw ParserException("Bad json format");
            }
            auto figurePointArr = figurePointArrRef.toArray();
            QVector<BezierPoint> curve;
            for(auto pointRef : figurePointArr) {
                if(!pointRef.isObject()) {
                    throw ParserException("Bad json format");
                }
                auto point = pointRef.toObject();
                BezierPoint bezierPoint;
                bezierPoint.loadFromJson(point);
                curve.push_back(bezierPoint);
            }
            curveArr.push_back(curve);
        }
    }

    this->xOffset = xObj;
    this->yOffset = yObj;
    this->scale = scaleObj;
    this->fill = fillObj;
    this->outline = outlineObj;
    this->originPointsSet = curveArr;
}

QVector<QVector<BezierPoint> > BezierCurve::separateToPrimitiveSegments(QVector<BezierPoint> points){
    QVector<QVector<BezierPoint> > segments;
    for(int i = 0;; ++i) {
        if(i >= points.size()) {
            break;
        }

        if(getPoint(i + 1, points).onCurve) {
            QVector<BezierPoint> line;
            line.push_back(getPoint(i, points));
            line.push_back(getPoint(i + 1, points));
            segments.push_back(line);
            continue;
        }
        auto p0 = getPoint(i, points);
        auto p1 = getPoint(i + 1, points);
        auto p2 = getPoint(i + 2, points);

        QVector<BezierPoint> bezierSegment;
        bezierSegment.push_back(p0);
        bezierSegment.push_back(p1);
        bezierSegment.push_back(p2);
        segments.push_back(bezierSegment);

        i++;
    }
    return segments;
}

QVector<BezierPoint> BezierCurve::intersectBezierSegmentWithHorizontalLine(QVector<BezierPoint> bezier, double y){
    auto y0 = bezier[0].y;
    auto y1 = bezier[1].y;
    auto y2 = bezier[2].y;

    QVector<BezierPoint> intersectionPoints;
    double denominator = y0 - 2 * y1 + y2;
    double discriminant = sqr(y1) - y0 * y2 + y * denominator;
    if(discriminant < 0) {
        return intersectionPoints;
    }

    double t1 = ((y0 - y1) + sqrt(discriminant)) / denominator;
    double t2 = ((y0 - y1) - sqrt(discriminant)) / denominator;

    if(t1 > 0 && (t1 - 1) <= ERR) {
        auto point = getBezierPointByParameter(bezier[0], bezier[1], bezier[2], t1);
        intersectionPoints.push_back(point);
        if(fabs(t1 - t2) < ERR) {
            auto yt1 = y0 + (y1 - y0) * t1;
            auto yt2 = y2 + (y1 - y2) * t1;
            //when tangent line to point is parallel to Ox:
            if(!(fabs(yt1 - yt2) < 10e-5)) {
                return intersectionPoints;
            }
        }
//        auto p1Intersections = handleBezierBorderConditions(point, bezier, nextLine, y, t1);
//        std::copy(p1Intersections.begin(), p1Intersections.end(), std::back_inserter(intersectionPoints));
    }
    if(t2 > 0 && (t2 - 1) <=  ERR) {
        auto point = getBezierPointByParameter(bezier[0], bezier[1], bezier[2], t2);
        intersectionPoints.push_back(point);
//        auto p2Intersections = handleBezierBorderConditions(point, bezier, nextLine, y, t2);
//        std::copy(p2Intersections.begin(), p2Intersections.end(), std::back_inserter(intersectionPoints));
    }
    return intersectionPoints;

}

QVector<BezierPoint> BezierCurve::intersectLineWithHorizontalLine(QVector<BezierPoint> line, double y){
    double y0 = line[0].y;
    double y1 = line[1].y;
    double x0 = line[0].x;
    double x1 = line[1].x;
    QVector<BezierPoint> result;
    if(y0 == y1 && y0  == y) { // change to eps?
        result.push_back(line[1]);
        return result;
    }
    double t = (y - y0) / (y1 - y0);

    if(t >= 0 && t <= 1) { // it doesn't need that?
        double x = line[0].x + (line[1].x - line[0].x) * t;

        if(fabs(x - x0) >= ERR || fabs(y - y0) >= ERR) {
            result.push_back(BezierPoint(x,y, false));
        }
    }
    return result;
}

BezierPoint BezierCurve::getBezierPointByParameter(BezierPoint p0, BezierPoint p1, BezierPoint p2, double t){
    return sqr(1 - t) * p0 + 2 * (1 - t) * t * p1 + sqr(t) * p2;
}

QVector<QPair<BezierPoint, BezierPoint> > BezierCurve::getFillingArea(){
    QVector<QPair<BezierPoint, BezierPoint> > fillingLines;
    for(int y = std::round(minY) - 1; y < std::round(maxY) + 1; ++y){
        QVector<BezierPoint> intersections;
//        y = -113; // DEBUG
        for(auto curve : primitiveCurvesSegments) {
            for(int i = 0; i < curve.size(); ++i) {
                QVector<BezierPoint> segment = curve[i];
                auto nextSegment = curve[(i+1) % curve.size()];
                if(segment.size() == 3) {
                    auto segIntersections = intersectBezierSegmentWithHorizontalLine(segment, y);
                    std::copy(segIntersections.begin(), segIntersections.end(), std::back_inserter(intersections));
                }
                if(segment.size() == 2) {
                    auto segIntersections = intersectLineWithHorizontalLine(segment, y);
                    std::copy(segIntersections.begin(), segIntersections.end(), std::back_inserter(intersections));
                }
                if(fabs(segment[segment.length() - 1].y - y) <= ERR) { // handle peeks, add extra point
                    auto previousY = segment[segment.length() - 2].y;
                    if((previousY < y && nextSegment[1].y < y) || (previousY > y && nextSegment[1].y > y)) {
                        intersections.push_back(segment[segment.length() - 1]);
                    }
                }
            }

        }
        std::sort(intersections.begin(), intersections.end(), [](BezierPoint a, BezierPoint b) { return a.x < b.x;});
        if((intersections.size() % 2) != 0) { // DEBUG
            qDebug() <<  y << " with size " << intersections.size();
        }
        for(int i = 0; i < intersections.size() / 2; ++i) {
            fillingLines.push_back(QPair<BezierPoint, BezierPoint>(intersections[i * 2], intersections[i * 2 + 1]));
        }
    }
    return fillingLines;
}

void BezierCurve::extractPrimitiveCurveSegments(QImage* image){
    QVector<QVector<BezierPoint>> scaledAndShiftedCurvesPoints;
    QVector<double> minYPoints;
    QVector<double> maxYPoints;
    for(auto originCurve : originPointsSet) {
        QVector<BezierPoint> scaledAndShiftedPoints;
        for(auto originPoint : originCurve) {
            scaledAndShiftedPoints.push_back(scaleAndShiftPoint(originPoint));
        }
        scaledAndShiftedCurvesPoints.push_back(scaledAndShiftedPoints);
        minYPoints.push_back((std::min_element(scaledAndShiftedPoints.begin(), scaledAndShiftedPoints.end(), [](BezierPoint a, BezierPoint b) { return a.y < b.y;}))->y);
        maxYPoints.push_back((std::max_element(scaledAndShiftedPoints.begin(), scaledAndShiftedPoints.end(), [](BezierPoint a, BezierPoint b) { return a.y < b.y;}))->y);
    }
    minY = *std::min_element(minYPoints.begin(), minYPoints.end());
    maxY = *std::max_element(maxYPoints.begin(), maxYPoints.end());
//    auto height = image->height();
//    minY = minY < (- height / 2) ? (- height / 2) : minY;
//    maxY = maxY > height / 2 ? height / 2 : maxY;

    primitiveCurvesSegments.clear();
    for(auto curvePoints : scaledAndShiftedCurvesPoints) {
        primitiveCurvesSegments.push_back(separateToPrimitiveSegments(curvePoints));
    }
}

//QVector<BezierPoint> BezierCurve::handleBorderConditions(BezierPoint point, BezierPoint currentStartingPoint, BezierPoint currentEndPoint, QVector<BezierPoint> nextSegment, double y, BezierPoint previousSignificantPoint){
//    auto x = point.x;
//    QVector<BezierPoint> result;

//    auto yRounded = round(y);
//    if(abs(round(currentStartingPoint.y) - yRounded) < 10e-10) { // point is near to start
//        return result;
//    }
//    if(abs(round(currentEndPoint.y) - yRounded) <= 10e-10) { // point is near to end
//        bool is2Pointed = false;
//        if(previousSignificantPoint.y < y) { // check if all next segment points bellow y
//            if(nextSegment.size() == 2) {
//                if(nextSegment[1].y < y) {
//                    is2Pointed = true;
//                }
//            } else {
//                if(nextSegment[1].y < y && nextSegment[2].y < y) {
//                    is2Pointed = true;
//                }
//            }
//        } else if (previousSignificantPoint.y > y) { // check if all next segment points above y
//            if(nextSegment.size() == 2) {
//                if(nextSegment[1].y > y) {
//                    is2Pointed = true;
//                }
//            } else {
//                if(nextSegment[1].y > y && nextSegment[2].y > y) {
//                    is2Pointed = true;
//                }
//            }
//        }
//        if(is2Pointed) {
//            result.push_back(BezierPoint(x, y, false));
//            result.push_back(BezierPoint(x, y, false));
//            return result;
//        } else {
//            result.push_back(BezierPoint(x, y, false));
//            return result;
//        }
//    }
//    result.push_back(BezierPoint(x, y, false));
//    return result;
//}

BezierCurve::BezierCurve() : scale(0), fill(true), outline(true), xOffset(0), yOffset(0), maxY(0), minY(0) {
    fillColor = new QColor(0, 0, 255);
}

void BezierCurve::draw(QImage *image){
    if(!outline && !fill) {
        return;
    }
    extractPrimitiveCurveSegments(image);

    if(outline) {
        drawOutline(image);
    }

    if(fill) {
        auto area = getFillingArea();
        for(QPair<BezierPoint, BezierPoint> line : area) {
            int y = std::round(line.first.y);
            int startingX = std::round(line.first.x);
            int endingX = std::round(line.second.x);
            int width = image->width();
            startingX = startingX < (- width / 2) ?  - width / 2 : startingX;
            endingX = endingX > width / 2 ? width / 2 : endingX;
            for(int x = startingX; x <= endingX; ++x) {
                DrawingTools::drawPixel(image, x, y, fillColor); // TODO
            }
        }
    }
    if(outline){
        for(auto curve : primitiveCurvesSegments) {
            for(auto segment : curve) {
                for(auto point : segment) {
                    Circle circle;
                    circle.setRadius(2);
                    circle.setX(point.x);
                    circle.setY(-point.y);
                    circle.draw(image);
                }
            }
        }
    }
}

void BezierCurve::drawOutline(QImage *image){
    for(QVector<QVector<BezierPoint>> curve : primitiveCurvesSegments){
        for(QVector<BezierPoint> segment : curve) {
            if(segment.size() == 2) {
                Line line;
                line.setCoordinates(segment[0], segment[1]);
                line.draw(image);
            } else if (segment.size() == 3) {
                drawCurve(image, segment[0], segment[1], segment[2]);
            }
        }
    }
}

void BezierCurve::drawCurve(QImage *image, BezierPoint p0, BezierPoint p1, BezierPoint p2){
    BezierPoint middle = getMiddlePoint(p0, p1, p2);
    if(p0 == p1 || p1 == p2 || p1 == p2){
        return;
    }
    if(computeDistanceToLine(p0, p2, middle) < MIN_DISTANCE_TRESHOLD) {
        Line line1;
        line1.setCoordinates(p0, p2);
        line1.draw(image);
    } else {
        BezierPoint p3 = (p0 + p1) * 0.5;
        BezierPoint p4 = (p1 + p2) * 0.5;
        drawCurve(image, p0, p3, middle);
        drawCurve(image, middle, p4, p2);
    }
}

BezierPoint BezierCurve::getMiddlePoint(BezierPoint p0, BezierPoint p1, BezierPoint p2){
    return sqr(1 - 0.5) * p0 + 2 * (1 - 0.5) * 0.5 * p1 + sqr(0.5) * p2;
}

double BezierCurve::computeDistanceToLine(BezierPoint p1, BezierPoint p2, BezierPoint target){
    auto v1 = (p2.y - p1.y) * target.x;
    auto v2 = (p2.x - p1.x) * target.y;
    auto v3 = p2.x * p1.y - p2.y * p1.x;
    double numerator = std::abs(v1 - v2 + v3);
    double denominator = std::sqrt(sqr(p2.y - p1.y) + sqr(p2.x - p1.x));
    return numerator / denominator;
}

BezierPoint BezierCurve::getPoint(int index, QVector<BezierPoint> points){
    return points[index % points.size()];
}
