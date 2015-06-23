#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "ui_mainwindow.h"
#include "graphscene.h"
#include "../graphs.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void openGraph(char* filename);
    void drawGraph();

    ~MainWindow();
    //vector<Page> *pages;


private:

    vector<GraphScene*> scene ;
    //BookEmbeddedGraph* begraph;
    //Graph* graph;

};





#endif // MAINWINDOW_H
