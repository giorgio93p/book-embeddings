#include "commands.h"
#include "pagescene.h"
#include "graphscene.h"

EdgeMoveCommand::EdgeMoveCommand(Edge &e, int fromPage, int toPage, std::vector<QGraphicsView*> *pageViews, BookEmbeddedGraph* g, GraphScene* gs, QLabel* crossings){
    setText("Move edge");
    edge = e;
    from = fromPage;
    to = toPage;
    graph = g;
    views = pageViews;
    graphScene = gs;
    crossingsIndicator = crossings;
}

void EdgeMoveCommand::moveEdge(bool reverse){



    graph->moveToPage(edge,reverse?from:to);

    PageScene* fromScene = (PageScene*)(views->at(reverse?to:from)->scene());
    PageScene* toScene = (PageScene*)(views->at(reverse?from:to)->scene());

    fromScene->removeEdge(edge);
    toScene->addEdge(edge);
    fromScene->update();
    toScene->update();

    graphScene->removeEdge(edge);
    graphScene->addEdge(edge, toScene->getColour());

    crossingsIndicator->setNum(graph->getNcrossings());
}

void EdgeMoveCommand::redo(){
    moveEdge(false);
}

void EdgeMoveCommand::undo(){
    moveEdge(true);
}


PageAddCommand::PageAddCommand(BookEmbeddedGraph* g, MainWindow *w){
    window = w;
    graph = g;
    setText("Add page");
}

void PageAddCommand::redo(){
    graph->addPage();
    window->add_page_drawing(graph->getNpages()-1);
}

void PageAddCommand::undo(){
    int pageNo = graph->getNpages()-1;
    graph->removePage(pageNo);
    window->remove_page_drawing(pageNo);
}


PageRemoveCommand::PageRemoveCommand(int p, BookEmbeddedGraph* g, MainWindow *w){
    window = w;
    page = p;
    graph = g;
    setText("Remove page");
}

void PageRemoveCommand::redo(){
    graph->removePage(page);
    window->remove_page_drawing(page);
}

void PageRemoveCommand::undo(){
    graph->addPage(page);
    window->add_page_drawing(page);
}


NodeMoveCommand::NodeMoveCommand(Node v, MainWindow *w){
    window = w;
    node = v;
    setText("Move node");
}

void NodeMoveCommand::redo(){

}

void NodeMoveCommand::undo(){

}

void NodeMoveCommand::move(int from, int to){

}
