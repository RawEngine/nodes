
#include "PCH.hpp"

#include <QMainWindow>

#include "MainWindow.hpp"
#include "GraphNode.hpp"
#include "GraphNodeSlot.hpp" // TEMP?
#include "GraphGizmo.hpp"   // TEMP?

// TEMP?
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

constexpr auto NodesFileName = "nodes.json";

MainWindow::MainWindow()
    : QMainWindow(nullptr)
    , mpScene(new QGraphicsScene(this))
{
    mUI.setupUi(this);

    mUI.graphicsView->setScene(mpScene);
    mUI.graphicsView->setRenderHints(QPainter::Antialiasing);
    mUI.graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Show main application window as "maximized" by default.
    bool isMaximized = true;

    // Try and read some configs from the Json file if it's available.
    // Else, create the "default/initial" scene.
    if (!this->ReadConfigs(isMaximized))
    {
        auto pNodeA = this->AddNode("Test A", QPointF(-200, -200));
        auto pNodeB = this->AddNode("Test B", QPointF(100, 150));

        // TEMP.
        auto pNodeSlotA = pNodeA->mInputSlots.at(0);
        auto pNodeSlotB = pNodeB->mInputSlots.at(1);

        auto pTestGizmo = new GraphGizmo(pNodeSlotA, pNodeSlotB);
        mpScene->addItem(pTestGizmo);
    }

    // Show the main window maximized (or not) depending on saved configurations.
    isMaximized ? QMainWindow::showMaximized() : QMainWindow::show();
}

MainWindow::~MainWindow()
{
    this->WriteConfigs();
}

GraphNode* MainWindow::AddNode(const QString& rName, const QPointF& rPosition)
{
    auto pNode = new GraphNode(*mpScene, rName, rPosition);

    mGraphNodes.append(pNode);

    return pNode;
}

bool MainWindow::ReadConfigs(bool& rIsMaximized)
{
    QFile file(NodesFileName);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    const auto fileData(file.readAll());

    QJsonDocument jsonDocument(QJsonDocument::fromJson(fileData));

    auto jsonScene = jsonDocument.object();

    if (!jsonScene["maximized"].isUndefined())
        rIsMaximized = jsonScene["maximized"].toBool();

    QJsonArray jsonNodes = jsonScene["nodes"].toArray();

    for (const auto& rJsonNode : jsonNodes)
    {
        const auto& rJsonNodeObject = rJsonNode.toObject();

        QString nodeName;
        QPointF nodePosition;

        if (!rJsonNodeObject["name"].isUndefined())
            nodeName = rJsonNodeObject["name"].toString();

        if (!rJsonNodeObject["position"].isUndefined())
        {
            const auto list(rJsonNodeObject["position"].toString().split(", "));
            nodePosition.setX(list.at(0).toDouble());
            nodePosition.setY(list.at(1).toDouble());
        }

        this->AddNode(nodeName, nodePosition);
    }

    return true;
}

void MainWindow::WriteConfigs()
{
    QFile file(NodesFileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject jsonScene;

        jsonScene["maximized"] = QMainWindow::isMaximized();

        {
            QJsonArray jsonNodes;

            for (auto pNode : mGraphNodes)
            {
                QJsonObject jsonNode;

                if (!pNode->GetName().isEmpty())
                    jsonNode["name"] = pNode->GetName();

                jsonNode["position"] = QString("%1, %2").arg(pNode->pos().x()).arg(pNode->pos().y());

                jsonNodes.append(jsonNode);
            }

            jsonScene["nodes"] = jsonNodes;
        }

        QJsonDocument jsonDocument(jsonScene);

    //  file.write(jsonDocument.toBinaryData());
        file.write(jsonDocument.toJson(/*QJsonDocument::Compact*/));
    }
}
