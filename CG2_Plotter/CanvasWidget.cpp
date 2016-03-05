#include "CanvasWidget.h"
#include <QPainter>
#include <QResizeEvent>
#include <cassert>

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent){
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
