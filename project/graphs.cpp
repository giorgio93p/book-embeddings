#include "graphs.h"

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

void BookEmbeddedGraph::setPage(const Edge& e, const int newPage){
	int p = pageOfEdge[e];
	pages[p].erase(e);
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
