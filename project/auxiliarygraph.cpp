#include "auxiliarygraph.h"
#include "graphs.h"
#include "biconnectedcomponent.h"
#include <ogdf/internal/planarity/ConnectedSubgraph.h>
#include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/FMMMLayout.h>



AuxiliaryGraph::AuxiliaryGraph(BookEmbeddedGraph* mg):
    originalGraph(mg),
    //pername sto original graph to mainGraph mesw pointer (ara deixnoun sto idio akrivws antikeimeno)
    bCTreeObj((*originalGraph)),
    //ftiaxnoume to BCTreeObject, pou dimiourgei kai apothikevei mesa tou ena ogdf::Graph
    //me titlo Auxiliary Graph. Einai simantiko oti kai edw oti pername pointer, ara ousiastika
    //to bctreeobj vlepei akrivws to idio maingraph antikeimeno pou vlepei kai to main window.
    g(bCTreeObj.getAuxiliaryGraph()),
    //edw dinoume timi sto g. To g tha einai to grafima pou deixnoume sto katw apo to main graph.
    //to legomeno auxiliarygraph (apoteleitai apo oles tis biconnectedcomponents tou maingraph
    //kai ta cut vertices).
    //Einai typou const& ogdf::Graph. Einai const epeidh afto epistrefei to ogdf::BCTree::auxiliaryGraph()

    attr(g,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics)
  //arxikopoioume kanonika ta graph attributes, opws kanoume kai sto class Graph.

