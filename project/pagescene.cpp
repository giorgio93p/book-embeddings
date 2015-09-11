#include "pagescene.h"
#include "mainwindow.h"
#include <iostream>
#include <QGraphicsView>



const qreal PageScene::margin = 10;
const qreal PageScene::minIntervalBetweenNodes = 10;
const qreal PageScene::bottomMargin = 30;


PageScene::PageScene(const BookEmbeddedGraph *g, const int p, MainWindow* w, QColor col,
                     QLabel *pageNumber, QLabel *crossings, QPushButton *del, int width, int height) {

    //first we should store the given values to the corresponding member fields.
    colour = col;
    mainWindow = w;
    page = p; //page is the page number.
    pageNumberIndicator = pageNumber;
    crossingsIndicator = crossings;
    deletePageButton = del;

    deletePageButton->setEnabled(true);
    pageNumberIndicator->setNum(page);

    nodes = new std::unordered_map<Node,PageNode*>();
    nodePositions.resize(g->numberOfNodes());
    setSize(QSize(width,height));


    //prepare drawing tools
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

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
    edges = new std::unordered_map<Edge, PageEdge*>();

    for (Edge e : g->edgesIn(page)) {
        this->addEdge(e);
    }
    this->update();

}




    //we have to distinguish between two cases:

    /*
    if (wholeGraphMode) { //1.wholeGraphMode is on


    else // case 2. we are drawing a page of a single biconnected component.
    {
        int i=0;
        Node v;
        forall_nodes_embedded(v,*g){
            PageNode* el = new PageNode(this, v, &nodePositions);
            el->setPosition(i);
            el->set int i=0;
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
        edges = new std::unordered_map<Edge, PageEdge*>();

        for (Edge e : g->edgesIn(page)) {
            this->addEdge(e);
        }
        this->update();

    } Parent(this);
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
        edges = new std::unordered_map<Edge, PageEdge*>();

        for (Edge e : g->edgesIn(page)) {
            this->addEdge(e);
        }
        this->update();
    }




        BiconnectedComponent* currBC = w->getCurrBC();
        qreal interval=(2*width/currBC->numberOfNodes()); //space between two consequent vertices

        //int i=0; //latestchange2
        Node n;
        forall_nodes(n,*currBC){ //latestchange2 (removed the "embedded")

            //all we have to do is get a reference to the corresponding node in
            // the main graph. Then we use this node instead of the one in our
            //biconncted component, except for the last part: we use the original
            //node as key for the graphics item we create.

            int num = currBC->getNumberOf(n);  //DEBUG: is this number vaild??
            const Node& v = graph.getNodeConst(num);
            int i = graph.getPosition(v);

            QString indexStr  = QString::number(v->index());
            QGraphicsSimpleTextItem* labelItem = this->addSimpleText(indexStr);



            PageNode* el = new PageNode(this, v, i, currBC->numberOfNodes(), interval);
            el->setBrush(redBrush);
            el->setPen(blackPen);
            el->setRect(-width+i*interval,0,12,12);

            //QGraphicsEllipseItem* el = this->addEllipse(-width+i*interval,0,12,12,blackPen,redBrush);

            addItem(el);

            labelItem->setPos(-width+i*interval,15);

            (*nodes)[n] = el; //latest change
            //i++;
            connect(el,SIGNAL(was_selected(Node&,int)),mainWindow,SLOT(on_node_selected(Node&,int)));
            connect(el,SIGNAL(was_deselected(Node&)),mainWindow,SLOT(on_node_deselected(Node&)));
            //connect(el,SIGNAL(move(Node&,int)),mainWindow,SLOT(move_node(Node&,int)));
        }

        //Paint Edges
        edges = new std::unordered_map<Edge, embedding_edge*>();

        pen = QPen(colour);
        pen.setWidth(2);

        for (Edge e : currBC->edgesIn(page)) {

            this->addEdge(e);
        }
    }
    else
    {


}

*/

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
    connect(path,SIGNAL(move(Edge&,int)),mainWindow,SLOT(move_edge_request(Edge&,int)));
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





