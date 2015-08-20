#include "pagescene.h"
#include <iostream>
#include <QGraphicsView>


PageScene::PageScene(const BookEmbeddedGraph& g, const int p, MainWindow* w, int width, int height){
    window = w;
    page = p;

    //Paint Nodes
    nodes = new std::unordered_map<Node,QGraphicsEllipseItem*>();
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
        (*nodes)[v] = el;
        i++;
        //connect(el,SIGNAL(was_selected(Node&)),w,SLOT(on_node_selected(Node&)));
        //connect(el,SIGNAL(was_deselected(Node&)),w,SLOT(on_node_deselected(Node&)));
    }

    //Paint Edges
    //std::printf("%d is the number of edges here\n",m);
    edges = new std::unordered_map<Edge, embedding_edge*>();

    pen = QPen(getPageColor(page));
    pen.setWidth(2);

    for (Edge e : g.edgesIn(page)) {
        this->addEdge(e);
    }
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

    connect(path,SIGNAL(was_selected(Edge&)),window,SLOT(on_edge_selected(Edge&)));
    connect(path,SIGNAL(was_deselected(Edge&)),window,SLOT(on_edge_deselected(Edge&)));
    connect(path,SIGNAL(move(Edge&)),window,SLOT(move_edge(Edge&)));
}

void PageScene::removeEdge(const Edge &e){
    this->removeItem(edges->at(e));
    delete edges->at(e);
}

void PageScene::setPageNumber(int p){
    page = p;
    emit page_number_changed(page);
}
