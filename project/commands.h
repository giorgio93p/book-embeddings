#ifndef COMMANDS_H
#define COMMANDS_H
#include <QUndoCommand>
#include "graphs.h"
#include "mainwindow.h"

class EdgeMoveCommand : public QUndoCommand{

    Edge edge;
    int from;
    int to;
    std::vector<PageView*>* views;
    GraphScene* graphScene;
    QLabel* crossingsIndicator;
    BookEmbeddedGraph* graph;

public:
    EdgeMoveCommand(Edge &e, int fromPage, int toPage, std::vector<PageView *> *pageViews, BookEmbeddedGraph* g, GraphScene* gs, QLabel* crossings);
    void undo();
    void redo();
private:
    void moveEdge(int,int);
};

class PageAddCommand : public QUndoCommand{
    MainWindow* window;
    BookEmbeddedGraph* graph;

public:
    PageAddCommand(BookEmbeddedGraph *g, MainWindow* w);
    void undo();
    void redo();
};

class PageRemoveCommand : public QUndoCommand{
    MainWindow* window;
    BookEmbeddedGraph* graph;
    int page;

public:
    PageRemoveCommand(int p, BookEmbeddedGraph *g, MainWindow* w);
    void undo();
    void redo();
};

class NodeMoveCommand : public QUndoCommand{
    int from;
    int to;
    Node node;
    BookEmbeddedGraph* graph;
    std::vector<PageView *>* pageViews;

public:
    NodeMoveCommand(Node& v, BookEmbeddedGraph*, int, std::vector<PageView *> *);
    void undo();
    void redo();
private:
    void move(int fromPosition, int toPosition);
};

#endif // COMMANDS_H
