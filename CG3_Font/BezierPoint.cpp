#include "BezierPoint.h"

#include <cmath>
#include "Tools.h"

using Tools::sqr;

const double DELTA = 0.0004;

BezierPoint::BezierPoint() : BezierPoint(0, 0, false) {

}

BezierPoint::BezierPoint(double x, double y, bool onCurve) : x(x), y(y), onCurve(onCurve) {

}

BezierPoint::BezierPoint(const BezierPoint &point){
    x = point.x;
    y = point.y;
    onCurve = point.onCurve;
}

BezierPoint BezierPoint::operator=(const BezierPoint &point){
    x = point.x;
    y = point.y;
    onCurve = point.onCurve;
    return *this;
}

double BezierPoint::distance(BezierPoint point){
    return std::sqrt(sqr(x - point.x) + sqr(y - point.y));
}

BezierPoint BezierPoint::operator +(const BezierPoint &point){
    return BezierPoint(point.x + x, point.y + y, onCurve);
}

BezierPoint BezierPoint::operator -(const BezierPoint &point){
    return BezierPoint(x - point.x, y - point.y, onCurve);
}

BezierPoint BezierPoint::operator *(double multiplier){
    return BezierPoint(x * multiplier, y * multiplier, onCurve);
}

bool BezierPoint::operator ==(const BezierPoint &point){
    return onCurve == point.onCurve && x - point.x < DELTA && y - point.y < DELTA;
}

BezierPoint::operator Point(){
    return Point(std::round(x), std::round(y));
}

BezierPoint operator *(double multiplier, const BezierPoint &point){
    return BezierPoint(point.x * multiplier, point.y * multiplier, false);
}
