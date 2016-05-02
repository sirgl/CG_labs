#include "CurveController.h"
#include <assert.h>
#include <QDebug>

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

void CurveController::setFillGroup(CheckBoxGroup *group){
    fillGroup = group;
    connect(fillGroup, SIGNAL(stateChanged(bool)), this, SLOT(fillStateChanged(bool)));
}

void CurveController::setOutlineGroup(CheckBoxGroup *group){
    outlineGroup = group;
    connect(outlineGroup, SIGNAL(stateChanged(bool)), this, SLOT(outlineStateChanged(bool)));
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

void CurveController::fillStateChanged(bool newState){
    qDebug() << "fill" << newState;
    curve->setFill(newState);
}

void CurveController::outlineStateChanged(bool newState){
    qDebug() << "outline" << newState;
    curve->setOutline(newState);
}

BezierCurve *CurveController::getCurve() const{
    return curve;
}

void CurveController::setCurve(BezierCurve *value){
    assert(xGroup && yGroup && scaleGroup && fillGroup && outlineGroup);
    curve = value;
    xGroup->setValue(curve->getXOffset());
    yGroup->setValue(curve->getYOffset());
    scaleGroup->setValue(curve->getScale());
    fillGroup->setState(curve->getFill());
    outlineGroup->setState(curve->getOutline());
}

void CurveController::centerPositionChanged(double deltaX, double deltaY){
    auto scale = curve->getScale();
    double scaleK = scale <= 0 ? 1 + (scale / 1000) :  1 + (scale / 100);

    auto newX = curve->getXOffset() + deltaX / scaleK;
    auto newY = curve->getYOffset() + deltaY / scaleK;
    xGroup->setValue(newX);
    yGroup->setValue(newY);
    curve->setXOffset(newX);
    curve->setYOffset(newY);
}

CurveController::CurveController(){

}
