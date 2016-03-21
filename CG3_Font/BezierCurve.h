#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QVector>
#include "Drawable.h"
#include "BezierPoint.h"

class BezierCurve : public Drawable
{
    QVector<BezierPoint> points;
    bool fill;
    bool outline;

    static const int DELTA = 1;
public:
    BezierCurve();
    void draw(QImage* image) override;
    void drawOutline(QImage* image);
    void drawCurve(QImage* image, BezierPoint p0, BezierPoint p1, BezierPoint p2);
    BezierPoint getMiddlePoint(BezierPoint p0, BezierPoint p1, BezierPoint p2);
    double computeDistanceToLine(BezierPoint linePoint1, BezierPoint linePoint2, BezierPoint target);

    QVector<BezierPoint> getPoints() const;
    void setPoints(const QVector<BezierPoint> &value);
    bool getFill() const;
    void setFill(bool value);
    bool getOutline() const;
    void setOutline(bool value);
};

#endif // BEZIERCURVE_H
