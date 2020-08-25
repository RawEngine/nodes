
#pragma once

#include "ui_MainWindow.h"

class GraphNode;

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    void AddNode(const QString& rName, const QPointF& rPosition);

private:

    bool ReadConfigs(bool& rIsMaximized);
    void WriteConfigs();

private:   

    Ui::MainWindow mUI;

    QGraphicsScene* mpScene;

    QVector<GraphNode*> mGraphNodes;
};
