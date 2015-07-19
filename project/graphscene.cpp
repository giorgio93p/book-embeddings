#include "graphscene.h"
#include <iostream>
using namespace std;



GraphScene::GraphScene(const BookEmbeddedGraph& g, const int page){
    n = g.numberOfNodes();
    std::unordered_map<Node,QGraphicsEllipseItem*> nodes = std::unordered_map<Node,QGraphicsEllipseItem*>();

    //Paint Nodes
    int i=0;
    Node v;
    forall_nodes(v,g){
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
    std::unordered_map<Edge, QGraphicsItem*> edges = std::unordered_map<Edge, QGraphicsItem*>();
    m = g.pageSize(page);
    QColor color;
    switch(page){
        case 0: color = Qt::red; break;
        case 1: color =  Qt::green; break;
        case 2: color =  Qt::blue; break;
        case 3: color =  Qt::cyan; break;
        case 4: color =  Qt::magenta; break;
        case 5: color =  Qt::yellow; break;
        case 6: color =  Qt::gray; break;
        case 7: color =  Qt::darkRed; break;
        case 8: color =  Qt::darkGreen; break;
        case 9: color =  Qt::darkBlue; break;
        case 10: color =  Qt::darkCyan; break;
        case 11: color =  Qt::darkMagenta; break;
        case 12: color =  Qt::darkYellow; break;
        case 13: color =  Qt::lightGray; break;
        default: color = Qt::black;
    }
    std::cout << "Page number: " << page << std::endl;
    std::cout << "Color: " << (color.name().toUtf8().toStdString()) << std::endl;
    QPen pen(color);
    pen.setWidth(2);

    for (auto &e : g.edgesIn(page)) {

        qreal x1 = nodes[e->source()]->boundingRect().center().x();
        qreal x2 = nodes[e->target()]->boundingRect().center().x();

        double vscalingfactor = 0.7;
        double height = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-height,(x2-x1),2*height,0,180);


        QGraphicsItem * path = new embedding_edge(height, x1, x2, edg, pen);
        path->setFlags(QGraphicsItem::ItemIsSelectable);

        edges[e] = path;
    }
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    this->addText("καλημέρα !!!");
}

