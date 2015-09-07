#ifndef PAGENODE_H
#define PAGENODE_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include "graphs.h"
#include "embedding_edge.h"

class PageScene;

class PageNode : public QObject, public QGraphicsEllipseItem {
        Q_OBJECT
        int position;
        bool nodeDragged;
        PageScene *pageScene;
        Node node;
        QGraphicsSimpleTextItem* label;
        std::unordered_set<embedding_edge*> incidentEdges;
        std::vector<QPointF> *nodePositions;


public:
        PageNode(PageScene*, const Node &v, std::vector<QPointF> *positions);
        QVariant itemChange(GraphicsItemChange, const QVariant&);
        PageScene* scene();
        void addIncidentEdge(embedding_edge* e);
        void removeIncidentEdge(embedding_edge *e);
        void setPosition(int p);
        void resetPosition();
protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent (QGraphicsSceneMouseEvent*);
signals:
        void was_selected(Node&,int);
        void was_deselected(Node&);

        void move(Node&,int);
private:
        void adjustIncidentEdges(QPointF newPosition);
};

#endif
