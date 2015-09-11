#include "edge_graphics.h"
#include <QMenu>
#include <QAction>
#include <QActionEvent>
#include <cmath>
#include "pagescene.h"

const qreal GraphEdge::defaultWidth = 2;
const QPen GraphEdge::highlightPen = QPen(QBrush(QColor(230,0,230)),4);
const qreal GraphEdge::zValue = 0;

GraphEdge::GraphEdge(QPointF sourceC, QPointF targetC, QColor col, const Edge &e){
    pen = QPen(QBrush(col),defaultWidth);
    this->setPen(pen);
    edge = e;
    highlighted = false;
    sourceCenter = sourceC;
    targetCenter = targetC;

    setFlag(QGraphicsItem::ItemIsSelectable);
    setLine(QLineF(sourceCenter,targetCenter));
    setZValue(zValue);
}

QVariant GraphEdge::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value){
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

void GraphEdge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    this->setSelected(true);
    QMenu menu;
    //QAction *removeAction = menu.addAction("Remove");
    QAction *moveAction = menu.addAction(tr("Move to Page"));
    connect(moveAction, SIGNAL(triggered()), this, SLOT(on_move_request()));
    QAction *selectedAction = menu.exec(event->screenPos());
}

void GraphEdge::toggleHighlight(bool enable){
    highlighted = enable;
    prepareGeometryChange();
    if(enable){
        setPen(highlightPen);
        setZValue(zValue+1);
    }else{
        setPen(pen);
        setZValue(zValue);
    }

}

void GraphEdge::adjust(Node &v, QPointF newPosition){
    if(v == edge->source()) sourceCenter = newPosition;
    if(v == edge->target()) targetCenter = newPosition;
    prepareGeometryChange();
    this->setLine(QLineF(sourceCenter, targetCenter));
}


const qreal PageEdge::defaultWidth = 2;
const QPen PageEdge::highlightPen = QPen(QBrush(QColor(230,0,230)),4);
const qreal PageEdge::zValue = 0;
const qreal PageEdge::dx = 3; //dx and dy are used to make clicking on edge simpler
const qreal PageEdge::dy = 6; //essentialy, we use shape() to draw two ellipses -which define the clickable area- around painterPath

void PageEdge::setVScalingFactor(const qreal &value){
    vScalingFactor = value;
}

void PageEdge::keyReleaseEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Escape:
    case Qt::Key_Enter:
        clearFocus();
        setSelected(false);
        break;
    case Qt::Key_Up: emit move(edge,(int)MoveDirection::Up); break;
    case Qt::Key_Down: emit move(edge,(int)MoveDirection::Down); break;
    case Qt::Key_0: emit move(edge,0); break;
    case Qt::Key_1: emit move(edge,1); break;
    case Qt::Key_2: emit move(edge,2); break;
    case Qt::Key_3: emit move(edge,3); break;
    case Qt::Key_4: emit move(edge,4); break;
    case Qt::Key_5: emit move(edge,5); break;
    case Qt::Key_6: emit move(edge,6); break;
    case Qt::Key_7: emit move(edge,7); break;
    case Qt::Key_8: emit move(edge,8); break;
    case Qt::Key_9: emit move(edge,9); break;
    default:
        QGraphicsItem::keyReleaseEvent(event);
        return;
    }
    //setFocus();
    //setSelected(true);
}

PageEdge::PageEdge(QPointF sourceC, QPointF targetC, QColor col, const Edge &e, qreal vScaling) {
    pen = QPen(QBrush(col),defaultWidth);
    sourceCenter = sourceC;
    targetCenter = targetC;
    edge = e;
    highlighted = false;
    vScalingFactor = vScaling;

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setZValue(zValue);

    adjustPainterPath();
    //std::cout << "Drawing edge " << (*e)->source() << "," << (*e)->target() << std::endl;
}

void PageEdge::adjust(Node& v, QPointF newPosition){
    if(v == edge->source()) sourceCenter = newPosition;
    if(v == edge->target()) targetCenter = newPosition;
    adjustPainterPath();
}

void PageEdge::adjustPainterPath(){
    const qreal left = qMin(mapFromScene(sourceCenter).x(),mapFromScene(targetCenter).x());
    const qreal right = qMax(mapFromScene(sourceCenter).x(),mapFromScene(targetCenter).x());
    const qreal width = right-left;
    const qreal height = width*vScalingFactor;

    prepareGeometryChange();
    painterPath = QPainterPath(QPointF(right,mapFromScene(sourceCenter).y()));
    painterPath.arcTo(left,-height,width,2*height,0,180);//The first two arguments are the coordinates of the top-left point.
}

void PageEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setBackgroundMode(Qt::TransparentMode);
    if(highlighted) painter->setPen(highlightPen);
    else painter->setPen(pen);
    painter->drawPath(painterPath);
    //painter->drawPath(shape());//for debugging
}

QRectF PageEdge::boundingRect() const{
    return painterPath.boundingRect();
}

QPainterPath PageEdge::shape() const{
    const qreal width = painterPath.boundingRect().width();
    const qreal height = painterPath.boundingRect().height();
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

QVariant PageEdge::itemChange(GraphicsItemChange change, const QVariant & value) {
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

void PageEdge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    this->setSelected(true);
    QMenu menu;
    //QAction *removeAction = menu.addAction("Remove");
    QAction *moveAction = menu.addAction(tr("Move to Page"));
    connect(moveAction, SIGNAL(triggered()), this, SLOT(on_move_request()));
    QAction *selectedAction = menu.exec(event->screenPos());
}

void PageEdge::toggleHighlight(bool enable){
    highlighted = enable;
    prepareGeometryChange();
    this->setZValue(enable ? zValue+1 : zValue);
}
