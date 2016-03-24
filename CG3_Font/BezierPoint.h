#ifndef BEZIERPOINT_H
#define BEZIERPOINT_H

struct BezierPoint{
    double x;
    double y;
    bool onCurve;
    BezierPoint();
    BezierPoint(double x, double y, bool onCurve);
    BezierPoint(const BezierPoint& point);
    BezierPoint operator=(const BezierPoint& point);

    double distance(BezierPoint point);

    BezierPoint operator +(const BezierPoint& point);
    BezierPoint operator -(const BezierPoint& point);
    BezierPoint operator *(double multiplier);
    bool operator == (const BezierPoint& point);
};

BezierPoint operator *(double multiplier, const BezierPoint& point);
#endif // BEZIERPOINT_H
