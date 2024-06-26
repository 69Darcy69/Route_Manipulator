#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include "point.h"
#include <QColor>

//Структура информации для рисования
struct PaintInfo {
    int id; //Id манипулятора
    QColor color;   //Цвет линии
    QVector<double> x_points, y_points; //Вектора с точками прямой по x и y
    int current_index = 0;  //Текущая точка прямой

    PaintInfo(int id, QColor color);
};
//Класс манипулятора
class Manipulator {
    double R;   //Радиус манипулятора
    Point current_position; //Текущее положение манипулятора
public:
    PaintInfo paint;    //Информация для рисования
    Manipulator(double x, double y, double R, int id, QColor color);
    Manipulator();
    //Можно ли разместить манипулятор в данной точке?
    bool canMove(const Point& point) const;
    //Размещение манипулятора в новой точке
    void move(const Point& point);
    //Получение текущего положения манипулятора
    Point getPosition() const;
};

#endif // MANIPULATOR_H
