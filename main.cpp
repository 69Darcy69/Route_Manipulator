#include "mainwindow.h"
#include <QApplication>

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
