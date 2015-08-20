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
    int page;

public:
    PageScene(const BookEmbeddedGraph& g, const int p, MainWindow *w, int width=250, int height=50);

    void addEdge(const Edge &e);
    void removeEdge(const Edge &e);
    void setPageNumber(int p);
private:
    std::unordered_map<Node, QGraphicsEllipseItem*> *nodes;
    std::unordered_map<Edge, embedding_edge*> *edges;
public slots:
    void on_remove_page_request(){emit remove_page(page);}
signals:
    void remove_page(int);
    void page_number_changed(int);
    void crossings_changed(int);
};


#endif
