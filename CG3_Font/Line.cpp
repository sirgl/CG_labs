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
    double x1 = this->x1;
    double x2 = this->x2;
    double y1 = this->y1;
    double y2 = this->y2;

    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const double dx = x2 - x1;
    const double dy = std::abs(std::round(y2) - std::round(y1)); // changed, was (y2 - y1) without rounding

    double error = dx / 2.0;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = std::round(y1);

    if(steep) { // added
        DrawingTools::drawPixel(image, Point(std::round(y1), std::round(x1)));
        DrawingTools::drawPixel(image, Point(std::round(y2), std::round(x2)));
    } else {
        DrawingTools::drawPixel(image, Point(std::round(x1), std::round(y1)));
        DrawingTools::drawPixel(image, Point(std::round(x2), std::round(y2)));
    }

    const int maxX = std::round(x2);

    for(int x = std::round(x1); x <= maxX; x++) { // changed, was <
        if(steep) {
            DrawingTools::drawPixel(image, Point(y, x));
        }
        else{
            DrawingTools::drawPixel(image, Point(x, y));
        }

        error -= dy;
        if(error < 0) {
            y += ystep;
            error += dx;
        }
    }
}
