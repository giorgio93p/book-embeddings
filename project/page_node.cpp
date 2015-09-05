#include "page_node.h"
#include "pagescene.h"
#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

PageNode::PageNode(PageScene *scene, const Node& v, int i, int n, qreal inv) : pageScene(scene), node(v), index(i), numNodes(n), interval(inv){
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
}

QVariant PageNode::itemChange(GraphicsItemChange change, const QVariant & value){
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        qreal distCovered = newPos.rx();

        if (distCovered < -index * interval - interval / 4) {
            newPos.setX(-index * interval - interval / 4);
        }else if (distCovered > (numNodes - index - 1) * interval + interval / 4) {
            newPos.setX((numNodes - index - 1) * interval + interval / 4);
        }

        newPos.setY(0);

        return newPos;
    } else if (change == QGraphicsItem::ItemSelectedChange) {
        if(value.toBool()) {
            emit this->was_selected(node,pageScene->pageNumber());
        }
        else
        {
            emit this->was_deselected(node);
        }
    }

    return QGraphicsEllipseItem::itemChange(change, value);
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

void PageNode::mouseReleaseEvent(QGraphicsSceneMouseEvent * event){
    if (nodeDragged == true){
        if (event->button() == Qt::LeftButton){
            QPointF finalCoords = (event->scenePos());
            qreal width = scene()->width();
            bool needToRedraw = (finalCoords.rx() < (-width + (index - 1) * interval)) ||
                                (finalCoords.rx() > (-width + (index + 1) * interval));
            //all pages need to be updated
            //so we should redraw the whole graph
            //with the new permutation
            if (needToRedraw){
                //calculate finalPos
                int finalPos = ceil((finalCoords.rx() + width) / interval);
                scene()->window()->getMainGraph()->updatePermutation(index, finalPos);

                scene()->window()->enableRedraw();
            }else{

            }
        }

        nodeDragged = false;
    }

    QGraphicsEllipseItem::mouseReleaseEvent(event);
}
