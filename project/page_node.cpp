#include "page_node.h"
#include "pagescene.h"
#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

PageNode::PageNode(PageScene *scene, const Node& v, std::vector<QPointF>* positions) : pageScene(scene), node(v), nodePositions(positions){
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    position = v->index();
    nodeDragged = false;
    incidentEdges = std::unordered_set<embedding_edge*>();

    label = pageScene->addSimpleText(QString::number(v->index()));

    setZValue(1);
}

QVariant PageNode::itemChange(GraphicsItemChange change, const QVariant & value){
    if (change == ItemPositionChange && !nodeDragged && scene()) { //if node position was changed from the code
        QPointF newPosition = value.toPointF();
        label->setPos(QPointF(newPosition.x(), pos().y()+15));
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

        label->setPos(QPointF(newPos.x(), newPos.y()+15));
        adjustIncidentEdges(newPos);
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
    for(embedding_edge *e : incidentEdges){
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

void PageNode::addIncidentEdge(embedding_edge *e){
    incidentEdges.insert(e);
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
        }

    }

    QGraphicsEllipseItem::mouseReleaseEvent(event);
}
