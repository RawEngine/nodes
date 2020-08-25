
#include <QMainWindow>
#include <QGraphicsScene>

#include "MainWindow.hpp"
#include "GraphNode.hpp"

MainWindow::MainWindow()
    : QMainWindow(nullptr)
    , mpScene(new QGraphicsScene(this))
{
    mUI.setupUi(this);

    mUI.graphicsView->setScene(mpScene);
    mUI.graphicsView->setRenderHints(QPainter::Antialiasing);

    // TEST.
    mpScene->addItem(new GraphNode("Test"));
}

MainWindow::~MainWindow()
{ }
