#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),  timer(nullptr), file(nullptr), fileIn(nullptr),
time_timer(0), step(0.1), draw_timer(nullptr), count_manipulators(2), current_graph(-1) {
    ui->setupUi(this);
    //Поместить путь к файлу по умолчанию в поле для ввода
    ui->editDirectory->setText("C:/QTrepos/z1/points.txt");
    //Включение зума и скролинга для плоскостей графиков
    ui->linePlot->setInteraction(QCP::iRangeDrag, true);
    ui->linePlot->setInteraction(QCP::iRangeZoom, true);
    ui->pointPlot->setInteraction(QCP::iRangeDrag, true);
    ui->pointPlot->setInteraction(QCP::iRangeZoom, true);
    //Установка диапазона осей
    ui->linePlot->xAxis->setRange(-7, 4);
    ui->linePlot->yAxis->setRange(-10, 4);
    ui->pointPlot->xAxis->setRange(-7, 4);
    ui->pointPlot->yAxis->setRange(-10, 4);

        //------------Отладочная информация--------------
        qDebug().nospace() << "Приложение создано.";
        //-----------------------------------------------
}

MainWindow::~MainWindow() {
    if(file.isOpen()){
        file.close();
    }
    if (timer != nullptr) {
        delete timer;
    }
    if (draw_timer != nullptr) {
        delete draw_timer;
    }
    delete ui;

        //------------Отладочная информация--------------
        qDebug().nospace() << "Приложение закрыто.";
        //-----------------------------------------------
}
//Добавление новой точки
void MainWindow::addPoint(Point point, QColor color, int flag)
{
    //Получение координат точки
    QVector<double> x_vec(1), y_vec(1);
    x_vec[0] = point.getX();
    y_vec[0] = point.getY();
    if (flag == 0) {
    //Точка на график маршрута
        QCPGraph *pointLineGraph = ui->linePlot->addGraph();
        current_graph++;
        pointLineGraph->setLineStyle(QCPGraph::lsNone); //Отключение линий
        pointLineGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, color, color, 10)); //Размер точки = 10
        pointLineGraph->setData(x_vec, y_vec);
        // Перерисовка графика
        ui->linePlot->replot();

            //------------Отладочная информация--------------
            qDebug().nospace() << "Точка (маршрут): " << point << " установлена.";
            //-----------------------------------------------
    }
    else if (flag == 1) {
    //Точка на график передвижения
        QCPGraph *pointGraph = ui->pointPlot->addGraph();
        pointGraph->setLineStyle(QCPGraph::lsNone); //Отключение линий
        pointGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, color, color, 10)); //Размер точки = 10
        pointGraph->setData(x_vec, y_vec);
        // Перерисовка графика
        ui->pointPlot->replot();

            //------------Отладочная информация--------------
            qDebug().nospace() << "Точка (передвижение): " << point << " установлена.";
            //-----------------------------------------------
    }
    else {
        QMessageBox::critical(this, "Ошибка", "Неверный тип графика.");

            //------------Отладочная информация--------------
            qWarning() << "Ошибка флага при вызвове функции добавления точки!";
            //-----------------------------------------------
    }
}
//Задание манипулятора на отрисовку
void MainWindow::draw(int id, Point start_point, Point end_point){
    //Получение координат точек, расстояния между ними и количества точек рисования
    double x_start = start_point.getX();
    double x_end = end_point.getX();
    double y_start = start_point.getY();
    double y_end = end_point.getY();
    double distance = start_point.distance(end_point);
    int add_points = static_cast<int>(distance / step) + 1;
    //Измение размеров векторов с точками для рисования
    manipulators[id].paint.x_points.resize(add_points);
    manipulators[id].paint.y_points.resize(add_points);
    //Заполнение векторов с точками для рисования
    for (int i = 0; i < add_points; ++i){
        double t = static_cast<double>(i) / (add_points - 1);
        manipulators[id].paint.x_points[i] = x_start + t * (x_end - x_start);
        manipulators[id].paint.y_points[i] = y_start + t * (y_end - y_start);
    }
    //Данные для рисования маршрута
    manipulators[id].paint.current_index = 0;
    ui->linePlot->addGraph();
    current_graph++;
    QPen pen;
    pen.setColor(manipulators[id].paint.color);
    ui->linePlot->graph(current_graph)->setPen(pen);
    //Данные для рисования передвижения
    QColor color = manipulators[id].paint.color;
    ui->pointPlot->clearPlottables();
    ui->pointPlot->addGraph();
    for (int i = 0; i < count_manipulators; ++i) {
        if (i != id) {
            addPoint(manipulators[i].getPosition(), manipulators[i].paint.color, 1);
        }
    }
    ui->pointPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->pointPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, color, color, 10));
    //Запуск таймер
    draw_timer = new QTimer();
    connect(draw_timer, SIGNAL(timeout()), this, SLOT(updateGraph()));
    draw_timer->start(static_cast<int>(time_timer * 700 / add_points));

        //------------Отладочная информация--------------
        qDebug().nospace() << "Таймер отрисовки запущен.";
        //-----------------------------------------------
}
//Обработка таймера обновления графиков
void MainWindow::updateGraph(){
    bool all_graphs_completed = true; //Все графики отрисованы?
    //Цикл отрисовки
    for (int i = 0; i < count_manipulators; ++i) {
        if (manipulators[i].paint.current_index < manipulators[i].paint.x_points.size()) {
            all_graphs_completed = false;   //изменить, если графики рисуются
            //Отрисовка графика маршрута
            QVector<double> current_vector_x = manipulators[i].paint.x_points.mid(0, manipulators[i].paint.current_index + 1);
            QVector<double> current_vector_y = manipulators[i].paint.y_points.mid(0, manipulators[i].paint.current_index + 1);
            ui->linePlot->graph(current_graph)->setData(current_vector_x, current_vector_y);
            //Отрисовка графика передвижения
            double current_x = manipulators[i].paint.x_points[manipulators[i].paint.current_index];
            double current_y = manipulators[i].paint.y_points[manipulators[i].paint.current_index];
            QVector<double> x_vec(1), y_vec(1);
            x_vec[0] = current_x;
            y_vec[0] = current_y;
            ui->pointPlot->graph(0)->setData(x_vec, y_vec);
            //Перерисовка графика передвижения
            ui->pointPlot->replot();
            //Увеличение индекса текущей точки
            manipulators[i].paint.current_index++;
        }
    }
    //Перерисовка графика маршрута
    ui->linePlot->replot();
    if (all_graphs_completed) {
    //Если все графики отрисованы
        //Удаление таймера
        delete draw_timer;
        draw_timer = nullptr;

            //------------Отладочная информация--------------
            qDebug().nospace() << "Графики отрисованы. Таймер остановлен.";
            //-----------------------------------------------

        //Установка точки окончания итерации на графике маршрута
        QPen pen = ui->linePlot->graph(current_graph)->pen();
        QColor color = pen.color();
        for (int i = 0; i < count_manipulators; i++) {
            if (color == manipulators[i].paint.color) {
                addPoint(manipulators[i].getPosition(), manipulators[i].paint.color, 0);
                break;
            }
        }
    }
}

