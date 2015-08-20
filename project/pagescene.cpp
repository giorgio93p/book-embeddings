#include "pagescene.h"
#include <iostream>
#include <QGraphicsView>

/*
 * This class used to be called "BookEmbeddedScene", but for obvious reasons it was renamed.
 * It is used as a scene that shows a single page.
 */


PageScene::PageScene(const BookEmbeddedGraph& g, const int page, int width, int height){
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
    edges = new std::unordered_map<Edge, embedding_edge*>();

    QColor color = getPageColor(page);

    //std::cout << "Page number: " << page << std::endl;
    //std::cout << "Color: " << (color.name().toUtf8().toStdString()) << std::endl;
    QPen pen(color);
    pen.setWidth(2);

    for (Edge e : g.edgesIn(page)) {

        qreal x1 = std::min(nodes[e->source()]->boundingRect().center().x(),nodes[e->target()]->boundingRect().center().x());
        qreal x2 = std::max(nodes[e->source()]->boundingRect().center().x(),nodes[e->target()]->boundingRect().center().x());

        Edge f = e;
        double vscalingfactor = 0.7;
        double h = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-h,(x2-x1),2*h,0,180);


        embedding_edge * path = new embedding_edge(h, x1, x2, edg, pen);
        this->addItem(path);
        path->setFlags(QGraphicsItem::ItemIsSelectable);


        (*edges)[e]= path;
        //connect(edges->at(e),SIGNAL(was_clicked( embedding_edge* )),this,SLOT(createDialog(embedding_edge* )));
    }
}

void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    this->addText("καλημέρα !!!");

}

void PageScene::createDialog(embedding_edge* a)
{\
    //const embedding_edge* b = a;

    Edge key;
    for ( auto it =edges->begin(); it != edges->end(); ++it )
        if ( it->second == a) key = it->first;

    //views().at(0)->parentWidget()->parentWidget()->getMainGraph()->getPageNo(key);

    //kosmas: edw ypotithetai pos prepei na ftiaksw tin apo panw grammi kwdika

    //Edge e = edges->find(a)->first;
    //((views()).at(0))->edgeDialogOpen(e);


}
