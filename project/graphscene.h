#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <vector>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
#include "embedding_edge.h"

#define DEFAULT_NODE_WIDTH 4

class MainWindow;

class GraphScene : public QGraphicsScene
{
public:
    GraphScene(BookEmbeddedGraph &g,MainWindow* w, const double width=250.0, const double height=50.0);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

    void addEdgeInitial(const Edge &e, const int page);
    void addEdge(const Edge &e, const QColor, const int width = 2);
    void removeEdge(const Edge &e);
    void changeEdgeColourAndWidth(const Edge &e, const QColor col, const int width = 2);

    void addNode(const Node &, QRectF boundingRect, const QColor col=Qt::black);
    void removeNode(const Node &v);
    void changeNodeColourAndWidth(const Node& v, const QColor col=Qt::black, const int width = DEFAULT_NODE_WIDTH);
private:
    std::unordered_map<Node, QGraphicsEllipseItem*> *nodes;
    std::unordered_map<Edge, QGraphicsItem*> *edges;
    MainWindow* mainWindow;


};

#endif // GRAPHSCENE_H
