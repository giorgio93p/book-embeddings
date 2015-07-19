#ifndef EMBE_H
#define EMBE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QtDebug>

class embedding_edge : public QGraphicsItem {
    double height;
    qreal left;
    qreal right;
    QPainterPath *painterPath;
    QPen pen;

    public:
        embedding_edge(double, qreal, qreal, QPainterPath *, QPen);

        QRectF boundingRect() const;
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        QPainterPath shape() const;
        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);
};

#endif