{


    //arxiko mas melima einai na ftiaksoume mappings apo to ag sto mg.
    //(ag= the auxiliary graph which is stored in g, mg = our maingraph)
    // We need these mappings (e eipa na to gyrisw se egglezika den maresan ta greeklish)
    // so that we can associate a node clicked on a biconnected component with
    // the corresponding node in the main graph. That way, we can automatically
    // update the main graph each time a biconnected component of a graph changes.
    // One important necessity is to create mappings from the mainGraph to each biconnected component
    // too, soon to be done.  (TODO!!)

    attr.setDirected(false);

    const std::unordered_map<Node,Node> agN_to_mgN = bCTreeObj.generateNodeMapping();
    const std::unordered_map<Edge,Edge> agE_to_mgE = bCTreeObj.generateEdgeMapping();
    //agN_to_mgN : a(uxiliary)g(raph)N(ode)_to_m(ain)g(raph)N(ode)
    // agE_to_mgE: same but for edges.
    //the generate{Node,Edge}Mapping() methods create unordered_maps from
    //Nodes/edges in ag to nodes/edges in mg. They use the ogdf::BCTree::original(Node/Edge)
    //method. This takes a Node/Edge from ag (but not from a copy of it!! ) and returns
    //the corresponding node in the main graph.


    //Now we have all the biconnected components
    //of the graph, without the edges between them, stored in g.
    //This way, we can get every B.C. by extracting each connected subgraph
    //of g.


    std::unordered_map<Node,int> explored;
    //explored: we hold for each vertex of g
    //an exploration status. i.e. if we have visited them
    //or not. Visiting a node here means 'extracting'
    //its biconnected component from g.
    //This way we can ignore the vertices
    //of already "extracted" connected subgraphs.

    Node v;
    forall_nodes(v,g) {
        explored[v]=0;
        Node w = agN_to_mgN.at(v);

        bool found=false;


        Node vv;

        forall_nodes(vv,*mg) {
            if (vv == w) found=true;
        }


        if ( found )
            cout << "found" << endl;
        else
            cout << "notfound" << endl;


        //int nn = mg->getNumberOf(w);

    }
    //initially, we set all nodes as unexplored.Now we shall proceed to initilize our data structures:

    bCs = std::vector<BiconnectedComponent*>();    //bCs: here we store independent BiconnectedComponents
    subGraphs = std::vector<ogdf::Graph*>();       //subGraphs: here we store the ogdf::Graphs of the aforementioned B.C's
    agN_to_bc = std::unordered_map<Node,BiconnectedComponent*>(); //agN_to_bc this maps each node in ag to its bc.
    is_cut_node = std::unordered_map<Node,bool>(); //this seperates the cut-nodes from the others.
    //the cut nodes exist more than one times in g. One on their own, as lonely vertices, and also in all the
    //biconnected components they exist in.

    forall_nodes(v,g) {
        if (explored[v]==0) { //check if the vertex belongs to an already extracted subgraph

            ogdf::Graph* newBC =new ogdf::Graph();

            //ogdf::NodeArray< Node > nodeMapping(); <-- attention! this doesnt work!

            ogdf::NodeArray< Node > sgN_to_agN; // a mapping from of nodes in newBC (our S(ub)g(raph)) to nodes in ag
            ogdf::EdgeArray< Edge > sgE_to_agE; // similarly for edges
            ogdf::NodeArray<Node> nG_to_nSG;    //likewise
            ogdf::EdgeArray<Edge> eG_to_eSG;    // >>

            //the above are to be used in the static method of ogdf::ConnectedSubgraph class 'call'.
            //which btw is overloaded as fuck.



            ogdf::ConnectedSubgraph<int>::call(g,*newBC,v,sgN_to_agN,           //"save in *newBC the conn.subgraph
                                               sgE_to_agE,nG_to_nSG,eG_to_eSG); //in which Node v is contained"




            if (newBC->numberOfNodes() <2){   //in case the newly extracted bc is a cut vertex
                explored[v]=1;                //we won't add it to the vertex of bc's
                is_cut_node[v]=true;
                continue;                     //continue means that we skip the lines till the end of the loopblock

            }

            if (newBC->numberOfNodes()== mg->numberOfNodes()) { //in case this is the whole graph
                mg->setAsBiconnected(); //TODO: CREATE FUN
                break;

            }



            //else : we are facing a proper biconnected component
            //i.e. it contains more than one vertex
            //Now we have to create mappings from each vertex/edge in newBC
            //to each vertex/edge in maingraph. To do that, we use the sgN/E_to_agN/E arrays
            //and the previously created agN/E_to_mgN/E.



            std::unordered_map<Node,Node> nMapping; //these won't work
            std::unordered_map<Edge,Edge> eMapping; //with the parentheses

            Node n;
            forall_nodes(n,*newBC) {

                const Node& agNode = sgN_to_agN[n]; //returns a reference.
                nMapping[n]= agN_to_mgN.at(agNode);


            }

            int size = sizeof(Node);

            Edge e;
            forall_edges(e,*newBC) {

                eMapping[e]= agE_to_mgE.at(sgE_to_agE[e]);

            }

            //at last, now we can create the biconnectedComponent object!

            BiconnectedComponent *bc = new BiconnectedComponent(*newBC,mg,nMapping,eMapping);

            // 2 last things:
            // 1.map each node of the new bc to the this bc (fill agN_to_bc)
            // 2.set all the vertices in the extracted subgraph as explored

            forall_nodes (n,*newBC) {
                Node correspondingNode = sgN_to_agN[n];



                //setting the maps
                agN_to_bc[correspondingNode]=bc;
                is_cut_node[correspondingNode]=false;


                //setting the vertex as explored, with error-handling!!
                std::unordered_map<Node,int>::const_iterator got = explored.find(correspondingNode);
                if ( got == explored.end() ) {
                    perror("ERROR: Node not found on auxiliary graph..exiting.\n");
                    exit(1);
                }

                else {

                    explored.at(got->first) = 1;
                }


            }


            bCs.push_back(bc);   //storing the results

            if (bCs.size() == 1) mg->setAsBiconnected(); //if this condition holds true
                                                         //that means that our graph
                                                         //has one biconnected component
                                                         //and a number of lonely vertices.

            subGraphs.push_back(newBC);// in the vectors

        }


    }



    subGraphs.shrink_to_fit();


    cout << "Auxiliary graph created. Number of binconnected components found: " << bCs.size() << endl;


}


BiconnectedComponent* AuxiliaryGraph::getBCOf(const Node& v) const {

    auto got = agN_to_bc.find(v);

    if ( got == agN_to_bc.end() )
        std::cout << "could find corresponding bc node belongs to! exiting." << endl;


    return agN_to_bc.at(v);
}


const std::vector<BiconnectedComponent*>& AuxiliaryGraph::getBiconnectedComponents() const{
    const std::vector<BiconnectedComponent*>& ret = bCs;
    return ret;



}


void AuxiliaryGraph::buildLayout(const double xmin, const double ymin, const double xmax, const double ymax){



    ogdf::FMMMLayout drawer = ogdf::FMMMLayout();


    drawer.useHighLevelOptions(true);
    drawer.unitEdgeLength(15.0);
    drawer.newInitialPlacement(true);
    drawer.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);


    drawer.call(attr);



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


