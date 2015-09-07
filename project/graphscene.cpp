#include "graphscene.h"
#include "colors.h"
#include "mainwindow.h"

#include <iostream>

GraphScene::GraphScene(BookEmbeddedGraph& g,MainWindow* w, const double width, const double height){
    //Paint Nodes
    nodes = new std::unordered_map<Node,QGraphicsEllipseItem*>();
    mainWindow=w;

    Node v;
    forall_nodes(v,g){
        int w = DEFAULT_NODE_WIDTH;
        qreal x = g.getXcoord(v);
        qreal y = g.getYcoord(v);
        QRectF rect = QRectF(QPointF(x-w,y-w),QPointF(x+w,y+w));
        addNode(v,rect);
    }

    //Paint Edges
    edges = new std::unordered_map<Edge, QGraphicsItem*>();

    Edge e;
    forall_edges(e,g) {
        addEdgeInitial(e,g.getPageNo(e));
    }
}

void GraphScene::addEdgeInitial(const Edge &e, const int page){
    addEdge(e,mainWindow->getPageColour(page),2);
}

void GraphScene::addEdge(const Edge &e, const QColor col, const int width){
    QPen pen = QPen();
    pen.setWidth(width);
    pen.setColor(col);
    qreal x1 = (*nodes)[e->source()]->boundingRect().center().x();
    qreal y1 = (*nodes)[e->source()]->boundingRect().center().y();
    qreal x2 = (*nodes)[e->target()]->boundingRect().center().x();
    qreal y2 = (*nodes)[e->target()]->boundingRect().center().y();

    QGraphicsItem * path = this->addLine(x1,y1,x2,y2,pen);
    path->setFlags(QGraphicsItem::ItemIsSelectable);
    path->setZValue(0);
    (*edges)[e] = path;
}

void GraphScene::changeEdgeColourAndWidth(const Edge& e,const QColor col,const int width) {

    this->removeEdge(e);
    this->addEdge(e,col,width);
}

void GraphScene::removeEdge(const Edge &e){
    this->removeItem(edges->at(e));
    delete edges->at(e);
}

void GraphScene::addNode(const Node& v, QRectF boundingRect, const QColor col){
    QBrush brush(col);
    QPen pen(col);
    //std::cout << g.getPosition(v) << std::endl;
    QGraphicsEllipseItem* el = this->addEllipse(boundingRect,pen,brush);

    el->setFlag(QGraphicsItem::ItemIsSelectable, true);
    el->setZValue(1);
    (*nodes)[v] = el;
}

void GraphScene::removeNode(const Node& v){
    this->removeItem(nodes->at(v));
    delete nodes->at(v);
}

void GraphScene::changeNodeColourAndWidth(const Node& v, const QColor col, const int width){
    qreal x = (*nodes)[v]->boundingRect().center().x();
    qreal y = (*nodes)[v]->boundingRect().center().y();
    QRectF rect = QRectF(QPointF(x-width,y-width),QPointF(x+width,y+width));//create new rectangle aligned at the center of the old one
    this->removeNode(v);
    this->addNode(v,rect,col);
}


