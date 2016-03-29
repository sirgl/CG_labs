#include "BezierCurve.h"
#include "Tools.h"
#include "Line.h"
#include "Circle.h"
#include "DrawingTools.h"
#include <QDebug>
#include <set>

using Tools::sqr;
static const double DELTA = 0.1;

QVector<BezierPoint> BezierCurve::getPoints() const{
    return originPoints;
}

void BezierCurve::setPoints(const QVector<BezierPoint> &value){
    originPoints = value;
}

bool BezierCurve::getFill() const{
    return fill;
}

void BezierCurve::setFill(bool value){
    fill = value;
}

bool BezierCurve::getOutline() const{
    return outline;
}

void BezierCurve::setOutline(bool value){
    outline = value;
}

BezierPoint BezierCurve::scaleAndShiftPoint(BezierPoint point){
    point.x += xOffset;
    point.y += yOffset;
    double scaleK = scale <= 0 ? 1 + (scale / 1000) :  1 + (scale / 100);
    return point * scaleK;
}

int BezierCurve::getXOffset() const{
    return xOffset;
}

void BezierCurve::setXOffset(int value){
    xOffset = value;
    emit redraw();
    emit xOffsetChanged(value);
}

int BezierCurve::getYOffset() const{
    return yOffset;
}

void BezierCurve::setYOffset(int value){
    yOffset = value;
    emit redraw();
    emit yOffsetChanged(value);
}

double BezierCurve::getScale() const{
    return scale;
}

void BezierCurve::setScale(int value){
    scale = value;
    emit redraw();
    emit scaleChanged(value);
}

QVector<QVector<BezierPoint> > BezierCurve::separateToPrimitiveSegments(QVector<BezierPoint> points){
    QVector<QVector<BezierPoint> > segments;
    for(int i = 0;; ++i) {
        if(i >= originPoints.size()) {
            break;
        }
        if(getPoint(i + 1, points).onCurve) {
            QVector<BezierPoint> line;
            line.push_back(getPoint(i, points));
            line.push_back(getPoint(i + 1, points));
            segments.push_back(line);
            continue;
        }
        auto p0 = getPoint(i, points);
        auto p1 = getPoint(i + 1, points);
        auto p2 = getPoint(i + 2, points);

        QVector<BezierPoint> bezierSegment;
        bezierSegment.push_back(p0);
        bezierSegment.push_back(p1);
        bezierSegment.push_back(p2);
        segments.push_back(bezierSegment);

        i++;
    }
    return segments;
}

QVector<BezierPoint> BezierCurve::intersectBezierSegmentWithHorizontalLine(QVector<BezierPoint> bezier, double y){
    auto y0 = bezier[0].y;
    auto y1 = bezier[1].y;
    auto y2 = bezier[2].y;
    double sqrtDiscriminant = std::sqrt(4 * sqr(y1 - y0) - 4 * (y0 - y) * (y0 - 2 * y1 + y2));
    double t1 = (2 * (y0 - y1) + sqrtDiscriminant) / (2 * (y0 - 2 * y1 + y2));
    double t2 = (2 * (y0 - y1) - sqrtDiscriminant) / (2 * (y0 - 2 * y1 + y2));
    QVector<BezierPoint> intersectionPoints;
    if(t1 >= 0 && t1 <= 1) {
        intersectionPoints.push_back(getBezierPointByParameter(bezier[0], bezier[1], bezier[2], t1));
    }
    if(t2 >= 0 && t2 <= 1) {
        intersectionPoints.push_back(getBezierPointByParameter(bezier[0], bezier[1], bezier[2], t2));
    }
    if(2 == intersectionPoints.size() && intersectionPoints[0] == intersectionPoints[1]) {
        intersectionPoints.pop_back();
    }
    return intersectionPoints;
}

QVector<BezierPoint> BezierCurve::intersectLineWithHorizontalLine(QVector<BezierPoint> line, double y){
    double y0 = line[0].y;
    double y1 = line[1].y;
    if(y0 == y1 && y0  == y) {
        return line;
    }
    QVector<BezierPoint> result;
    double t = (y - y0) / (y1 - y0);
    if(t >= 0 && t <= 1) {
        double x = line[0].x + (line[1].x - line[0].x) * t;
        result.push_back(BezierPoint(x, y, false));
    }
    return result;
}

BezierPoint BezierCurve::getBezierPointByParameter(BezierPoint p0, BezierPoint p1, BezierPoint p2, double t){
    return sqr(1 - t) * p0 + 2 * (1 - t) * t * p1 + sqr(t) * p2;
}

