#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsView>
#include "pagescene.h"

class PageView : public QGraphicsView
{
    Q_OBJECT
public:
    PageView();
    PageScene *scene();
    #ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
    #endif
public slots:
    void zoomIn();
    void zoomOut();
private:
    void scaleView(qreal scaleFactor);
};

#endif // PAGEVIEW_H
