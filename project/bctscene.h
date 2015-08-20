#ifndef BCTSCENE_H
#define BCTSCENE_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
#include "embedding_edge.h"
#include "colors.h"

/*
 *
 * scene to display a bctree. Its constructor creates
 * all the nodes and edges in the form of QGraphicsItems.
 */

class BCTScene : public QGraphicsScene
{
public:
    BCTScene(const Graph& g, const double width=250.0, const double height=50.0);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

private:

    std::unordered_map<Node, QGraphicsEllipseItem*> nodes;
    std::unordered_map<Edge, QGraphicsItem*> edges;

};

#endif // BCTSCENE_H
