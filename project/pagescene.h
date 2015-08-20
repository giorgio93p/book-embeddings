#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <vector>
#include <unordered_map>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
#include "embedding_edge.h"
#include "node_graphics_item.h"
#include "colors.h"
#include "mainwindow.h"

class PageScene : public QGraphicsScene
{
    Q_OBJECT
    MainWindow* window;
    QPen pen;

public:
    PageScene(const BookEmbeddedGraph& g, const int page, MainWindow *w, int width=250, int height=50);

    void addEdge(const Edge &e);
    void removeEdge(const Edge &e);
private:
    std::unordered_map<Node, QGraphicsEllipseItem*> *nodes;
    std::unordered_map<Edge, embedding_edge*> *edges;

};


#endif
