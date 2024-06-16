#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    //Можно ли разместить манипулятор в данной точке?
    bool canMove(const Point& point) const;
    //Размещение манипулятора в новой точке
    void move(const Point& point);
    //Получение текущего положения манипулятора
    Point getPosition() const;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonRead_clicked();   //Слот обработки кнопки начала чтения
    void timerAlarm();  //Слот обработки таймера на считывание данных
    void updateGraph(); //Слот обработки таймера на обновление графиков

private:
    Ui::MainWindow *ui;
    QTimer *timer;  //Таймер считывания
    QVector<Manipulator> manipulators;  //Вектор с манипуляторами
    QFile file; //Файл с точками
    QTextStream *fileIn;    //Данные из файла с точками
    int time_timer; //Время установленное для таймера на считывание в секундах
    //Для графиков
    double step;    //Шаг рисования графиков
    QTimer *draw_timer; //Таймер обновления графиков
    int count_manipulators; //Количество манипуляторов
    int current_graph;  //Текущий граф в отрисовке

    void addPoint(Point point, QColor color, int flag); //Добавление новой точки
    void draw(int id, Point start_point, Point end_point);  //Задание манипулятора на отрисовку
};

#endif // MAINWINDOW_H
