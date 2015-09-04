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
    qreal interval=(2*width/g.numberOfNodes()); //space between two consequent vertices

    int i=0;
    Node v;
    forall_nodes_embedded(v,g){
        //std::cout << g.getPosition(v) << std::endl;
        QString indexStr = QString::number(v->index());
        QGraphicsSimpleTextItem* labelItem = this->addSimpleText(indexStr);
        PageNode* el = new PageNode(this, i, g.numberOfNodes(), interval);
        el->setBrush(redBrush);
        el->setPen(blackPen);
        el->setRect(-width+i*interval,0,12,12);
        addItem(el);
        //QGraphicsEllipseItem* el = this->addEllipse(-width+i*interval,0,12,12,blackPen,redBrush);

        labelItem->setPos(-width+i*interval,15);
        (*nodes)[v] = el;
        //nodes[v] = el;
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
}

void PageScene::repaintEdge(const Edge e) {
    (*edges)[e]->update();
}

void PageScene::addEdge(const Edge& e){
    qreal x1 = std::min((*nodes)[e->source()]->boundingRect().center().x(),(*nodes)[e->target()]->boundingRect().center().x());
    qreal x2 = std::max((*nodes)[e->source()]->boundingRect().center().x(),(*nodes)[e->target()]->boundingRect().center().x());

    double vscalingfactor = 0.7;
    double h = ((x2-x1)/2)*vscalingfactor;
    QPainterPath* edg = new QPainterPath();
    edg->moveTo(x2,0);
    edg->arcTo(x1,-h,(x2-x1),2*h,0,180);

    embedding_edge * path = new embedding_edge(edg, pen, e);
    this->addItem(path);
    path->setFlags(QGraphicsItem::ItemIsSelectable);

    (*edges)[e]= path;

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
