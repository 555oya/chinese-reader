#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("555oya");
    QApplication::setApplicationName("字树");

    QIcon appIcon(":/icon/icon.ico");

    if (appIcon.isNull()) {
        qDebug() << "Failed to load icon from resources!";
        // Попробуем альтернативный путь
        appIcon = QIcon(":/icon/ico_256.png");
    }

    if (!appIcon.isNull()) {
        a.setWindowIcon(appIcon);
        qDebug() << "Icon loaded successfully";
    } else {
        qDebug() << "All icon loading attempts failed!";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
