#include "graphscene.h"
#include <iostream>
using namespace std;

Colors::Colors(){
    pageColors = {Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::gray};
}

QColor& Colors::operator[](int i){
    return pageColors[i];
}

GraphScene::GraphScene(const BookEmbeddedGraph& g, const int page){
    n = g.numberOfNodes();
    m = g.pageSize(page);
    std::unordered_map<Node,QGraphicsEllipseItem*> nodesReverse = std::unordered_map<Node,QGraphicsEllipseItem*>();

    //Paint Nodes
    int i=0;
    Node v;
    forall_nodes(v,g){
        QBrush redBrush(Qt::red);
        QPen blackPen(Qt::black);
        blackPen.setWidth(2);

        QGraphicsEllipseItem* el = this->addEllipse(-LEN+i*(2*LEN/n),0,12,12,blackPen,redBrush);

        //el->setMovable();
        el->setFlag(QGraphicsItem::ItemIsSelectable, true);
        nodes[el] = v;
        nodesReverse[v] = el;
        i++;
    }

    printf("%d is the number of edges here\n",m);

    //Paint Edges
    for (auto &e : g.edgesIn(page)) {
        QPen blackPen(Qt::black);
        blackPen.setWidth(2);

        qreal interval=(2*LEN/n); //space between two consequent vertices

        qreal x1 = nodesReverse[e->source()]->boundingRect().center().x();
        qreal x2 = nodesReverse[e->target()]->boundingRect().center().x();
        std::cout << x1 << "," << x2 << std::endl;
        double vscalingfactor = 0.7;
        double height = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-height,(x2-x1),2*height,0,180);

        blackPen.setColor(Qt::black);


        QGraphicsItem * path = this->addPath(*edg,blackPen);
        path->setFlag(QGraphicsItem::ItemIsSelectable, true);

        edges[path] = e;
    }



}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    this->addText("καλημέρα !!!");

    //this->repaint();

}

