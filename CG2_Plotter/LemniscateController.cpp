#include "LemniscateController.h"
#include <cassert>

Lemniscate *LemniscateController::getLemniscate() const
{
    return lemniscate;
}

LemniscateController::LemniscateController(){

}

void LemniscateController::setX1Group(PickGroup *group){
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(x1Changed(int)));
    this->x1Group = group;
}

void LemniscateController::setY1Group(PickGroup *group){
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(y1Changed(int)));
            this->y1Group = group;
}

void LemniscateController::setX2Group(PickGroup *group){
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(x2Changed(int)));
    this->x2Group = group;
}

void LemniscateController::setY2Group(PickGroup *group){
    connect(group, SIGNAL(valueChanged(int)), this, SLOT(y2Changed(int)));
    this->y2Group = group;
}

void LemniscateController::setLemniscate(Lemniscate *lem){
    assert(x1Group && y1Group && x2Group && y2Group);
    lemniscate = lem;
    x1Group->setValue(lem->getX1());
    y1Group->setValue(lem->getY1());
    x2Group->setValue(lem->getX2());
    y2Group->setValue(lem->getY2());
}

void LemniscateController::x1Changed(int x1){
    lemniscate->setX1(x1);
}

void LemniscateController::y1Changed(int y1){
    lemniscate->setY1(y1);
}

void LemniscateController::x2Changed(int x2){
    lemniscate->setX2(x2);
}

void LemniscateController::y2Changed(int y2){
    lemniscate->setY2(y2);
}
