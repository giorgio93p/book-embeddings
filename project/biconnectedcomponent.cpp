#include "biconnectedcomponent.h"

BiconnectedComponent::BiconnectedComponent(ogdf::Graph& gr, BookEmbeddedGraph* mainGraph,
                     std::unordered_map<Node, Node> nMapping, std::unordered_map<Edge, Edge> eMapping) :
                     //Graph(gr,true),
                     BookEmbeddedGraph(gr,true), wholeGraph(mainGraph), edgeMapping(eMapping),
                     nodeMapping(nMapping)
                   //latestchange2

{
    cout << "DEBUG: entering biconnected component constructor body" << endl;



    //we shall set the node and edge numbering according to the numbers of the original graph.




    Node v;
    ogdf::Graph& g=(use_g2) ? g2: g1;

    std::unordered_map<Node,int> n_to_i;
    std::unordered_map<int,Node> i_to_n;
    std::unordered_map<Edge,int> e_to_i;
    std::unordered_map<int,Edge> i_to_e;


    forall_nodes(v,g) {


        Node corresponding = nodeMapping.at(v); //getting the corresponding node. if this fails,
                                                // then nodes in g are other than the



        int num = mainGraph->getNumberOf(corresponding);


        n_to_i[v]=num;


        i_to_n[num]=v;


    }



    Edge e;
    forall_edges(e,g){

        Edge corresponding;
        corresponding = edgeMapping.at(e);
        int num = wholeGraph->getNumberOf(corresponding);
        e_to_i[e]=num;

        i_to_e[num]=e;

    }



    setNumbering(e_to_i,n_to_i,i_to_n,i_to_e);



    cout << "DEBUG: exiting biconnected component constructor body" << endl;




}

