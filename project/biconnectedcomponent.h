#ifndef BICONNECTEDCOMPONENT_H
#define BICONNECTEDCOMPONENT_H

#include "graphs.h"

#include <ogdf/basic/Graph_d.h> //TODO: delete if program works

class BiconnectedComponent : public BookEmbeddedGraph
{
public:
    BiconnectedComponent(ogdf::Graph& gr, BookEmbeddedGraph* mainGraph,
                         std::unordered_map<Node,Node> nMapping, std::unordered_map<Edge,Edge> eMapping);




private:

    BookEmbeddedGraph* wholeGraph;
    std::unordered_map<Edge,Edge> edgeMapping;

    std::unordered_map<Node,Node> nodeMapping;
};

#endif // BICONNECTEDCOMPONENT_H

