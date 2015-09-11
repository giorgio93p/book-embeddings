#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QGraphicsView>
#include "pagescene.h"

class PageView : public QGraphicsView
{
    Q_OBJECT
    BookEmbeddedGraph* graph;
    bool isBeingResized;
public:
    PageView(BookEmbeddedGraph *g);
    PageScene *scene();
    #ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
    #endif
public slots:
    void zoomIn();
    void zoomOut();
private:
    void scaleView(qreal scaleFactor);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#endif // PAGEVIEW_H
