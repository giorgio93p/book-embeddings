

//#include <LEDA/graph.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Node /*: public node*/{
	
};

class Edge /*: public edge*/{
   public:
	bool operator==(const Edge& e) const;
};

/**
 * This block of code is needed for hashing Edge objects
 **/
namespace std{
	template <>
    struct hash<Edge>{
        size_t operator()(const Edge& e) const{
			return 0;
            //return (/*first node hash*/ ^ /*second node hash*/ << 1)) >> 1);
        };
    };
}

class BookEmbeddedGraph;

class Graph /*: public graph*/ {
   public:
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
};

typedef std::unordered_set<Edge> Page;

class BookEmbeddedGraph : public Graph {
    std::vector<Page> pages;
    std::unordered_map<Edge,int> pageOfEdge; //this is used as a mapping from edges to pages
    
    std::unordered_map<Edge,std::unordered_set<Edge> > crossings;
    int ncrossings;
    
   public:
    void setPage(const Edge& e, const int newPage);


    //int drawEmbeddingPages(vector<int> pagenums,Graphic *result); //draw one or more pages in one image (using coloured edges)
    //int drawEmbeddingAllPages(Graphic *result);                   //draw all pages in one image (using coloured edges)
    // these 2 create a window/tab/whatever
    // (which they store as an Object of class "Graphic)
    // displaying several or all pages of our 
    // book embedding, using our graphics library(/ies).  
    // "Graphic" is not a specific class
    // but something that can  store the result
    // that should be passed on to the front end.
    
    
    std::vector<Page>* getPages();
    int getNpages();
    int getNcrossings();
    int getNcrossings(const Edge& e);
    std::unordered_set<Edge> getcrossings(const Edge& e);
    
    virtual ~BookEmbeddedGraph();
    
   private:
    void   calculateCrossings();
    void recalculateCrossings();
};
