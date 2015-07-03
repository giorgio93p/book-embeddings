#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "ui_mainwindow.h"
#include "graphscene.h"
//#include "graphs.h"
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/fileformats/GraphIO.h>


#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void openGraph(char* filename);
    void drawGraph();

    ~MainWindow();
    //vector<Page> *pages;

public slots:

    void on_actionOpen_triggered();


private:

    ogdf::Graph mainGraph;

    vector<GraphScene*> scene ;
    //BookEmbeddedGraph* begraph;
    //Graph* graph;

};





#endif // MAINWINDOW_H
