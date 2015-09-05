/*
 *
 * Main window class. This is the class of our mainwindow object, and it
 * derives from the QMainWindow class AND from the Ui::MainWindow class.
 * This is a bit complicated:we load the ui, so  deriving from Ui::MainWindow is the way
 * we connect our application to our form.
 * (The form is the document that describes every change we have made
 * with qtdesigner, and it is named 'mainwindow.ui')


 * This is the "control center" of our application.
 * In the constructor we load the interface we have designed
 * with QtDesigner, using the setupUi method. Now we can access each object
 * we have added to the designer by its name, as if it was a field of the
 * mainwindow class. That's all about the designer, we don't need to know
 * much more about that.


 *
 *
 *
 */








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
    //this holds a number of available colors.
    //we use it to control the colours in pages
    //see [1]

    std::vector<QGraphicsView*> pageViews;
    //this holds the views that show
    //each pagescene. A pagescene
    //simply displays a page.


    BookEmbeddedGraph* mainGraph;
    //a pointer to the mainGraph object.
    //this is the graph we read from the gml
    //file, along with the information about the
    //pages, and more.
    std::string currentFile;
    //full name of the file we have opened.
    //might be useful, might not.
    AuxiliaryGraph* auxiliaryGraph;
    //this contains a vector of graphs,
    //or to be more precise, a vector BiconnectedComponent
    //objects. These derive from the Graph class, and are
    //the biconnected components of mainGraph. They are
    //completely independent, and they can 'communicate'
    //with the graph through mappings from each node of
    //theirs to the corresponding node in mainGraph.

    QUndoGroup* commandHistory;

    //commandHistory is a stack of recently made changes.
    //we use it to undo/redo actions performed on our graph,


    bool wholeGraphMode;
    //this flag indicates if our pages are displaying the whole graph
    //or just a biconnectedComponent of it.

    BiconnectedComponent* currBC;

    //this is the biconnected component under examination, if
    //of course wholeGraphMode is false.

public:
    explicit MainWindow(QWidget *parent = 0);
    bool openBookEmbeddedGraph(std::string filename);

    ~MainWindow() = default;
    //vector<Page> *pages;(
    BookEmbeddedGraph* getMainGraph() const { return mainGraph;}
    void enableRedraw();

    void redrawPages();

    void remove_page_drawing(int page);
    void add_page_drawing(int p);
    bool isWholeGraphModeOn() {
        return wholeGraphMode;
    }


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
    void on_node_selected(Node &, int);
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
