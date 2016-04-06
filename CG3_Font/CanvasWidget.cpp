#include "CanvasWidget.h"
#include <QPainter>
#include <QResizeEvent>
#include <cassert>
#include <QDebug>

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent), centerX(0), centerY(0){
    canvas = new Canvas();
    connect(canvas, SIGNAL(redraw()), this, SLOT(update()));
    canvas->setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

void CanvasWidget::add(Drawable *drawable){
    canvas->add(drawable);
}

void CanvasWidget::setCanvasSize(int width, int height){
    canvas->setSize(width, height);
}

void CanvasWidget::removeAll(){
    canvas->removeAll();
}

int CanvasWidget::getCanvasWidth(){
return canvas->getWidth();
}

int CanvasWidget::getCanvasHeight(){
    return canvas->getHeight();
}

void CanvasWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawImage(0, 0, *canvas->getImage());
}

void CanvasWidget::resizeEvent(QResizeEvent *e){
    QWidget::resizeEvent(e);
    canvas->setSize(width(), height());
}

void CanvasWidget::mousePressEvent(QMouseEvent *event){
    centerX = event->x();
    centerY = event->y();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event){
    emit centerPositionChanged(event->x() - centerX, event->y() - centerY);
    centerX = event->x();
    centerY = event->y();
}
