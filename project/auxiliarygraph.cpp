#include "auxiliarygraph.h"
#include "graphs.h"
#include "biconnectedcomponent.h"
#include <ogdf/internal/planarity/ConnectedSubgraph.h>
#include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/FMMMLayout.h>



AuxiliaryGraph::AuxiliaryGraph(BookEmbeddedGraph* mg):
    originalGraph(mg),
    bCTreeObj((*originalGraph)),g(bCTreeObj.getAuxiliaryGraph()),
    attr(g,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics)


{

    //attr = ogdf::GraphAttributes(g,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    attr.setDirected(false);

    const std::unordered_map<Node,Node> agN_to_mgN = bCTreeObj.generateNodeMapping();
    const std::unordered_map<Edge,Edge> agE_to_mgE = bCTreeObj.generateEdgeMapping();

    Node v;


    cout << "debug \n";
    forall_nodes (v,(g)) {

        Node n = agN_to_mgN.at(v);
        int pos = mg->getPosition(n); //DEBUG
        cout << "this should be a valid position baby: " << pos << endl;


    }
    cout << "debug end\n";

    // The auxilliary graph is all the biconnectedcomponents
    //of the graphs, without the edges between them.
    //This way, we can get every B.C. by extracting each connectedSubgraph
    //of graf.


    std::unordered_map<Node,int> explored;


    forall_nodes(v,g) {  //explored: we hold for each vertex of graf
        explored[v]=0;     //an exploration status. i.e. if we have visited them
        //or not. This way we can ignore the vertices
        //of already "is a way of  between
    }                      //vertices of already "extracted" connected subgraphs.

    bCs = std::vector<BiconnectedComponent*>(); //initialize the vector holding
    //our bc's
    agN_to_bc = std::unordered_map<Node,BiconnectedComponent*>();



    //now we shall "crack" g (the auxiliarygraph)
    forall_nodes(v,g) {

        if (explored[v]==0) { //check if the vertex belongs to an already extracted subgraph

            ogdf::Graph newBC;
            //ogdf::NodeArray< Node > nodeMapping(); <-- attention! this doesnt work!
            ogdf::NodeArray< Node > sgN_to_agN;// a mapping from of nodes in SG to nodes in ag
            ogdf::EdgeArray< Edge > sgE_to_agE;// similarly for edges
            ogdf::NodeArray<Node> nG_to_nSG;
            ogdf::EdgeArray<Edge> eG_to_eSG;




            ogdf::ConnectedSubgraph<int>::call(g,newBC,v,sgN_to_agN,
                                               sgE_to_agE,nG_to_nSG,eG_to_eSG);

            if (newBC.numberOfNodes() <2){   //in case the newly extracted bc is a cut vertex
                explored[v]=1;               //we won't add it to the vertex of bc's
                continue;
            }

            //else : we are facing a proper biconnected component
            //ie it contains more than one vertex

            //create the mappings
            std::unordered_map<Node,Node> nMapping; //these won't work
            std::unordered_map<Edge,Edge> eMapping; //with the parentheses

            Node n;


            //for(Node n=newBC.firstNode();n;n=n->succ())
            forall_nodes(n,newBC) {

                Node agNode = sgN_to_agN[n];
                nMapping[n]= agN_to_mgN.at(agNode);

            }


            n = newBC.firstNode();
            while (n) {
                cout<<"got a node!\n";
                n=n->succ();
            }
            forall_nodes(n,newBC) {

                cout << "got a node!\n";


            }
            Edge e;


            forall_edges(e,newBC) {

                eMapping[e]= agE_to_mgE.at(sgE_to_agE[e]);

            }

            ogdf::Graph& nuBC=newBC; //creating a reference to newBC


            BiconnectedComponent *bc = new BiconnectedComponent(nuBC,mg,nMapping,eMapping);
            //one last thing.
            //does the nMapping work for copies of the newBC graph variable?
            //as you can see, we're not passing it by reference.
            // We are passing it by value, and we are storing it to the bc object's
            // field g. We will try to access the mainGraph nodes from bc through this constructor.

            //now we must
            // 1.map each node of the new bc to the correct bc.
            // 2.set all the vertices in the extracted subgraph as explored

            n = newBC.firstNode();
            while (n) {


                Node correspondingNode = sgN_to_agN[n];
                //agN_to_bc[correspondingNode]=bc;
                std::unordered_map<Node,int>::const_iterator got = explored.find(correspondingNode);
                if ( got == explored.end() )
                    std::cout << "not found" << endl;
                else {

                    explored.at(got->first) = 1;
                    cout << "xplored a n0de " << endl;


                }
                n=n->succ();

            }


            bCs.push_back(bc);




        }
    }

    cout << "we have a number of binconnected components here: " << bCs.size() << endl;


}


BiconnectedComponent* AuxiliaryGraph::getBCOf(Node v) {
    return agN_to_bc[(v)];
    //return agN_to_bc.at(v); //etsi prpei na einai kanonika.
}


const std::vector<BiconnectedComponent*>& AuxiliaryGraph::getBiconnectedComponents() const{
    const std::vector<BiconnectedComponent*>& ret = bCs;
    return ret;



}


void AuxiliaryGraph::buildLayout(const double xmin, const double ymin, const double xmax, const double ymax){
    cout << "entering buildLayout" << endl;



    ogdf::FMMMLayout drawer = ogdf::FMMMLayout();


    drawer.useHighLevelOptions(true);
    drawer.unitEdgeLength(15.0);
    drawer.newInitialPlacement(true);
    drawer.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);

    cout << "before call of drawer" << endl;

    drawer.call(attr);

    cout << "after  call of drawer" << endl;


    ogdf::SpringEmbedderFR drawer2 = ogdf::SpringEmbedderFR();
    drawer2.scaling(ogdf::SpringEmbedderFR::scUserBoundingBox);
    drawer2.userBoundingBox(0.0, 0.0, xmax, ymax);
    drawer2.call(attr);

}

double AuxiliaryGraph::getXcoord(const Node& v) const{
    return attr.x(v);
}


double AuxiliaryGraph::getYcoord(const Node& v) const{
    return attr.y(v);
}


