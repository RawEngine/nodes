
#include "PCH.hpp"

#include <QMainWindow>

#include "Headers/MainWindow.hpp"
#include "Headers/NodeDialog.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp" // TEMP?
#include "Headers/GraphGizmo.hpp"   // TEMP?

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
#if 1
        auto pNodeA = this->AddNode("Test A", QPointF(-200, -200));
        auto pNodeB = this->AddNode("Test B", QPointF(100, 150));

        // TEMP.
        {
            pNodeA->AddInputPort(GraphPortType::Integer);
            pNodeA->AddInputPort(GraphPortType::Float);
            pNodeA->AddInputPort(GraphPortType::Integer);

            pNodeA->AddOutputPort(GraphPortType::Float);
            pNodeA->AddOutputPort(GraphPortType::Integer);
        }
        {
            pNodeB->AddInputPort(GraphPortType::Integer);
            pNodeB->AddInputPort(GraphPortType::Float);
            pNodeB->AddInputPort(GraphPortType::Integer);

            pNodeB->AddOutputPort(GraphPortType::Float);
            pNodeB->AddOutputPort(GraphPortType::Integer);
        }
/*
        // TEMP.
        auto pNodeSlotA = pNodeA->mInputPorts.at(0);
        auto pNodeSlotB = pNodeB->mInputPorts.at(1);

        auto pTestGizmo = new GraphGizmo(pNodeSlotA, pNodeSlotB);
        mpScene->addItem(pTestGizmo);
*/
#endif
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
    auto pNode = new GraphNode(rName, rPosition);

    mpScene->addItem(pNode);

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

    const auto jsonNodesArrays(jsonScene["nodes"].toArray());

    for (const auto& rJsonNode : jsonNodesArrays)
    {
        const auto jsonNodeObject(rJsonNode.toObject());

        QString nodeName;
        QPointF nodePosition;

        if (!jsonNodeObject["name"].isUndefined())
            nodeName = jsonNodeObject["name"].toString();

        if (!jsonNodeObject["position"].isUndefined())
        {
            const auto list(jsonNodeObject["position"].toString().split(", "));
            nodePosition.setX(list.at(0).toDouble());
            nodePosition.setY(list.at(1).toDouble());
        }

        auto pNode = this->AddNode(nodeName, nodePosition);

        // Read Inputs.
        if (!jsonNodeObject["inputs"].isUndefined())
        {
            const auto jsonInputsArray(jsonNodeObject["inputs"].toArray());

            for (const auto& rJsonInput : jsonInputsArray)
            {
                const auto jsonInputObject(rJsonInput.toObject());

                if (!jsonInputObject["data_type"].isUndefined())
                {
                    const auto dataTypeStr(jsonInputObject["data_type"].toString());

                    pNode->AddInputPort(GraphNodeSlot::DataTypeFromString(dataTypeStr));
                }
            }
        }

        // Read Outputs.
        // TODO
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

                // Write input ports.
                if (pNode->IsUsingInputs())
                {
                    QJsonArray jsonPorts;

                    for (const auto p : pNode->GetInputs())
                    {
                        QJsonObject jsonPort;
                        jsonPort["data_type"] = p->DataTypeToString();
                        jsonPorts.append(jsonPort);
                    }
                    
                    jsonNode["inputs"] = jsonPorts;
                }

                // Write input ports.
                if (pNode->IsUsingOutputs())
                {
                    QJsonArray jsonPorts;

                    for (const auto p : pNode->GetInputs())
                    {
                        QJsonObject jsonPort;
                        jsonPort["data_type"] = p->DataTypeToString();
                        jsonPorts.append(jsonPort);
                    }

                    jsonNode["outputs"] = jsonPorts;
                }

                jsonNodes.append(jsonNode);
            }

            jsonScene["nodes"] = jsonNodes;
        }

        QJsonDocument jsonDocument(jsonScene);

    //  file.write(jsonDocument.toBinaryData());
        file.write(jsonDocument.toJson(/*QJsonDocument::Compact*/));
    }
}

void MainWindow::Slot_BtnAddNode()
{
    NodeDialog dialog;

    if (dialog.exec() == QDialog::Accepted)
    {
        int numInputs = dialog.GetNumInputs();
        int numOutputs = dialog.GetNumOutputs();

        auto pNode = this->AddNode(dialog.GetNodeName(), QPointF(0.0, 0.0));
    }
}
