#include <QApplication>
#include "DarkStyle.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    DarkStyle style;
    a.setStyle(&style);




    MainWindow w;
    w.show();

    auto result = a.exec();

    return result;
}


