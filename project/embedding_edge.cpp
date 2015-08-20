#include "embedding_edge.h"
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QActionEvent>

/**
 * n1 <= n2
 */
embedding_edge::embedding_edge(double h, qreal n1, qreal n2, QPainterPath *path, QPen p, const Edge& e, QMainWindow *w) {
    height = h;

    left = n1;
    right = n2;

    painterPath = path;
    pen = p;

    edge = e;
    window = w;
    //std::cout << "Drawing edge " << (*e)->source() << "," << (*e)->target() << std::endl;
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

QVariant embedding_edge::itemChange(GraphicsItemChange change, const QVariant & value) {
    if (change == QGraphicsItem::ItemSelectedChange) {
        if(value.toBool()) emit was_selected(edge);
        else emit was_deselected(edge);
    }

    return QGraphicsItem::itemChange(change, value);
}

void embedding_edge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    //emit was_selected(edge);
    QMenu menu;
    //QAction *removeAction = menu.addAction("Remove");
    QAction *moveAction = menu.addAction(tr("Move to Page"));
    connect(moveAction, SIGNAL(triggered()), this, SLOT(moveDialog()));
    QAction *selectedAction = menu.exec(event->screenPos());
}

void embedding_edge::moveDialog(){
    bool ok;
    int newPage = QInputDialog::getInt(window, tr("Move edge to page"),
                                         tr("New page:"),0,0,2147483647,1,&ok);
    if (ok) emit move_to_page(edge, newPage);
}
