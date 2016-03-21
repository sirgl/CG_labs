#pragma once
#include <QImage>
#include <QColor>
#include "Point.h"

//x and y in new coordinates
namespace DrawingTools {
    void drawPixel(QImage* image, int x, int y, QColor* color);
    void drawPixel(QImage* image, Point point, QColor* color);
    void drawPixel(QImage* image, Point point);

}
