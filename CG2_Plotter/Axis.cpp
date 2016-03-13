#include "Axis.h"
#include <QDebug>

Axis::Axis(){
    this->isHorisontal = false;
}

void Axis::draw(QImage *image){
    int width = image->width();
    int height = image->height();
    if(isHorisontal) {
        int y = height / 2;
        unsigned char* point = image->bits() + y * image->bytesPerLine();
        for (int x = 0; x < width; ++x) {
            *point++ = 0;
            *point++ = 0;
            *point++ = 0;
        }
    } else {
        int x = width / 2;
        qDebug() << x << " when width is " << width;
        for(int y = 0; y < height; ++y) {
            unsigned char* point = image->bits() + y * image->bytesPerLine() + x * 3;
            *point++ = 0;
            *point++ = 0;
            *point = 0;
        }
    }
}

void Axis::setMode(bool isHorizontal){
    this->isHorisontal = isHorizontal;
    emit redraw();
}


