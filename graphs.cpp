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

void BookEmbeddedGraph::setPage(const Edge& e, const int newPage){
	int p = this->inf(e);
	pages[p].erase(e);
	pages[newPage].insert(e);
	this->assign(e, newPage);
	
	//recalculateCrossings();
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
	//TODO
}

//void BookEmbeddedGraph::calculateCrossings(const int pages&){
	//TODO
//}

int main(){
	
}
