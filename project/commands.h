#ifndef COMMANDS_H
#define COMMANDS_H
#include <QUndoCommand>
#include "graphs.h"
#include "mainwindow.h"

class EdgeMoveCommand : public QUndoCommand{

    Edge edge;
    PageScene* fromScene;
    PageScene* toScene;
    GraphScene* graphScene;
    QLabel* crossingsIndicator;
    BookEmbeddedGraph* graph;

public:
    EdgeMoveCommand(Edge &e, PageScene* fromScene, PageScene* to, BookEmbeddedGraph* g, GraphScene* gs, QLabel* crossings);
    void undo();
    void redo();
private:
    void moveEdge(bool);
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
    MainWindow* window;
    Node node;

public:
    NodeMoveCommand(Node v, MainWindow* w);
    void undo();
    void redo();
private:
    void move(int from, int to);
};

#endif // COMMANDS_H
