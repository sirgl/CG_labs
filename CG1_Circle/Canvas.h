#ifndef CANVAS_H
#define CANVAS_H

#include "Drawable.h"
#include <QList>
#include <QImage>

class Canvas : public QObject
{
    Q_OBJECT
    QList<Drawable *> drawables;
    QImage* image;
    int width;
    int height;
public:
    Canvas(QObject* parent = 0);

    void add(Drawable* drawable);

    void setSize(int width, int height);

    void removeAll();

    void clear();

    void draw();

    QImage* getImage();
    int getWidth() const;
    int getHeight() const;

signals:
    void redraw();

private slots:
    void elementsChanged();
};

#endif // CANVAS_H
