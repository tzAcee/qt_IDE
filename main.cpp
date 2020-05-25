#include <QApplication>
#include "DarkStyle.h"
#include "framelesswindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    DarkStyle style;
    a.setStyle(&style);




    MainWindow w;
    w.setWindowFlag(Qt::CustomizeWindowHint);
     w.show();

     auto result = a.exec();

     return result;
}


