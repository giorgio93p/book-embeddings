#include "pagescene.h"
#include "mainwindow.h"
#include <iostream>
#include <QGraphicsView>
#include "embedding_edge.h"


/*
 * This class used to be called "BookEmbeddedScene", but for obvious reasons it was renamed.
 * It is used as a scene that shows a single page.
 */



PageScene::PageScene(const BookEmbeddedGraph& g, const int p, MainWindow* w, QColor col, QLabel *pageNumber, QLabel *crossings, QPushButton *del, int width, int height) : m_width(width){
    //Paint Nodes

    colour = col;
    mainWindow = w;
    page = p;
    pageNumberIndicator = pageNumber;
    crossingsIndicator = crossings;
    deletePageButton = del;

    deletePageButton->setEnabled(true);
    pageNumberIndicator->setNum(page);

    nodes = new std::unordered_map<Node,PageNode*>();
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    nodePositions.reserve(g.numberOfNodes());
    qreal interval=width/(g.numberOfNodes()-1); //space between two consequent vertices
    for(int i=0; i<g.numberOfNodes(); i++){
        nodePositions.push_back(QPointF(i*interval,0));
    }

    int i=0;
    Node v;
    forall_nodes_embedded(v,g){
        PageNode* el = new PageNode(this, v, &nodePositions);
        el->setBrush(redBrush);
        el->setPen(blackPen);
        el->setRect(0,0,12,12);
        el->setPosition(i);
        el->setParent(this);
        addItem(el);
        //std::cout << "nodePositions[" << i << "].x(): " << nodePositions[i].x() << std::endl;
        //std::cout << "Node " << v->index() <<  " drawn at position " << i << " (" << el->scenePos().x() << "," << el->scenePos().y() << ")" << std::endl;

        (*nodes)[v] = el;

        connect(el,SIGNAL(was_selected(Node&,int)),mainWindow,SLOT(on_node_selected(Node&,int)));
        connect(el,SIGNAL(was_deselected(Node&)),mainWindow,SLOT(on_node_deselected(Node&)));
        connect(el,SIGNAL(move(Node&,int)),mainWindow,SLOT(move_node(Node&,int)));

        i++;
    }

    //Paint Edges
    //std::printf("%d is the number of edges here\n",m);
    edges = new std::unordered_map<Edge, embedding_edge*>();


    pen = QPen(colour);
    pen.setWidth(2);

    for (Edge e : g.edgesIn(page)) {
        this->addEdge(e);
    }
    this->update();
}

void PageScene::repaintEdge(const Edge e) {
    (*edges)[e]->update();
}

void PageScene::addEdge(const Edge& e){
    PageNode* source = (*nodes)[e->source()];
    const QPointF sourceCenter = source->mapToScene(source->boundingRect().center());
    PageNode* target = (*nodes)[e->target()];
    const QPointF targetCenter = target->mapToScene(target->boundingRect().center());

    embedding_edge * path = new embedding_edge(sourceCenter, targetCenter, pen, e);
    this->addItem(path);
    path->setFlags(QGraphicsItem::ItemIsSelectable);
    path->setParent(this);

    (*edges)[e]= path;
    source->addIncidentEdge(path);
    target->addIncidentEdge(path);

    deletePageButton->setEnabled(false);

    connect(path,SIGNAL(was_selected(Edge&)),mainWindow,SLOT(on_edge_selected(Edge&)));
    connect(path,SIGNAL(was_deselected(Edge&)),mainWindow,SLOT(on_edge_deselected(Edge&)));
    connect(path,SIGNAL(move(Edge&)),mainWindow,SLOT(move_edge(Edge&)));
}

void PageScene::removeEdge(const Edge &e){
    this->removeItem(edges->at(e));
    delete edges->at(e);
    edges->erase(e);
    if(edges->size() == 0) deletePageButton->setEnabled(true);
}

void PageScene::setPageNumber(int p){
    page = p;
    pageNumberIndicator->setNum(p);
}

void PageScene::setCrossings(int crossings){
    crossingsIndicator->setNum(crossings);
}

int PageScene::width() {
    return m_width;
}

MainWindow* PageScene::window(){
    return mainWindow;
}

void PageScene::redraw(BookEmbeddedGraph& g){
    //Move nodes
    Q_ASSERT(g.numberOfNodes()==nodes->size());
    int i=0;
    Node v;
    forall_nodes_embedded(v,g){
        PageNode* nodeGraphic = (*nodes)[v];
        //std::cout << "node " << v->index() << " from " << nodeGraphic->scenePos().x() << " to " << nodePositions[i].x() << std::endl;
        nodeGraphic->setPosition(i);

        i++;
    }

    this->update();
}

void PageScene::deselectAll() {

    for ( auto it = edges->begin(); it != edges->end(); ++it ) {
        QGraphicsItem* curr = (QGraphicsItem*)it->second;
        curr->setSelected(false);
    }

    for ( auto it = nodes->begin(); it != nodes->end(); ++it ) {
        QGraphicsItem* curr = (QGraphicsItem*)it->second;
        curr->setSelected(false);
    }

}
