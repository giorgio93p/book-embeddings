#include "graphs.h"


bool Edge::operator==(const Edge& e) const{
	return false;
}

BookEmbeddedGraph* Graph::bookEmbed(const int npages,const std::vector<int>& vertexPermutation){
	
	return new BookEmbeddedGraph;
}
BookEmbeddedGraph* Graph::bookEmbedOptimalPermutation(const int npages){
	
	return new BookEmbeddedGraph;
}
BookEmbeddedGraph* Graph::bookEmbedWithLeastPages(){
	
	return new BookEmbeddedGraph;
}

void BookEmbeddedGraph::setPage(const Edge& e, const int newPage){
	int p = pageOfEdge[e];
	pages[p].erase(e);
	pages[newPage].insert(e);
	pageOfEdge[e] = newPage;
	
	recalculateCrossings();
}

std::vector<Page>* BookEmbeddedGraph::getPages(){
	return &pages;
}
    
int BookEmbeddedGraph::getNpages(){
	return pages.size();
}

int BookEmbeddedGraph::getNcrossings(){
	return ncrossings;
}

int BookEmbeddedGraph::getNcrossings(const Edge& e){
	return crossings[e].size();
}
std::unordered_set<Edge> BookEmbeddedGraph::getcrossings(const Edge& e){
	return crossings[e];
}

BookEmbeddedGraph::~BookEmbeddedGraph(){
}

void BookEmbeddedGraph::calculateCrossings(){
	
}

void BookEmbeddedGraph::recalculateCrossings(){
	
}

int main(){
	
}
