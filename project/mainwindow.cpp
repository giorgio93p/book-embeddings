
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphscene.h"
#include "pagescene.h"
#include "agscene.h"
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QAction>
#include <QPushButton>
#include <QPainterPath>
#include <QRect>
#include <QTransform>
#include <QUndoStack>
#include <ogdf/basic/GraphList.h>

#include "auxiliarygraph.h"
#include "commands.h"
#include "pagescene.h"

#define WINDOW_TITLE tr("P.E.O.S.")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle(WINDOW_TITLE);
    connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(this, SIGNAL(number_of_nodes_changed(int)), number_of_nodes_indicator, SLOT(setNum(int)));
    connect(this, SIGNAL(number_of_edges_changed(int)), number_of_edges_indicator, SLOT(setNum(int)));
    connect(this, SIGNAL(crossings_changed(std::vector<int>)), this, SLOT(on_crossings_changed(std::vector<int>)));
    connect(this, SIGNAL(planarity_changed(QString)), planarity_indicator, SLOT(setText(QString)));

    //embedding_drawing->setScaledContents(true);

    actionSave_as->setEnabled(false);
    actionSave->setEnabled(false);
    actionAddPage->setEnabled(false);

    pageViews = std::vector<PageView*>();

    commandHistory = new QUndoGroup(this);
    commandHistory->addStack(new QUndoStack(commandHistory));
    QAction* undoAction = commandHistory->createUndoAction(this);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setIcon(QIcon(":previous"));
    toolBar->addAction(undoAction);
    menuEdit->addAction(undoAction);
    QAction* redoAction = commandHistory->createRedoAction(this);
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setIcon(QIcon(":next"));
    toolBar->addAction(redoAction);
    menuEdit->addAction(redoAction);

    wholeGraphMode=true;
}

void MainWindow::drawBookEmbeddedGraph(){

    //kosmas 6/9: changed the order. first pages are drawn, then the main graph.
    //the reason for this is because it was more convenient to give colours to each page
    // and then paint each maingraph edge according to the colour assigned to the pageview it is in.

    colourCloset.returnAll(); // the returnAll method simply resets the closet to its intial state
    // that is, having all of its colours inside

    //Remove previous page drawings
    for(PageView* view : pageViews){
        delete view->parent();
    }
    pageViews.clear();

    //Draw pages
    for(int p=0; p<mainGraph->getNpages(); p++){
        add_page_drawing(p);
    }

    actionRedraw->setEnabled(false);

    commandHistory->setActiveStack(commandHistory->stacks().at(0));
    //Draw graph
    delete graphView->scene(); //delete scene that shows the whole graph
    if(!mainGraph->hasLayout()){
        mainGraph->buildLayout(-graphView->width(),-graphView->height(),graphView->width(),graphView->height());
    }
    GraphScene* gs = new GraphScene(mainGraph,this);
    graphView->setScene(gs);
    //graphView->fitInView(gs->sceneRect());




}

