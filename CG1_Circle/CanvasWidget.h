#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include "Drawable.h"
#include <QWidget>
#include <QList>
#include "Canvas.h"

class   CanvasWidget : public QWidget {
    Q_OBJECT

    Canvas* canvas;

    static const int DEFAULT_WIDTH = 300;
    static const int DEFAULT_HEIGHT = 200;
public:
    explicit CanvasWidget(QWidget *parent = 0);

    void add(Drawable* drawable);

    void setCanvasSize(int width, int height);

    void removeAll();

    int getCanvasWidth();
    int getCanvasHeight();

protected:

    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*);

signals:

public slots:
};

#endif // CANVASWIDGET_H
