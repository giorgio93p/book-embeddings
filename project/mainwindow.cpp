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
    connect(this, SIGNAL(crossings_changed(std::vector<int>)), this, SLOT(on_crossings_changed(std::vector<int>)));
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
    mainGraph->buildLayout(-graphView->width(),-graphView->height(),graphView->width(),graphView->height());
    GraphScene* gs = new GraphScene(*mainGraph);
    graphView->setScene(gs);
    //graphView->fitInView(gs->sceneRect());

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
    view->setScene(new PageScene(*mainGraph,page,this));

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
        emit crossings_changed(std::vector<int>());
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

void MainWindow::on_edge_selected(Edge& e){
    node_stats->setEnabled(false);
    edge_stats->setEnabled(true);
    std::cout << "Edge (" << e->source()->index() << "," << e->target()->index() << ") selected" << endl;
    //edge_crossings_indicator->setNum(mainGraph->getNcrossings(e));
    edge_source_indicator->setNum(e->source()->index());
    edge_target_indicator->setNum(e->target()->index());
    edge_page_indicator->setNum(mainGraph->getPageNo(e));
    stats->setCurrentWidget(edge_stats);
}

void MainWindow::on_edge_deselected(Edge& e){
    edge_stats->setEnabled(false);
    //edge_crossings_indicator->clear();
    edge_source_indicator->clear();
    edge_target_indicator->clear();
    edge_page_indicator->clear();
    std::cout << "Edge (" << e->source()->index() << "," << e->target()->index() << ") deselected" << endl;
}

void MainWindow::on_node_selected(Node& v){
    edge_stats->setEnabled(false);
    node_stats->setEnabled(true);
    std::cout << "Node " << v->index() << " selected" << endl;
    node_outdeg_indicator->setNum(v->outdeg());
    node_indeg_indicator->setNum(v->indeg());
    node_index_indicator->setNum(v->index());
    stats->setCurrentWidget(node_stats);
}

void MainWindow::on_node_deselected(Node& v){
    node_stats->setEnabled(false);
    node_outdeg_indicator->clear();
    node_indeg_indicator->clear();
    node_index_indicator->clear();
    std::cout << "Node " << v->index() << " deselected" << endl;
}

void MainWindow::on_edge_move_to_page(Edge &e , int p){
    if(p < 0 || p >= mainGraph->getNpages()) return;
    int currPage = mainGraph->getPageNo(e);
    mainGraph->moveToPage(e,p);
    ((PageScene*)(pageViews[currPage]->scene()))->removeEdge(e);
    ((PageScene*)(pageViews[p]->scene()))->addEdge(e);
    std::vector<int> temp = {p, currPage};
    emit crossings_changed(temp);
}

void MainWindow::on_crossings_changed(std::vector<int> pagesChanged){
    total_crossings_indicator->setNum(mainGraph->getNcrossings());
}

void MainWindow::on_actionSave_triggered(){
    if(mainGraph->writeGML(currentFile)) std::cout << "Graph saved to " << currentFile << endl;
}
