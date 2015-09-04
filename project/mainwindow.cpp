
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphscene.h"
#include "pagescene.h"
#include "bctscene.h"
#include "colors.h"
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
#include "commands.h"

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

    pageViews = std::vector<QGraphicsView*>();

    commandHistory = new QUndoGroup(this);
    commandHistory->addStack(new QUndoStack(commandHistory));
    QAction* undoAction = commandHistory->createUndoAction(this);
    undoAction->setShortcut(QKeySequence::Undo);
    toolBar->addAction(undoAction);
    menuEdit->addAction(undoAction);
    QAction* redoAction = commandHistory->createRedoAction(this);
    redoAction->setShortcut(QKeySequence::Redo);
    toolBar->addAction(redoAction);
    menuEdit->addAction(redoAction);
}

void MainWindow::drawBookEmbeddedGraph(){
    //Draw graph
    delete graphView->scene(); //delete scene that shows the whole graph
    mainGraph->buildLayout(-graphView->width(),-graphView->height(),graphView->width(),graphView->height());
    GraphScene* gs = new GraphScene(*mainGraph);
    graphView->setScene(gs);
    //graphView->fitInView(gs->sceneRect());

    colourCloset.returnAll(); // the returnAll method simply resets the closet to its intial state
                              // that is, having all of its colours inside

    //Remove previous page drawings
    QLayoutItem *child;
    while ((child = embedding_drawing->layout()->takeAt(0)) != 0) {
        delete child;
    }

    //Draw pages
    for(int p=0; p<mainGraph->getNpages(); p++){
        add_page_drawing(p);
    }

    actionRedraw->setEnabled(false);

    commandHistory->setActiveStack(commandHistory->stacks().at(0));
}

void MainWindow::add_page_drawing(int page){

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
    ((QVBoxLayout*)embedding_drawing->layout())->insertWidget(page,pageDrawing);

    QGraphicsView* view = new QGraphicsView(pageDrawing);
    pageViews.push_back(view);
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
    pageSidebar->addWidget(del);

    PageScene* scene = new PageScene(*mainGraph,page,this,colourCloset.getPaint(),page_number,crossings_of_page, del);//getting a new colour for the scene
    view->setScene(scene);
    scene->setCrossings(mainGraph->getNcrossings(page));
    connect(scene,SIGNAL(remove_page(int)),this,SLOT(on_remove_page(int)));
    connect(del,SIGNAL(pressed()),scene,SLOT(on_remove_page_request()));

    number_of_pages_indicator->setNum(mainGraph->getNpages());
}

void MainWindow::remove_page_drawing(int page){
    PageScene *ps = (PageScene*)this->pageViews[page]->scene();
    QColor colourToBeReturned = ps->getColour(); //here we get the colour of the page
    this->colourCloset.returnPaint(colourToBeReturned); // and we return it to the closet

    delete pageViews[page]->parent();

    this->pageViews.erase(this->pageViews.begin()+page);

    this->number_of_pages_indicator->setNum(this->mainGraph->getNpages());
    for(int i=page; i<this->mainGraph->getNpages(); i++){
        ((PageScene*)(this->pageViews[i]->scene()))->setPageNumber(i);
    }

    embedding_drawing->layout()->update();
}

