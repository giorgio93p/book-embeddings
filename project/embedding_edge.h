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

    double height;
    qreal left;
    qreal right;
    QPainterPath *painterPath;
    QPen pen;
    Edge edge;
    QMainWindow *window;

    public:
        embedding_edge(double, qreal, qreal, QPainterPath *, QPen, const Edge &e, QMainWindow *w);

        QRectF boundingRect() const;
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        QPainterPath shape() const;
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
public slots:
        void moveDialog();
signals:
        void was_selected(Edge&);
        void was_deselected(Edge&);

        void move_to_page(Edge&, int);
};

#endif
