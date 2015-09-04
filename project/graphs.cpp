#include "graphs.h"
#include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/FMMMLayout.h>

Graph::Graph():g2(g1)
{
    attr = ogdf::GraphAttributes(g1,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    attr.setDirected(false);



}

void Graph::setDefaultNumbering() {

    ogdf::Graph& g = (use_g2) ? g2:g1;

    //this methods sets the edge and node numbering tables to the default numbering
    std::unordered_map<Node,int> n_to_i;
    std::unordered_map<int,Node> i_to_n;
    std::unordered_map<Edge,int> e_to_i;
    std::unordered_map<int,Edge> i_to_e;


    Node n;
    int i=0;
    forall_nodes(n,g) {

        n_to_i[n] = i;
        i_to_n[i] = n;
        i++;

    }

    i=0;
    Edge e;
    forall_edges(e,g)
    {
        e_to_i[e] = i;
        i_to_e[i] = e;
        i++;
    }

    setNumbering(e_to_i,n_to_i,i_to_n,i_to_e);

}

Graph::Graph(Graph* graph)
    :g2(g1)
{
    g1 = ogdf::Graph(graph->toOGDF());
    attr.setDirected(false);
    attr = ogdf::GraphAttributes(g1,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    setDefaultNumbering();
    use_g2=false;

}


void Graph::setNumbering(std::unordered_map<Edge,int> e_to_num,
                  std::unordered_map<Node,int> n_to_num,
                  std::unordered_map<int,Node> num_to_n,
                  std::unordered_map<int,Edge> num_to_e) {
    e_to_int=e_to_num;
    n_to_int=n_to_num;
    int_to_n=num_to_n;
    int_to_e=num_to_e;

}

Graph::Graph(ogdf::Graph graph): g2(g1){
    use_g2=false;
    g1=graph;
    attr = ogdf::GraphAttributes(g1);//no value given
    setDefaultNumbering();
}

Graph::Graph(ogdf::Graph& graph,bool weareusingreferences):g2(graph){
    use_g2=true;

    attr = ogdf::GraphAttributes(g2,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    setDefaultNumbering();
}

Node Graph::addNode(){

    ogdf::Graph& g = (use_g2) ? g2:g1;


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
    return blPlanarityCheck.isPlanar(toOGDFval());
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
    use_g2=false;
    ogdf::Graph& g = (use_g2) ? g2:g1;

    //vertexOrder = NULL;

    Node n;
    int i=0;
    forall_nodes(n,g){
        attr.label(n) = std::to_string(i);
        i++;
    }

    pages = std::vector<Page>();
    addPage();

    Edge e;

    forall_edges(e,g) addEdgeToPage(e,0);

    permutation = std::vector<Node>(numberOfNodes());

    bucketsNeedToBeGenerated = true;
    crossings = std::unordered_map<Edge,std::unordered_set<Edge> >();
    ncrossings = 0;
    calculateCrossings(); //commented out because it wasnt working (segfault)
    std::cout << "BookEmbeddedGraph created" << std::endl;
}

BookEmbeddedGraph::BookEmbeddedGraph() : BookEmbeddedGraph(new Graph()) {}

BookEmbeddedGraph::BookEmbeddedGraph(ogdf::Graph graph){
    use_g2=false;
    ogdf::Graph& g = (use_g2) ? g2:g1;




    g=graph;
    pages = std::vector<Page>();
    attr = ogdf::GraphAttributes(g,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    attr.setDirected(false);
    attr.initAttributes(ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeLabel);
    addPage();

    Node v;
    int i=0;
    forall_nodes(v,g){
        attr.label(v) = std::to_string(i);
        i++;
    }

    Edge e;
    forall_edges(e,g) addEdgeToPage(e,0);
    permutation = std::vector<Node>(numberOfNodes());

    bucketsNeedToBeGenerated = true;
    crossings = std::unordered_map<Edge,std::unordered_set<Edge> >();
    ncrossings = 0;
    calculateCrossings();
    std::cout << "BookEmbeddedGraph created" << std::endl;


}

BookEmbeddedGraph::BookEmbeddedGraph(ogdf::Graph& graph, bool weareusingreferences)
    :Graph(graph,weareusingreferences)
{


    cout << "entered BookEmbeddedGraph constructor (with references)" << endl;
    //use_g2=true;
    //g2=graph;
    //attr = ogdf::GraphAttributes(g2,ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);
    //already taken care of by the graph constructor

    const ogdf::Graph& g = (use_g2) ? g2 : g1;

    pages = std::vector<Page>();
    attr.setDirected(false);
    attr.initAttributes(ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::edgeLabel);
    addPage();

    Node v;
    int i=0;
    forall_nodes(v,g){
        attr.label(v) = std::to_string(i);
        i++;
        //cout << "DEBUG: graph newBC has nodes";
    }

    cout << endl;

    Edge e;
    forall_edges(e,g) addEdgeToPage(e,0);
    permutation = std::vector<Node>(numberOfNodes());

    bucketsNeedToBeGenerated = true;
    crossings = std::unordered_map<Edge,std::unordered_set<Edge> >();
    ncrossings = 0;
    calculateCrossings();

    forall_nodes(v,g){

        cout << "DEBUG: graph newBC has nodes";
    }
    std::cout << "exiting BookEmbeddedGraph Constructor (with references)" << std::endl;


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

Edge BookEmbeddedGraph::firstEdge() {
    ogdf::Graph& g = (use_g2) ? g2:g1;

    return g.firstEdge();
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
    ogdf::Graph& g = (use_g2) ? g2:g1;


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

BCTree::BCTree(Graph &g) : originalGraph(g.toOGDF()),ogBCT(originalGraph,true),
auxiliaryGraph(ogBCT.auxiliaryGraph()) {


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

const std::unordered_map<Node,Node> BCTree::generateNodeMapping(){

    const ogdf::Graph& aux = getAuxiliaryGraph();
    agN_to_mgN = std::unordered_map<Node,Node>();

    Node n;
    forall_nodes(n,aux) {

        agN_to_mgN[n]= ogBCT.original(n);

    }

    //const std::unordered_map<Node,Node>& ret = agN_to_mgN; //we create a const reference to the map we
                                                           //just created.
    return agN_to_mgN;


}

const std::unordered_map<Edge,Edge> BCTree::generateEdgeMapping(){

    const ogdf::Graph& aux = getAuxiliaryGraph();
    agE_to_mgE = std::unordered_map<Edge,Edge>();

    Edge e;
    forall_edges(e,aux) {

        agE_to_mgE[e]= ogBCT.original(e);

    }

    //const std::unordered_map<Edge,Edge>& ret = agE_to_mgE; //we create a const reference to the map we
                                                           //just created.
    return agE_to_mgE;


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
