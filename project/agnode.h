#ifndef AGNODE_H
#define AGNODE_H
#include <QGraphicsEllipseItem>
#include <QObject>
#include "graphs.h"

class BiconnectedComponent;

class AGNode : public QObject, public QGraphicsEllipseItem
{

    Q_OBJECT

public:
    AGNode(Node n,BiconnectedComponent* b);
private:
    Node node;
    BiconnectedComponent* bc;
protected:

    void mousePressEvent(QGraphicsSceneMouseEvent*);

signals:

    void was_selected(BiconnectedComponent*);


};


#endif // AGNODE_H
