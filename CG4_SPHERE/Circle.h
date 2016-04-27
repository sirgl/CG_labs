#pragma once

#include <QJsonObject>
#include "Drawable.h"
#include <QColor>
#include <QImage>

class Circle : public Drawable {
    Q_OBJECT
    int xCenter;
    int yCenter;
    int radius;
    QColor* color;
public:
    void setPosition(int xCenter, int yCenter);
    void setX(int x);
    void setY(int y);
    void setRadius(int radius);
    int getX();
    int getY();
    int getR();
    void setColor(QColor* color);

	void draw(QImage* image) override;

    Circle(QObject* parent = 0);

    QJsonObject saveToJson();

    void loadFromJson(QJsonObject& object);

    ~Circle();

    QColor *getColor() const;
signals:
    void redraw();
};
