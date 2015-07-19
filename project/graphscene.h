#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"


#define LEN 250

class GraphScene : public QGraphicsScene
{
public:
    GraphScene(const BookEmbeddedGraph& g, const int page);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

private:
    std::unordered_map<Node, QGraphicsEllipseItem*> nodes;
    std::unordered_map<Edge, QGraphicsItem*> edges;
    int n;
    int m;


};

#endif // GRAPHSCENE_H
