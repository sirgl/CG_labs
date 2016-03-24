#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H

#include <QObject>
#include "PickGroup.h"

#include "BezierCurve.h"

class CurveController : public QObject {
    Q_OBJECT

    PickGroup* xGroup;
    PickGroup* yGroup;
    PickGroup* scaleGroup;
    BezierCurve* curve;
public:
    CurveController();
    void setXGroup(PickGroup *value);
    void setYGroup(PickGroup *value);
    void setScaleGroup(PickGroup *value);
    BezierCurve *getCurve() const;
    void setCurve(BezierCurve *value);

private slots:
    void xChanged(int x);
    void yChanged(int y);
    void scaleChanged(int scale);
};

#endif // CURVECONTROLLER_H
