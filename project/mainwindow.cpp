#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphscene.h"
#include "pagescene.h"

#define WINDOW_TITLE tr("P.E.O.S.")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle(WINDOW_TITLE);

    connect(this, SIGNAL(number_of_nodes_changed(int)), number_of_edges_indicator, SLOT(setNum(int)));
    connect(this, SIGNAL(number_of_edges_changed(int)), number_of_nodes_indicator, SLOT(setNum(int)));
    connect(this, SIGNAL(number_of_pages_changed(int)), number_of_pages_indicator, SLOT(setNum(int)));
    connect(this, SIGNAL(crossings_changed(int)), total_crossings_indicator, SLOT(setNum(int)));
    connect(this, SIGNAL(planarity_changed(bool)), planarity_indicator, SLOT(setChecked(bool)));

    //embedding_drawing->setScaledContents(true);

    actionSave_as->setEnabled(false);
    actionSave->setEnabled(false);
    actionAddPage->setEnabled(false);

    pageViews = std::vector<QGraphicsView*>();
}

void MainWindow::drawBookEmbeddedGraph(){
    //Draw graph
    delete graphView->scene(); //delete scene that shows the whole graph
    mainGraph->buildLayout(0.0,0.0,graphView->width(),graphView->height());
    GraphScene* gs = new GraphScene(*mainGraph);
    graphView->setScene(gs);
    graphView->fitInView(gs->sceneRect());

    //Draw pages
    for(int p=pageViews.size()-1; p>=0; p--){
        embedding_drawing->layout()->removeWidget(pageViews[p]);
        delete pageViews[p]->scene();
        delete pageViews[p];
        pageViews.pop_back();
    }
    for(int p=0; p<mainGraph->getNpages(); p++){
        add_page_drawing(p);
    }
}

void MainWindow::add_page_drawing(int page){
    QGraphicsView* view = new QGraphicsView(embedding_drawing);

    /* embedding_drawing: a widget that we have created with QTDesigner.
     * It is used as the parent widget for all page views.
     * Itself it is contained in a ScrollArea Widget
     * which we have named, not surprisignly, scrollArea.
     * All this was made in QTDesigner.
     * kosm
     */


    embedding_drawing->layout()->addWidget(view);
    pageViews.push_back(view);
    view->setScene(new PageScene(*mainGraph,page));

    view->show();
}

bool MainWindow::openBookEmbeddedGraph(std::string filename){
    BookEmbeddedGraph* temp = new BookEmbeddedGraph();
    if (temp->readGML(filename)){
        currentFile = filename;
        //delete mainGraph;
        mainGraph = temp;
        //std::cout << "Read Successful!!!!!" << std::endl;
        //std::cout << "Number of nodes in read graph ==" << mainGraph->numberOfNodes() << endl;
        //std::cout << "Number of edges in read graph ==" << mainGraph->numberOfEdges() << endl;
        this->drawBookEmbeddedGraph();
        emit number_of_nodes_changed(mainGraph->numberOfNodes());
        emit number_of_edges_changed(mainGraph->numberOfEdges());
        emit number_of_pages_changed(mainGraph->getNpages());
        emit crossings_changed(mainGraph->getNcrossings());
        emit planarity_changed(mainGraph->graphIsPlanar());
        return true;
    } else {
        //delete temp;
        return false;
    }
}

void MainWindow::on_actionAddPage_triggered(){
    mainGraph->addPage();
    add_page_drawing(mainGraph->getNpages()-1);
    emit number_of_pages_changed(mainGraph->getNpages());
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
        if(openBookEmbeddedGraph(fileNameStr)){
            QMessageBox::information(this,tr("Read Book Embedded Graph"),tr("Successful"));
            setWindowTitle(WINDOW_TITLE + tr(" - ") + fileName);
            actionSave_as->setEnabled(true);
            actionSave->setEnabled(true);
            actionAddPage->setEnabled(true);
        } else QMessageBox::warning(this,tr("Read Book Embedded Graph"),tr("Unsuccessful"));
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"), QString(),
            tr("GraphModellingLanguage files (*.gml);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        std::string fileNameStr = fileName.toUtf8().constData();//PROSOXI PAIZEI NA MIN PAIZEI PADOU
        mainGraph->writeGML(fileNameStr);
        actionSave->setEnabled(true);
    }
}

void MainWindow::on_actionSave_triggered(){
    if(mainGraph->writeGML(currentFile)) std::cout << "Graph saved to " << currentFile << endl;
}

#endif
