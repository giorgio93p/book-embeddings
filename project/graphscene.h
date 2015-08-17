#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
#include "embedding_edge.h"

class EmbeddedGraphScene : public QGraphicsScene
{
public:
    EmbeddedGraphScene(const BookEmbeddedGraph& g, const int page, const double width=250.0, const double height=50.0);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

private:
    std::unordered_map<Node, QGraphicsEllipseItem*> nodes;
    std::unordered_map<Edge, QGraphicsItem*> edges;
};

class GraphScene : public QGraphicsScene
{
public:
    GraphScene(const BookEmbeddedGraph &g, const double width=250.0, const double height=50.0);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

private:
    std::unordered_map<Node, QGraphicsEllipseItem*> nodes;
    std::unordered_map<Edge, QGraphicsItem*> edges;
};

#endif // GRAPHSCENE_H
