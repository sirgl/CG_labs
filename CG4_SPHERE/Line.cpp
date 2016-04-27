#include "Line.h"
#include "DrawingTools.h"

Line::Line(){

}

void Line::setCoordinates(double x1, double y1, double x2, double y2){
    this->x1 = std::round(x1);
    this->y1 = std::round(y1);
    this->x2 = std::round(x2);
    this->y2 = std::round(y2);
    emit redraw();
}

void Line::setCoordinates(BezierPoint p1, BezierPoint p2){
    setCoordinates(p1.x, p1.y, p2.x, p2.y);
}

void Line::draw(QImage *image){
    int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;){
      DrawingTools::drawPixel(image, Point(x1, y1));
      if (x1==x2 && y1==y2) break;
      e2 = err;
      if (e2 >-dx) { err -= dy; x1 += sx; }
      if (e2 < dy) { err += dx; y1 += sy; }
    }
}
