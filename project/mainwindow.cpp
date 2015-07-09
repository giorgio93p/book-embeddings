#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphscene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //QGraphicsView * p;

    setupUi(this);
    setWindowTitle("P.E.O.S.");

    views = std::vector<QGraphicsView*>();
    views.push_back(graphicsView);
    views.push_back(graphicsView_2);
    views.push_back(graphicsView_3);
    views.push_back(graphicsView_4);

    //QGridLayout *layout = new QGridLayout;
}

void MainWindow::drawGraph(Graph& g){}

void MainWindow::drawBookEmbeddedGraph(BookEmbeddedGraph& g){
    std::cout << "Number of pages: " << g.getNpages() << std::endl;
    std::cout << "Number of views: " << views.size() << std::endl;
    for(int p=0; p<std::min((long)g.getNpages(),(long)views.size()); p++){
        views[p]->setScene(new GraphScene(g,p));
        //scene->show();
        views[p]->update();
    }
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("GraphModellingLanguage files (*.gml);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        std::string fileNameStr = fileName.toUtf8().constData();//PROSOXI PAIZEI NA MIN PAIZEI PADOU
        file.close();
        bool readSuccessful = mainGraph.readGML(fileNameStr);
        if (readSuccessful){
            //std::cout << "Read Successful!!!!!" << std::endl;
            std::cout << "Number of nodes in read graph ==" << mainGraph.numberOfNodes() << endl;
        }

        this->drawBookEmbeddedGraph(mainGraph);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"), QString(),
            tr("GraphModellingLanguage files (*.gml);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.exists()) {
            QMessageBox::critical(this, tr("Error"), tr("File would be overwritten"));
            return;
        }
        std::string fileNameStr = fileName.toUtf8().constData();//PROSOXI PAIZEI NA MIN PAIZEI PADOU
        mainGraph.writeGML(fileNameStr);
    }
}

#endif
