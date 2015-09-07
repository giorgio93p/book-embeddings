#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsView>
#include "pagescene.h"

class PageView : public QGraphicsView
{
    Q_OBJECT
    void scaleView(qreal scaleFactor);
public:
    PageView();
    PageScene *scene();
    #ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
    #endif
public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();
};

#endif // PAGEVIEW_H
