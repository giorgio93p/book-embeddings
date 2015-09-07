#ifndef EDGE_GRAPHICS_H
#define EDGE_GRAPHICS_H

#include <QPainter>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QtDebug>
#include <QObject>
#include "graphs.h"



class GraphEdge : public QObject, public QGraphicsLineItem{
    Q_OBJECT
    Edge edge;
    QPen pen;
    bool highlighted;

    static const qreal defaultWidth;
    static const QPen highlightPen;
    static const qreal zValue;
public:
    GraphEdge(QPointF sourceC, QPointF targetC, QColor col, const Edge &e);
    QVariant itemChange(GraphicsItemChange, const QVariant&);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void toggleHighlight(bool enable);
public slots:
    void on_move_request(){emit move(edge);}
signals:
    void was_selected(Edge&);
    void was_deselected(Edge&);
    void move(Edge&);
};

class PageEdge : public QObject, public QGraphicsItem
{
    Q_OBJECT
    QPainterPath painterPath;
    QPointF sourceCenter;
    QPointF targetCenter;
    //sourceCenter and targetCenter must have the same y coordinate
    QPen pen;
    Edge edge;
    bool highlighted;

    static const qreal defaultWidth;
    static const qreal vScalingFactor;
    static const qreal dx;
    static const qreal dy;
    static const QPen highlightPen;
    static const qreal zValue;
public:
        PageEdge(QPointF sourceC, QPointF targetC, QColor col, const Edge &e);

        QRectF boundingRect() const;
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        QPainterPath shape() const;
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        void adjust(Node &v, QPointF newPosition);

        void toggleHighlight(bool enable);
public slots:
        void on_move_request(){emit move(edge);}
protected:
signals:
        void was_selected(Edge&);
        void was_deselected(Edge&);

        void move(Edge&);
private:
        void adjustPainterPath();
};

#endif