void MainWindow::findConnectedComponentsOfMainGraph() {



    ogdf::Graph g = mainGraph->toOGDF(); //one question here. Are g's nodes and edges the same
                                         //as mainGraph's? If they are, then all we have to do
                                        // is to create an nodearray with pairs of nodes.
                                        //(node_in_bct,node_in_g).
                                        //then using this nodearray we can access the nodes
                                        //of main graph. (e.g. change a node lable or smthing)
                                        //perhaps another way would be to use ...(fuck it lets
                                        //check this way out first)


    ogdf::BCTree bctree(g,true); //attention, passing by reference here.
                                 //as soon as we get out of scope of this method
                                // this bctree wont be of any use.

    //note: thie above constructor takes a reference to a graph.
    //for some reason, we cannot the return value of a method.
    // we have to pass a variable! This means that the following line won't compile:
    //ogdf::BCTree bctree(mainGraph->toOGDF(),true);


    ogdf::Graph graf = bctree.auxiliaryGraph();

    //now to create mappings from the edges and nodes of the bct to the main graph.
    ogdf::EdgeArray<Edge> bct_to_main_graph_edges(graf);
    ogdf::NodeArray<Node> bct_to_main_graph_nodes(graf); //attention
                                                          //this wouldnt work
                                                         //without setting graf as
                                                         //the associated graph




    Edge e;
    ogdf::Graph G = graf;

    for((e)=(G).firstEdge(); (e); (e)=(e)->succ()){}
/*
    for (e=graf.firstEdge();(e);e->succ()){
         cout << "infinite loop!!" << endl;
    }
*/
    //e=graf.firstEdge();
    //e=e->succ();
    //bct_to_main_graph_edges[e]=bctree.original(e);

    forall_edges(e,graf) {
    }

    forall_edges(e,graf) {
        bct_to_main_graph_edges[e] = bctree.original(e); //same as above with edges
    }

    Node v;
    forall_nodes(v,graf) {

        bct_to_main_graph_nodes[v]= bctree.original(v); //original returns the corresponding
                                                        //node of the original graph
                                                        //(from which the bct was derived)
    }

    //v = bct_to_main_graph_nodes[graf.firstNode()];
    //int pos = mainGraph->getPosition(v); //DEBUG
    //cout << "this should be a valid position baby: " << pos << endl;            //DEBUG

    //now we shall start extracting each biconnected component from "graf" (graf is a variable).
    //we remind that "graf" is the so-called auxilliary graph of the bctree
    //of mainGraph. The auxilliary graph is all the biconnectedcomponents
    //of the graphs, without the edges between them.
    //This way, we can get every B.C. by extracting each connectedSubgraph
    //of graf.

    std::unordered_map<Node,int> explored;


    forall_nodes(v,graf) { //explored: we hold for each vertex of graf
        explored[v]=0;     //an exploration status. i.e. if we have visited them
                           //or not. This way we can ignore the vertices
                            //of already "is a way of  between
    }                      //vertices of already "extracted" connected subgraphs.

    biconnectedComponents = std::vector<BiconnectedComponent*>(); //initialize the vector holding
                                                                  //our bc's

    forall_nodes(v,graf) {

        if (explored[v]==0) { //check if the vertex belongs to an already extracted subgraph

            ogdf::Graph nuGraf;
            //ogdf::NodeArray< Node > nodeMapping(); <-- attention! this doesnt work!
            ogdf::NodeArray< Node > subgraph_to_bct_nodes;// a mapping from of nodes in G to nodes in SG
            ogdf::EdgeArray< Edge > subgraph_to_bct_edges;// similarly
            ogdf::NodeArray<Node> nG_to_nSG;
            ogdf::EdgeArray<Edge> eG_to_eSG;


            ogdf::ConnectedSubgraph<int>::call(graf,nuGraf,v,subgraph_to_bct_nodes,
                                               subgraph_to_bct_edges,nG_to_nSG,eG_to_eSG);

            if (nuGraf.numberOfNodes() <2){
                explored[v]=1;
                continue;
            }
            //else : we are facing a proper biconnected component
            //ie it contains more than one vertex

            //create the mappings
            ogdf::NodeArray<Node> nMapping(nuGraf);
            ogdf::EdgeArray<Edge> eMapping(nuGraf);

            Node vv;

            forall_nodes(vv,nuGraf) {



                nMapping[vv]= bct_to_main_graph_nodes[subgraph_to_bct_nodes[vv]];

            }

            forall_edges(e,nuGraf) {

                eMapping[e]= bct_to_main_graph_edges[subgraph_to_bct_edges[e]];

            }

            BiconnectedComponent *bc = new BiconnectedComponent(nuGraf,mainGraph,nMapping,eMapping);
            //TODO: test if the mappings work

            //now we must set all the vertices in the extracted subgraph as explored
            Node n;
            forall_nodes(n,nuGraf) {

                Node correspondingNode = subgraph_to_bct_nodes[n];
                std::unordered_map<Node,int>::const_iterator got = explored.find(correspondingNode);
                if ( got == explored.end() )
                    std::cout << "not found" << endl;
                  else {

                    explored.at(got->first) = 1;
                    cout << "xplored a n0de " << endl;


                }
            }




            biconnectedComponents.push_back(bc);




        }
    }

    cout << "we have a number of binconnected components here: " << biconnectedComponents.size() << endl;

}



void MainWindow::drawBCTree() {


    //this draws the Biconnected Compenents Tree of the input graph on Tab1, right below the mainGraph.
    //it is drawn in a QGraphicsView object which was made in Desinger, named bCTView.

    bctree = new BCTree(*mainGraph);  //make a new object of clss BCTree
    cout << bctree->numberOfNodes(true) << endl; //testing if class works
    ogdf::Graph graf = bctree->getBCTree();     //get the bct in ogdf::graph representation




    findConnectedComponentsOfMainGraph(); //this splits the graph into
                                          //the biconnected components
                                          //and stores them in the
                                          //vector biconnectedComponents

    //ogdf::GraphIO::writeGML(graf, "/home/kosmas/sierpinski_04-layout.gml");




    Graph* bCGraph = new Graph(graf);    //create a new Graph object that represents the bct.
                                        //check the Graph constructor that takes an ogdf::Graph
                                        //as a parameter




    delete bCTView->scene();             //deleting previous scene if any


    bCGraph->buildLayout(0.0,0.0,bCTView->width(),bCTView->height()); //building layout: this is where the program
                                                                     //segfaults :(.
                                                                     //we use it to make some
                                                                    // settings in the attr member of graphs

    QGraphicsScene *gs =new BCTScene(*bCGraph,bCTView->width(),bCTView->height());
                                                                    //BCTScene: new class, custom made to
                                                                    //show bctrees.

    bCTView->setScene(gs);                                           //final
    bCTView->fitInView((gs)->sceneRect());                           //things

    //now we will display the first biconnected component of the graph in the 2nd tab.

    BiconnectedComponent* bc = biconnectedComponents[0];
    delete bCView->scene();
    bc->buildLayout(0.0,0.0,bCView->width(),bCView->height());


    gs =new BCTScene(*bc,250,50);              //BCTScene: new class, custom made to
                                                                    //show bctrees. (it can also be used
                                                                    // to display graphs in general)

    bCView->setScene(gs);                                           //final
    bCView->fitInView((gs)->sceneRect());                           //things



}

