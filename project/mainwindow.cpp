
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
#include <QPainterPath>
#include <QRect>
#include <QTransform>
#include <ogdf/basic/GraphList.h>


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

    colourCloset.returnAll(); // the returnAll method simply resets the closet to its intial state
                              // that is, having all of its colours inside
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
        currentFile = filename;
        //delete mainGraph;
        mainGraph = temp;
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

    /*
    int pageNo = mainGraph->getPageNo(e);

    for (std::vector<QGraphicsView*>::iterator it = pageViews.begin() ; it != pageViews.end(); ++it) {

        QGraphicsView* page = *it;

        if (page == pageViews[pageNo]) continue;

        QPainterPath path;
        path.addRect(0.0,0.0,0.0,0.0);
        QGraphicsScene* pageScene = page->scene();
        pageScene->setSelectionArea(path,QTransform());


    }

    */

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
        QColor colourToBeReturned = ps->getColour(); //here we get the colour of the page
        colourCloset.returnPaint(colourToBeReturned); // and we return it to the closet
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
