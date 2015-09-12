#include "graphscene.h"
#include "mainwindow.h"

#include <iostream>

GraphScene::GraphScene(BookEmbeddedGraph* g,MainWindow* w, const double width, const double height){
    //Paint Nodes
    nodes = new std::unordered_map<Node,GraphNode*>();
    mainWindow=w;

    Node v;
    forall_nodes(v,*g){
        addNode(v,QPointF(g->getXcoord(v),g->getYcoord(v)));
    }

    //Paint Edges
    edges = new std::unordered_map<Edge, GraphEdge*>();

    Edge e;
    forall_edges(e,*g) {
        addEdge(e,mainWindow->getPageColour(g->getPageNo(e)));
    }
}

void GraphScene::addEdge(const Edge &e, const QColor col){
    GraphNode* source = (*nodes)[e->source()];
    const QPointF sourceCenter = source->mapToScene(source->boundingRect().center());
    GraphNode* target = (*nodes)[e->target()];
    const QPointF targetCenter = target->mapToScene(target->boundingRect().center());

    GraphEdge * path = new GraphEdge(sourceCenter,targetCenter,col,e);
    this->addItem(path);
    (*edges)[e] = path;

    source->addIncidentEdge(path);
    target->addIncidentEdge(path);

    connect(path,SIGNAL(was_selected(Edge&)),mainWindow,SLOT(on_edge_selected(Edge&)));
    connect(path,SIGNAL(was_deselected(Edge&)),mainWindow,SLOT(on_edge_deselected(Edge&)));
    connect(path,SIGNAL(move(Edge&)),mainWindow,SLOT(move_edge_request(Edge&)));
}

void GraphScene::removeEdge(const Edge &e){
    this->removeItem(edges->at(e));
    delete edges->at(e);
}

void GraphScene::addNode(const Node& v, QPointF position){
    //std::cout << g.getPosition(v) << std::endl;
    GraphNode* el = new GraphNode(v);
    this->addItem(el);
    el->setPos(position);
    (*nodes)[v] = el;
    connect(el,SIGNAL(was_selected(Node&)),mainWindow,SLOT(on_node_selected(Node&)));
    connect(el,SIGNAL(was_deselected(Node&)),mainWindow,SLOT(on_node_deselected(Node&)));
    connect(el,SIGNAL(coordinates_changed(Node&,QPointF)),mainWindow,SLOT(node_coordinates_changed(Node&,QPointF)));
}

void GraphScene::removeNode(const Node& v){
    this->removeItem(nodes->at(v));
    delete nodes->at(v);
}

void GraphScene::highlightNode(const Node& v, bool enable){
    nodes->at(v)->toggleHighlight(enable);
}

void GraphScene::highlightEdge(const Edge& e, bool enable){
    edges->at(e)->toggleHighlight(enable);
}

void GraphScene::deselectAll() {

    for ( auto it = edges->begin(); it != edges->end(); ++it ) {
        QGraphicsItem* curr = (QGraphicsItem*)it->second;
        curr->setSelected(false);
    }

    for ( auto it = nodes->begin(); it != nodes->end(); ++it ) {
        QGraphicsItem* curr = (QGraphicsItem*)it->second;
        curr->setSelected(false);
    }

}

