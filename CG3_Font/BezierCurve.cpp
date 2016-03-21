#include "BezierCurve.h"
#include "Tools.h"
#include "Line.h"
#include "Circle.h"

using Tools::sqr;

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

BezierCurve::BezierCurve(){

}

void BezierCurve::draw(QImage *image){
    if(outline) {
        drawOutline(image);
    }
    drawCurve(image, BezierPoint(10,10, true), BezierPoint(10, 400, false), BezierPoint(200, 200, true));
    //TODO fill
}

void BezierCurve::drawOutline(QImage *image){

}

void BezierCurve::drawCurve(QImage *image, BezierPoint p0, BezierPoint p1, BezierPoint p2){
    BezierPoint middle = getMiddlePoint(p0, p1, p2);

//    Circle circle1;
//    circle1.setPosition(p0.x, -p0.y);
//    circle1.setRadius(1);
//    circle1.draw(image);
//    Circle circle2;
//    circle2.setPosition(p2.x, -p2.y);
//    circle2.setRadius(1);
//    circle2.draw(image);


    if(computeDistanceToLine(p0, p2, middle) * p0.distance(p2) < DELTA) {
        Line line1;
//        line1.setCoordinates(p0, middle);
        line1.setCoordinates(p0, p2);
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
