#ifndef GRAPHS_H
#define GRAPHS_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <functional>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/planarity/BoothLueker.h>
#include <ogdf/decomposition/BCTree.h>

//#include "page_node.h"

typedef ogdf::node Node;
typedef ogdf::edge Edge;
typedef std::set<Edge> Page;
//typedef std::set<Edge, bool (*)(const Edge&, const Edge&)>  Page;
typedef std::set<Edge>      Bucket;
typedef std::vector<Bucket> Buckets;

class BookEmbeddedGraph;

class Graph  {




    protected:
        ogdf::Graph g1;
        ogdf::Graph& g2; //this is a reference to an ogdf graph!
        ogdf::GraphAttributes attr;
        bool use_g2; //this is set to true when we use a reference
        //to an ogdf graph instead of a graphz
    private:
        std::unordered_map<Edge,int> e_to_int;
        std::unordered_map<Node,int> n_to_int;
        std::unordered_map<int,Node> int_to_n;
        std::unordered_map<int,Edge> int_to_e;
        bool listshavebeenset; //this is to ensure that the above lists are
                               //given their values only once.
                               //we could have used the const keyword, but
                               //this would create a number of problems,
                               //because we want the capability of initializing
                               //outside the constructor of class Graph.




    public:
        Graph();
        Graph(ogdf::Graph graph);
        Graph(Graph*);
        Graph(ogdf::Graph& graph,bool weareusingreference);

        //the second arguement allows us to choose
        //if the graph will create the four unordered_maps
        // automatically. This allows the user to use the method
        // setNumbering to use custom numbering for the nodes and
        // edges

        void setNumbering(std::unordered_map<Edge,int> e_to_num,
                          std::unordered_map<Node,int> n_to_num,
                          std::unordered_map<int,Node> num_to_n,
                          std::unordered_map<int,Edge> num_to_e);

        void setDefaultNumbering();

        int getNumberOf(Node n) {
            return n_to_int[n];
        }

        int getNumberOf(Edge e) {
            return e_to_int[e];
        }

        Node getNode(int num) {
            return int_to_n[num];
        }

        Edge getEdge(int num) {
            return int_to_e[num];
        }

        ogdf::Graph toOGDFval() const {
            ogdf::Graph& g = g2 ;


            return g;
        }

         ogdf::Graph& toOGDF() {
            ogdf::Graph& g = (use_g2) ? g2:g1;

            ogdf::Graph& graf= g;
            return graf;
        }


        virtual bool readGML(std::string& fileName){
            ogdf::Graph& g = (use_g2) ? g2:g1;


            return ogdf::GraphIO::readGML(attr,g,fileName);
        }
        virtual bool writeGML(std::string& fileName){
            return ogdf::GraphIO::writeGML(attr,fileName);
        }
        bool empty() {
            ogdf::Graph& g = (use_g2) ? g2:g1;

            return g.empty();
        }
        int numberOfNodes() const {
            const ogdf::Graph& g = (use_g2) ? g2:g1;


            return g.numberOfNodes();
        }
        int numberOfEdges() const {
            const ogdf::Graph& g = (use_g2) ? g2:g1;

            return g.numberOfEdges();
        }

        Node addNode();

        virtual Edge addEdge(Node& from, Node& to){
            ogdf::Graph& g = (use_g2) ? g2:g1;

            return g.newEdge(from, to);
        }

        virtual Node firstNode() {
            ogdf::Graph& g = (use_g2) ? g2:g1;

            return g.firstNode();
        }

        virtual Edge firstEdge()  {
            ogdf::Graph& g = (use_g2) ? g2:g1;

            return g.firstEdge();
        }

        ogdf::GraphAttributes getGraphAttributes() {

            return attr;
        }

        ogdf::Graph getGraph() {
            ogdf::Graph& g = (use_g2) ? g2:g1;

            return g;
        }

        void buildLayout(const double xmin, const double ymin, const double xmax, const double ymax);
        double getXcoord(const Node &v) const;
        double getYcoord(const Node &v) const;
        double getBoxWidth(const Node &v) const;
        double getBoxHeight(const Node &v) const;

        bool graphIsPlanar() const;

        //TODO:
        //orismos methodon epanasxediasmou (px gia dinatotita allagis akmwn / switching koryfwn)

        BookEmbeddedGraph* bookEmbed(const int npages,const std::vector<int>& vertexPermutation);
        BookEmbeddedGraph* bookEmbedOptimalPermutation(const int npages);
        BookEmbeddedGraph* bookEmbedWithLeastPages();
        //the above 3 functions will do the most serious job 
        //bookEmbed will try and find the best book embedding for a 
        //given number of available pages and a given sequence of vertices
        //bookEmbedOptimalPermutation will try and find the best book embedding with **any** permutation
        //bookEmbedWithLeastPages will try and find the least number of pages required to book-embed
        //this Graph with zero crossings.

        //TODO:
        //prosthiki synartisewn pou kanoun bookembedding gia dedomeno permutation alla agnosti selidopoisi
        //kai to antistrofo

