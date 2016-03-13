#ifndef LEMNISCATE_H
#define LEMNISCATE_H

#include "Drawable.h"
#include "Point.h"
#include <QVector>
#include <QPair>
#include <exception>

struct QuarterTraits{
    Point firstPoint;
    Point secondPoint;
    bool isPositive; // sign of vector multiplication

    QuarterTraits() : QuarterTraits(Point(0, 0), Point(0, 0), false) {}
    QuarterTraits(Point firstPoint, Point secondPoint, bool isPositive) :
        firstPoint(firstPoint),
        secondPoint(secondPoint),
        isPositive(isPositive){
    }
};

class NoAvailablePointsException : public std::exception {
};

class Lemniscate : public Drawable{
    Q_OBJECT
    int x1;
    int y1;
    int x2;
    int y2;
    long long param; // == 16 * p^2
    QVector<Point> neighbors;
    QVector<Point> centralPoints;

    int preLemCanonicPointNumber;

    void computeParam();
    void computeCentralPoints();
    Point getNeighbor(int i);
    Point getNeighborReversed(int i);
public:
    Lemniscate();
    void setFocuses(int x1, int y1, int x2, int y2);




    void loadFromJson(QJsonObject object);
    QJsonObject saveToJson();
    void draw(QImage *image);

    QPair<Point, Point> getStartingPoints();
    Point getStartingPoint(Point nextPoint);
    QVector<QuarterTraits> computeQuartersTraits();
    QPair<QPair<Point, bool>, QPair<Point, bool>> computeSecondPoints(Point startingPoint, Point firstPoint, Point secondPoint);
    bool isPositiveSided(Point point, Point firstPoint, Point secondPoint);
    void drawQuarter(QuarterTraits traits, QImage* image, Point firstPoint, Point secondPoint);
    int getNeighborPosition(Point originPoint, Point neighborPoint);
    Point getNextPoint(Point currentPoint, Point previousPoint, bool positive, Point firstPoint, Point secondPoint);

    QVector<QPair<Point, long>> getNeighborParams(Point point); // debug
    QVector<QPair<Point, bool>> getNeighborSides(Point point); // debug

    long long getR(int x, int y);
    long long getR(Point p);
    Point getNextPoint(Point previousLemPoint);
    Point getNextPointReversed(Point previousLemPoint);

    //New  way
//    int pickStartingPointPosition(Point currentPoint, bool positive);
//    bool getQuarterSign(Point point);

    int getX1() const;
    int getY1() const;
    int getX2() const;
    int getY2() const;
    void setX1(int value);
    void setY1(int value);
    void setX2(int value);
    void setY2(int value);
};

#endif // LEMNISCATE_H