bool MainWindow::openBookEmbeddedGraph(std::string filename){
    BookEmbeddedGraph* temp = new BookEmbeddedGraph();
    if (temp->readGML(filename)){
        for(QUndoStack* s : commandHistory->stacks()){
            commandHistory->removeStack(s);
        }
        commandHistory->addStack(new QUndoStack(commandHistory));//move this operation to drawBCTree() when we can work independently across Biconnected Components
        currentFile = filename;
        //delete mainGraph;
        mainGraph = temp;

        cout << endl << endl << mainGraph->getNcrossings() << endl;
        //std::cout << "Read Successful!!!!!" << std::endl;
        //std::cout << "Number of nodes in read graph ==" << mainGraph->numberOfNodes() << endl;
        //std::cout << "Number of edges in read graph ==" << mainGraph->numberOfEdges() << endl;
        this->drawBCTree(); //drawBCTree
                            //added by kosms
                            //made to draw the Biconnected Components Tree
                            // and to split the maingraph into
                            // its biconnected components and storing
                            // them into the vector called biconnectedComponenets
        this->drawBookEmbeddedGraph();
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


    node_stats->setEnabled(false);
    edge_stats->setEnabled(true);
    std::cout << "Edge (" << e->source()->index() << "," << e->target()->index() << ") selected" << endl;
    //edge_crossings_indicator->setNum(mainGraph->getNcrossings(e));
    edge_source_indicator->setNum(e->source()->index());
    edge_target_indicator->setNum(e->target()->index());
    edge_page_indicator->setNum(mainGraph->getPageNo(e));
    stats->setCurrentWidget(edge_stats);

    int pgno =mainGraph->getPageNo(e);
    PageScene* pg = (PageScene*)pageViews[pgno]->scene();
    //pg->changeEdgeColour(e,Qt::black); //TODO: implement this shit
                                         //this method must change the colour of an edge
                                         //we can use it to show which edge is currently selected.

    GraphScene* gs = (GraphScene*)graphView->scene();
    gs->changeEdgeColourAndWidth(e,MY_COLOR,5);
}

void MainWindow::deselectEverythingInAllPagesBut(int pageNo) {


    for (int i=0; i<pageViews.size(); i++) {

        if (i==pageNo) continue; //if the page under examination
                                //is the page of the selected edge
                                //then we won't touch

        PageScene* ps = (PageScene*) pageViews[i]->scene();
        ps->deselectAll();


    }
}



void MainWindow::on_edge_deselected(Edge& e){
    edge_stats->setEnabled(false);
    //edge_crossings_indicator->clear();
    edge_source_indicator->clear();
    edge_target_indicator->clear();
    edge_page_indicator->clear();

    //kosmas added the following 3 lines:
    //int pageNo = mainGraph->getPageNo(e);
    //PageScene* pageScene = (PageScene*) pageViews[pageNo]->scene();
    //pageScene->repaintEdge(e);


    int pageNo = mainGraph->getPageNo(e);
    PageScene* ps = (PageScene*)pageViews[pageNo]->scene();

    //ps->changeEdgeColour(e); //TODO: implement this shit:
                             //changeEdgeColour(Edge e) must redraw edge e with the page colour

    GraphScene* gs = (GraphScene*)graphView->scene();
    gs->changeEdgeColourAndWidth(e,ps->getColour(),2);

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
    int currPage = mainGraph->getPageNo(e);
    bool ok;
    int newPage = QInputDialog::getInt(this, tr("Move edge to page"),
                                         tr("New page:"),currPage,0,mainGraph->getNpages()-1,1,&ok);
    if (!ok || newPage==currPage) return;
    commandHistory->activeStack()->push(new EdgeMoveCommand(e,(PageScene*)pageViews[currPage]->scene(),(PageScene*)pageViews[newPage]->scene(),mainGraph,(GraphScene*)graphView->scene(),total_crossings_indicator));
}

void MainWindow::on_remove_page(int page){
    Q_ASSERT(mainGraph->pageSize(page) == 0);
    commandHistory->activeStack()->push(new PageRemoveCommand(page, mainGraph, this));
}

void MainWindow::on_crossings_changed(std::vector<int> pagesChanged){
    total_crossings_indicator->setNum(mainGraph->getNcrossings());
    for(int i : pagesChanged){
        ((PageScene*)(pageViews[i]->scene()))->setCrossings(mainGraph->getNcrossings(i));
    }
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
    colourCloset.returnAll();//kosm: this is necessary in order
                             //for our colouring mechanism to work.

    for(int p=0; p<mainGraph->getNpages(); p++){
        add_page_drawing(p);
    }

    actionRedraw->setEnabled(false);
}