void MainWindow::add_page_drawing(int page){


    BookEmbeddedGraph* graphToDraw=(wholeGraphMode)?mainGraph : currBC;
    //selecting which graph to draw according to the mode.

    //if (!wholeGraphMode) return;
    //haven't yet implemented the drawing of a Biconnected Component.



    /* embedding_drawing: a widget that we have created with QTDesigner.
     * It is used as ancestor widget for all page views.
     * Its layout type is vertical.
     * Itself it is contained in a ScrollArea Widget
     * which we have named, not surprisignly, scrollArea.
     * All this was made in QTDesigner.
     *
     */
    QWidget* pageDrawing = new QWidget(embedding_drawing);
    pageDrawing->setLayout(new QHBoxLayout());
    pageDrawing->layout()->setContentsMargins(4,5,4,5);
    ((QVBoxLayout*)embedding_drawing->layout())->insertWidget(page,pageDrawing);

    PageView* view = new PageView(graphToDraw);
    pageViews.insert(pageViews.begin()+page,view);
    pageDrawing->layout()->addWidget(view);

    QVBoxLayout* pageSidebar = new QVBoxLayout();
    ((QHBoxLayout*)pageDrawing->layout())->addLayout(pageSidebar);

    QLabel* page_number = new QLabel(pageDrawing);
    page_number->setToolTip(tr("Page number"));
    pageSidebar->addWidget(page_number);

    QLabel* crossings_of_page = new QLabel(pageDrawing);
    crossings_of_page->setToolTip(tr("Crossings"));
    pageSidebar->addWidget(crossings_of_page);

    QPushButton* del = new QPushButton("Delete",pageDrawing);
    del->setToolTip(tr("Delete page"));
    del->setIcon(QIcon(":remove"));
    pageSidebar->addWidget(del);




    QHBoxLayout* zoomButtons = new QHBoxLayout();
    pageSidebar->addLayout(zoomButtons);
    QPushButton* zoomin = new QPushButton(pageDrawing);
    zoomin->setToolTip(tr("Zoom In"));
    zoomin->setIcon(QIcon(":zoom-in"));
    zoomButtons->addWidget(zoomin);
    connect(zoomin,SIGNAL(pressed()),view,SLOT(zoomIn()));
    QPushButton* zoomout = new QPushButton(pageDrawing);
    zoomout->setToolTip(tr("Zoom Out"));
    zoomout->setIcon(QIcon(":zoom-out"));
    zoomButtons->addWidget(zoomout);
    connect(zoomout,SIGNAL(pressed()),view,SLOT(zoomOut()));

    PageScene* scene = new PageScene(graphToDraw,page,this,colourCloset.getPaint(),//getting a new colour for the scene
                                    page_number,crossings_of_page, del,view->width(), view->height());


    view->setScene(scene);
    scene->setCrossings(graphToDraw->getNcrossings(page));
    connect(scene,SIGNAL(remove_page(int)),this,SLOT(on_remove_page(int)));
    connect(del,SIGNAL(pressed()),scene,SLOT(on_remove_page_request()));

    number_of_pages_indicator->setNum(graphToDraw->getNpages());

    for(int i=page+1; i<pageViews.size(); i++){
        pageViews[i]->scene()->setPageNumber(i);
    }
}

void MainWindow::remove_page_drawing(int page){
    PageScene *ps = this->pageViews[page]->scene();
    QColor colourToBeReturned = ps->getColour(); //here we get the colour of the page
    this->colourCloset.returnPaint(colourToBeReturned); // and we return it to the closet

    delete pageViews[page]->parent();

    this->pageViews.erase(this->pageViews.begin()+page);

    this->number_of_pages_indicator->setNum(this->mainGraph->getNpages());
    for(int i=page; i<this->pageViews.size(); i++){
        this->pageViews[i]->scene()->setPageNumber(i);
    }

    embedding_drawing->layout()->update();
}


void MainWindow::drawBCTree() {

    auxiliaryGraph = new AuxiliaryGraph(mainGraph);
    delete bCTView->scene();    //deleting previous scene if any

    if (mainGraph->getIsBiconnected()) {
        //the main graph is already biconnected, so we won't use the auxiliary graph at all
        delete auxiliaryGraph;
        QGraphicsScene * gs = new QGraphicsScene(this); //the scene now won't show any graph at all
        //QFont * qf = new QFont("Dejavu Serif",6);

        QGraphicsSimpleTextItem* txt =new QGraphicsSimpleTextItem("it's biconnectd");
        txt->setPos(0,0);
        txt->setFont(QFont("Dejavu Serif",10));
        gs->addItem(txt);                              //the scene will display this text
        bCTView->setScene(gs);                          //setting the view's scene as gs.
        bCTView->fitInView((gs)->sceneRect());
        return;                                         //done.

    }




    auxiliaryGraph->buildLayout(0.0,0.0,bCTView->width(),bCTView->height()); //building layout: this is where the program
    //segfaults :(.
    //we use it to make some
    // settings in the attr member of graphs



    QGraphicsScene *gs =new AGScene(auxiliaryGraph,this,bCTView->width(),bCTView->height());
    //AGScene: new class, custom made to
    //show bctrees.

    bCTView->setScene(gs);                                           //final
    bCTView->fitInView((gs)->sceneRect());                           //things




}

