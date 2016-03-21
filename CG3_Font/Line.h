#ifndef LINE_H
#define LINE_H

#include "Drawable.h"
#include "BezierPoint.h"
#include <QImage>

class Line : public Drawable
{
    double x1;
    double y1;
    double x2;
    double y2;
public:
    Line();
    void setCoordinates(double x1, double y1, double x2, double y2);
    void setCoordinates(BezierPoint p1, BezierPoint p2);
    void draw(QImage* image) override;

};

#endif // LINE_H
