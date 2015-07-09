
#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H
using namespace std;
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <vector>
#include <QGraphicsPathItem>
#include <QMouseEvent>
#include "graphs.h"


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
    GraphScene(const BookEmbeddedGraph& g, const int page);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) Q_DECL_OVERRIDE;



private:
    std::unordered_map<QGraphicsEllipseItem*, Node> nodes;
    std::unordered_map<QGraphicsItem*, Edge> edges;
    int n;
    int m;


};

#endif // GRAPHSCENE_H