        virtual ~Graph();
};

class BookEmbeddedGraph : public Graph {

    public:
        BookEmbeddedGraph(Graph* g);

        BookEmbeddedGraph(ogdf::Graph&,bool weareusingreferences);

        BookEmbeddedGraph();
        BookEmbeddedGraph(ogdf::Graph );

        Edge addEdge(Node& from, Node& to, int pageNo);

        Edge addEdge(Node& from, Node& to) override{
            return addEdge(from,to,0);
        }
        Edge firstEdge() ;

        Node addNode();

        void addPage(int pageNo);
        void addPage(){addPage(getNpages());}
        void removePage(int pageNo);

        /*
         * Does not recalculate crossings
        */
        void moveToPage(Edge& e, const int newPage);

        int getPageNo(const Edge& e) const;
        int getPosition(const Node& v) const;
        Node nodeAt(int position) const;
        void swap(Node& v1, Node& v2);
        void moveTo(Node& v, const int position);

        Page edgesIn(int page) const {
            return pages[page];
        }
        int getNpages() const {
            return pages.size();
        }
        int getNcrossings() const {
            return ncrossings;
        }
        int getNcrossings(const int page) const{
            int result = 0;
            for(Edge e : pages[page]){
                result += getNcrossings(e);
            }
            return result/2; //we have counted each crossing twice
        }
        int getNcrossings(const Edge& e) const{
            return 0;//crossings.at(e).size();
        }
        std::unordered_set<Edge> getcrossings(const Edge& e) const{
            return crossings.at(e);
        }
        int pageSize(int p) const{
            return pages[p].size();
        }
        /*
        void setVertexOrder(int *order){
            free(vertexOrder);
            vertexOrder = order;
        }*/

        bool readGML(std::string& fileName) override;
        virtual ~BookEmbeddedGraph() = default;

        void updatePermutation(int, int);

    private:
        //std::vector<int> nodeOrderOnSpine;
        std::vector<Page> pages;
        std::unordered_map<Edge,std::unordered_set<Edge> > crossings;
        std::vector<Node> permutation;

        int ncrossings;

        bool bucketsNeedToBeGenerated;
        std::vector<Buckets> startBuckets;
        std::vector<Buckets> endBuckets;

        void addEdgeToPage(Edge& e, int pageNo);
        void generateBuckets();

        /**
         * @param pagesChanged The pages that need to be recalculated. If the vector is empty, all pages need calculation.
         */
        void calculateCrossings(const std::vector<int> pagesChanged = std::vector<int>());

};

bool edgeCmp (const Edge&, const Edge&);

//BookEmbeddedGraph iteration macros
#define forall_nodes_embedded(v,g) int ijklm=0; for((v) = (g).nodeAt(0); ijklm < (g).numberOfNodes(); (v) = ijklm < (g).numberOfNodes()-1 ? (g).nodeAt(++ijklm) : (g).nodeAt(ijklm++))

//BCTree iteration macros
//TODO: populate as needed
#define forall_nodes_bc(v,T,b) for ((v) = (T).firstNode(b); (v); (v) = (v)->succ())
#define forall_edges_bc(e,T,b) for ((e) = (T).firstEdge(b); (e); (e) = (e)->succ())

enum onode_type {
    ONODETYPE_NORMAL, 
    ONODETYPE_CUT
};

enum bnode_type {
    BNODETYPE_BCOMP,
    BNODETYPE_CCOMP
};

class BCTree {
    private:
        //NOTE : originalGraph should not even be here, but we can't call
        //BCTree's constructor with a temp argument, so we need to use
        //that (see constructor definition)
        
        ogdf::Graph& originalGraph;
        ogdf::BCTree ogBCT;
        const ogdf::Graph& auxiliaryGraph;  //auxiliary graph: this is the graph that contains all biconnected components
                                            //without any edges between them
                                            //we chose const reference, because ogdf::BCTree::auxiliaryGraph()
                                           //returns const

        std::unordered_map<Node,Node> agN_to_mgN; //a(uxiliary)g(raph)N(ode)_to_m(ain)g(raph)N(ode)
        std::unordered_map<Edge,Edge> agE_to_mgE; //similarly with edges




    public:
        BCTree(Graph g);

        const ogdf::Graph& getAuxiliaryGraph() {
            //ogdf::Graph &ax = ogBCT.auxiliaryGraph();
            return auxiliaryGraph;
        }

        ogdf::Graph getBCTree2() const {
            return ogBCT.auxiliaryGraph();
        }

        const std::unordered_map<Node,Node> generateNodeMapping();
        const std::unordered_map<Edge,Edge> generateEdgeMapping();

        Node firstNode(bool);
        Node lastNode(bool);

        int numberOfNodes(bool);
        int numberOfBComps ();
        int numberOfCComps ();
        onode_type typeOfVertexInOriginalGraph(Node);
        bnode_type typeOfVertexInBCTree(Node);
};

#endif
