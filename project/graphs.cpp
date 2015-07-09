#include "graphs.h"

Graph::Graph(){
    ogdf::Graph g;
    attr = ogdf::GraphAttributes(g, ogdf::GraphAttributes::nodeLabel);
    attr.setDirected(false);

    g.newNode();
    attr.label(g.firstNode()) = "a";
}

BookEmbeddedGraph* Graph::bookEmbed(const int npages,const std::vector<int>& vertexPermutation){
	//TODO
	return new BookEmbeddedGraph;
}
BookEmbeddedGraph* Graph::bookEmbedOptimalPermutation(const int npages){
	//TODO
	return new BookEmbeddedGraph;
}
BookEmbeddedGraph* Graph::bookEmbedWithLeastPages(){
	//TODO
	return new BookEmbeddedGraph;
}

Graph::~Graph(){
}

BookEmbeddedGraph::BookEmbeddedGraph(){
    pages = std::vector<Page>();
    pageOfEdge = std::unordered_map<Edge,int>();

    crossings = std::unordered_map<Edge,std::unordered_set<Edge> >();
    ncrossings = 0;
}

void BookEmbeddedGraph::addPage(){
    pages.push_back(Page());
}

void BookEmbeddedGraph::setPage(const Edge& e, const int newPage){
    try{
        int p = pageOfEdge.at(e);
        pages[p].erase(e);
    } catch (std::out_of_range& e) {}
	pages[newPage].insert(e);
	pageOfEdge[e] = newPage;
	
    //recalculateCrossings();
}

BookEmbeddedGraph::~BookEmbeddedGraph(){
}

void BookEmbeddedGraph::calculateCrossings(){
	//TODO
}

//void BookEmbeddedGraph::calculateCrossings(const int pages&){
	//TODO
//}
