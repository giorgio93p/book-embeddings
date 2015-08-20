#include <QMainWindow>
#include <QGridLayout>
#include "ui_mainwindow.h"
#include "graphscene.h"
#include "graphs.h"

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
    BCTree *bctree;


public:
    explicit MainWindow(QWidget *parent = 0);
    bool openBookEmbeddedGraph(std::string filename);
    ~MainWindow() = default;
    //vector<Page> *pages;(
    BookEmbeddedGraph* getMainGraph() const { return mainGraph;}



private:
    void drawBCTree();
    void drawBookEmbeddedGraph();
    void add_page_drawing(int p);

public slots:
    void on_actionAddPage_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

signals:
    void number_of_nodes_changed(int i);
    void number_of_edges_changed(int i);
    void number_of_pages_changed(int i);
    void crossings_changed(int i);
    void planarity_changed(bool b);
};

