#include "SphereProjectorController.h"

SphereProjector *SphereProjectorController::getSphereProjector() const
{
    return sphereProjector;
}

void SphereProjectorController::setSphereProjector(SphereProjector *projector)
{
    if(!(xGroup && yGroup && scaleGroup)){
        throw std::runtime_error("Groups not set");
    }
    sphereProjector = projector;
    xGroup->setValue(projector->getX());
    yGroup->setValue(projector->getY());
    scaleGroup->setValue(projector->getScale());
}

void SphereProjectorController::xChanged(int x)
{
    sphereProjector->setX(x);
}

void SphereProjectorController::yChanged(int y)
{
    sphereProjector->setY(y);
}

void SphereProjectorController::scaleChanged(int scale)
{
    sphereProjector->setScale(scale);
}

void SphereProjectorController::xyOffsetChanged(int dx, int dy){
    auto newX = sphereProjector->getX() + dx;
    auto newY = sphereProjector->getX() + dx;
    xGroup->setValue(newX);
    yGroup->setValue(newY);
    sphereProjector->setX(newX);
    sphereProjector->setY(newY);
}

SphereProjectorController::SphereProjectorController()
{

}

void SphereProjectorController::setXGroup(PickGroup *group)
{
    xGroup = group;
    connect(xGroup, SIGNAL(valueChanged(int)), this, SLOT(xChanged(int)));
}

void SphereProjectorController::setYGroup(PickGroup *group)
{
    yGroup = group;
    connect(yGroup, SIGNAL(valueChanged(int)), this, SLOT(yChanged(int)));
}

void SphereProjectorController::setScaleGroup(PickGroup *group)
{
    scaleGroup = group;
    connect(scaleGroup, SIGNAL(valueChanged(int)), this, SLOT(scaleChanged(int)));
}
