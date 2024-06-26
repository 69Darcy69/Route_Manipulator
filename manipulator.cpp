#include "manipulator.h"

PaintInfo::PaintInfo(int id, QColor color) : id(id), color(color) {}

Manipulator::Manipulator(double x, double y, double R, int id, QColor color) : R(R), current_position(x, y), paint({id, color}){}

Manipulator::Manipulator(): R(0), current_position(0,0), paint({0, Qt::red}) {}

bool Manipulator::canMove(const Point& point) const {
    return current_position.distance(point) <= R;
}

void Manipulator::move(const Point& point) {
    current_position = point;
}

Point Manipulator::getPosition() const {
    return current_position;
}
