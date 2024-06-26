#include "point.h"
#include <QtMath>

Point::Point(double x, double y) : x(x), y(y) {}

double Point::distance(const Point& other) const {
    return qSqrt(qPow(x - other.x, 2) + qPow(y - other.y, 2));
}

double Point::getX() const{
    return x;
}

double Point::getY() const{
    return y;
}
