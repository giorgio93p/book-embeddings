#ifndef GRAPHS_H
#define GRAPHS_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <functional>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/planarity/BoothLueker.h>
#include <ogdf/decomposition/BCTree.h>

typedef ogdf::node Node;
typedef ogdf::edge Edge;
typedef std::set<Edge> Page;
//typedef std::set<Edge, bool (*)(const Edge&, const Edge&)>  Page;
typedef std::set<Edge>      Bucket;
typedef std::vector<Bucket> Buckets;

class BookEmbeddedGraph;

class Graph  {
    protected:
        ogdf::Graph g;
        ogdf::GraphAttributes attr;

    public:
        Graph();
        Graph(Graph* graph);

        ogdf::Graph toOGDF() const{
            return g;
        }

        virtual bool readGML(std::string& fileName){
            return ogdf::GraphIO::readGML(attr,g,fileName);
        }
        virtual bool writeGML(std::string& fileName){
            return ogdf::GraphIO::writeGML(attr,fileName);
        }
        bool empty() const {
            return g.empty();
        }
        int numberOfNodes() const{
            return g.numberOfNodes();
        }
        int numberOfEdges() const{
            return g.numberOfEdges();
        }

        Node addNode();

        virtual Edge addEdge(Node& from, Node& to){
            return g.newEdge(from, to);
        }

        Node firstNode() const{
            return g.firstNode();
        }

        ogdf::GraphAttributes getGraphAttributes() {
            return attr;
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

        BookEmbeddedGraph();

        Edge addEdge(Node& from, Node& to, int pageNo);

        Edge addEdge(Node& from, Node& to) override{
            return addEdge(from,to,0);
        }
        Edge firstEdge() const;

        Node addNode();

        void addPage();
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
        int getNcrossings(const Edge& e) const{
            return crossings.at(e).size();
        }
        std::unordered_set<Edge> getcrossings(const Edge& e) const{
            return crossings.at(e);
        }
        int pageSize(int p) const{
            return pages[p].size();
        }

        bool readGML(std::string& fileName) override;
        virtual ~BookEmbeddedGraph() = default;

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
        
        ogdf::Graph originalGraph;
        ogdf::BCTree ogBCT;
    public:
        BCTree(Graph &);

        Node firstNode(bool);
        Node lastNode(bool);

        int numberOfNodes(bool);
        int numberOfBComps ();
        int numberOfCComps ();
        onode_type typeOfVertexInOriginalGraph(Node);
        bnode_type typeOfVertexInBCTree(Node);
};

#endif
