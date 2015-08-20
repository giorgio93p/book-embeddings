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
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    std::vector<QGraphicsView*> pageViews;
    BookEmbeddedGraph* mainGraph;
    std::string currentFile;


public:
    explicit MainWindow(QWidget *parent = 0);
    bool openBookEmbeddedGraph(std::string filename);
    ~MainWindow() = default;
    //vector<Page> *pages;(
    BookEmbeddedGraph* getMainGraph() const { return mainGraph;}


private:
    void drawBookEmbeddedGraph();
    void add_page_drawing(int p);

public slots:
    void on_actionAddPage_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

    void on_edge_selected(Edge &);
    void on_edge_deselected(Edge &);
    void on_node_selected(Node &);
    void on_node_deselected(Node &);

    void move_edge(Edge&);
    void on_crossings_changed(std::vector<int>);

signals:
    void number_of_nodes_changed(int i);
    void number_of_edges_changed(int i);
    void number_of_pages_changed(int i);
    void crossings_changed(std::vector<int>);
    void planarity_changed(bool b);
};

#endif
