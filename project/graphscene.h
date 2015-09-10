#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <vector>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
#include "edge_graphics.h"
#include "node_graphics.h"
class MainWindow;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphScene(BookEmbeddedGraph *g, MainWindow* w, const double width=250.0, const double height=50.0);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

    void addEdgeInitial(const Edge &e, const int page);
    void addEdge(const Edge &e, const QColor);
    void removeEdge(const Edge &e);

    void addNode(const Node &, QPointF position);
    void removeNode(const Node &v);
    void highlightNode(const Node &v, bool enable);
    void deselectAll();
    void highlightEdge(const Edge &e, bool enable);
private:
    std::unordered_map<Node, GraphNode*> *nodes;
    std::unordered_map<Edge, GraphEdge*> *edges;
    MainWindow* mainWindow;
};

#endif // GRAPHSCENE_H
