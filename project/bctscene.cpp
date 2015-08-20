#include "bctscene.h"
#include <iostream>

BCTScene::BCTScene(const Graph& g, const double width, const double height){

    nodes = std::unordered_map<Node,QGraphicsEllipseItem*>();
    QBrush blackBrush(Qt::black);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    Node v;
    forall_nodes(v,g){
        QGraphicsEllipseItem* el = this->addEllipse(g.getXcoord(v),g.getYcoord(v),6,6,blackPen,blackBrush);

        el->setFlag(QGraphicsItem::ItemIsSelectable, true);
        nodes[v] = el;
    }

    //Paint Edges
    edges = std::unordered_map<Edge, QGraphicsItem*>();
    QPen pen = QPen();
    pen.setWidth(2);

    Edge e;
    forall_edges(e,g) {

        qreal x1 = nodes[e->source()]->boundingRect().center().x();
        qreal y1 = nodes[e->source()]->boundingRect().center().y();
        qreal x2 = nodes[e->target()]->boundingRect().center().x();
        qreal y2 = nodes[e->target()]->boundingRect().center().y();

        QGraphicsItem * path = this->addLine(x1,y1,x2,y2,pen);//TODO: make it an arrow
        path->setFlags(QGraphicsItem::ItemIsSelectable);

        edges[e] = path;
    }
}
