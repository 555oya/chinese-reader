#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("555oya");
    QApplication::setApplicationName("chinese-reader");

    MainWindow w;
    w.show();
    return a.exec();
}
