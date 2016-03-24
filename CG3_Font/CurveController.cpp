#include "CurveController.h"
#include <assert.h>

void CurveController::setXGroup(PickGroup *group)
{
    xGroup = group;
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
}

void CurveController::setYGroup(PickGroup *group){
    yGroup = group;
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
}

void CurveController::setScaleGroup(PickGroup *group){
    scaleGroup = group;
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(scaleChanged(int)));
}

void CurveController::xChanged(int x){
    curve->setXOffset(x);
}

void CurveController::yChanged(int y){
    curve->setYOffset(y);
}

void CurveController::scaleChanged(int scale){
    curve->setScale(scale);
}

BezierCurve *CurveController::getCurve() const
{
    return curve;
}

void CurveController::setCurve(BezierCurve *value){
    assert(xGroup && yGroup && scaleGroup);
    curve = value;
    xGroup->setValue(curve->getXOffset());
    yGroup->setValue(curve->getYOffset());
    scaleGroup->setValue(curve->getScale());
}

CurveController::CurveController(){

}
