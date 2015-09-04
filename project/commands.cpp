#include "commands.h"
#include "pagescene.h"
#include "graphscene.h"

EdgeMoveCommand::EdgeMoveCommand(Edge &e, PageScene* from, PageScene* to, BookEmbeddedGraph* g, GraphScene* gs, QLabel* crossings){
    setText("Move edge");
    edge = e;
    fromScene = from;
    toScene = to;
    graph = g;
    graphScene = gs;
    crossingsIndicator = crossings;
}

void EdgeMoveCommand::moveEdge(bool reverse){
    graph->moveToPage(edge,(reverse?fromScene:toScene)->pageNumber());

    if(!reverse){
        fromScene->removeEdge(edge);
        toScene->addEdge(edge);
    }else{
        toScene->removeEdge(edge);
        fromScene->addEdge(edge);
    }
    fromScene->update();
    toScene->update();

    graphScene->removeEdge(edge);
    graphScene->addEdge(edge, (reverse?fromScene:toScene)->getColour());

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
