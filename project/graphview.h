#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H
#include <QGraphicsView>
#include "graphscene.h"

class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView();
    GraphView(QWidget* parent = 0);
    GraphScene *scene();
    #ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event);
    #endif
public slots:
    void zoomIn();
    void zoomOut();
private:
    void scaleView(qreal scaleFactor);
};

#endif // GRAPHVIEW_H
