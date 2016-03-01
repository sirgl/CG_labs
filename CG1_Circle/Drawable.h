#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QObject>

class Drawable : public QObject{
    Q_OBJECT
public:
    explicit Drawable(QObject *parent = 0);
    virtual void draw(QImage* image) = 0;
signals:
    void redraw();
};

#endif // DRAWABLE_H
