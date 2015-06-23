
#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H
using namespace std;
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <vector>
#include <QGraphicsPathItem>
#include <QMouseEvent>


#define LEN 250

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

#endif // GRAPHSCENE_H
