#include "mainwindow.h"
#include "graphs.h"
#include <QApplication>

#include <iostream>


/*
 *
 * added colors.h
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.openBookEmbeddedGraph("../test-files/1.gml");
    w.show();

    return a.exec();
}
