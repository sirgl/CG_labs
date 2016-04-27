#include "SphereProjector.h"

#include "Tools.h"
#include <QDebug>
#include "DrawingTools.h"
#include <cmath>

using Tools::sqr;

QVector3D SphereProjector::computeSphereIntersectionCoordinates(QVector2D screenVector){
    auto x = screenVector.x();
    auto y = screenVector.y();
    auto z = -std::sqrt(sqr(rScaled) - sqr(x) - sqr(y));
    return QVector3D(x, y, z);
}

bool SphereProjector::computeSphereIntersectionCoordinates(QVector2D screenVector, QVector3D& result){
    auto x = screenVector.x();
    auto y = screenVector.y();
    auto a = sqr(rScaled) - sqr(x) - sqr(y);
    if(a <= 0) {
        return false;
    }
    auto z = - std::sqrt(a);
    result = QVector3D(x, y, z);
    return true;
}

QPair<double, double> SphereProjector::computeSphericalCoordinates(QVector3D vector){
    auto x = vector.x();
    auto y = vector.y();
    auto z = vector.z();
    auto theta = std::acos(x / sqrt(sqr(x) + sqr(y) + sqr(z)));
    auto phi = std::atan(y / x);
    return qMakePair(theta, phi);
}

QVector2D SphereProjector::computeTextureCoordinates(double theta, double phi){
//    int phiShiftedDegrees = (int)std::round((phi / (2 * M_PI)) * 360 + x) % 360;
//    int thetaShiftedDegrees = (int) std::round((theta / (2 * M_PI)) * 360 + y) % 360;
//    double newPhi = ((double)phiShiftedDegrees / 360) * 2 * M_PI;
//    double newTheta = ((double)thetaShiftedDegrees / 360) * 2 * M_PI;

    auto newPhi = (((double)normalizedX / 360) * 2 * M_PI) + phi + 0.5 * M_PI;
    double intpart;
    double fracpart = std::modf(newPhi / ( M_PI), &intpart);
    newPhi = fracpart * (M_PI) - 0.5 * M_PI;

    auto newTheta = (((double)normalizedY / 360) * 2 * M_PI) + theta;
    fracpart = std::modf(newTheta / (2 * M_PI), &intpart);
    newTheta = fracpart * (2 * M_PI);

    auto u = (newPhi / M_PI + 0.5);
    auto v = (newTheta / (2 * M_PI));
    return QVector2D(u, v);
}

//Bad method
QVector2D SphereProjector::computeTextureCoordinates(QVector3D vector){
    auto x = vector.x();
    auto y = vector.y();
    auto z = vector.z();
    auto u = 0.5 + std::atan2(z, x) / 2 * M_PI;
    auto v = 0.5 - std::asin(y) / M_PI;
    return QVector2D(u, v);
}

QColor SphereProjector::computeColorByTextureCoordinates(double u, double v){ // u,v from 0 to 1
//    auto newU = u - std::floor(u);
//    auto newV = v - std::floor(v);
//    if((newU < 0.5 && newV < 0.5) || (newU >= 0.5 && newV >= 0.5)) {
//        return QColor(0, 0, 0);
//    }
//    return QColor(255, 255, 255);

    if(filtration == FiltrationType::nearest) {
        auto newU = 0.5 + u * texture->height();
        auto newV = 0.5 + v * texture->width();
        auto uRounded = std::round(newU);
        auto vRounded = std::round(newV);
        return DrawingTools::getColor(texture, uRounded, vRounded);
    } else if (filtration == FiltrationType::bilinear) {
        if (std::isnan(u) || std::isnan(v)) {
            return QColor(0, 0, 0);
        }
        u *= texture->width();
        v *= texture->height();
        int x = std::floor(u);
        int y = std::floor(v);
        double u_ratio = u - x;
        double v_ratio = v - y;
        double u_opposite = 1 - u_ratio;
        double v_opposite = 1 - v_ratio;
        auto c1 = DrawingTools::getColor(texture, x,     y);
        auto c2 = DrawingTools::getColor(texture, x + 1, y);
        auto c3 = DrawingTools::getColor(texture, x,     y + 1);
        auto c4 = DrawingTools::getColor(texture, x + 1, y + 1);
        auto r = computeBilinearFiltrationForOneChannel(u_ratio, v_ratio, u_opposite, v_opposite, c1.red(), c2.red(), c3.red(), c4.red());
        auto g = computeBilinearFiltrationForOneChannel(u_ratio, v_ratio, u_opposite, v_opposite, c1.green(), c2.green(), c3.green(), c4.green());
        auto b = computeBilinearFiltrationForOneChannel(u_ratio, v_ratio, u_opposite, v_opposite, c1.blue(), c2.blue(), c3.blue(), c4.blue());
        if(r > 255 || g > 255 || b > 255) {
             auto d = 1;
        }
        if(r < 0 || g < 0 || b < 0) {
             auto d = 1;
        }
        return QColor(r, g, b);
    }


}

int SphereProjector::computeBilinearFiltrationForOneChannel(double u_ratio, double v_ratio, double u_opposite, double v_opposite, int c1, int c2, int c3, int c4){
    return std::round((c1 * u_opposite + c2 * u_ratio) * v_opposite + (c3 * u_opposite + c4 * u_ratio) * v_ratio);
}

FiltrationType SphereProjector::getFiltration() const
{
    return filtration;
}

void SphereProjector::setFiltration(const FiltrationType &value)
{
    filtration = value;
}


int SphereProjector::getY() const
{
    return y;
}

void SphereProjector::setY(int value)
{
    y = value;
    normalizedY = (360 + y % 360) % 360;
    emit redraw();
}

int SphereProjector::getX() const
{
    return x;
}

void SphereProjector::setX(int value)
{
    x = value;
    normalizedX = (360 + x % 360) % 360;
    emit redraw();
}



int SphereProjector::getScale() const
{
    return scale;
}

void SphereProjector::setScale(int value)
{
    scale = value;
    rScaled = scale <= 0 ? r * (1 + (double)scale / 1000) : r * (1 + (double)scale / 100);
    emit redraw();
}

double SphereProjector::getR() const
{
    return r;
}

void SphereProjector::setR(double value)
{
    r = value;

}

SphereProjector::SphereProjector() : r(0), scale(0), rScaled(0), x(0), y(0), filtration(FiltrationType::nearest)
{
    texture = new QImage();
//    texture->load("Lenna.png");
    texture->load("1.jpg");
}

void SphereProjector::draw(QImage *image){
    auto height = image->height();
    auto width = image->width();
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            auto newX = x - width / 2;
            auto newY = y - height / 2;


            QVector3D coords;
            auto success = computeSphereIntersectionCoordinates(QVector2D(newX, newY), coords);
            if(!success) {
                continue;
            }


            //DEBUG

//            auto mockX = (double)x / image->width();
//            auto mockY = (double)y / image->height();
//            QVector2D mockCoords(mockX, mockY);
//            auto color = computeColorByTextureCoordinates(mockCoords.x(), mockCoords.y());
//            DrawingTools::drawPixel(image, Point(newX, newY), &color);

            //END DEBUG

            auto sphericalCoords = computeSphericalCoordinates(coords);
            auto textureCoords = computeTextureCoordinates(sphericalCoords.first, sphericalCoords.second);
            auto color = computeColorByTextureCoordinates(textureCoords.x(), textureCoords.y());
            DrawingTools::drawPixel(image, Point(newX, newY), &color);
        }
    }
}
