#include "agnode.h"
#include "biconnectedcomponent.h"

AGNode::AGNode(Node n,BiconnectedComponent* b)
{

    node = n;

    bc=b;

}

void AGNode::mousePressEvent(QGraphicsSceneMouseEvent* e){

    emit was_selected(bc);
}
