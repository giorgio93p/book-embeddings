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
#include "colors.h"

class GraphScene : public QGraphicsScene
{
public:
    GraphScene(const BookEmbeddedGraph &g, const double width=250.0, const double height=50.0);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

    void addEdgeInitial(const Edge &e, const int page);
    void addEdge(const Edge &e, const QColor);

    void removeEdge(const Edge &e);

private:
    std::unordered_map<Node, QGraphicsEllipseItem*> *nodes;
    std::unordered_map<Edge, QGraphicsItem*> *edges;

};

#endif // GRAPHSCENE_H
