#include "mainwindow.h"
#include <QApplication>
#include <mapwidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapWidget mapWidget;
    mapWidget.show();

//    MainWindow w;
//    w.show();

    return a.exec();
}
