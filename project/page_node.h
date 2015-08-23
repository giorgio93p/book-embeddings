#ifndef PAGENODE_H
#define PAGENODE_H

#include <QGraphicsEllipseItem>
#include <QPainter>

class PageScene;

class PageNode : public QGraphicsEllipseItem {
    private:
        int index;
        int numNodes;
        qreal interval;
        bool nodeDragged;
        PageScene *pageScene;

    public:
        PageNode(PageScene*, int, int, qreal);
        QVariant itemChange(GraphicsItemChange, const QVariant&);
        PageScene* scene();
    protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent (QGraphicsSceneMouseEvent*);
};

#endif
