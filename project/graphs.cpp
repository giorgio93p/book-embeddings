#include "graphs.h"

Graph::Graph(){
    attr = ogdf::GraphAttributes(g,0);
    attr.setDirected(false);
}

Graph::Graph(Graph* graph){
    g = ogdf::Graph(graph->toOGDF());
    attr.setDirected(false);
    attr = ogdf::GraphAttributes(g, 0);
    
    //NOTE:You might not always want to delete the original graph
    //delete graph;
}

Node Graph::addNode(){
    return g.newNode();
}

Graph::~Graph(){
}

/******************************************** BookEmbeddedGraph Implementation ********************************************/

BookEmbeddedGraph::BookEmbeddedGraph(Graph* graph) : Graph(graph){
    attr.initAttributes(ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeLabel);

    Node n;
    int i=0;
    forall_nodes(n,g){
        attr.label(n) = std::to_string(i);
        i++;
    }

    //pages = std::vector<Page>();
    addPage();
    Edge e;
    forall_edges(e,g) addEdgeToPage(e,0);

    permutation = std::vector<Node>(numberOfNodes());

    crossings = std::unordered_map<Edge,std::unordered_set<Edge> >();
    //TODO: calculatecrossings
    ncrossings = 0;
    std::cout << "BookEmbeddedGraph created" << std::endl;
}

BookEmbeddedGraph::BookEmbeddedGraph() : BookEmbeddedGraph(new Graph()) {}

BookEmbeddedGraph::~BookEmbeddedGraph(){
}

Node BookEmbeddedGraph::addNode(){
    Node n = Graph::addNode();
    attr.label(n) = std::to_string(numberOfNodes()-1);
    permutation.push_back(n);
    return n;
}

Edge BookEmbeddedGraph::addEdge(Node& from, Node& to, int pageNo){
    Edge e = Graph::addEdge(from, to);
    addEdgeToPage(e,pageNo);
    return e;
}

void BookEmbeddedGraph::addPage(){
    Page newPage = std::set<Edge>();
    //bool (*cmpPtr)(const Edge&, const Edge&) = edgeCmp;
    //std::set<Edge, bool (*)(const Edge&, const Edge&)> newPage(cmpPtr);
    pages.push_back(newPage);
}

void BookEmbeddedGraph::removePage(int pageNo){
    for(int i = pageNo; i < getNpages() - 1; i++){
        pages[i] = pages[i+1];
        for(auto e : pages[i]) attr.label(e) = std::to_string(i);
    }

    pages.pop_back();
}

void BookEmbeddedGraph::moveToPage(Edge& e, const int newPage){
    int p = getPageNo(e);
    pages[p].erase(e);
    addEdgeToPage(e,newPage);
    //recalculate crossings of old and new page?
}

int BookEmbeddedGraph::getPageNo(const Edge &e) const{
    return std::stoi(attr.label(e));
}

int BookEmbeddedGraph::getPosition(const Node &v) const{
    return std::stoi(attr.label(v));
}

void BookEmbeddedGraph::swap(Node &v1, Node &v2){
    std::string temp = attr.label(v1);
    attr.label(v1) = attr.label(v2);
    attr.label(v2) = temp;
    permutation[getPosition(v1)] = v1;
    permutation[getPosition(v2)] = v2;
    //recalculate crossings?
}

void BookEmbeddedGraph::moveTo(Node &v, const int position){
    if(getPosition(v) > position){
        for(int i=getPosition(v); i>position; i--){
            attr.label(permutation[i-1]) = std::to_string(i);
            permutation[i] = permutation[i-1];
        }
    } else if(getPosition(v) < position){
        for(int i=getPosition(v); i<position; i++){
            attr.label(permutation[i+1]) = std::to_string(i);
            permutation[i] = permutation[i+1];
        }
    }
    permutation[position] = v;
    attr.label(v) = std::to_string(position);
    //recalculate crossings?
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

bool BookEmbeddedGraph::readGML(std::string &fileName){
    if(! ogdf::GraphIO::readGML(attr,g,fileName)) return false;

    Node v;
    forall_nodes(v,g){
        permutation[getPosition(v)] = v;
    }

    Edge e;
    forall_edges(e,g){
        while(getPageNo(e) >= getNpages()) addPage();
        pages[getPageNo(e)].insert(e);
        //TODO: calculatecrossings
    }
    return true;
}

//void BookEmbeddedGraph::calculateCrossings(const int pages&){
//TODO
//}

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

bool edgeCmp (const Edge &e1, const Edge &e2) {
    int sourceLabel1 = e1->source()->index();
    int sourceLabel2 = e2->source()->index();
    int targetLabel1 = e1->target()->index();
    int targetLabel2 = e2->target()->index();

    return ((sourceLabel1 != sourceLabel2) ? (sourceLabel1 < sourceLabel2) : (targetLabel1 > targetLabel2));
}
