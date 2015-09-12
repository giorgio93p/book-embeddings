#ifndef BICONNECTEDCOMPONENT_H
#define BICONNECTEDCOMPONENT_H

#include "graphs.h"

#include <ogdf/basic/Graph_d.h> //TODO: delete if program works

class BiconnectedComponent : public BookEmbeddedGraph
{
public:
    BiconnectedComponent(ogdf::Graph& gr, BookEmbeddedGraph* mainGraph,
                         std::unordered_map<Node, Node> nMapping, std::unordered_map<Edge, Edge> eMapping);




private:

    BookEmbeddedGraph* wholeGraph;
    std::unordered_map<Edge,Edge> edgeMapping;

    std::unordered_map<Node,Node> nodeMapping;
    std::unordered_map<int,int> permutationMappng;
    //this is a maps the index of a maingraphnode in maingraph->permutation
    //to the index of the corresponding node of this bc in this permutation table.
    //i.e. permutationMapping.at(i) =j
    // mainGraph->permutation.at(i) ==
    //this.permutation.at(permutationMapping.at(i))
};

#endif // BICONNECTEDCOMPONENT_H

