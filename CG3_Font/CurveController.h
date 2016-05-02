#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H

#include <QObject>
#include "PickGroup.h"
#include "CheckBoxGroup.h"
#include "BezierCurve.h"

class CurveController : public QObject {
    Q_OBJECT

    PickGroup* xGroup;
    PickGroup* yGroup;
    PickGroup* scaleGroup;
    CheckBoxGroup* outlineGroup;
    CheckBoxGroup* fillGroup;
    BezierCurve* curve;
public:
    CurveController();
    void setXGroup(PickGroup *value);
    void setYGroup(PickGroup *value);
    void setScaleGroup(PickGroup *value);
    void setFillGroup(CheckBoxGroup *group);
    void setOutlineGroup(CheckBoxGroup *group);
    BezierCurve *getCurve() const;
    void setCurve(BezierCurve *value);
public slots:
    void centerPositionChanged(double deltaX, double deltaY);
private slots:
    void xChanged(int x);
    void yChanged(int y);
    void scaleChanged(int scale);
    void fillStateChanged(bool newState);
    void outlineStateChanged(bool newState);
};

#endif // CURVECONTROLLER_H
