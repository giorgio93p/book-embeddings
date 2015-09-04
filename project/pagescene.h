#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <vector>
#include <unordered_map>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
//#include "node_graphics_item.h"
#include "colors.h"
#include "page_node.h"
#include <unordered_map>

class embedding_edge;
class MainWindow;

class PageScene : public QGraphicsScene
{
    Q_OBJECT
    QColor colour;

    QPen pen;
    int page;

public:
    PageScene(BookEmbeddedGraph& g, const int p, MainWindow *w, QColor col, int width=250, int height=50);
    int width();
    MainWindow* window();

    void deselectAll();

    void addEdge(const Edge &e);
    void removeEdge(const Edge &e);
    void setPageNumber(int p);
    QColor getColour() {
        return colour;
    }
    void repaintEdge(const Edge e);

private:
    std::unordered_map<Node, PageNode*> *nodes;
    std::unordered_map<Edge, embedding_edge*> *edges;
    QRectF movementPath;
    MainWindow* mainWindow;

    int m_width;

public slots:
    void on_remove_page_request(){emit remove_page(page);}
signals:
    void remove_page(int);
    void page_number_changed(int);
    void crossings_changed(int);
};


#endif