QVector<QPair<BezierPoint, BezierPoint> > BezierCurve::getFillingArea(){
    QVector<QPair<BezierPoint, BezierPoint> > fillingLines;

    ///TODO fit to size of monitor
    for(int y = std::round(minY); y < std::round(maxY); ++y){
        QVector<BezierPoint> intersections;
//        y = 20;
        for(auto segment : primitiveSegments) {
            if(segment.size() == 3) {
                auto segIntersections = intersectBezierSegmentWithHorizontalLine(segment, y);
                std::copy(segIntersections.begin(), segIntersections.end(), std::back_inserter(intersections));
            }
            if(segment.size() == 2) {
                auto segIntersections = intersectLineWithHorizontalLine(segment, y);
                std::copy(segIntersections.begin(), segIntersections.end(), std::back_inserter(intersections));
            }
        }
        std::unique(intersections.begin(), intersections.end(), [](BezierPoint a, BezierPoint b) { return a.x - b.x < 0.0004 && a.y - b.y < 0.0004;});
        std::sort(intersections.begin(), intersections.end(), [](BezierPoint a, BezierPoint b) { return a.x < b.x;});
        if((intersections.size() % 2) != 0) {
            qDebug() <<  y << " with size " << intersections.size();
        }
        for(int i = 0; i < intersections.size() / 2; ++i) {
            fillingLines.push_back(QPair<BezierPoint, BezierPoint>(intersections[i * 2], intersections[i * 2 + 1]));
        }
    }
    return fillingLines;
}

BezierCurve::BezierCurve() : scale(0), fill(true), outline(true), xOffset(0), yOffset(0), maxY(0), minY(0) {
}

void BezierCurve::draw(QImage *image){
    if(!outline && !fill) {
        return;
    }
    QVector<BezierPoint> scaledAndShiftedPoints;
    for(auto originPoint : originPoints) {
        scaledAndShiftedPoints.push_back(scaleAndShiftPoint(originPoint));
    }
    minY = (std::min_element(scaledAndShiftedPoints.begin(), scaledAndShiftedPoints.end(), [](BezierPoint a, BezierPoint b) { return a.y < b.y;}))->y;
    maxY = (std::max_element(scaledAndShiftedPoints.begin(), scaledAndShiftedPoints.end(), [](BezierPoint a, BezierPoint b) { return a.y < b.y;}))->y;
    primitiveSegments = separateToPrimitiveSegments(scaledAndShiftedPoints);

    if(outline) {
        drawOutline(image);
    }
    if(fill) {
        auto area = getFillingArea();
        auto bits = image->bits();
        auto bytesPerLine = image->bytesPerLine();
        for(QPair<BezierPoint, BezierPoint> line : area) {
            int y = std::round(line.first.y);
            for(int x = std::round(line.first.x); x < std::round(line.second.x); ++x) {
//                int position = y * bytesPerLine + x * 3;
//                bits[position] = 255;
//                bits[position + 1] = 255;
//                bits[position + 2] = 255;
                DrawingTools::drawPixel(image, Point(x, y)); // TODO
            }
        }
    }
}

void BezierCurve::drawOutline(QImage *image){
    for(QVector<BezierPoint> segment : primitiveSegments) {
        if(segment.size() == 2) {
            Line line;
            line.setCoordinates(segment[0], segment[1]);
            line.draw(image);
        } else if (segment.size() == 3) {
            drawCurve(image, segment[0], segment[1], segment[2]);
        }
    }
}

void BezierCurve::drawCurve(QImage *image, BezierPoint p0, BezierPoint p1, BezierPoint p2){
    BezierPoint middle = getMiddlePoint(p0, p1, p2);
    if(computeDistanceToLine(p0, p2, middle) * p0.distance(p2) < DELTA) {
        Line line1;
        line1.setCoordinates(p0, p2);
        line1.draw(image);
    } else {
        BezierPoint p3 = (p0 + p1) * 0.5;
        BezierPoint p4 = (p1 + p2) * 0.5;
        drawCurve(image, p0, p3, middle);
        drawCurve(image, middle, p4, p2);
    }
}

BezierPoint BezierCurve::getMiddlePoint(BezierPoint p0, BezierPoint p1, BezierPoint p2){
    return sqr(1 - 0.5) * p0 + 2 * (1 - 0.5) * 0.5 * p1 + sqr(0.5) * p2;
}

double BezierCurve::computeDistanceToLine(BezierPoint p1, BezierPoint p2, BezierPoint target){
    auto v1 = (p2.y - p1.y) * target.x;
    auto v2 = (p2.x - p1.x) * target.y;
    auto v3 = p2.x * p1.y - p2.y * p1.x;
    double numerator = std::abs(v1 - v2 + v3);
    double denominator = std::sqrt(sqr(p2.y - p1.y) + sqr(p2.x - p1.x));
    return numerator / denominator;
}

BezierPoint BezierCurve::getPoint(int index, QVector<BezierPoint> points){
    return points[index % points.size()];
}
