#ifndef PAGENODE_H
#define PAGENODE_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include "graphs.h"

class PageScene;

class PageNode : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    private:
        int index;
        int numNodes;
        qreal interval;
        bool nodeDragged;
        PageScene *pageScene;
        Node node;

    public:
        PageNode(PageScene*, const Node &v, int, int, qreal);
        QVariant itemChange(GraphicsItemChange, const QVariant&);
        PageScene* scene();
    protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent (QGraphicsSceneMouseEvent*);
signals:
        void was_selected(Node&,int);
        void was_deselected(Node&);

        void move(Node&,int);
};

#endif
