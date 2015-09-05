#include "mainwindow.h"
#include "graphs.h"
#include <QApplication>

#include <iostream>


/*
 *
 * cleaned some garbage comments
 **/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.openBookEmbeddedGraph("../test-files/1.gml");
    w.show();

    return a.exec();
}
