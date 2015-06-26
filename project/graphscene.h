
#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H
using namespace std;
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <vector>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include <Kernel/graph/graph_draw.h>
#include "../graphs.h"


#define LEN 250

class Colors {
    vector<QColor> pageColors;

  public:
    Colors();
    QColor& operator[](int i);
};

typedef  pair<int,int> IntPair;

class GraphScene : public QGraphicsScene
{
public:
    GraphScene(int nn, int mm, std::vector< std::pair< int,int > > edgs, std::vector<int> permutation);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;



private:
    std::vector<QGraphicsEllipseItem*> nodes;
    vector< IntPair > *edges;
    int n;
    int m;




};

class SimpleGraphScene : public QGraphicsScene{
    std::vector<QGraphicsEllipseItem*> nodes;

  public:
    void drawGraph(Graph g);
};


#endif // GRAPHSCENE_H
