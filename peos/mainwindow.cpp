#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphscene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("2D Painting on Native and OpenGL Widgets");



    std::pair<int,int> p1(1,1);
    std::vector< std::pair<int,int> > fakeEdges(5,p1);
    std::vector<int> fakePerm(5,5);
    GraphScene *scene = new GraphScene(5,5,fakeEdges,fakePerm);


    setupUi(this);
    graphicsView->setScene(scene);

    QGridLayout *layout = new QGridLayout;
    //layout->addWidget(native, 0, 0);





}

MainWindow::~MainWindow()
{
    //delete ui;
}
