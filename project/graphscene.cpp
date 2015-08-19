#include "graphscene.h"
#include <iostream>

GraphScene::GraphScene(const BookEmbeddedGraph& g, const double width, const double height){
    //Paint Nodes
    nodes = std::unordered_map<Node,QGraphicsEllipseItem*>();
    QBrush redBrush(Qt::black);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    Node v;
    forall_nodes(v,g){
        //std::cout << g.getPosition(v) << std::endl;
        QGraphicsEllipseItem* el = this->addEllipse(g.getXcoord(v),g.getYcoord(v),6,6,blackPen,redBrush);

        //el->setMovable();
        el->setFlag(QGraphicsItem::ItemIsSelectable, true);
        nodes[v] = el;
    }

    //Paint Edges
    edges = std::unordered_map<Edge, QGraphicsItem*>();
    QPen pen = QPen();
    pen.setWidth(2);

    Edge e;
    forall_edges(e,g) {
        //std::cout << "Page number: " << page << std::endl;
        //std::cout << "Color: " << (color.name().toUtf8().toStdString()) << std::endl;
        cout << "agadgasgs" << endl;

        pen.setColor(getPageColor(g.getPageNo(e)));
        qreal x1 = nodes[e->source()]->boundingRect().center().x();
        qreal y1 = nodes[e->source()]->boundingRect().center().y();
        qreal x2 = nodes[e->target()]->boundingRect().center().x();
        qreal y2 = nodes[e->target()]->boundingRect().center().y();

        QGraphicsItem * path = this->addLine(x1,y1,x2,y2,pen);
        path->setFlags(QGraphicsItem::ItemIsSelectable);

        edges[e] = path;
    }
}
