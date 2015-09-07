#include "pageview.h"

PageView::PageView(){
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}


void PageView::scaleView(qreal scaleFactor){ //copied from elasticnodes example
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void PageView::zoomIn(){
    scaleView(qreal(1.2));
}

void PageView::zoomOut(){
    scaleView(1 / qreal(1.2));
}

PageScene *PageView::scene(){
    return (PageScene*) QGraphicsView::scene();
}

#ifndef QT_NO_WHEELEVENT
//! [5]
void PageView::wheelEvent(QWheelEvent *event){
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scaleView(pow((double)2, event->delta() / 240.0));//copied from elasticnodes example
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}
//! [5]
#endif
