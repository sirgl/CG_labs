#ifndef SPHEREPROJECTOR_H
#define SPHEREPROJECTOR_H

#include "Drawable.h"
#include <QVector3D>
#include <QVector2D>
#include <QColor>
#include <QImage>
#include <QJsonObject>

enum class FiltrationType {
    nearest,
    bilinear
};

class SphereProjector : public Drawable
{
    QVector3D computeSphereIntersectionCoordinates(QVector2D screenVector);
    bool computeSphereIntersectionCoordinates(QVector2D screenVector, QVector3D &result);
    //theta, phi
    QPair<double, double> computeSphericalCoordinates(QVector3D vector);
    QVector2D computeTextureCoordinates(double theta, double phi);

    QColor computeColorByTextureCoordinates(double u, double v);
    int computeBilinearFiltrationForOneChannel(double u_ratio, double v_ratio, double u_opposite, double v_opposite, int c1, int c2, int c3, int c4);

    int scale;
    double rScaled;
    double r;

    int x;
    int normalizedX;
    int y;
    int normalizedY;
    FiltrationType filtration;
    QImage* texture;
    QString source;
public:
    SphereProjector();

    void draw(QImage *image);
    QJsonObject saveToJson();
    void loadFromJson(QJsonObject object);

    void setTexutureFile(QString filename);
    double getR() const;
    void setR(double value);
    int getScale() const;
    void setScale(int value);
    int getX() const;
    void setX(int value);
    int getY() const;
    void setY(int value);
    FiltrationType getFiltration() const;
    void setFiltration(const FiltrationType &value);
};

#endif // SPHEREPROJECTOR_H
