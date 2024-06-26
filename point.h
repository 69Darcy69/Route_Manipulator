#ifndef POINT_H
#define POINT_H
#include <QDebug>

//Класс точки
class Point {
    double x, y;    //Координаты точки
public:
    Point(double x = 0, double y = 0);
    //Расчет расстояния от А до В
    double distance(const Point& other) const;
    //Перегрузка оператора вывода для точки
    friend QDebug operator<<(QDebug stream, const Point& point) {
        stream.nospace() << "{" << point.x << "; " << point.y << "}";
        return stream;
    }
    //Получение координаты X
    double getX() const;
    //Получение координаты Y
    double getY() const;
};

#endif // POINT_H
