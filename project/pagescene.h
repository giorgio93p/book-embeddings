#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <vector>
#include <unordered_map>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include "graphs.h"
#include "node_graphics.h"
#include <unordered_map>

class PageEdge;
class MainWindow;

class PageScene : public QGraphicsScene
{
    Q_OBJECT
    QColor colour;

    int page;

public:

    PageScene(const BookEmbeddedGraph& g, const int p, MainWindow *w, QColor col, QLabel* pageNumber, QLabel* crossings, QPushButton* del, int width=250, int height=50);
    int width();

    void deselectAll();

    void addEdge(const Edge &e);
    void removeEdge(const Edge &e);
    void setPageNumber(int p);
    int pageNumber(){
        return page;
    }
    QColor getColour() {
        return colour;
    }
    void repaintEdge(const Edge e);

    void redraw(BookEmbeddedGraph &g);
    void moveNode(Node &v, QPointF toPos);
    void highlightNode(Node &v, bool enable);
    void highlightEdge(Edge &v, bool enable);
private:
    std::unordered_map<Node, PageNode*> *nodes;
    std::unordered_map<Edge, PageEdge*> *edges;
    QRectF movementPath;
    MainWindow* mainWindow;
    QLabel* crossingsIndicator;
    QLabel* pageNumberIndicator;
    QPushButton* deletePageButton;
    std::vector<QPointF> nodePositions;

    int m_width;

public slots:
    void on_remove_page_request(){emit remove_page(page);}
    void setCrossings(int crossings);
signals:
    void remove_page(int);
};


#endif
