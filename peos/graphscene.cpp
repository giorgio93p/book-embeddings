#include "graphscene.h"
#include <stdio.h>
using namespace std;


GraphScene::GraphScene(int nn, int mm, vector< pair<int,int> > edgs,vector<int> permutation)  :
    n(nn),m(mm)
{
    int i;
    nodes.resize(nn);
    edges = new vector<myEdge>();
    for (i=0;i<n;i++) {
        QBrush redBrush(Qt::red);
        QPen blackPen(Qt::yellow);
        blackPen.setWidth(2);

        //nodes.push_back(this->addEllipse(-LEN+i*(2*LEN/n),0,12,12,blackPen,redBrush));
        QGraphicsEllipseItem* el = this->addEllipse(-LEN+i*(2*LEN/n),0,12,12,blackPen,redBrush);

        //nodes[i]->setMovable();
        el->setFlag(QGraphicsItem::ItemIsSelectable, true);

    }


    printf("%d is the number of edges here\n",m);

    myPair p1(0,1);
    myEdge e1(p1,1);

    myPair p2(0,2);
    myEdge e2(p2,0);


    edges->push_back(e1);
    edges->push_back(e2);
    edges->push_back(myEdge ( myPair(2,3),2)  );
    edges->push_back(myEdge ( myPair(2,4),1)  );
    edges->push_back(myEdge ( myPair(1,4),0)  );

    printf("e1 is an edge between nodes numbered %d and %d\n",edges->at(0).first.first,edges->at(0).first.second);




    qreal toplevel = -150;
    qreal levelQuantum = -50;



    for (i=0; i<mm;i++) {

        //QColor c=Qt::red;

        QPen blackPen(Qt::black);
        blackPen.setWidth(6);

        qreal interval=(2*LEN/nn); //space between two consequent vertices


        qreal x1 = 6 + ((qreal)edges->at(i).first.first) * interval-LEN;
        qreal x2 = 6 + ((qreal)edges->at(i).first.second) * interval-LEN;


        double vscalingfactor = 0.7;
        double height = ((x2-x1)/2)*vscalingfactor;
        QPainterPath* edg = new QPainterPath();
        edg->moveTo(x2,0);
        edg->arcTo(x1,-height,(x2-x1),2*height,0,180);


        //QGraphicsPathItem* myPath = new QGraphicsPathItem(edg,this);



        blackPen.setColor(Qt::black);


        QGraphicsItem * path = this->addPath(*edg,blackPen);
        path->setFlag(QGraphicsItem::ItemIsSelectable, true);




    }



}
