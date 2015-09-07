#include "node_graphics.h"
#include "pagescene.h"
#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

const QPen GraphNode::defaultPen = QPen(QBrush(Qt::black),2);
const QBrush GraphNode::defaultBrush = QBrush(Qt::black);
const qreal GraphNode::defaultWidth = 6;
const qreal GraphNode::defaultHeight = 6;
const qreal GraphNode::highlightScalingFactor = 1.2;
const QColor GraphNode::highlightColor = QColor(230,0,230);
const qreal GraphNode::zValue = 1;

GraphNode::GraphNode(const Node& v){
    node = v;

    setFlags(QGraphicsItem::ItemIsSelectable);
    setRect(0,0,defaultWidth,defaultHeight);

    setBrush(defaultBrush);
    setPen(defaultPen);

    setZValue(zValue);
}

QVariant GraphNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedChange) {
            if(value.toBool()) {
                emit this->was_selected(node,-1);
            }else{
                emit this->was_deselected(node);
            }
        }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void GraphNode::toggleHighlight(bool enable){
    prepareGeometryChange();
    QPen p = QPen(this->pen());
    QBrush b = QBrush(this->brush());
    this->setTransformOriginPoint(this->boundingRect().center());
    if(enable){
        p.setColor(highlightColor);
        b.setColor(highlightColor);
        this->setScale(highlightScalingFactor);
    } else {
        p.setColor(defaultPen.color());
        b.setColor(defaultBrush.color());
        this->setScale(1);
    }
    this->setPen(p);
    this->setBrush(b);
}

const QPen PageNode::defaultPen = QPen(QBrush(Qt::black),2);
const QBrush PageNode::defaultBrush = QBrush(Qt::red);
const qreal PageNode::defaultWidth = 12;
const qreal PageNode::defaultHeight = 12;
const qreal PageNode::labelOffset = 15;
const qreal PageNode::highlightScalingFactor = 1.1;
const QColor PageNode::highlightColor = QColor(230,0,230);
const qreal PageNode::zValue = 1;

PageNode::PageNode(PageScene *scene, const Node& v, std::vector<QPointF>* positions) : node(v), pageScene(scene), nodePositions(positions){
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    nodeDragged = false;
    position = v->index();
    incidentEdges = std::unordered_set<PageEdge*>();

    label = scene->addSimpleText(QString::number(v->index()));
    label->setPos(QPointF(nodePositions->at(position).x(), nodePositions->at(position).y()+labelOffset));

    setBrush(defaultBrush);
    setPen(defaultPen);
    setRect(0,0,defaultWidth,defaultHeight);

    setZValue(zValue);
}

QVariant PageNode::itemChange(GraphicsItemChange change, const QVariant & value){
    if (change == ItemPositionChange && !nodeDragged && scene()) { //if node position was changed from the code
        QPointF newPosition = value.toPointF();
        label->setPos(QPointF(newPosition.x(), pos().y()+labelOffset));
        adjustIncidentEdges(newPosition);
    }else if (change == ItemPositionChange && nodeDragged && scene()) { //if node position was changed by dragging
        QPointF newPos = value.toPointF();
        newPos.setY(pos().y()); //we leave the y coordinate unchanged

        const qreal lastNodeX = nodePositions->back().x();
        const qreal firstNodeX = nodePositions->front().x();
        const qreal margin = (lastNodeX-firstNodeX)/(nodePositions->size()-1)/2; //this is the maximum number of pixels past the ends that we can drag the node
                                                                                //it is intented to be half the interval between two nodes
        if(newPos.x() < firstNodeX - margin){//if the node has been dragged too much to the left
            newPos.setX(firstNodeX - margin);
        } else if(newPos.x() > lastNodeX + margin){//if the node has been dragged too much to the right
            newPos.setX(lastNodeX + margin);
        }

        label->setPos(QPointF(newPos.x(), newPos.y()+labelOffset));
        adjustIncidentEdges(newPos);
        emit was_dragged(node,pageScene->pageNumber(),newPos);
        return newPos;
    } else if (change == QGraphicsItem::ItemSelectedChange) {
        if(value.toBool()) {
            emit this->was_selected(node,pageScene->pageNumber());
        }else{
            emit this->was_deselected(node);
        }
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}

void PageNode::adjustIncidentEdges(QPointF newPosition){
    for(PageEdge *e : incidentEdges){
        e->adjust(node,newPosition + QPointF(this->boundingRect().width()/2,this->boundingRect().height()/2));
    }
}

void PageNode::setPosition(int p){
    position = p;
    setPos(nodePositions->at(position));
}

void PageNode::resetPosition(){
    setPosition(position);
}

void PageNode::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsEllipseItem::mousePressEvent(event);
}

void PageNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    nodeDragged = true;
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

PageScene* PageNode::scene() {
    return pageScene;
}

void PageNode::addIncidentEdge(PageEdge *e){
    incidentEdges.insert(e);
}

void PageNode::removeIncidentEdge(PageEdge *e){
    incidentEdges.erase(e);
}

void PageNode::mouseReleaseEvent(QGraphicsSceneMouseEvent * event){
    if (nodeDragged == true){
        nodeDragged = false;
        if (event->button() == Qt::LeftButton){
            //std::cout << "pos() " << this->pos().x() << "," << this->pos().y() << std::endl;
            //std::cout << "event->scenePos() " << event->scenePos().x() << "," << event->scenePos().y() << std::endl;
            const qreal finalX = this->pos().x();
            //check if node was moved past another node to the left
            for(int i=0; i<position; i++){
                //std::cout << "position " << i << " at " << nodePositions->at(i).x() << std::endl;
                if(finalX < nodePositions->at(i).x()){
                    emit move(node, i);
                    return;
                }
            }
            //check if node was moved past another node to the right
            for(int i=nodePositions->size()-1; i>position; i--){
                //std::cout << "position " << i << " at " << nodePositions->at(i).x() << std::endl;
                if(finalX > nodePositions->at(i).x()){
                    emit move(node, i);
                    return;
                }
            }
            //execution goes here if node was not moved past another node
            resetPosition();
            emit was_dragged(node,pageScene->pageNumber(),pos());
        }

    }

    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void PageNode::toggleHighlight(bool enable){
    prepareGeometryChange();
    QPen p = QPen(this->pen());
    QBrush b = QBrush(this->brush());
    this->setTransformOriginPoint(this->boundingRect().center());
    if(enable){
        p.setColor(highlightColor);
        b.setColor(highlightColor);
        this->setScale(highlightScalingFactor);
    } else {
        p.setColor(defaultPen.color());
        b.setColor(defaultBrush.color());
        this->setScale(1);
    }
    this->setPen(p);
    this->setBrush(b);
}
