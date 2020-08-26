
#pragma once

#include "ui_MainWindow.h"

class GraphNode;

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    GraphNode* AddNode(const QString& rName, const QPointF& rPosition);

private:

    bool ReadConfigs(bool& rIsMaximized);
    void WriteConfigs();

private Q_SLOTS:

    void Slot_BtnAddNode();

private:   

    Ui::MainWindow mUI;

    QVector<GraphNode*> mGraphNodes;

    QGraphicsScene* mpScene = nullptr;
};