/*
void PageScene::addEdge(const Edge& e){


    bool wholeGraphMode = mainWindow->isWholeGraphModeOn();
    BiconnectedComponent* currBC = mainWindow->getCurrBC();
    BookEmbeddedGraph* mainGraph = mainWindow->getMainGraph();

    //if (wholeGraphMode) {









        //case1 we are drawing a page of the wholeGraph.
        //do the usual stuff..

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


        qreal interval=(2*width/g->numberOfNodes()); //space between two consequent vertices

        int i=0;
        Node v;
        forall_nodes_embedded(v,*g){




            QString indexStr  = QString::number(v->index());
            QGraphicsSimpleTextItem* labelItem = this->addSimpleText(indexStr);



<<<<<<< HEAD
            PageNode* el = new PageNode(this, v, i, g->numberOfNodes(), interval);
            el->setBrush(redBrush);
            el->setPen(blackPen);
            el->setRect(-width+i*interval,0,12,12);

            //QGraphicsEllipseItem* el = this->addEllipse(-width+i*interval,0,12,12,blackPen,redBrush);

            addItem(el);

            labelItem->setPos(-width+i*interval,15);

            (*nodes)[v] = el; //latest change
            i++;
            connect(el,SIGNAL(was_selected(Node&,int)),mainWindow,SLOT(on_node_selected(Node&,int)));
            connect(el,SIGNAL(was_deselected(Node&)),mainWindow,SLOT(on_node_deselected(Node&)));
            //connect(el,SIGNAL(move(Node&,int)),mainWindow,SLOT(move_node(Node&,int)));
        }

        //Paint Edges
        //std::printf("%d is the number of edges here\n",m);
        edges = new std::unordered_map<Edge, embedding_edge*>();


        pen = QPen(colour);
        pen.setWidth(2);

        for (Edge e : g->edgesIn(page)) {
            this->addEdge(e);
        }
=======

>>>>>>> 0b0a91174265dc7b9fc8d91c187761ecc6fabdb3
    }
    this->update();
//=======
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
/*>>>>>>> 0b0a91174265dc7b9fc8d91c187761ecc6fabdb3


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

        deletePageButton->setEnabled(false);

        connect(path,SIGNAL(was_selected(Edge&)),mainWindow,SLOT(on_edge_selected(Edge&)));
        connect(path,SIGNAL(was_deselected(Edge&)),mainWindow,SLOT(on_edge_deselected(Edge&)));
        connect(path,SIGNAL(move(Edge&)),mainWindow,SLOT(move_edge(Edge&)));





    } else { //case2: we are drawing the pages of only 1 biconnected component.

        int num = currBC->getNumberOf(e);
        Edge f = mainGraph->getEdge(num);
        //got the corresponding edge of the main graph.
        //now the new PageEdge item will have this edge.

        PageNode* source = (*nodes)[f->source()];
        const QPointF sourceCenter = source->mapToScene(source->boundingRect().center());
        PageNode* target = (*nodes)[f->target()];
        const QPointF targetCenter = target->mapToScene(target->boundingRect().center());

        PageEdge * path = new PageEdge(sourceCenter, targetCenter, colour, f, vScalingFactor);
        this->addItem(path);
        path->setParent(this);

        (*edges)[e]= path;
        source->addIncidentEdge(path);
        target->addIncidentEdge(path);



        qreal x1 = std::min((*nodes)[e->source()]->boundingRect().center().x(),(*nodes)[e->target()]->boundingRect().center().x());
        qreal x2 = std::max((*nodes)[e->source()]->boundingRect().center().x(),(*nodes)[e->target()]->boundingRect().center().x());

        double vscalingfactor = 0.7;
        double h = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-h,(x2-x1),2*h,0,180);

        embedding_edge * path = new embedding_edge(edg, pen, f);
        this->addItem(path);
        path->setFlags(QGraphicsItem::ItemIsSelectable);

        (*edges)[e]= path;

        deletePageButton->setEnabled(false);

        connect(path,SIGNAL(was_selected(Edge&)),mainWindow,SLOT(on_edge_selected(Edge&)));
        connect(path,SIGNAL(was_deselected(Edge&)),mainWindow,SLOT(on_edge_deselected(Edge&)));
        connect(path,SIGNAL(move(Edge&)),mainWindow,SLOT(move_edge(Edge&)));

    }
}

*/



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

    vScalingFactor = qMin((newSize.height()-bottomMargin)/(nodePositions.back().x()-nodePositions.front().x()),0.75);
    //edited by kosmas
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
