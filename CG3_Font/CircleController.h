#ifndef CIRCLECONTROLLER_H
#define CIRCLECONTROLLER_H

#include <QObject>
#include "Circle.h"
#include "PickGroup.h"

class CircleController : public QObject{
    Q_OBJECT

    Circle* circle;

    PickGroup* xGroup;
    PickGroup* yGroup;
    PickGroup* rGroup;
public:
    explicit CircleController(QObject *parent = 0);

    void setCircle(Circle* circle);
    Circle* getCircle();
    void setXGroup(PickGroup* group);
    void setYGroup(PickGroup* group);
    void setRGroup(PickGroup* group);
private slots:
    void xChanged(int x);
    void yChanged(int y);
    void rChanged(int r);
};

#endif // CIRCLECONTROLLER_H
