#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "graphscene.h"
#include "graphs.h"
#include <vector>
#include <QMainWindow>
#include <QGridLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <QUndoGroup>
#include <ogdf/internal/planarity/ConnectedSubgraph.h>

#include "biconnectedcomponent.h"
#include "auxiliarygraph.h"

#include "colourcloset.h"


class PageScene; //forward declaration

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    ColourCloset colourCloset;
    std::vector<QGraphicsView*> pageViews;
    BookEmbeddedGraph* mainGraph;
    std::string currentFile;
    AuxiliaryGraph* auxiliaryGraph;
    BCTree *bctree;
    std::vector<BiconnectedComponent*> biconnectedComponents;
    QUndoGroup* commandHistory;

    bool wholeGraphMode;
    BiconnectedComponent* currBC;

public:
    explicit MainWindow(QWidget *parent = 0);
    bool openBookEmbeddedGraph(std::string filename);

    ~MainWindow() = default;
    //vector<Page> *pages;(
    BookEmbeddedGraph* getMainGraph() const { return mainGraph;}
    void enableRedraw();

    void redrawPages();

    //pitsi added
    void remove_page_drawing(int page);
    void add_page_drawing(int p);


private:
    void drawBCTree();
    void drawBookEmbeddedGraph();

    void findBiconnectedComponentsOfMainGraph();


    void deselectEverythingInAllPagesBut(int page = -1);

public slots:
    void on_actionAddPage_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionRedraw_triggered();


    void on_edge_selected(Edge &);
    void on_edge_deselected(Edge &);
    void on_node_selected(Node &);
    void on_node_deselected(Node &);

    void move_edge(Edge&);
    void on_remove_page(int);
    void on_crossings_changed(std::vector<int>);

    void loadBC(BiconnectedComponent*); //this should locate the bc given node is in and
                           //do stuff.
                           // node n is a part of bctree graph.
                           // possibly we can find an ogdf::bctree method
                           // that gives us the original graph node.
                           // then we have to map the original graph node to
                           // the bookembedding it belongs to.
                           // in order to do that, we have to search in the mapping table
                           // of every bcomponent. This takes time, so we should also
                            // have a mappning from each node to the BiconnectedComponent that
                           // it corresponds to.

signals:

    void number_of_nodes_changed(int i);
    void number_of_edges_changed(int i);
    void crossings_changed(std::vector<int>);
    void planarity_changed(QString);
};

#endif
