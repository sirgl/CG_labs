#include "DrawingTools.h"
#include <QSet>
#include <QPair>
#include <QDebug>

namespace DrawingTools {
    void drawPixel(QImage *image, int x, int y, QColor* color){
        if(!image) {
            return;
        }
        auto height = image->height();
        auto width = image->width();
        int newX = x + width / 2;
        int newY = y + height / 2;
        if(newX < 0 || newX >= width || newY < 0 || newY >= height) {
            return;
        }

        //DEBUG

//        qDebug() << "Drawing " << x << " " << y;

        //END


        unsigned char* point = image->bits() + newY * image->bytesPerLine() + newX * 3;
        *point++ = (char)color->red();
        *point++ = (char)color->green();
        *point = (char)color->blue();
    }

    void drawPixel(QImage *image, Point point, QColor *color){
        drawPixel(image, point.x, point.y, color);auto height = image->height();
        auto width = image->width();
    }

    void drawPixel(QImage* image, Point point) {
        drawPixel(image, point, new QColor(0, 0, 0));
    }


    QColor getColor(QImage* image, int x, int y) {
        if(x >= image->width()) {
            x = image->width() - 1;
        }
        if(x < 0) {
            x = 0;
        }
        if(y >= image->height()) {
            y = image->height() - 1;
        }
        if(y < 0) {
            y = 0;
        }
//        if(x >= image->width() || x < 0 || y >= image->height() || y < 0) {
//            return QColor(255,255,255);
//        }
//        if(x > ) // TODO
//        unsigned char* point = image->bits() + y * image->bytesPerLine() + x * 3;
//        auto r = *point++;
//        auto g = *point++;
//        auto b = *point;
//        return QColor(r,g,b);
        QRgb pixel = image->pixel(x,y);
        return QColor(pixel);

    }

}
