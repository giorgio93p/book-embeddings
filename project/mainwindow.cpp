#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphscene.h"
#include "pagescene.h"
#include "bctscene.h"
#include <QInputDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QAction>
#include <QPushButton>

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

    colourCloset.returnAll();

    //Draw pages
    for(int p=pageViews.size()-1; p>=0; p--){
        embedding_drawing->layout()->removeWidget(pageViews[p]);
        QGraphicsScene *pp = pageViews[p]->scene();
        delete pageViews[p]->scene();
        delete pageViews[p];
        pageViews.pop_back();
    }

    for(int p=0; p<mainGraph->getNpages(); p++){
        add_page_drawing(p);
    }

    actionRedraw->setEnabled(false);
}

void MainWindow::add_page_drawing(int page){

    /* embedding_drawing: a widget that we have created with QTDesigner.
     * It is used as ancestor widget for all page views.
     * Itself it is contained in a ScrollArea Widget
     * which we have named, not surprisignly, scrollArea.
     * All this was made in QTDesigner.
     *
     * Below, we create the view for the page and, along with it,
     * two QLabels (page_number and crossings_of_page)
     * and one QPushButton (delete)
     * which we align
     *
     */
    int rowNumber = ((QGridLayout*)embedding_drawing->layout())->rowCount();
    QGraphicsView* view = new QGraphicsView();
    pageViews.push_back(view);
    PageScene* scene = new PageScene(*mainGraph,page,this,colourCloset.getPaint());//getting a new colour for the scene
    view->setScene(scene);
    connect(scene,SIGNAL(remove_page(int)),this,SLOT(on_remove_page(int)));
    ((QGridLayout*)embedding_drawing->layout())->addWidget(view,rowNumber,0);

    QWidget* vert = new QWidget(embedding_drawing);
    vert->setLayout(new QVBoxLayout());
    embedding_drawing->layout()->addWidget(vert);
    connect(view,SIGNAL(destroyed(QObject*)),vert,SLOT(deleteLater()));
    ((QGridLayout*)embedding_drawing->layout())->addWidget(vert,rowNumber,1);

    QLabel* page_number = new QLabel();
    page_number->setNum(page);
    page_number->setToolTip(tr("Page number"));
    vert->layout()->addWidget(page_number);
    connect(scene,SIGNAL(page_number_changed(int)),page_number,SLOT(setNum(int)));

    QLabel* crossings_of_page = new QLabel();
    crossings_of_page->setNum(mainGraph->getNcrossings(page));
    crossings_of_page->setToolTip(tr("Crossings"));
    vert->layout()->addWidget(crossings_of_page);
    connect(scene,SIGNAL(crossings_changed(int)),crossings_of_page,SLOT(setNum(int)));

    QPushButton* del = new QPushButton("Delete");
    del->setToolTip(tr("Delete page"));
    //if(mainGraph->pageSize(page)>0) del->setEnabled(false);
    connect(del,SIGNAL(pressed()),scene,SLOT(on_remove_page_request()));
    vert->layout()->addWidget(del);

}

void MainWindow::drawBCTree() {


    //this draws the Biconnected Compenents Tree of the input graph on Tab1, right below the mainGraph.
    //it is drawn in a QGraphicsView object which was made in Desinger, named bCTView.


    bctree = new BCTree(*mainGraph);  //make a new object of clss BCTree
    cout << bctree->numberOfNodes(true) << endl; //testing if class works
    ogdf::Graph graf = bctree->getBCTree();     //get the bct in ogdf::graph representation

    //ogdf::GraphIO::writeGML(graf, "/home/kosmas/sierpinski_04-layout.gml");



    Graph* bCGraph = new Graph(graf);    //create a new Graph object that represents the bct.
                                        //check the Graph constructor that takes an ogdf::Graph
                                        //as a parameter



    delete bCTView->scene();             //deleting previous scene if any


    bCGraph->buildLayout(0.0,0.0,bCTView->width(),bCTView->height()); //building layout: this is where the program
                                                                     //segfaults :(.
                                                                     //we use it to make some
                                                                    // settings in the attr member of graphs

    QGraphicsScene *gs =new BCTScene(*bCGraph,250,50);              //BCTScene: new class, custom made to
                                                                    //show bctrees.

    bCTView->setScene(gs);                                           //final
    bCTView->fitInView((gs)->sceneRect());                           //things

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
        this->drawBCTree(); //drawBCTree
                            //added by kosms
                            //made to draw the Biconnected Components Tree.
                            //which is shown at the scene named bCTScene
        this->drawBookEmbeddedGraph();
        emit number_of_nodes_changed(mainGraph->numberOfNodes());
        emit number_of_edges_changed(mainGraph->numberOfEdges());
        emit number_of_pages_changed(mainGraph->getNpages());
        emit crossings_changed(std::vector<int>());
        emit planarity_changed(mainGraph->graphIsPlanar());
        stats->setCurrentWidget(graph_stats);
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

void MainWindow::move_edge(Edge &e){
    bool ok;
    int newPage = QInputDialog::getInt(this, tr("Move edge to page"),
                                         tr("New page:"),0,0,mainGraph->getNpages()-1,1,&ok);
    if (!ok) return;
    int currPage = mainGraph->getPageNo(e);
    mainGraph->moveToPage(e,newPage);
    ((PageScene*)(pageViews[currPage]->scene()))->removeEdge(e);
    ((PageScene*)(pageViews[newPage]->scene()))->addEdge(e);
    ((GraphScene*)(graphView->scene()))->removeEdge(e);

    PageScene * p = (PageScene*)pageViews[newPage]->scene();
    ((GraphScene*)(graphView->scene()))->addEdge(e,p->getColour());
    std::vector<int> temp = {newPage, currPage};
    emit crossings_changed(temp);
}

void MainWindow::on_remove_page(int page){
    if(mainGraph->pageSize(page) == 0){
        mainGraph->removePage(page);
        QGraphicsView* temp = pageViews[page];
        PageScene *ps = (PageScene*)pageViews[page]->scene();
        QColor colourToBeReturned = ps->getColour(); //attention here
        colourCloset.returnPaint(colourToBeReturned);
        pageViews.erase(pageViews.begin()+page);
        delete temp;
        emit number_of_pages_changed(mainGraph->getNpages());
        for(int i=page; i<mainGraph->getNpages(); i++){
            ((PageScene*)(pageViews[i]->scene()))->setPageNumber(i);
        }
    }
}

void MainWindow::on_crossings_changed(std::vector<int> pagesChanged){
    total_crossings_indicator->setNum(mainGraph->getNcrossings());
}

void MainWindow::on_actionSave_triggered(){
    if(mainGraph->writeGML(currentFile)) std::cout << "Graph saved to " << currentFile << endl;
}

void MainWindow::enableRedraw(){
    actionRedraw->setEnabled(true);
}

void MainWindow::on_actionRedraw_triggered(){
    for(int p=pageViews.size()-1; p>=0; p--){
        embedding_drawing->layout()->removeWidget(pageViews[p]);
        delete pageViews[p]->scene();
        delete pageViews[p];
        pageViews.pop_back();
    }

    for(int p=0; p<mainGraph->getNpages(); p++){
        add_page_drawing(p);
    }

    actionRedraw->setEnabled(false);
}
