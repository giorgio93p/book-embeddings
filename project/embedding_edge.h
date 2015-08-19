#ifndef EMBE_H
#define EMBE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QtDebug>
#include "graphs.h"
#include <QObject>

class embedding_edge : public QObject, public QGraphicsItem
{

    Q_OBJECT

    double height;
    qreal left;
    qreal right;
    QPainterPath *painterPath;
    QPen pen;
    Edge* edge;

    public:
        embedding_edge(double, qreal, qreal, QPainterPath *, QPen);

        QRectF boundingRect() const;
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        QPainterPath shape() const;
        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    signals:

        int was_clicked(embedding_edge*);

};

#endif
