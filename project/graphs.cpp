#include "graphs.h"
#include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/FMMMLayout.h>

Graph::Graph(){
    attr = ogdf::GraphAttributes(g,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    attr.setDirected(false);
}

Graph::Graph(Graph* graph){
    g = ogdf::Graph(graph->toOGDF());
    attr.setDirected(false);
    attr = ogdf::GraphAttributes(g,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    
    //NOTE:You might not always want to delete the original graph
    //delete graph;


}

Graph::Graph(ogdf::Graph graph){
    g=graph;
    attr = ogdf::GraphAttributes(g);//no value given
}

Node Graph::addNode(){
    return g.newNode();
}

void Graph::buildLayout(const double xmin, const double ymin, const double xmax, const double ymax){
    cout << "entering buildLayout" << endl;



    ogdf::FMMMLayout drawer = ogdf::FMMMLayout();


        drawer.useHighLevelOptions(true);
        drawer.unitEdgeLength(15.0);
        drawer.newInitialPlacement(true);
        drawer.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);

    cout << "before call of drawer" << endl;

    drawer.call(attr);

    cout << "after  call of drawer" << endl;


    ogdf::SpringEmbedderFR drawer2 = ogdf::SpringEmbedderFR();
    drawer2.scaling(ogdf::SpringEmbedderFR::scUserBoundingBox);
    drawer2.userBoundingBox(0.0, 0.0, xmax, ymax);
    drawer2.call(attr);

}

double Graph::getXcoord(const Node& v) const{
    return attr.x(v);
}


double Graph::getYcoord(const Node& v) const{
    return attr.y(v);
}

bool Graph::graphIsPlanar() const{
    ogdf::BoothLueker blPlanarityCheck;
    return blPlanarityCheck.isPlanar(toOGDF());
}

Graph::~Graph(){
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

/******************************************** BookEmbeddedGraph Implementation ********************************************/

BookEmbeddedGraph::BookEmbeddedGraph(Graph* graph) : Graph(graph){
    attr.initAttributes(ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeLabel);

    //vertexOrder = NULL;

    Node n;
    int i=0;
    forall_nodes(n,g){
        attr.label(n) = std::to_string(i);
        i++;
    }

    pages = std::vector<Page>();
    addPage();

    crossings = std::unordered_map<Edge,std::unordered_set<Edge> >();

    Edge e;
    forall_edges(e,g){
        crossings[e] = std::unordered_set<Edge>();
        addEdgeToPage(e,0);
    }

    permutation = std::vector<Node>(numberOfNodes());

    bucketsNeedToBeGenerated = true;
    ncrossings = 0;
    calculateCrossings(); //commented out because it wasnt working (segfault)
    std::cout << "BookEmbeddedGraph created" << std::endl;
}

BookEmbeddedGraph::BookEmbeddedGraph() : BookEmbeddedGraph(new Graph()) {}

BookEmbeddedGraph::BookEmbeddedGraph(ogdf::Graph graph) : BookEmbeddedGraph(new Graph(graph)) {}


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

Edge BookEmbeddedGraph::firstEdge() const{
    return g.firstEdge();
}

void BookEmbeddedGraph::addPage(int pageNo){
    Page newPage = std::set<Edge>();
    //bool (*cmpPtr)(const Edge&, const Edge&) = edgeCmp;
    //std::set<Edge, bool (*)(const Edge&, const Edge&)> newPage(cmpPtr);
    pages.push_back(newPage);//this just increments the size of the vector
    for(int i = getNpages()-1; i > pageNo; i--){
        pages[i] = pages[i-1];
        for(Edge e : pages[i]) attr.label(e) = std::to_string(i);
    }
    pages[pageNo] = newPage;
}

void BookEmbeddedGraph::removePage(int pageNo){
    for(int i = pageNo; i < getNpages() - 1; i++){
        pages[i] = pages[i+1];
        for(Edge e : pages[i]) attr.label(e) = std::to_string(i);
    }
    pages.pop_back();
}

void BookEmbeddedGraph::moveToPage(Edge& e, const int newPage){
    int p = getPageNo(e);
    pages[p].erase(e);
    addEdgeToPage(e,newPage);
    std::vector<int> temp = {p,newPage};

    ncrossings=0;
    bucketsNeedToBeGenerated=true;
    calculateCrossings(/*temp*/); //TODO: Fix if prog works.
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

Node BookEmbeddedGraph::nodeAt(int position) const{
    return permutation[position];
}


void BookEmbeddedGraph::addEdgeToPage(Edge& e, const int pageNo){
    attr.label(e) = std::to_string(pageNo);
    pages[pageNo].insert(e);
}

void BookEmbeddedGraph::generateBuckets(){


    startBuckets.clear();
    endBuckets.clear();

    startBuckets.reserve(getNpages()); // ta  startBuckets kai endBuckets
    endBuckets.reserve(getNpages());   // exoun ena vector<Bucket> (typedef "Buckets")
                                       // gia kathe selida. Afto to vector me ti seira
                                       // tou exei ena bucket gia kathe koryfi
                                       // to opoio einai ena set apo edges
                                       // pou periexei tis akmes
                                       // pou ksekinane apo ekeini tin koryfh (startBuckets)
                                       // i ftanoun se ekeini tin koryfh    (endBuckets)

    for (Page p : pages) {
        Bucket b1,b2;
        Buckets pageStart(numberOfNodes(),b1 );
        Buckets pageEnd(numberOfNodes(),b2);


        //pageStart.reserve(numberOfNodes());
        //pageEnd.reserve(numberOfNodes());





        for (Edge e : p) {
            int i=e->source()->index();
            int j=e->target()->index();
            pageStart[i].insert(e);
            pageEnd[j].insert(e);
        }

        startBuckets.push_back(pageStart);
        endBuckets.push_back(pageEnd);
    }

    bucketsNeedToBeGenerated = false;

}

void BookEmbeddedGraph::calculateCrossings(const std::vector<int> pagesChanged){
    if (bucketsNeedToBeGenerated)
        generateBuckets();

    //if this was called with no pages given:
    ncrossings=0; //this is new (kosmas)
    for (Buckets bs : startBuckets){//bs are a single page's buckets (diladi foreach page)
        for (Bucket b : bs){        // for each node
            for (Edge e : b) {      // for each edge starting from the node
                int startNode = e->source()->index();  //
                int endNode = e->target()->index();

                int leftNode = (startNode < endNode )? startNode : endNode;
                int rightNode = (startNode < endNode )? endNode : startNode;
;

                for (int i = leftNode+1; i < rightNode; i++){  //for each node v between the startnode and end node
                    //if (bs[i].size() > 0) {
                        for (Edge candidate : bs[i]){        //for every edge ("candidate") starting from v
                            if ((candidate->target()->index() > rightNode) || (candidate->target()->index() < leftNode)){
                                crossings[e].insert(candidate);
                                ncrossings++;
                            }
                        }
                    //}
                }
            }
        }
    }

    cout << "calculated number of crossings: " << ncrossings << endl;
}

bool BookEmbeddedGraph::readGML(std::string &fileName){
    if(! ogdf::GraphIO::readGML(attr,g,fileName)) return false;

    permutation.clear();
    permutation.reserve(numberOfNodes());
    Node v;
    forall_nodes(v,g){
        permutation[getPosition(v)] = v;
    }

    pages.clear();
    Edge e;
    forall_edges(e,g){
        while(getPageNo(e) >= getNpages()) addPage();
        pages[getPageNo(e)].insert(e);
    }

    bucketsNeedToBeGenerated=true;
    ncrossings=0;
    calculateCrossings();
    return true;
}

void BookEmbeddedGraph::updatePermutation(int originalIndex, int finalPos) {
    //int originalIndex = movedNode->getIndex();
    Node originalNode = permutation[originalIndex];

    if (originalIndex < finalPos) { //moved to the right
        for (int i = originalIndex; i < finalPos; i++){
            //attr.label(permutation[i + 1]) = attr.label(permutation[i]);
            permutation[i] = permutation[i + 1];
            attr.label(permutation[i]) = std::to_string(i);

        }

        permutation[finalPos] = originalNode;
        attr.label(permutation[finalPos]) = std::to_string(finalPos);

        //return true;
    }else if (originalIndex > finalPos){ //moved to the left
        for (int i = originalIndex; i > finalPos; i--){
            //attr.label(permutation[i + 1]) = attr.label(permutation[i]);
            permutation[i] = permutation[i - 1];
            attr.label(permutation[i]) = std::to_string(i);

        }

        permutation[finalPos] = originalNode;
        attr.label(permutation[finalPos]) = std::to_string(finalPos);

        //return true;
    }

    //return false; didn't move, don't need to redraw
}

//void BookEmbeddedGraph::calculateCrossings(const int pages&){
//TODO
//}

// BookEmbeddedGraph end

/******************************************** BCTree Implementation ********************************************/

BCTree::BCTree(Graph &g) : originalGraph(g.toOGDF()), ogBCT(originalGraph,true){


}

Node BCTree::firstNode(bool biconnected){
    if (!biconnected)
        return ogBCT.bcTree().firstNode();

    return ogBCT.auxiliaryGraph().firstNode();
}

Node BCTree::lastNode(bool biconnected){
    if (!biconnected)
        return ogBCT.bcTree().lastNode();

    return ogBCT.auxiliaryGraph().lastNode();

}



int BCTree::numberOfNodes(bool biconnected) {
    if (!biconnected)
        return ogBCT.bcTree().numberOfNodes();

    return ogBCT.auxiliaryGraph().numberOfNodes();

}

int BCTree::numberOfBComps () {
    return ogBCT.numberOfBComps();
}

int BCTree::numberOfCComps () {
    return ogBCT.numberOfCComps();
}

onode_type BCTree::typeOfVertexInOriginalGraph(Node n) {
    if (ogBCT.typeOfGNode(n) == ogdf::BCTree::Normal)
        return ONODETYPE_NORMAL;

    return ONODETYPE_CUT;
}

bnode_type BCTree::typeOfVertexInBCTree(Node n) {
    if (ogBCT.typeOfBNode(n) == ogdf::BCTree::BComp)
        return BNODETYPE_BCOMP;

    return BNODETYPE_CCOMP;
}

// BCTree end

bool edgeCmp (const Edge &e1, const Edge &e2) { //kosmas: does this work when we have switched to another permutation??
    int sourceLabel1 = e1->source()->index();
    int sourceLabel2 = e2->source()->index();
    int targetLabel1 = e1->target()->index();
    int targetLabel2 = e2->target()->index();

    return ((sourceLabel1 != sourceLabel2) ? (sourceLabel1 < sourceLabel2) : (targetLabel1 > targetLabel2));
}

bool edgeCmp2 (const Edge e1, const Edge e2){



    //int sourcePos = std::stoi(attr.label(v));
    return true;
}
