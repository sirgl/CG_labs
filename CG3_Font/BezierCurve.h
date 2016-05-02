#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QVector>
#include "Drawable.h"
#include "BezierPoint.h"
#include <QObject>
#include "Point.h"
#include <QJsonObject>
#include <QColor>


class BezierCurve : public Drawable{
    Q_OBJECT

    const double ERR = 10e-10;

    bool fill;
    bool outline;
    double xOffset;
    double yOffset;
    double scale;

    double minY;
    double maxY;
    int maxX;
    int minX;
    QColor* fillColor;

    QVector<QVector<BezierPoint>> originPointsSet;
    QVector<QVector<QVector<BezierPoint>>> primitiveCurvesSegments;

    QVector<QVector<BezierPoint>> separateToPrimitiveSegments(QVector<BezierPoint> points);
    QVector<BezierPoint> intersectBezierSegmentWithHorizontalLine(QVector<BezierPoint> bezier, double y);
    QVector<BezierPoint> intersectLineWithHorizontalLine(QVector<BezierPoint> line, double y);
    static BezierPoint getBezierPointByParameter(BezierPoint p0, BezierPoint p1, BezierPoint p2, double t);
    QVector<QPair<BezierPoint, BezierPoint>> getFillingArea();
    void extractPrimitiveCurveSegments(QImage* image);

    QVector<BezierPoint> handleBorderConditions(BezierPoint point, BezierPoint currentStartingPoint, BezierPoint currentEndPoint, QVector<BezierPoint> nextSegment, double y, BezierPoint previousSignificantPoint);
    QVector<BezierPoint> handleBezierBorderConditions(BezierPoint point, QVector<BezierPoint> bezier, QVector<BezierPoint> nextSegment, double y, double t);
public:
    BezierCurve();
    void draw(QImage* image) override;
    void drawOutline(QImage* image);
    void drawCurve(QImage* image, BezierPoint p0, BezierPoint p1, BezierPoint p2);
    BezierPoint getMiddlePoint(BezierPoint p0, BezierPoint p1, BezierPoint p2);
    double computeDistanceToLine(BezierPoint linePoint1, BezierPoint linePoint2, BezierPoint target);
    BezierPoint getPoint(int index, QVector<BezierPoint> points);

    QVector<BezierPoint> getPoints() const;
    void setPoints(const QVector<BezierPoint> &value);
    void setPointsSets(QVector<QVector<BezierPoint>> value);
    bool getFill() const;
    void setFill(bool value);
    bool getOutline() const;
    void setOutline(bool value);


    BezierPoint scaleAndShiftPoint(BezierPoint point);
    double getXOffset() const;
    void setXOffset(double value);

    double getYOffset() const;
    void setYOffset(double value);

    double getScale() const;
    void setScale(int value);

    void setOriginPointsSet(const QVector<QVector<BezierPoint> > &value);

    QJsonObject saveToJson();
    void loadFromJson(QJsonObject object);
signals:
    void xOffsetChanged(int xOffset);
    void yOffsetChanged(int xOffset);
    void scaleChanged(int scale);
};

#endif // BEZIERCURVE_H
