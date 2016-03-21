#ifndef AXIS_H
#define AXIS_H

#include "Drawable.h"
#include <QImage>


class Axis : public Drawable {
    Q_OBJECT
    bool isHorisontal;
public:
    Axis();
    void draw(QImage* image) override;
    void setMode(bool isHorizontal);
signals:
    void redraw();
};

#endif // AXIS_H
