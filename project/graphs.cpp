#include "graphs.h"

Graph::Graph(){
    attr = ogdf::GraphAttributes(g,ogdf::GraphAttributes::nodeLabel);
    attr.setDirected(false);
}

Graph::Graph(const Graph& graph){
    g = ogdf::Graph(graph.toOGDF());
    attr.setDirected(false);
    attr = ogdf::GraphAttributes(g,ogdf::GraphAttributes::nodeLabel);
    Node n;
    int i=0;
    forall_nodes(n,g){
        attr.label(n) = std::to_string(i);
        i++;
    }
}

Node Graph::addNode(){
    Node n = g.newNode();
    attr.label(n) = std::to_string(numberOfNodes()-1);
    return n;
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

BookEmbeddedGraph::BookEmbeddedGraph(Graph& graph) : Graph(graph){
    attr = ogdf::GraphAttributes(g, ogdf::GraphAttributes::nodeLabel);

    //pages = std::vector<Page>();
    addPage();
    Edge e;
    forall_edges(e,g) addEdgeToPage(e,0);

    //crossings = std::unordered_map<Edge,std::unordered_set<Edge> >();
    ncrossings = 0;
}

void BookEmbeddedGraph::addPage(){
    Page newPage = std::set<Edge, std::function <bool (const Edge&, const Edge&)>(
                [this](const Edge& e1, const Edge& e2) {
                    return std::stoi(this->attr.label(e1->source())) != std::stoi(this->attr.label(e2->source()))
                        ? stoi(this->attr.label(e1->source())) < std::stoi(this->attr.label(e2->source()))
                        : stoi(this->attr.label(e1->target())) > std::stoi(this->attr.label(e2->target()));
                 })
    );

    pages.push_back(newPage);
}

void BookEmbeddedGraph::removePage(int pageNo){
    for(int i=pageNo; i<getNpages()-1; i++){
        pages[i] = pages[i+1];
        for(auto e : pages[i]) attr.label(e) = std::to_string(i);
    }

    pages.pop_back();
}

void BookEmbeddedGraph::moveToPage(Edge& e, const int newPage){
    int p = getPageNo(e);
    pages[p].erase(e);
    addEdgeToPage(e,newPage);
}

int BookEmbeddedGraph::getPageNo(const Edge &e) const{
    return std::stoi(attr.label(e));
}

BookEmbeddedGraph::~BookEmbeddedGraph(){
}

void BookEmbeddedGraph::addEdgeToPage(Edge& e, const int pageNo){
    attr.label(e) = std::to_string(pageNo);
    pages[pageNo].insert(e);
}

void BookEmbeddedGraph::generateBuckets(){
    for (Page p : pages) {
        Buckets pageStart;
        Buckets pageEnd;

        pageStart.reserve(numberOfNodes());
        pageEnd.reserve(numberOfNodes());

        for (Edge e : p) {
            pageStart[e->source()->index()].insert(e);
            pageEnd[e->target()->index()].insert(e);
        }

        startBuckets.push_back(pageStart);
        endBuckets.push_back(pageEnd);
    }

    bucketsNeedToBeGenerated = false;

}

void BookEmbeddedGraph::calculateCrossings(){
    if (bucketsNeedToBeGenerated)
        generateBuckets();

    for (Buckets bs : startBuckets){//bs are a single page's buckets
        for (Bucket b : bs){
            for (Edge e : b) {
                int startNode = e->source()->index();
                int endNode = e->target()->index();

                for (int i = startNode; i <= endNode; i++){
                    if (bs[i].size() > 0) {
                        for (Edge candidate : bs[i]){
                            if ((candidate->target()->index() > endNode) || (candidate->target()->index() < startNode)){
                                crossings[e].insert(candidate);
                                ncrossings++;
                            }
                        }
                    }
                }
            }
        }
    }
}

//void BookEmbeddedGraph::calculateCrossings(const int pages&){
	//TODO
//}
