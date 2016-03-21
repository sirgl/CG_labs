#include "Circle.h"
#include "ParserException.h"
#include <QDebug>

QColor *Circle::getColor() const{
    return color;
}

void Circle::setPosition(int x, int y){
    this->xCenter = x;
    this->yCenter = y;
    emit redraw();
}

void Circle::setX(int x){
    this->xCenter = x;
    emit redraw();
}

void Circle::setY(int y){
    this->yCenter = y;
    emit redraw();
}

void Circle::setRadius(int radius){
    this->radius = radius;
    emit redraw();
}

int Circle::getX(){
    return xCenter;
}

int Circle::getY(){
    return yCenter;
}

int Circle::getR(){
    return radius;
}

void Circle::setColor(QColor *color){
    if(this->color) {
        delete color;
    }
    this->color = color;
    emit redraw();
}

void Circle::draw(QImage* image) {
    int height = image->height();
    int width = image->width();

    int upBorder = yCenter - radius;
    upBorder = -(height / 2) >= upBorder ? -(height / 2) + 1 : upBorder;
    int downBorder = yCenter + radius;
    downBorder = height / 2 < downBorder ? (height / 2)  : downBorder;

    for (int y = upBorder; y <= downBorder; y++) {
        int d = (int)(sqrt((double)(radius * radius - (y - yCenter) * (y - yCenter))));
        int leftBorder = xCenter - d;
        int rightBorder = xCenter + d;
        leftBorder = -(width / 2) > leftBorder ? -(width / 2) : leftBorder;
        rightBorder = (width / 2) <= rightBorder ? (width / 2) - 1 : rightBorder;

        int newX = (width / 2 + leftBorder);
        int newY = (height / 2 - y);
        unsigned char* point = image->bits() + (newY * image->bytesPerLine() + newX * 3);
        for(int x = leftBorder; x <= rightBorder; ++x) {
            *point++ = color->red();
            *point++ = color->green();
            *point++ = color->blue();
        }
    }
}

Circle::Circle(QObject *parent) : Drawable(parent){
    color = new QColor(100, 200, 50);
}


QJsonObject Circle::saveToJson() {
	QJsonObject position;
    position["x"] = xCenter;
    position["y"] = yCenter;
	QJsonObject json;
	json["R"] = radius;
	json["position"] = position;
	return json;
}

void Circle::loadFromJson(QJsonObject& object) {
	auto r = object["R"];
	if (!r.isDouble()) {
		throw ParserException("R must be numeric");
	}
	auto radius = r.toInt();
	auto positionTmp = object["position"];
	if (!positionTmp.isObject()) {
		throw ParserException("Position must be object");
	}
	auto position = positionTmp.toObject();
	auto xTmp = position["x"];
	if (!xTmp.isDouble()) {
		throw ParserException("X must be numeric");
	}
	auto x = xTmp.toInt();
	auto yTmp = position["y"];
    if (!yTmp.isDouble()) {
		throw ParserException("Y must be numeric");
	}
	auto y = position["y"].toInt();
    this->xCenter = x;
    this->yCenter = y;
    this->radius = radius;
}

Circle::~Circle(){
}
