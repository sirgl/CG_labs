#pragma once

struct Point{
    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Point() : Point(0, 0) {}

    Point(const Point& point) {
        this->x = point.x;
        this->y = point.y;
    }

    Point& operator=(const Point& point) {
        this->x = point.x;
        this->y = point.y;
        return *this;
    }

    Point operator+(const Point& point){
        return Point(x + point.x, y + point.y);
    }

    Point operator-(const Point& point){
        return Point(x - point.x, y - point.y);
    }
};
