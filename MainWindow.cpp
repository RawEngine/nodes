
#include "PCH.hpp"

#include <QMainWindow>

#include "MainWindow.hpp"
#include "GraphNode.hpp"

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


    //--------------------------------------------------
    // TEMP location..
    {
        QFile file(NodesFileName);

        if (file.open(QIODevice::ReadOnly))
        {
            const auto fileData(file.readAll());

            QJsonDocument jsonDocument(QJsonDocument::fromJson(fileData));

            auto jsonScene = jsonDocument.object();

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
        }
        else
        {
            // TEST.
            this->AddNode("Test A", QPointF(-200, -200));
            this->AddNode("Test B", QPointF(100, 150));
        }
    }
}

MainWindow::~MainWindow()
{
    QFile file(NodesFileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject jsonScene;
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

void MainWindow::AddNode(const QString& rName, const QPointF& rPosition)
{
    mGraphNodes.append(new GraphNode(*mpScene, rName, rPosition));
}
