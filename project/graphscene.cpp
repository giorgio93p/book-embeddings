#include "graphscene.h"
#include <iostream>

QColor pageColors[] = {Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::gray,
                  Qt::darkRed,Qt::darkGreen,Qt::darkBlue,Qt::darkCyan,Qt::darkMagenta,Qt::darkYellow,Qt::lightGray};
int pageColors_length = 14;
QColor getPageColor(int page){
    return pageColors[page % pageColors_length];
}

EmbeddedGraphScene::EmbeddedGraphScene(const BookEmbeddedGraph& g, const int page, const double width, const double height){
    //Paint Nodes
    nodes = std::unordered_map<Node,QGraphicsEllipseItem*>();
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    qreal interval=(2*width/g.numberOfNodes()); //space between two consequent vertices

    int i=0;
    Node v;
    forall_nodes_embedded(v,g){
        //std::cout << g.getPosition(v) << std::endl;

        QGraphicsEllipseItem* el = this->addEllipse(-width+i*interval,0,12,12,blackPen,redBrush);

        //el->setMovable();
        el->setFlag(QGraphicsItem::ItemIsSelectable, true);
        nodes[v] = el;
        i++;
    }

    //Paint Edges
    //std::printf("%d is the number of edges here\n",m);
    edges = std::unordered_map<Edge, QGraphicsItem*>();
    QColor color = getPageColor(page);
    //std::cout << "Page number: " << page << std::endl;
    //std::cout << "Color: " << (color.name().toUtf8().toStdString()) << std::endl;
    QPen pen(color);
    pen.setWidth(2);

    for (Edge e : g.edgesIn(page)) {

        qreal x1 = std::min(nodes[e->source()]->boundingRect().center().x(),nodes[e->target()]->boundingRect().center().x());
        qreal x2 = std::max(nodes[e->source()]->boundingRect().center().x(),nodes[e->target()]->boundingRect().center().x());

        double vscalingfactor = 0.7;
        double h = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-h,(x2-x1),2*h,0,180);


        QGraphicsItem * path = new embedding_edge(h, x1, x2, edg, pen);
        this->addItem(path);
        path->setFlags(QGraphicsItem::ItemIsSelectable);

        edges[e] = path;
    }
}

void EmbeddedGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    //this->addText("καλημέρα !!!");
}

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
    //std::printf("%d is the number of edges here\n",m);
    edges = std::unordered_map<Edge, QGraphicsItem*>();
    QPen pen = QPen();
    pen.setWidth(2);

    Edge e;
    forall_edges(e,g) {
        //std::cout << "Page number: " << page << std::endl;
        //std::cout << "Color: " << (color.name().toUtf8().toStdString()) << std::endl;
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
