#include "agscene.h"
#include "auxiliarygraph.h"
#include <iostream>
#include "agnode.h"
#include "biconnectedcomponent.h"
#include "mainwindow.h"

AGScene::AGScene(AuxiliaryGraph& g,MainWindow* mainwindow,const double width, const double height){

    nodes = std::unordered_map<Node,AGNode*>();
    QBrush blackBrush(Qt::black);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    Node v;
    forall_nodes(v,g.toOGDF()){



        BiconnectedComponent* b = g.getBCOf(v);
        AGNode* n = new AGNode(v,b);
        double x=g.getXcoord(v);
        double y=g.getYcoord(v);
        n->setBrush(blackBrush);
        n->setPen(blackPen);
        n->setRect(x,y,12,12);
        n->setFlag(QGraphicsItem::ItemIsSelectable, false);
        this->addItem(n);
        nodes[v] = n;
        connect(n,SIGNAL(was_selected(BiconnectedComponent*)),mainwindow,SLOT(loadBC(BiconnectedComponent*)));






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
