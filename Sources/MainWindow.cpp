
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
    mUI.graphicsView->setSceneRect(0, 0, 4096, 4096);

    // Show main application window as "maximized" by default.
    bool isMaximized = true;

    // Try and read some configs from the Json file if it's available.
    // Else, create the "default/initial" scene.
    if (!this->ReadConfigs(isMaximized))
    {
#if 1
        auto pNodeA = this->AddNode("Test A", QPointF(100, 150));
        auto pNodeB = this->AddNode("Test B", QPointF(450, 200));

        // TEMP.
        auto pNodeAPortI1 = pNodeA->AddInputPort(GraphPortDataType::Integer);
        auto pNodeAPortI2 = pNodeA->AddInputPort(GraphPortDataType::Float);
        auto pNodeAPortI3 = pNodeA->AddInputPort(GraphPortDataType::Integer);

        auto pNodeAPortO1 = pNodeA->AddOutputPort(GraphPortDataType::Float);
        auto pNodeAPortO2 = pNodeA->AddOutputPort(GraphPortDataType::Integer);

        //----------------------------------------------------

        auto pNodeBPortI1 = pNodeB->AddInputPort(GraphPortDataType::Integer);
        auto pNodeBPortI2 = pNodeB->AddInputPort(GraphPortDataType::Float);
        auto pNodeBPortI3 = pNodeB->AddInputPort(GraphPortDataType::Integer);

        auto pNodeBPortO1 = pNodeB->AddOutputPort(GraphPortDataType::Float);
        auto pNodeBPortO2 = pNodeB->AddOutputPort(GraphPortDataType::Integer);

        pNodeAPortO1->AddConnection(pNodeBPortI2);
        pNodeAPortO2->AddConnection(pNodeBPortI3);
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
            const auto jsonArray(jsonNodeObject["inputs"].toArray());

            for (const auto& rJsonValue : jsonArray)
            {
                const auto jsonInputObject(rJsonValue.toObject());

                if (!jsonInputObject["data_type"].isUndefined())
                {
                    const auto dataTypeStr(jsonInputObject["data_type"].toString());

                    pNode->AddInputPort(GraphNodeSlot::DataTypeFromString(dataTypeStr));
                }
            }
        }

        // Read Outputs.
        if (!jsonNodeObject["outputs"].isUndefined())
        {
            const auto jsonArray(jsonNodeObject["outputs"].toArray());

            for (const auto& rJsonValue : jsonArray)
            {
                const auto jsonOutputObject(rJsonValue.toObject());

                if (!jsonOutputObject["data_type"].isUndefined())
                {
                    const auto dataTypeStr(jsonOutputObject["data_type"].toString());

                    pNode->AddOutputPort(GraphNodeSlot::DataTypeFromString(dataTypeStr));
                }
            }
        }
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

                        const auto inputGizmos(p->GetInputGizmos());

                        qDebug() << "inputGizmos: " << inputGizmos.size();

                        if (!inputGizmos.isEmpty())
                        {
/*
                            QJsonArray jsonPortInputs;

                            for (auto port : inputGizmos)
                            {

                            }

                            jsonPort["ports"] = jsonPortInputs;
*/
                        }

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

        if (numInputs > 0)
        {
            for (int i = 0; i < numInputs; ++i)
            {
                // Just a quick and dirty way to add some variety for the port data types... 
                auto dataType = (i % 2) ? GraphPortDataType::Integer : GraphPortDataType::Float;

                pNode->AddInputPort(dataType);
            }
        }

        if (numOutputs > 0)
        {
            for (int i = 0; i < numOutputs; ++i)
            {
                // Just a quick and dirty way to add some variety for the port data types... 
                auto dataType = (i % 2) ? GraphPortDataType::Integer : GraphPortDataType::Float;

                pNode->AddOutputPort(dataType);
            }
        }
    }
}
