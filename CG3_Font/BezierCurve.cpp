#include "BezierCurve.h"
#include "Tools.h"
#include "Line.h"
#include "Circle.h"

using Tools::sqr;
static const double DELTA = 0.5;

QVector<BezierPoint> BezierCurve::getPoints() const{
    return points;
}

void BezierCurve::setPoints(const QVector<BezierPoint> &value){
    points = value;
}

bool BezierCurve::getFill() const{
    return fill;
}

void BezierCurve::setFill(bool value){
    fill = value;
}

bool BezierCurve::getOutline() const{
    return outline;
}

void BezierCurve::setOutline(bool value){
    outline = value;
}

BezierPoint BezierCurve::scaleAndShiftPoint(BezierPoint point){
    point.x += xOffset;
    point.y += yOffset;
    double scaleK = scale <= 0 ? 1 + (scale / 1000) :  1 + (scale / 100);
    return point * scaleK;
}

int BezierCurve::getXOffset() const{
    return xOffset;
}

void BezierCurve::setXOffset(int value){
    xOffset = value;
    emit redraw();
    emit xOffsetChanged(value);
}

int BezierCurve::getYOffset() const{
    return yOffset;
}

void BezierCurve::setYOffset(int value){
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

BezierCurve::BezierCurve() : scale(0), fill(true), outline(true), xOffset(0), yOffset(0) {
}

void BezierCurve::draw(QImage *image){
    if(outline) {
        drawOutline(image);
    }
}

void BezierCurve::drawOutline(QImage *image){
    for(int i = 0;; ++i) {
        if(getPoint(i + 1).onCurve) {
            Line line;
            line.setCoordinates(scaleAndShiftPoint(getPoint(i)), scaleAndShiftPoint(getPoint(i + 1)));
            line.draw(image);
            continue;
        }
        drawCurve(image, getPoint(i), getPoint(i + 1), getPoint(i + 2));
        i += 1;
        if(i >= points.size()) {
            break;
        }
    }
}

void BezierCurve::drawCurve(QImage *image, BezierPoint p0, BezierPoint p1, BezierPoint p2){
    BezierPoint middle = getMiddlePoint(p0, p1, p2);




    if(computeDistanceToLine(p0, p2, middle) * p0.distance(p2) < DELTA) {
        Line line1;
//        line1.setCoordinates(p0, middle);
//        p0.x += xOffset;
//        p0.y += yOffset;
//        p2.x += xOffset;
//        p2.y += yOffset;
        line1.setCoordinates(scaleAndShiftPoint(p0), scaleAndShiftPoint(p2));
        line1.draw(image);
//        Line line2;
//        line2.setCoordinates(p2, middle);
//        line2.draw(image);
    } else {
        BezierPoint p3 = (p0 + p1) * 0.5;
        BezierPoint p4 = (p1 + p2) * 0.5;
        drawCurve(image, p0, p3, middle);
        drawCurve(image, middle, p4, p2);
    }

//    Circle circle1;
//    circle1.setPosition(std::round(p0.x), std::round(-p0.y));
//    circle1.setRadius(1);
//    circle1.draw(image);
//    Circle circle2;
//    circle2.setPosition(std::round(p2.x), std::round(-p2.y));
//    circle2.setRadius(1);
//    circle2.draw(image);
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

BezierPoint BezierCurve::getPoint(int index){
    return points[index % points.size()];
}
