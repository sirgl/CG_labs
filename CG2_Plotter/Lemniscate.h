#ifndef LEMNISCATE_H
#define LEMNISCATE_H

#include "Drawable.h"
#include "Point.h"
#include <QVector>

class Lemniscate : public Drawable{
    Q_OBJECT
    int x1;
    int y1;
    int x2;
    int y2;
    long param; // == 16 * p^2
    QVector<Point> neighbors;

    int preLemCanonicPointNumber;
public:
    Lemniscate();
    void setFocuses(int x1, int y1, int x2, int y2);
    void loadFromJson(QJsonObject object);
    QJsonObject saveToJson();
    void draw(QImage *image);
    Point getStartingPoint();
    long getR(int x, int y);
    long getR(Point p);
    Point getNextPoint(Point previousLemPoint);
    Point getNeighbor(int i);
};

#endif // LEMNISCATE_H
