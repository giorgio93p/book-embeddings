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
    QPainterPath *painterPath;
    QPen pen;
    Edge edge;

    public:
        embedding_edge(QPainterPath *, QPen, const Edge &e);

        QRectF boundingRect() const;
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        QPainterPath shape() const;
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
public slots:
        void on_move_request(){emit move(edge);}
signals:
        void was_selected(Edge&);
        void was_deselected(Edge&);

        void move(Edge&);
};

#endif
