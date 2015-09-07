#ifndef EMBE_H
#define EMBE_H

#include <QPainter>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QtDebug>
#include <QObject>
#include "graphs.h"




class embedding_edge : public QObject, public QGraphicsItem
{

    Q_OBJECT
    QPainterPath painterPath;
    QPointF sourceCenter;
    QPointF targetCenter;
    //sourceCenter and targetCenter must have the same y coordinate
    QPen pen;
    Edge edge;

public:
        embedding_edge(QPointF sourceC, QPointF targetC, QPen, const Edge &e);

        QRectF boundingRect() const;
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        QPainterPath shape() const;
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        void adjust(Node &v, QPointF newPosition);

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
