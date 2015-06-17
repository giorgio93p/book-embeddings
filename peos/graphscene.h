
#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H
using namespace std;
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <vector>


#define LEN 250

typedef  pair<int,int> myPair;
typedef  pair < myPair, int> myEdge;

class GraphScene : public QGraphicsScene
{
public:
    GraphScene(int nn, int mm, std::vector< std::pair< int,int > > edgs, std::vector<int> permutation);



private:
    std::vector<QGraphicsEllipseItem*> nodes;
    vector< myEdge > *edges;
    int n;
    int m;




};

#endif // GRAPHSCENE_H
