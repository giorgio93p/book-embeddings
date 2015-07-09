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
    vector<QGraphicsView*> views;
    BookEmbeddedGraph mainGraph;

public:
    explicit MainWindow(QWidget *parent = 0);
    void openGraph(char* filename);
    void drawGraph(Graph& g);
    void drawBookEmbeddedGraph(BookEmbeddedGraph& g);

    ~MainWindow();
    //vector<Page> *pages;

public slots:

    void on_actionOpen_triggered();
    void on_actionOpen_2_triggered() {on_actionOpen_triggered();};
    void on_actionSave_as_triggered();
};

