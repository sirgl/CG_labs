#include "DrawingTools.h"
#include <QSet>
#include <QPair>
#include <QDebug>

namespace DrawingTools {
    QSet<QPair<int, int>> uniquePoints;

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

        uniquePoints.insert(QPair<int, int>(x, y));
        qDebug() << "Drawing " << x << " " << y;

        //END


        unsigned char* point = image->bits() + newY * image->bytesPerLine() + newX * 3;
        *point++ = (char)color->red();
        *point++ = (char)color->green();
        *point = (char)color->blue();
    }

    void drawPixel(QImage *image, Point point, QColor *color){
        drawPixel(image, point.x, point.y, color);
    }

    void drawPixel(QImage* image, Point point) {
        drawPixel(image, point, new QColor(0, 0, 0));
    }

    int getUnique()    {
        return uniquePoints.size();
    }



}