//Обработка нажатия на кнопку "Начать чтение файла"
void MainWindow::on_pushButtonRead_clicked() {
    bool ok, ok1, ok2, ok3, ok4, ok5;
    //Закрытие файла и удаление таймера, если кнопка нажата не впервые
    if(file.isOpen()){
        file.close();

            //------------Отладочная информация--------------
            qDebug().nospace() << "Открытый файл был закрыт.";
            //-----------------------------------------------

    }
    if (timer != nullptr) {
        delete timer;

            //------------Отладочная информация--------------
            qDebug().nospace() << "Запущенный таймер чтения был удален.";
            //-----------------------------------------------
    }
    if (draw_timer != nullptr) {
        delete draw_timer;

            //------------Отладочная информация--------------
            qDebug().nospace() << "Запущенный таймер отрисовки был удален.";
            //-----------------------------------------------
    }
    manipulators.clear();
    //Очистка графиков
    ui->linePlot->clearPlottables();
    ui->pointPlot->clearPlottables();
    current_graph = -1;
    //Получение стартовых значений
    double startX1 = ui->editStartX_1->text().toDouble(&ok);
    double startX2 = ui->editStartX_2->text().toDouble(&ok1);
    double startY1 = ui->editStartY_1->text().toDouble(&ok2);
    double startY2 = ui->editStartY_2->text().toDouble(&ok3);
    double R1 = ui->editRadius_1->text().toDouble(&ok4);
    double R2 = ui->editRadius_2->text().toDouble(&ok5);
    //Проверка успешного преобразования стартовых значений
    if (!ok || !ok1 || !ok2 || !ok3 || !ok4 || !ok5) {
        QMessageBox::critical(this, "Ошибка", "Поля должны быть заполнены числами.\nВещественные числа вносятся через '.'.");

            //------------Отладочная информация--------------
            qWarning() << "Ошибка преобразования значения в double!";
            //-----------------------------------------------
    }
    else {
            //------------Отладочная информация--------------
            qDebug().nospace() << "Стартовые значения были получены.";
            //-----------------------------------------------

        //Получение расположения файла с точками
        QString directory = ui->editDirectory->text();
        file.setFileName(directory);
        //Открытие файла с точками
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл с точками.\nПроверьте существование файла.");

                //------------Отладочная информация--------------
                qWarning() << "Не удалось открыть файл с точками!";
                //-----------------------------------------------
        }
        else {
                //------------Отладочная информация--------------
                qDebug().nospace() << "Файл с точками открыт.";
                //-----------------------------------------------

            fileIn = new QTextStream(&file);
            //Создание манипуляторов
            manipulators.append(Manipulator(startX1, startY1, R1, 0, Qt::red));
            manipulators.append(Manipulator(startX2, startY2, R2, 1, Qt::blue));

                //------------Отладочная информация--------------
                qDebug().nospace() << manipulators[0].getPosition();
                qDebug().nospace() << manipulators[1].getPosition();
                //-----------------------------------------------

            //Отображение стартовых значений
            ui->editCurrentX_1->setText(QString::number(startX1));
            ui->editCurrentY_1->setText(QString::number(startY1));
            ui->editCurrentX_2->setText(QString::number(startX2));
            ui->editCurrentY_2->setText(QString::number(startY2));
            //Размещение стартовых точек
            for (int i = 0; i < count_manipulators; ++i){
                addPoint(manipulators[i].getPosition(), manipulators[i].paint.color, 0);
                addPoint(manipulators[i].getPosition(), manipulators[i].paint.color, 1);
            }
            //Получение времени таймера чтения
            time_timer = ui->editTimeTimer->text().toInt(&ok);
            if (ok) {
                    //------------Отладочная информация--------------
                    qDebug().nospace() << "Время получено верно.";
                    //-----------------------------------------------

                //Запуск таймера чтения
                timer = new QTimer();
                connect(timer, SIGNAL(timeout()), this, SLOT(timerAlarm()));
                timer->start(time_timer * 1000);

                    //------------Отладочная информация--------------
                    qDebug().nospace() << "Таймер чтения запущен.";
                    //-----------------------------------------------
            }
            else {
                QMessageBox::critical(this, "Ошибка", "Таймер должен иметь целочисленное значение в секундах");

                    //------------Отладочная информация--------------
                    qWarning() << "Ошибка преобразования значения в int!";
                    //-----------------------------------------------
            }
        }
    }
}
//Обработка срабатывания таймера чтения
void MainWindow::timerAlarm() {
    //Проверка на конец файла
    if (!fileIn->atEnd()) {
        //Чтение одной строки
        QString line = fileIn->readLine();
        QStringList coords = line.split(" ");
        //Проверка на правильность строки данных
        if (coords.size() == 2) {
            bool ok1, ok2;
            double x = coords[0].toDouble(&ok1);
            double y = coords[1].toDouble(&ok2);
            if (ok1 && ok2) {
                    //------------Отладочная информация--------------
                    qDebug().nospace() << "Строка считана верно.";
                    //-----------------------------------------------

                Point point(x, y);
                //Получение расстояния от манипуляторов до точки
                double dist1 = manipulators[0].getPosition().distance(point);
                double dist2 = manipulators[1].getPosition().distance(point);
                if (manipulators[0].canMove(point) && manipulators[1].canMove(point)) {
                //Можно разместить оба манипулятора
                    if (dist1 <= dist2) {
                    //Выгоднее разместить 1 манипулятор
                        ui->editCurrentX_1->setText(coords[0]);
                        ui->editCurrentY_1->setText(coords[1]);
                        draw(manipulators[0].paint.id, manipulators[0].getPosition(), point);
                        manipulators[0].move(point);


                            //------------Отладочная информация--------------
                            qDebug().nospace() << "Перемещен 1 манипулятор.";
                            qDebug().nospace() << "Манипулятор 1: " << manipulators[0].getPosition();
                            qDebug().nospace() << "Манипулятор 2: " << manipulators[1].getPosition();
                            //-----------------------------------------------
                    }
                    else {
                      //Выгоднее разместить 2 манипулятор
                        ui->editCurrentX_2->setText(coords[0]);
                        ui->editCurrentY_2->setText(coords[1]);
                        draw(manipulators[1].paint.id, manipulators[1].getPosition(), point);
                        manipulators[1].move(point);

                            //------------Отладочная информация--------------
                            qDebug().nospace() << "Перемещен 2 манипулятор.";
                            qDebug().nospace() << "Манипулятор 1: " << manipulators[0].getPosition();
                            qDebug().nospace() << "Манипулятор 2: " << manipulators[1].getPosition();
                            //-----------------------------------------------
                    }
                }
                else if (manipulators[0].canMove(point)) {
                  //Можно разместить только 1 манипулятор
                    ui->editCurrentX_1->setText(coords[0]);
                    ui->editCurrentY_1->setText(coords[1]);
                    draw(manipulators[0].paint.id, manipulators[0].getPosition(), point);
                    manipulators[0].move(point);

                        //------------Отладочная информация--------------
                        qDebug().nospace() << "Перемещен 1 манипулятор.";
                        qDebug().nospace() << "Манипулятор 1: " << manipulators[0].getPosition();
                        qDebug().nospace() << "Манипулятор 2: " << manipulators[1].getPosition();
                        //-----------------------------------------------
                }
                else if (manipulators[1].canMove(point)) {
                  //Можно разместить только 2 манипулятор
                    ui->editCurrentX_2->setText(coords[0]);
                    ui->editCurrentY_2->setText(coords[1]);
                    draw(manipulators[1].paint.id, manipulators[1].getPosition(), point);
                    manipulators[1].move(point);

                        //------------Отладочная информация--------------
                        qDebug().nospace() << "Перемещен 2 манипулятор.";
                        qDebug().nospace() << "Манипулятор 1: " << manipulators[0].getPosition();
                        qDebug().nospace() << "Манипулятор 2: " << manipulators[1].getPosition();
                        //-----------------------------------------------
                }
                else {
                  //Ни один манипулятор не может быть размещен

                        //------------Отладочная информация--------------
                        qDebug().nospace() << "Оба манипулятора на месте.";
                        qDebug().nospace() << "Манипулятор 1: " << manipulators[0].getPosition();
                        qDebug().nospace() << "Манипулятор 2: " << manipulators[1].getPosition();
                        //-----------------------------------------------
                }
            }
            else {
                QMessageBox::warning(this, "Предупреждение", "Строка должна содержать числа.\nВещественные числа вносятся через '.'.");

                    //------------Отладочная информация--------------
                    qWarning() << "Ошибка формата данных в строке файла!";
                    //-----------------------------------------------
            }
        }
        else {
            QMessageBox::warning(this, "Предупреждение", "Строка содержит не 2 значения.");

                //------------Отладочная информация--------------
                qWarning() << "Ошибка обработки строки файла!";
                //-----------------------------------------------
        }
    }
    else {
            //------------Отладочная информация--------------
            qDebug() << "Достигнут конец файла.";
            //-----------------------------------------------

        delete timer;
        timer = nullptr;
        file.close();
        QMessageBox::StandardButton its_ok = QMessageBox::information(this, "Завершено", "Работа с файлом завершена", QMessageBox::Ok);
        if (its_ok == QMessageBox::Ok){
            //Очистка текущих положений манипуляторов
            ui->editCurrentX_1->clear();
            ui->editCurrentY_1->clear();
            ui->editCurrentX_2->clear();
            ui->editCurrentY_2->clear();
        }
            //------------Отладочная информация--------------
            qDebug() << "Таймер чтения удален, файл закрыт, поля вывода очищены.";
            //-----------------------------------------------
    }
}