bool MainWindow::openBookEmbeddedGraph(std::string filename){
    wholeGraphMode=true;
    BookEmbeddedGraph* temp = new BookEmbeddedGraph();
    if (temp->readGML(filename)){
        for(QUndoStack* s : commandHistory->stacks()){
            commandHistory->removeStack(s);
        }
        commandHistory->addStack(new QUndoStack(commandHistory));//move this operation to drawBCTree() when we can work independently across Biconnected Components
        currentFile = filename;
        //delete mainGraph;
        mainGraph = temp;


        this->drawBookEmbeddedGraph();
        this->drawBCTree(); //drawBCTree
        //added by kosms
        //made to draw the Biconnected Components Tree
        // and to split the maingraph into
        // its biconnected components and storing
        // them into the vector called biconnectedComponenets
        emit number_of_nodes_changed(mainGraph->numberOfNodes());
        emit number_of_edges_changed(mainGraph->numberOfEdges());
        emit crossings_changed(std::vector<int>());
        emit planarity_changed(mainGraph->graphIsPlanar() ? tr("Yes") : tr("No"));
        stats->setCurrentWidget(graph_stats);
        return true;
    } else {
        //delete temp;
        return false;
    }
}

QColor MainWindow::getPageColour(int pageno) {
    //this method returns the colour that has been assigned to
    //a pagescene. Pageno is the index of the pageViews array
    //said scene is stored in.

    PageScene* scene = pageViews[pageno]->scene();
    return scene->getColour();
}

void MainWindow::on_actionAddPage_triggered(){
    commandHistory->activeStack()->push(new PageAddCommand(mainGraph,this));
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



    deselectEverythingInAllPagesBut(mainGraph->getPageNo(e));

    edge_stats->setEnabled(true);
    //std::cout << "Edge (" << e->source()->index() << "," << e->target()->index() << ") selected" << endl;
    //edge_crossings_indicator->setNum(mainGraph->getNcrossings(e));
    edge_source_indicator->setNum(e->source()->index());
    edge_target_indicator->setNum(e->target()->index());
    edge_page_indicator->setNum(mainGraph->getPageNo(e));
    stats->setCurrentWidget(edge_stats);

    int pgno =mainGraph->getPageNo(e);

    PageScene* pg = pageViews[pgno]->scene();
    pg->highlightEdge(e,true);

    GraphScene* gs = graphView->scene();
    gs->highlightEdge(e,true);
}

void MainWindow::deselectEverythingInAllPagesBut(int pageNo) {


    for (int i=0; i<pageViews.size(); i++) {

        if (i==pageNo) continue; //if the page under examination
        //is the page of the selected edge
        //then we won't touch

        PageScene* ps = pageViews[i]->scene();
        ps->deselectAll();
    }

    graphView->scene()->deselectAll();
}



void MainWindow::on_edge_deselected(Edge& e){
    edge_stats->setEnabled(false);
    //edge_crossings_indicator->clear();
    edge_source_indicator->clear();
    edge_target_indicator->clear();
    edge_page_indicator->clear();



    int pageNo = mainGraph->getPageNo(e);
    PageScene* ps = pageViews[pageNo]->scene();
    ps->highlightEdge(e,false);


    GraphScene* gs = graphView->scene();
    gs->highlightEdge(e,false);
    //std::cout << "Edge (" << e->source()->index() << "," << e->target()->index() << ") deselected" << endl;
}

void MainWindow::on_node_selected(Node& v, int onPage){

    deselectEverythingInAllPagesBut(onPage);


    node_stats->setEnabled(true);
    //std::cout << "Node " << v->index() << " selected" << endl;
    node_deg_indicator->setNum(v->degree());
    node_index_indicator->setNum(v->index());
    stats->setCurrentWidget(node_stats);

    GraphScene* gs = (GraphScene*)graphView->scene();

    if (wholeGraphMode) {
        for(PageView* view : pageViews){
            view->scene()->highlightNode(v,true);
        }
        gs->highlightNode(v,true);


    } else {
        int num = currBC->getNumberOf(v);
        Node n = mainGraph->getNode(num);
        gs->highlightNode(n,true);
    }

}

