#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphscene.h"

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>



class Edge;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{


    std::vector< IntPair > edges,edges2,edges3,edges4;
    edges.push_back(IntPair(0,1));
    edges.push_back(IntPair(1,2));
    edges.push_back(IntPair(1,3));
    edges.push_back(IntPair(1,4));
    edges2.push_back(IntPair(0,4));
    edges2.push_back(IntPair(2,4));

    edges2.push_back(IntPair(2,3));

    edges2.push_back(IntPair(1,2));
    edges2.push_back(IntPair(0,4));

    edges3.push_back(IntPair(2,3));
    edges3.push_back(IntPair(0,2));
    edges3.push_back(IntPair(3,4));

    edges4.push_back(IntPair(2,4));


    std::vector<int> fakePerm(5,5);
    scene.push_back( new GraphScene(5,edges.size(),edges,fakePerm));
    scene.push_back( new GraphScene(5,edges2.size(),edges2,fakePerm));
    scene.push_back( new GraphScene(5,edges3.size(),edges3,fakePerm));
    scene.push_back( new GraphScene(5,edges4.size(),edges4,fakePerm));


    QGraphicsView * p;

    setupUi(this);
    setWindowTitle("P.E.O.S.");

    graphicsView->setScene(scene.at(0));
    graphicsView_2->setScene(scene.at(1));
    graphicsView_3->setScene(scene.at(2));
    graphicsView_4->setScene(scene.at(3));

    QGridLayout *layout = new QGridLayout;
}

MainWindow::~MainWindow()
{
    //delete ui;
}

/*
void MainWindow::openGraph(char* filename) {

    //graph = readGraphFromFileInGmlForm(char* filename)
    //(LEDA)
    drawGraph();

}

void MainWindow::drawGraph(){

}






void MainWindow::MoveEdge(Edge e,int pg1,int pg2) {

   Graph.move(e,pg1,pg2);
   scene


}
*/

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("GraphModellingLanguage files (*.gml);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        std::string fileNameStr = fileName.toUtf8().constData();//PROSOXI PAIZEI NA MIN PAIZEI PADOU
        bool readSuccessful = ogdf::GraphIO::readGML(this->mainGraph,fileNameStr);
        if (readSuccessful){
            //std::cout << "Read Successful!!!!!" << std::endl;
            std::cout << "Number of nodes in read graph ==" << mainGraph.numberOfNodes() << endl;
        }

        //ogdf::GridLayout mylayout1(mainGraph);
        //mylayout1.init()

        file.close();
    }
}


