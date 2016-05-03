#include "SphereProjector.h"

#include "Tools.h"
#include <QDebug>
#include "DrawingTools.h"
#include <cmath>
#include "LoadingFileException.h"
#include "ParserException.h"
#include <QGraphicsRotation>
#include <QTransform>

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
    auto theta = std::acos(y / sqrt(sqr(x) + sqr(y) + sqr(z)));
    auto phi = std::atan2(z, x);
    return qMakePair(theta, phi);
}

QVector2D SphereProjector::computeTextureCoordinates(double theta, double phi){

    auto u = ((phi + M_PI) / (2 * M_PI));
    auto v = 1 -(theta / (M_PI));
//    auto v = theta / (2 * M_PI) + 0.5;
//    auto u = phi / M_PI + 0.5;
    return QVector2D(u, v);
}

QColor SphereProjector::computeColorByTextureCoordinates(double u, double v){ // u,v from 0 to 1
    if(filtration == FiltrationType::nearest) {
        auto newU = 0.5 + u * texture->width();
        auto newV = 0.5 + v * texture->height();
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
        return QColor(r, g, b);
    }
    return QColor(255, 255, 255);
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
    emit redraw();
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
    emit redraw();
}

SphereProjector::SphereProjector() : r(0), scale(0), rScaled(0), x(0), y(0), filtration(FiltrationType::nearest)
{
    setR(255);
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

            QMatrix4x4 ym;
            ym.rotate(normalizedX, 0, 1, 0);
            QMatrix4x4 zm;
            zm.rotate(normalizedY, 0, 0, 1);
            auto coords2 = coords * ym;
            auto coords3 = coords2 * zm;

            auto sphericalCoords = computeSphericalCoordinates(coords3);
            auto textureCoords = computeTextureCoordinates(sphericalCoords.first, sphericalCoords.second);
            auto color = computeColorByTextureCoordinates(textureCoords.x(), textureCoords.y());
            DrawingTools::drawPixel(image, Point(newX, newY), &color);
        }
    }
}

QJsonObject SphereProjector::saveToJson(){
    QJsonObject object;
    object["x"] = x;
    object["y"] = y;
    object["scale"] = scale;
    object["filter"] = filtration == FiltrationType::nearest ? "nearest" : "bilinear";
    object["source"] = source;
    return object;
}

void SphereProjector::loadFromJson(QJsonObject object){
    auto xRef = object["x"];
    if(!xRef.isDouble()) {
        throw ParserException("Bad json format");
    }
    auto xObj = xRef.toInt();

    auto yRef = object["y"];
    if(!yRef.isDouble()) {
        throw ParserException("Bad json format");
    }
    auto yObj = yRef.toInt();

    auto scaleRef = object["scale"];
    if(!scaleRef.isDouble()) {
        throw ParserException("Bad json format");
    }
    auto scaleObj = scaleRef.toDouble();

    auto filterRef = object["filter"];
    if(!filterRef.isString()) {
        throw ParserException("Bad json format");
    }
    auto filterObj = filterRef.toString();
    if(!(filterObj == "bilinear" || filterObj == "nearest")) {
        throw ParserException("Bad json format: bad filtration type");
    }
    FiltrationType filtrationType;
    if(filterObj == "bilinear") {
        filtrationType = FiltrationType::bilinear;
    }
    if(filterObj == "nearest") {
        filtrationType = FiltrationType::nearest;
    }
    auto sourceRef = object["source"];
    if(!sourceRef.isString()) {
        throw ParserException("Bad json format");
    }
    auto sourceObj = sourceRef.toString();

    setX(xObj);
    setY(yObj);
    setScale(scaleObj);
    setFiltration(filtrationType);
    setTexutureFile(sourceObj);
}

void SphereProjector::setTexutureFile(QString filename){
    texture = new QImage();
    source = filename;
    if(!texture->load(filename)){
        throw LoadingFileException("File not found");
    }
}
