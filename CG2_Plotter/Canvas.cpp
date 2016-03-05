#include "Canvas.h"
#include "cassert"

int Canvas::getWidth() const{
    return width;
}

int Canvas::getHeight() const{
    return height;
}

Canvas::Canvas(QObject* parent) : QObject(parent) {}

void Canvas::add(Drawable *drawable){
    drawables.push_back(drawable);
    draw();
    connect(drawable, SIGNAL(redraw()), this, SLOT(elementsChanged()));
}

void Canvas::setSize(int width, int height){
    image = new QImage(width, height, QImage::Format_RGB888);
    this->width = width;
    this->height = height;
    draw();
    emit redraw();
}

void Canvas::removeAll(){
    drawables.clear();
    emit redraw();
}

void Canvas::clear(){
    memset(image->bits(), 255, image->byteCount());
}

void Canvas::draw(){
    assert(image);
    clear();
    for (auto drawable : drawables) {
        drawable->draw(image);
    }
}

QImage *Canvas::getImage(){
    return image;
}

void Canvas::elementsChanged(){
    draw();
    emit redraw();
}
