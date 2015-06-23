#ifndef MYPATH_H
#define MYPATH_H
#include <QGraphicsPathItem>



class MyPath : QGraphicsPathItem
{
public:
    MyPath(QGraphicsItem * parent);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);


private:
    QPolygonF selectionPolygon;
    void createSelectionPolygon();

};


#endif // MYPATH_H