void MainWindow::on_node_deselected(Node& v){
    //slot that is called when a node is deselected.


    node_stats->setEnabled(false);
    node_deg_indicator->clear();
    node_index_indicator->clear();


    GraphScene* gs = (GraphScene*)graphView->scene();
    //grabbing the scene.


    //are we currently viewing/modifying the whole graph?

    if (wholeGraphMode) {//case1 : yes we are


        for(PageView* view : pageViews){
            view->scene()->highlightNode(v,false);
        }
        gs->highlightNode(v,false);

    } else {//case 2: no, we're facing only a biconnected subgraph of it.

        for(PageView* view : pageViews){
            view->scene()->highlightNode(v,false);
        }


        int num = currBC->getNumberOf(v); // we have to get the corresponding
        Node& n = mainGraph->getNode(num);// node on the main graph.
        gs->highlightNode(n,false);       // then we can request from the graphScene to highlight it.
    }
}

void MainWindow::move_edge_request(Edge &e, int direction){
    int currPage = mainGraph->getPageNo(e);
    if(direction == (int)PageEdge::MoveDirection::Unknown){
        bool ok;
        int newPage = QInputDialog::getInt(this, tr("Move edge to page"),
                                           tr("New page:"),currPage,0,mainGraph->getNpages()-1,1,&ok);
        if (!ok || newPage==currPage) return;
        move_edge(e,newPage);
    } else if(direction == (int)PageEdge::MoveDirection::Up && currPage > 0){
        move_edge(e,currPage - 1);
    } else if(direction == (int)PageEdge::MoveDirection::Down && currPage < mainGraph->getNpages()-1){
        move_edge(e,currPage + 1);
    } else if(direction >= 0 && direction <= mainGraph->getNpages()-1 && direction != currPage){
        move_edge(e,direction);
    }
}

void MainWindow::move_edge(Edge& e, int newPage){
    commandHistory->activeStack()->push(new EdgeMoveCommand(e,mainGraph->getPageNo(e),newPage,&pageViews,mainGraph,graphView->scene(),total_crossings_indicator));
}

void MainWindow::move_node(Node &v, int newPosition){
    if(newPosition < 0 || newPosition >= mainGraph->numberOfNodes() || mainGraph->getPosition(v) == newPosition) return;
    deselectEverythingInAllPagesBut(-1);
    commandHistory->activeStack()->push(new NodeMoveCommand(v,mainGraph, newPosition, &pageViews));
    on_node_selected(v,-1);
}

void MainWindow::on_node_dragged(Node&v, int atPage, QPointF toPos){
    for(PageView* view : pageViews){
        if(view->scene()->pageNumber() == atPage) continue;
        view->scene()->moveNode(v, toPos);
    }
}

void MainWindow::node_coordinates_changed(Node &v, QPointF to){
    mainGraph->setXcoord(v,to.x());
    mainGraph->setYcoord(v,to.y());
}

void MainWindow::on_remove_page(int page){



    Q_ASSERT(mainGraph->pageSize(page) == 0);
    commandHistory->activeStack()->push(new PageRemoveCommand(page, mainGraph, this));
}

void MainWindow::on_crossings_changed(std::vector<int> pagesChanged){
    total_crossings_indicator->setNum(mainGraph->getNcrossings());
    for(int i : pagesChanged){
        pageViews[i]->scene()->setCrossings(mainGraph->getNcrossings(i));
    }
}

void MainWindow::on_actionSave_triggered(){
    if(mainGraph->writeGML(currentFile)) std::cout << "Graph saved to " << currentFile << endl;
}
/*
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
    colourCloset.returnAll();//kosm: this is necessary in order
    //for our colouring mechanism to work.

    for(int p=0; p<mainGraph->getNpages(); p++){
        add_page_drawing(p);
    }

    actionRedraw->setEnabled(false);
}

*/


void MainWindow::loadBC(BiconnectedComponent* currbc) {

    cout << "loading another biconnected component!!" << endl;
    currBC = currbc;
    wholeGraphMode = false;
    cout << "  " << currBC->numberOfEdges() << endl;

    redrawPages();





}

void MainWindow::redrawPages() {


    colourCloset.returnAll(); // the returnAll method simply resets the closet to its intial state
    // that is, having all of its colours inside
    //Draw pages
    for(int p=pageViews.size()-1; p>=0; p--){
        embedding_drawing->layout()->removeWidget(pageViews[p]);
        //QGraphicsScene *pp = pageViews[p]->scene();
        delete pageViews[p]->scene();
        delete pageViews[p];
        pageViews.pop_back();
    }



    for(int p=0; p< currBC->getNpages(); p++){
        add_page_drawing(p);
    }

    actionRedraw->setEnabled(false);

}
