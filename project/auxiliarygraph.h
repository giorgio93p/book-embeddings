#ifndef AUXILIARYGRAPH_H
#define AUXILIARYGRAPH_H
#include "graphs.h"


class BiconnectedComponent;

class AuxiliaryGraph  //public Graph
{
    BookEmbeddedGraph* originalGraph;
    BCTree bCTreeObj;
    const ogdf::Graph& g;
    ogdf::GraphAttributes attr;


    std::vector<BiconnectedComponent*> bCs;
    std::unordered_map<Node,BiconnectedComponent*> agN_to_bc;






public:
    AuxiliaryGraph(BookEmbeddedGraph*); //class constructor



    double getXcoord(const Node &v) const;
    double getYcoord(const Node &v) const;
    double getBoxWidth(const Node &v) const;
    double getBoxHeight(const Node &v) const;

    const std::vector<BiconnectedComponent*>& getBiconnectedComponents() const; //this returns a const reference to
                                                                                //the biconnected components vector.
    BiconnectedComponent* getBCOf(Node v);

    bool empty() const {
        return g.empty();
    }
    int numberOfNodes() const{
        return g.numberOfNodes();
    }
    int numberOfEdges() const{
        return g.numberOfEdges();
    }


    virtual Node firstNode() const{
        return g.firstNode();
    }

    virtual Edge firstEdge() const {
        return g.firstEdge();
    }

    ogdf::GraphAttributes getGraphAttributes() {
        return attr;
    }

    const ogdf::Graph& toOGDF() {
        return g;
    }

    void buildLayout(const double xmin, const double ymin, const double xmax, const double ymax);


    bool graphIsPlanar() const;


};

#endif // AUXILIARYGRAPH_H
