#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "manipulator.h"
#include <QTimer>
#include <QFile>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void moveManipulator(int id, QStringList coords, Manipulator& manipulator, Point point); //Функция перемещения манипулятора
    void on_pushButtonRead_clicked();   //Слот обработки кнопки начала чтения
    void timerAlarm();  //Слот обработки таймера на считывание данных
    void updateGraph(); //Слот обработки таймера на обновление графиков

    void sockReady();
    void sockDisc();

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

    //Клиент-Сервер
    QTcpSocket * socket;
    QByteArray Data;
    QJsonDocument doc;
    QJsonParseError docError;
};

#endif // MAINWINDOW_H
