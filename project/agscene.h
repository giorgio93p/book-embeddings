#ifndef AGSCENE_H
#define AGSCENE_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
#include "embedding_edge.h"
#include "colors.h"

/*
 * scene to display a bctree. Its constructor creates
 * all the nodes and edges in the form of QGraphicsItems.
 */

class AGNode;
class AuxiliaryGraph;
class MainWindow;
class AGScene : public QGraphicsScene
{
public:
    AGScene( AuxiliaryGraph& g,MainWindow* mainwindow,const double width=250.0, const double height=50.0);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

private:

    std::unordered_map<Node, AGNode*> nodes;
    std::unordered_map<Edge, QGraphicsItem*> edges;

};

#endif // AGSCENE_H
