#include "pagescene.h"
#include "mainwindow.h"
#include <iostream>
#include <QGraphicsView>


/*
 * This class used to be called "BookEmbeddedScene", but for obvious reasons it was renamed.
 * It is used as a scene that shows a single page.
 */
const qreal PageScene::margin = 10;
const qreal PageScene::minIntervalBetweenNodes = 10;
const qreal PageScene::bottomMargin = 30;

PageScene::PageScene(const BookEmbeddedGraph *g, const int p, MainWindow* w, QColor col, QLabel *pageNumber, QLabel *crossings, QPushButton *del, int width, int height) {
    //Paint Nodes
    colour = col;
    mainWindow = w;
    page = p;
    pageNumberIndicator = pageNumber;
    crossingsIndicator = crossings;
    deletePageButton = del;

    deletePageButton->setEnabled(true);
    pageNumberIndicator->setNum(page);

    nodes = new std::unordered_map<Node,PageNode*>();
    nodePositions.resize(g->numberOfNodes());
    setSize(QSize(width,height));

    int i=0;
    Node v;
    forall_nodes_embedded(v,*g){
        PageNode* el = new PageNode(this, v, &nodePositions);
        el->setPosition(i);
        el->setParent(this);
        addItem(el);
        //std::cout << "nodePositions[" << i << "].x(): " << nodePositions[i].x() << std::endl;
        //std::cout << "Node " << v->index() <<  " drawn at position " << i << " (" << el->scenePos().x() << "," << el->scenePos().y() << ")" << std::endl;
        (*nodes)[v] = el;

        connect(el,SIGNAL(was_selected(Node&,int)),mainWindow,SLOT(on_node_selected(Node&,int)));
        connect(el,SIGNAL(was_deselected(Node&)),mainWindow,SLOT(on_node_deselected(Node&)));
        connect(el,SIGNAL(was_dragged(Node&,int,QPointF)),mainWindow,SLOT(on_node_dragged(Node&,int,QPointF)));
        connect(el,SIGNAL(move(Node&,int)),mainWindow,SLOT(move_node(Node&,int)));

        i++;
    }

    //Paint Edges
    //std::printf("%d is the number of edges here\n",m);
    edges = new std::unordered_map<Edge, PageEdge*>();

    for (Edge e : g->edgesIn(page)) {
        this->addEdge(e);
    }
    this->update();
}

void PageScene::repaintEdge(const Edge e) {
    (*edges)[e]->update();
}

void PageScene::addEdge(const Edge& e){
    PageNode* source = (*nodes)[e->source()];
    const QPointF sourceCenter = source->mapToScene(source->boundingRect().center());
    PageNode* target = (*nodes)[e->target()];
    const QPointF targetCenter = target->mapToScene(target->boundingRect().center());

    PageEdge * path = new PageEdge(sourceCenter, targetCenter, colour, e, vScalingFactor);
    this->addItem(path);
    path->setParent(this);

    (*edges)[e]= path;
    source->addIncidentEdge(path);
    target->addIncidentEdge(path);

    deletePageButton->setEnabled(false);

    connect(path,SIGNAL(was_selected(Edge&)),mainWindow,SLOT(on_edge_selected(Edge&)));
    connect(path,SIGNAL(was_deselected(Edge&)),mainWindow,SLOT(on_edge_deselected(Edge&)));
    connect(path,SIGNAL(move(Edge&)),mainWindow,SLOT(move_edge(Edge&)));
}

void PageScene::removeEdge(const Edge &e){
    PageNode* source = (*nodes)[e->source()];
    PageNode* target = (*nodes)[e->target()];
    source->removeIncidentEdge(edges->at(e));
    target->removeIncidentEdge(edges->at(e));
    this->removeItem(edges->at(e));
    delete edges->at(e);
    edges->erase(e);
    if(edges->size() == 0) deletePageButton->setEnabled(true);
}

void PageScene::setPageNumber(int p){
    page = p;
    pageNumberIndicator->setNum(p);
}

void PageScene::setCrossings(int crossings){
    crossingsIndicator->setNum(crossings);
}

void PageScene::setSize(QSize newSize) {
    int n = nodePositions.size();
    qreal interval=qMax((newSize.width()-2*margin)/n, minIntervalBetweenNodes); //space between two consequent vertices
    qreal pos=margin+interval/2;
    for(int i=0; i<n; i++){
        nodePositions[i] = (QPointF(pos,0));
        pos+=interval;
    }

    vScalingFactor = qMin((newSize.height()-bottomMargin)/(nodePositions.back().x()-nodePositions.front().x()),1.0);
    //std::cout << newSize.height()-30 << " / " << (nodePositions.back().x()-nodePositions.front().x()) << " = " << vScalingFactor << std::endl;
}

QRectF PageScene::redraw(BookEmbeddedGraph* g){
    //Notify edges of new height
    for ( auto it = edges->begin(); it != edges->end(); ++it ) {
        PageEdge* curr = (PageEdge*)it->second;
        curr->setVScalingFactor(vScalingFactor);
    }

    //Move nodes
    Q_ASSERT(g->numberOfNodes()==nodes->size());
    int i=0;
    Node v;
    forall_nodes_embedded(v,*g){
        PageNode* nodeGraphic = (*nodes)[v];
        //std::cout << "node " << v->index() << " from " << nodeGraphic->scenePos().x() << " to " << nodePositions[i].x() << std::endl;
        nodeGraphic->setPosition(i);

        i++;
    }

    this->update();

    //return sceneRect, so that the view can adjust scrollbars
    int n = nodePositions.size();
    qreal interval= (nodePositions.back().x()-nodePositions.front().x())/(n-1); //space between two consequent vertices

    QPointF topleft = QPointF(nodePositions.front().x()-interval/2-margin,-(nodePositions.back().x()-nodePositions.front().x())*vScalingFactor);
    QPointF bottomRight = QPointF(nodePositions.back()) + QPointF(interval/2+margin,bottomMargin);
    return QRectF(topleft,bottomRight);
}

void PageScene::moveNode(Node& v, QPointF toPos){
    (*nodes)[v]->setPos(toPos);
}

void PageScene::deselectAll() {

    for ( auto it = edges->begin(); it != edges->end(); ++it ) {
        QGraphicsItem* curr = (QGraphicsItem*)it->second;
        curr->setSelected(false);
    }

    for ( auto it = nodes->begin(); it != nodes->end(); ++it ) {
        QGraphicsItem* curr = (QGraphicsItem*)it->second;
        curr->setSelected(false);
    }

}

void PageScene::highlightNode(Node& v, bool enable){
    nodes->at(v)->toggleHighlight(enable);
}
void PageScene::highlightEdge(Edge& v, bool enable){
    edges->at(v)->toggleHighlight(enable);
}
