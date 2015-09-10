#include "graphview.h"

GraphView::GraphView(QWidget *parent) : QGraphicsView(parent){
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void GraphView::scaleView(qreal scaleFactor){ //copied from elasticnodes example
    qreal factor = transform().scale(scaleFactor, 1).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphView::zoomIn(){
    scaleView(qreal(1.2));
}

void GraphView::zoomOut(){
    scaleView(1 / qreal(1.2));
}

GraphScene *GraphView::scene(){
    return (GraphScene*) QGraphicsView::scene();
}

#ifndef QT_NO_WHEELEVENT
//! [5]
void GraphView::wheelEvent(QWheelEvent *event){
    scaleView(pow((double)2, event->delta() / 240.0));//copied from elasticnodes example
}
//! [5]
#endif
