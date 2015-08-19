#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"
#include "embedding_edge.h"
#include "colors.h"
#include <unordered_map>



class PageScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PageScene(const BookEmbeddedGraph& g, const int page, int width=250, int height=50);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;

private:
    std::unordered_map<Node, QGraphicsEllipseItem*> nodes;
    std::unordered_map<Edge, embedding_edge*> *edges;

public slots:

    void createDialog(embedding_edge*);
};


#endif
