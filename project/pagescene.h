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
#include "edge_graphics.h"
#include <unordered_map>

class PageEdge;
class MainWindow;

class PageScene : public QGraphicsScene
{
    Q_OBJECT
    QColor colour;
    int page;
    std::unordered_map<Node, PageNode*> *nodes;
    std::unordered_map<Edge, PageEdge*> *edges;
    QRectF movementPath;
    MainWindow* mainWindow;
    QLabel* crossingsIndicator;
    QLabel* pageNumberIndicator;
    QPushButton* deletePageButton;
    std::vector<QPointF> nodePositions;
    qreal vScalingFactor;

    static const qreal margin;
    static const qreal minIntervalBetweenNodes;
    /**
     * @brief bottomMargin It is used as a margin and also to make space for node labels
     */
    static const qreal bottomMargin;

public:

    PageScene(const BookEmbeddedGraph& g,
              const int p, MainWindow *w, QColor col, QLabel* pageNumber,
              QLabel* crossings, QPushButton* del, int width=250, int height=50);

    PageScene(const BookEmbeddedGraph* g, const int p, MainWindow *w, QColor col, QLabel* pageNumber,
              QLabel* crossings, QPushButton* del, int width, int height);

    /**
     * @brief PageScene::setSize We adjust the positions of the nodes in the scene.
     *  At the ends, we leave a small margin and some space for node moving.
     *  Then, we divide the rest of the new width equally between the nodes.
     *  Then, we calculate how high the edges can go. Attention: the scene is not redrawn automatically.
     */
    void setSize(QSize newSize);
    int width();
    MainWindow* window();

    void deselectAllEdgesBut(Edge *e = NULL);
    void deselectAllNodesBut(Node *v = NULL);

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

    /**
     * @brief PageScene::redraw Repositions the nodes in the scene.
     * @param g The graph. From this we get the current permutation.
     * @return A rectangle which is guaranteed to include everything in the scene. We use in PageView, in order to avoid scrollbars when not needed.
     */
    QRectF redraw(BookEmbeddedGraph *g);

    void moveNode(Node &v, QPointF toPos);
    void highlightNode(Node &v, bool enable);
    void highlightEdge(Edge &v, bool enable);
public slots:
    void on_remove_page_request(){emit remove_page(page);}
    void setCrossings(int crossings);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void remove_page(int);
    void deselect_all();
};


#endif
