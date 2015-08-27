#ifndef BICONNECTEDCOMPONENT_H
#define BICONNECTEDCOMPONENT_H

#include "graphs.h"

#include <ogdf/basic/Graph_d.h> //TODO: delete if program works

class BiconnectedComponent : public BookEmbeddedGraph
{
public:
    BiconnectedComponent(ogdf::Graph gr, BookEmbeddedGraph* mainGraph,
                         ogdf::NodeArray<Node> nMapping, ogdf::EdgeArray<Edge> eMapping) :
                         BookEmbeddedGraph(gr), wholeGraph(mainGraph), edgeMapping(eMapping),
                         nodeMapping(nMapping){};

private:

    BookEmbeddedGraph* wholeGraph;
    ogdf::EdgeArray<Edge> edgeMapping;

    ogdf::NodeArray<Node> nodeMapping;
};

#endif // BICONNECTEDCOMPONENT_H

