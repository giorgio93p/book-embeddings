#include "embedding_edge.h"
#include <QMenu>
#include <QAction>
#include <QActionEvent>
#include <cmath>
#include "pagescene.h"

#define VSCALINGFACTOR 0.7

embedding_edge::embedding_edge(QPointF sourceC, QPointF targetC, QPen p, const Edge &e) {
    pen = p;
    sourceCenter = sourceC;
    targetCenter = targetC;
    edge = e;

    setFlag(QGraphicsItem::ItemClipsToShape);
    setZValue(0);

    adjustPainterPath();
    //std::cout << "Drawing edge " << (*e)->source() << "," << (*e)->target() << std::endl;
}

void embedding_edge::adjust(Node& v, QPointF newPosition){
    if(v == edge->source()) sourceCenter = newPosition;
    if(v == edge->target()) targetCenter = newPosition;
    adjustPainterPath();
}

void embedding_edge::adjustPainterPath(){
    const qreal left = qMin(mapFromScene(sourceCenter).x(),mapFromScene(targetCenter).x());
    const qreal right = qMax(mapFromScene(sourceCenter).x(),mapFromScene(targetCenter).x());
    const qreal width = right-left;
    const qreal height = width*VSCALINGFACTOR;

    prepareGeometryChange();
    painterPath = QPainterPath(QPointF(right,mapFromScene(sourceCenter).y()));
    painterPath.arcTo(left,-height/2,width,height,0,180);//The first two arguments are the corrdinates of the top-left point.
}

void embedding_edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setBackgroundMode(Qt::TransparentMode);
    painter->setPen(pen);
    painter->drawPath(painterPath);
    //painter->drawPath(shape());//for debugging
}

QRectF embedding_edge::boundingRect() const{
    return painterPath.boundingRect();
}

QPainterPath embedding_edge::shape() const{
    const qreal width = painterPath.boundingRect().width();
    const qreal height = painterPath.boundingRect().height();
    const qreal dx = 3; //dx and dy are used to make clicking on edge simpler
    const qreal dy = 6; //essentialy, we draw two ellipses -which define the clickable area- around painterPath
    const QPointF center = QPointF(painterPath.boundingRect().center().x(),painterPath.boundingRect().bottom());

    //instead of ellipses, we will draw line segments (easier to calculate coordinates)
    const int intermediatePoints = 8;
    QPointF currPoint = QPointF(painterPath.boundingRect().right(),painterPath.boundingRect().bottom());
    QPainterPath shape = QPainterPath(currPoint);
    for(int i=1; i<=intermediatePoints; i++){//first the inner one
        currPoint = center + QPointF(std::cos(i*M_PI/intermediatePoints)*(width-dx)/2,-std::sin(i*M_PI/intermediatePoints)*(height-dy));
        shape.lineTo(currPoint);
    }
    for(int i=intermediatePoints-1; i>=0; i--){//and then the outer one
        currPoint = center + QPointF(std::cos(i*M_PI/intermediatePoints)*(width+dx)/2,-std::sin(i*M_PI/intermediatePoints)*(height+dy));
        shape.lineTo(currPoint);
    }

    return shape;
}

QVariant embedding_edge::itemChange(GraphicsItemChange change, const QVariant & value) {
    if (change == QGraphicsItem::ItemSelectedChange) {
        if(value.toBool()) {
            emit was_selected(edge);
        }
        else
        {
            emit was_deselected(edge);
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void embedding_edge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    //emit was_selected(edge);
    QMenu menu;
    //QAction *removeAction = menu.addAction("Remove");
    QAction *moveAction = menu.addAction(tr("Move to Page"));
    connect(moveAction, SIGNAL(triggered()), this, SLOT(on_move_request()));
    QAction *selectedAction = menu.exec(event->screenPos());
}
