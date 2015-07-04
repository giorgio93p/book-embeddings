#include "graphscene.h"
#include <iostream>
using namespace std;

Colors::Colors(){
    pageColors = {Qt::red,Qt::green,Qt::blue,Qt::cyan,Qt::magenta,Qt::yellow,Qt::gray};
}

QColor& Colors::operator[](int i){
    return pageColors[i];
}

GraphScene::GraphScene(int nn, int mm, vector< pair<int,int> > edgs,vector<int> permutation)  :
    n(nn),m(mm)
{
    int i;
    nodes.resize(nn);
    edges = new vector<IntPair>(edgs);
    for (i=0;i<n;i++) {
        QBrush redBrush(Qt::red);
        QPen blackPen(Qt::black);
        blackPen.setWidth(2);

        //nodes.push_back(this->addEllipse(-LEN+i*(2*LEN/n),0,12,12,blackPen,redBrush));
        QGraphicsEllipseItem* el = this->addEllipse(-LEN+i*(2*LEN/n),0,12,12,blackPen,redBrush);

        //nodes[i]->setMovable();
        el->setFlag(QGraphicsItem::ItemIsSelectable, true);
        nodes[i]=el;

    }

    printf("%d is the number of edges here\n",m);

    for (i=0; i<mm;i++) {

        //QColor c=Qt::red;

        QPen blackPen(Qt::black);
        blackPen.setWidth(2);

        qreal interval=(2*LEN/nn); //space between two consequent vertices


        qreal x1 = 6 + ((qreal)edges->at(i).first) * interval-LEN;
        qreal x2 = 6 + ((qreal)edges->at(i).second) * interval-LEN;


        double vscalingfactor = 0.7;
        double height = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-height,(x2-x1),2*height,0,180);

        blackPen.setColor(Qt::black);


        QGraphicsItem * path = this->addPath(*edg,blackPen);
        path->setFlag(QGraphicsItem::ItemIsSelectable, true);




    }



}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    this->addText("καλημέρα !!!");

    //this->repaint();

}

