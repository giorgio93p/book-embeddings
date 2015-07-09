#ifndef GRAPHS_H
#define GRAPHS_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>

typedef ogdf::node Node;
typedef ogdf::edge Edge;

class BookEmbeddedGraph;

class Graph  {
	ogdf::Graph g;
    ogdf::GraphAttributes attr;

  public:
    Graph();

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
    Node addNode(){
        return g.newNode();
    }
    Edge addEdge(Node& from, Node& to){
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

typedef std::set<Edge> Page;

class BookEmbeddedGraph : public Graph {
    std::vector<Page> pages;
    std::unordered_map<Edge,int> pageOfEdge; //this is used as a mapping from edges to pages
    
    std::unordered_map<Edge,std::unordered_set<Edge> > crossings;
    int ncrossings;
    
   public:
    BookEmbeddedGraph();

    void addPage();
    void setPage(const Edge& e, const int newPage);    

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
    void   calculateCrossings();
    //void recalculateCrossings();
};

#endif
