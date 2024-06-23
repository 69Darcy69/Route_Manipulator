#include "mainwindow.h"

#include <QApplication>

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setlocale(LC_ALL, "");
    try {
        MainWindow w;
        w.setWindowState(Qt::WindowMaximized);
        w.show();
        return a.exec();
    } catch (const std::exception& e) {
        qCritical() << "Ошибка:" << e.what();
        return 1;
    }
}
