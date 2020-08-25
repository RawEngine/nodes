
#include "PCH.hpp"

#include <QMainWindow>

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
    auto pGraphNode = new GraphNode(*mpScene, "Test");

}

MainWindow::~MainWindow()
{ }
