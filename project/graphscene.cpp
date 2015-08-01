#include "graphscene.h"
#include <iostream>

GraphScene::GraphScene(const BookEmbeddedGraph& g, const int page){
    n = g.numberOfNodes();
    nodes = std::unordered_map<Node,QGraphicsEllipseItem*>();

    //Paint Nodes
    int i=0;
    Node v;
    forall_nodes_embedded(v,g){
        //std::cout << g.getPosition(v) << std::endl;
        QBrush redBrush(Qt::red);
        QPen blackPen(Qt::black);
        blackPen.setWidth(2);
        qreal interval=(2*LEN/n); //space between two consequent vertices

        QGraphicsEllipseItem* el = this->addEllipse(-LEN+i*interval,0,12,12,blackPen,redBrush);

        //el->setMovable();
        el->setFlag(QGraphicsItem::ItemIsSelectable, true);
        nodes[v] = el;
        i++;
    }
    //Paint Edges
    //std::printf("%d is the number of edges here\n",m);
    edges = std::unordered_map<Edge, QGraphicsItem*>();
    m = g.pageSize(page);
    QColor colors[] = {Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::gray,
                      Qt::darkRed,Qt::darkGreen,Qt::darkBlue,Qt::darkCyan,Qt::darkMagenta,Qt::darkYellow,
                       Qt::lightGray};
    QColor color = page<14 ? colors[page] : Qt::black;
    //std::cout << "Page number: " << page << std::endl;
    //std::cout << "Color: " << (color.name().toUtf8().toStdString()) << std::endl;
    QPen pen(color);
    pen.setWidth(2);

    for (Edge e : g.edgesIn(page)) {

        qreal x1 = std::min(nodes[e->source()]->boundingRect().center().x(),nodes[e->target()]->boundingRect().center().x());
        qreal x2 = std::max(nodes[e->source()]->boundingRect().center().x(),nodes[e->target()]->boundingRect().center().x());

        double vscalingfactor = 0.7;
        double height = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-height,(x2-x1),2*height,0,180);


        QGraphicsItem * path = new embedding_edge(height, x1, x2, edg, pen);
        this->addItem(path);
        path->setFlags(QGraphicsItem::ItemIsSelectable);

        edges[e] = path;
    }
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    //this->addText("καλημέρα !!!");
}

