#include "mainwindow.h"
#include "graphs.h"
#include <QApplication>

#include <iostream>

int main(int argc, char *argv[])
{
    BookEmbeddedGraph mainGraph = BookEmbeddedGraph();

    std::vector<Node> nodes;
    for (int i=0; i<5; i++) nodes.push_back(mainGraph.addNode());
    Edge e;
    e = mainGraph.addEdge(nodes[0],nodes[1],0);
    e = mainGraph.addEdge(nodes[1],nodes[2],0);
    e = mainGraph.addEdge(nodes[1],nodes[3],0);
    e = mainGraph.addEdge(nodes[1],nodes[4],0);
    mainGraph.addPage();
    e = mainGraph.addEdge(nodes[0],nodes[4],1);
    e = mainGraph.addEdge(nodes[0],nodes[3],1);
    e = mainGraph.addEdge(nodes[0],nodes[2],1);
    mainGraph.addPage();
    e = mainGraph.addEdge(nodes[2],nodes[3],2);
    e = mainGraph.addEdge(nodes[3],nodes[4],2);
    mainGraph.addPage();
    e = mainGraph.addEdge(nodes[2],nodes[4],3);

    QApplication a(argc, argv);
    MainWindow w;
    w.drawBookEmbeddedGraph(mainGraph);
    w.show();

    return a.exec();
}
