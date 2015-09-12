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
    QPointF sourceCenter;
    QPointF targetCenter;
    /**
     * @brief ctrl_pressed records when the control key is pressed, so that we can successfully select multiple edges
     */
    bool ctrl_pressed;

    static const qreal defaultWidth;
    static const QPen highlightPen;
    static const qreal zValue;
public:
    GraphEdge(QPointF sourceC, QPointF targetC, QColor col, const Edge &e);
    QVariant itemChange(GraphicsItemChange, const QVariant&);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void toggleHighlight(bool enable);
    void adjust(Node &v, QPointF newPosition);
public slots:
    void on_move_request(){emit move(edge);}
signals:
    void was_selected(Edge&,bool);
    void was_deselected(Edge&);
    void move(Edge&);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
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
    qreal vScalingFactor;
    /**
     * @brief ctrl_pressed records when the control key is pressed, so that we can successfully select multiple edges
     */
    bool ctrl_pressed;

    static const qreal defaultWidth;
    static const qreal dx;
    static const qreal dy;
    static const QPen highlightPen;
    static const qreal zValue;
public:
        PageEdge(QPointF sourceC, QPointF targetC, QColor col, const Edge &e, qreal vScaling);

        QRectF boundingRect() const;
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        QPainterPath shape() const;
        QVariant itemChange(GraphicsItemChange change, const QVariant & value);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        void adjust(Node &v, QPointF newPosition);

        void toggleHighlight(bool enable);
        void setVScalingFactor(const qreal &value);

        /**
         * @brief The MoveDirection enum is used by MainWindow when we try to move an edge from a page to another.
         *
         * Up means that we want to move it to the previous page
         * Down means that we want to move it to the next page
         * Unknown causes the MainWindow to try and specify which edge we are trying to move
         *
         * It is important that their codes are negative integers, so that we can use non-negative integers for actual page numbers.
         */
        enum struct MoveDirection : int {Unknown = -1, Up = -2, Down = -3};

public slots:
        void on_move_request(){emit move(edge,(int)MoveDirection::Unknown);}
protected:
        void mousePressEvent(QGraphicsSceneMouseEvent * event);
        void keyReleaseEvent(QKeyEvent * event);
signals:
        void was_selected(Edge&,bool);
        void was_deselected(Edge&);

        void move(Edge&,int);
private:
        void adjustPainterPath();
};

#endif
