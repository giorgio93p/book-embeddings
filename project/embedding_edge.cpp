#include "embedding_edge.h"

/**
 * n1 <= n2
 */
embedding_edge::embedding_edge(double h, qreal n1, qreal n2, QPainterPath *path, QPen p) {
    height = h;

    left = n1;
    right = n2;

    painterPath = path;
    pen = p;
}

QRectF embedding_edge::boundingRect() const {
    qreal width = right - left;

    QRectF bounding =  QRectF(left + 0.45 * width, -height, 0.1 * width, 0.6 * height);/*QRectF(left, -height, width, height);*/
    return bounding;
}

void embedding_edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(pen);
    painter->drawPath(*painterPath);
}

QPainterPath embedding_edge::shape() const{
    qreal width = right - left;

    //NOTE:this is the _original_ bounding rect... should we change it?
    QRectF rect= QRectF(left, -height, width, height);

    //NOTE:enable the following line with the clipsToShape flag enabled to see
    //the area of the bounding rect used for selection
    //QRectF rect= QRectF(left + 0.45 * width, -height, 0.1 * width, 0.6 * height);

    QPainterPath path;
    path.addRect(rect);

    return path;
}

void embedding_edge::mousePressEvent(QGraphicsSceneMouseEvent *e){
    qDebug() << "An edge was clicked";
    QGraphicsItem::mousePressEvent(e);
}

QVariant embedding_edge::itemChange(GraphicsItemChange change, const QVariant & value) {
    if (change == QGraphicsItem::ItemSelectedChange) {
        qDebug() << "Selected embedding edge change";
    }

    return QGraphicsItem::itemChange(change, value);
}

