#include "pageview.h"

PageView::PageView(){

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

void PageView::resetZoom(){
    scale(1,1);
}

PageScene *PageView::scene(){
    return (PageScene*) QGraphicsView::scene();
}

#ifndef QT_NO_WHEELEVENT
//! [5]
void PageView::wheelEvent(QWheelEvent *event){ //copied from elasticnodes example
    scaleView(pow((double)2, -event->delta() / 240.0));
}
//! [5]
#endif
