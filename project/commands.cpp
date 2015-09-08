#include "commands.h"
#include "pagescene.h"
#include "graphscene.h"

EdgeMoveCommand::EdgeMoveCommand(Edge &e, int fromPage, int toPage, std::vector<PageView*> *pageViews, BookEmbeddedGraph* g, GraphScene* gs, QLabel* crossings){
    setText("Move edge");
    edge = e;
    from = fromPage;
    to = toPage;
    graph = g;
    views = pageViews;
    graphScene = gs;
    crossingsIndicator = crossings;
}

void EdgeMoveCommand::moveEdge(int fromPage, int toPage){
    graph->moveToPage(edge,toPage);

    PageScene* fromScene = views->at(fromPage)->scene();
    PageScene* toScene = views->at(toPage)->scene();

    fromScene->removeEdge(edge);
    toScene->addEdge(edge);
    fromScene->update();
    toScene->update();

    graphScene->removeEdge(edge);
    graphScene->addEdge(edge, toScene->getColour());

    crossingsIndicator->setNum(graph->getNcrossings());
}

void EdgeMoveCommand::redo(){
    moveEdge(from, to);
}

void EdgeMoveCommand::undo(){
    moveEdge(to, from);
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


NodeMoveCommand::NodeMoveCommand(Node& v, BookEmbeddedGraph* g, int newIdx, std::vector<PageView *> *views){
    node = v;
    from = g->getPosition(v);
    to = newIdx;
    graph = g;
    pageViews = views;
    setText("Move node");
}

void NodeMoveCommand::redo(){
    move(from,to);
}

void NodeMoveCommand::undo(){
    move(to,from);
}

void NodeMoveCommand::move(int fromPosition, int toPosition){
    graph->moveTo(node,toPosition);
    for(PageView* view : *pageViews){
        view->scene()->redraw(graph);
    }
}
