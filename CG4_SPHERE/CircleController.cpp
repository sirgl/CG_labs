#include "CircleController.h"
#include <cassert>

CircleController::CircleController(QObject *parent) : QObject(parent){

}

void CircleController::setCircle(Circle *circle){
    assert(xGroup && yGroup && rGroup);
    this->circle = circle;
    xGroup->setValue(this->circle->getX());
    yGroup->setValue(this->circle->getY());
    rGroup->setValue(this->circle->getR());
}

Circle *CircleController::getCircle(){
    return circle;
}

void CircleController::setXGroup(PickGroup *group){
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
    xGroup = group;
}

void CircleController::setYGroup(PickGroup *group){
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
    yGroup = group;
}

void CircleController::setRGroup(PickGroup *group){
    rGroup = group;
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(rChanged(int)));
}

void CircleController::xChanged(int x){
    circle->setX(x);
}

void CircleController::yChanged(int y){
    circle->setY(y);
}

void CircleController::rChanged(int r){
    circle->setRadius(r);
}
