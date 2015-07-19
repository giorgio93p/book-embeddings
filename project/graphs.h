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

typedef ogdf::node Node;
typedef ogdf::edge Edge;
typedef std::set<Edge> Page;
typedef std::set<Edge>      Bucket;
typedef std::vector<Bucket> Buckets;

class BookEmbeddedGraph;

class Graph  {
    protected:
        ogdf::Graph g;
        ogdf::GraphAttributes attr;

    public:
        Graph();
        Graph(const Graph& graph);

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
        BookEmbeddedGraph(){
            BookEmbeddedGraph(Graph());
        }
        BookEmbeddedGraph(Graph& g);

        Edge addEdge(Node& from, Node& to, int pageNo){
            Edge e = Graph::addEdge(from, to);
            addEdgeToPage(e,pageNo);
            return e;
        }

        /**
         * Creates a new edge and puts it in the first page
         *
         * @return a reference to the edge created
         */
        Edge addEdge(Node& from, Node& to){
            return addEdge(from,to,0);
        }

        void addPage();
        void removePage(int pageNo);

        /**
         * Does not recalculate crossings
         */
        void moveToPage(Edge& e, const int newPage);

        int getPageNo(const Edge& e) const;

        std::set<Edge> edgesIn(int page) const {
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

        virtual ~BookEmbeddedGraph();

    private:
        std::vector<int> nodeOrderOnSpine;
        std::vector<Page> pages;
        std::unordered_map<Edge,std::unordered_set<Edge> > crossings;
        int ncrossings;

        bool bucketsNeedToBeGenerated;
        std::vector<Buckets> startBuckets;
        std::vector<Buckets> endBuckets;

        void addEdgeToPage(Edge& e, int pageNo);
        void generateBuckets();
        void calculateCrossings();
        //void recalculateCrossings();
};

#endif
