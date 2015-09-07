#ifndef NODE_GRAPHICS_H
#define NODE_GRAPHICS_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include "graphs.h"
#include "edge_graphics.h"


class GraphNode : public QObject, public QGraphicsEllipseItem{
    Q_OBJECT
    Node node;

    static const QPen defaultPen;
    static const QBrush defaultBrush;
    static const qreal defaultWidth;
    static const qreal defaultHeight;
    static const qreal highlightScalingFactor;
    static const QColor highlightColor;
    static const qreal zValue;
public:
    GraphNode(const Node& v);
    QVariant itemChange(GraphicsItemChange, const QVariant&);
    void toggleHighlight(bool enable);
signals:
    void was_selected(Node&,int);
    void was_deselected(Node&);
};

class PageScene;

class PageNode : public QObject, public QGraphicsEllipseItem{
        Q_OBJECT
        Node node;
        int position;
        bool nodeDragged;
        PageScene *pageScene;
        QGraphicsSimpleTextItem* label;
        std::unordered_set<PageEdge*> incidentEdges;
        std::vector<QPointF> *nodePositions;

        static const QPen defaultPen;
        static const QBrush defaultBrush;
        static const qreal defaultWidth;
        static const qreal defaultHeight;
        static const qreal labelOffset;
        static const qreal highlightScalingFactor;
        static const QColor highlightColor;
        static const qreal zValue;
public:
        PageNode(PageScene*, const Node &v, std::vector<QPointF> *positions);
        QVariant itemChange(GraphicsItemChange, const QVariant&);
        PageScene* scene();
        void addIncidentEdge(PageEdge* e);
        void removeIncidentEdge(PageEdge *e);
        void setPosition(int p);
        void resetPosition();
        void toggleHighlight(bool enable);
protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent (QGraphicsSceneMouseEvent*);
signals:
        void was_selected(Node&,int);
        void was_deselected(Node&);
        void was_dragged(Node&,int atPage, QPointF toPos);
        void move(Node&,int atPage);
private:
        void adjustIncidentEdges(QPointF newPosition);
};

#endif
