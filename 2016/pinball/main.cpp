#include "mainwindow.h"
#include <QApplication>
#include "logic.h"

int main(int argc, char *argv[])
{
    Logic logic;
    logic.setup();


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
