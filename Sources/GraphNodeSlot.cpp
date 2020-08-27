
#include "PCH.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp"
#include "Headers/GraphGizmo.hpp"

#include <QGraphicsSceneMouseEvent>

GraphNodeSlot::GraphNodeSlot(GraphNode* pParentNode, GraphPortDataType dataType, IOType ioType, int index)
    : QGraphicsItem(pParentNode)
    , mDataType(dataType)
    , mIOType(ioType)
    , mIndex(index)
    , mColor(dataType == GraphPortDataType::Integer ? Qt::green : Qt::blue)
{
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable);

    this->UpdatePosition();
}

void GraphNodeSlot::AddConnection(GraphNodeSlot* pDstPort)
{
    Q_ASSERT(pDstPort->IsDataType(mDataType));

    if (!pDstPort->IsDataType(mDataType))
        return; // Can't connect if data type is different.

    auto pGizmo = new GraphGizmo(this, pDstPort);

    QGraphicsItem::scene()->addItem(pGizmo);

    if (pDstPort->IsIOType(IOType::Input))
    {
        mGizmosOut.append(pGizmo);

        pDstPort->AddInputGizmo(pGizmo);
    }
    else if (pDstPort->IsIOType(IOType::Output))
    {
        mGizmosIn.append(pGizmo);

        pDstPort->AddOutputGizmo(pGizmo);
    }
}

void GraphNodeSlot::AddInputGizmo(GraphGizmo* pGizmo)
{
    mGizmosIn.append(pGizmo);
}

void GraphNodeSlot::AddOutputGizmo(GraphGizmo* pGizmo)
{
    mGizmosOut.append(pGizmo);
}

GraphPortDataType GraphNodeSlot::DataTypeFromString(const QString& rStr)
{
    if (rStr.isEmpty())
        return GraphPortDataType::Unknown;

         if (rStr.front() == 'I') return GraphPortDataType::Integer;
    else if (rStr.front() == 'F') return GraphPortDataType::Float;

    return GraphPortDataType::Unknown;
}

QString GraphNodeSlot::DataTypeToString() const
{
    switch (mDataType)
    {
    case GraphPortDataType::Integer:    return "I";
    case GraphPortDataType::Float:      return "F";
    default:
        break;
    }

    return QString();
}

void GraphNodeSlot::UpdatePosition()
{
    if (!QGraphicsItem::parentItem())
        return; // Port is not attached to any parent (temporary/ghost).

    const int topDistance = 20;
    const int gapDistance = 20;

    qreal posX = 0; // IOType::Input

    // Inputs are located on the LEFT, outputs on the RIGHT side of the Node.
    if (mIOType == IOType::Output)
        posX = QGraphicsItem::parentItem()->boundingRect().right();

    QGraphicsItem::setPos(posX, topDistance + gapDistance * mIndex);
}

QRectF GraphNodeSlot::boundingRect() const
{
    return {-(NodePortSize/2), -(NodePortSize/2), NodePortSize, NodePortSize};
}

void GraphNodeSlot::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /* = nullptr */)
{
//  pPainter->setPen(Qt::black); // Looks prettier without the border?
    pPainter->setBrush(mColor);
    pPainter->drawEllipse(this->boundingRect());
}

void GraphNodeSlot::mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent)
{
    if (!mpGhostPort)
        return;

    mpGhostPort->setPos(QGraphicsItem::mapToScene(pEvent->pos()));
    mpGhostGizmo->UpdateColor(mColor);

    auto pClosestPort = this->FindClosestPort();
    if (!pClosestPort)
        return;

    // Check if port is compatible.
    // And allow only "input-to-outputs" or "outputs-to-inputs". 
    // (No "inputs-to-inputs", "outputs-to-outputs")
    if (pClosestPort->GetDataType() != mDataType || pClosestPort->GetIOType() == mpGhostPort->GetIOType())
    {
        mpGhostGizmo->UpdateColor(Qt::red);
        return;
    }

#if _DEBUG
    auto pDstNode = dynamic_cast<GraphNode*>(pClosestPort->parentItem());
    auto dstPortTypeStr(pClosestPort->IsIOType(IOType::Input) ? "IN" : "OUT");

    qDebug() << "Closes port found:" << pDstNode->GetName() << dstPortTypeStr << "portIndex:" << pClosestPort->GetIndex();
#endif

    mpClosestPort = pClosestPort;
}

void GraphNodeSlot::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mousePressEvent(pEvent);

    mpClosestPort = nullptr; // Reset, just in case.

    // Right mouse button serves as the "detach" action.
    if (pEvent->button() == Qt::RightButton)
    {
        if (mIOType == IOType::Input && !mGizmosIn.isEmpty())
        {
            this->CreateGhostPort(IOType::Output, pEvent->scenePos());

            mpGhostGizmo = mGizmosIn.takeLast();
            mpGhostGizmo->PortDST = mpGhostPort;
        }
        else if (mIOType == IOType::Output && !mGizmosOut.isEmpty())
        {
            this->CreateGhostPort(IOType::Input, pEvent->scenePos());

            mpGhostGizmo = mGizmosOut.takeLast();
            mpGhostGizmo->PortDST = mpGhostPort;
        }

#if _DEBUG
        if (mpGhostGizmo)
        {
            auto pSrcPort = mpGhostGizmo->PortSRC;
            auto pSrcNode = dynamic_cast<GraphNode*>(QGraphicsItem::parentItem());
            auto pDstNode = dynamic_cast<GraphNode*>(pSrcPort->parentItem());

            auto srcPortTypeStr(this->IsIOType(IOType::Input) ? "IN" : "OUT");
            auto dstPortTypeStr(pSrcPort->IsIOType(IOType::Input) ? "IN" : "OUT");

            qDebug() << "Disconnecting node" << pSrcNode->GetName() << srcPortTypeStr << "portIndex:" << mIndex
                << "from" << pDstNode->GetName() << dstPortTypeStr << "portIndex:" << pSrcPort->GetIndex();
        }
#endif
    }

    if (pEvent->button() == Qt::LeftButton)
    {
        // Create the ghost clone.
        this->CreateGhostPort(mIOType, pEvent->scenePos());

        // Add the temporary gizmo.
        mpGhostGizmo = new GraphGizmo(this, mpGhostPort);
        QGraphicsItem::scene()->addItem(mpGhostGizmo);
    }
}

void GraphNodeSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseReleaseEvent(pEvent);

    // Connect to a valid & the closest port that was found.
    if (mpClosestPort)
    {
        // NOTE: The Ghost gizmo ownership is passed to the scene.
        this->ConnectToPort(mpClosestPort, mpGhostGizmo);

        mpClosestPort = nullptr;
        mpGhostGizmo = nullptr;
    }
    else
    {
        QGraphicsItem::scene()->removeItem(mpGhostGizmo);
        delete mpGhostGizmo;
        mpGhostGizmo = nullptr;
    }

    if (mpGhostPort)
    {
        QGraphicsItem::scene()->removeItem(mpGhostPort);
        delete mpGhostPort;
        mpGhostPort = nullptr;
    }
}

// Determines the closes available port to the "ghost" port.
GraphNodeSlot* GraphNodeSlot::FindClosestPort()
{
    GraphNodeSlot* pClosestPort = nullptr;

    qreal closestDistance = 30.0;

    // Returns a list of all items that collide with the "ghost" item.
    auto collidingItems = QGraphicsItem::scene()->collidingItems(mpGhostPort);

    if (!collidingItems.isEmpty())
    {
        for (auto pItem : collidingItems)
        {
            // NOTE: 
            // Not using any signals/slots for "GraphNodeSlot" so there is no point in using "qobject_cast".
            // Using "dynamic_cast" to seek out the desired object and return it if possible. If it can't, it will return nullptr.
            auto pNodeSlot = dynamic_cast<GraphNodeSlot*>(pItem);

            // Make sure the "this" item object gets ignored too. 
            if (!pNodeSlot || pNodeSlot == this)
                continue;

            qreal dist = (pNodeSlot->scenePos() - mpGhostPort->scenePos()).manhattanLength();

            if (dist < closestDistance)
            {
                closestDistance = dist;
                pClosestPort = pNodeSlot;
            }
        }
    }

    return pClosestPort;
}

void GraphNodeSlot::ConnectToPort(GraphNodeSlot* pPort, GraphGizmo* pGizmo)
{
#if _DEBUG
    auto pSrcNode = dynamic_cast<GraphNode*>(QGraphicsItem::parentItem());
    auto pDstNode = dynamic_cast<GraphNode*>(pPort->parentItem());

    auto srcPortTypeStr(this->IsIOType(IOType::Input) ? "IN" : "OUT");
    auto dstPortTypeStr(pPort->IsIOType(IOType::Input) ? "IN" : "OUT");

    qDebug() << "Connecting node" << pSrcNode->GetName() << srcPortTypeStr << "portIndex:"<< mIndex
             << "to"              << pDstNode->GetName() << dstPortTypeStr << "portIndex:"<< pPort->GetIndex();
#endif
    // Keep gizmo informed about it's new destination port. 
    // (Because previously it was using the "ghost" port that will be deleted)
    pGizmo->PortDST = pPort;

    if (pPort->IsIOType(IOType::Input))
    {
        mGizmosOut.append(pGizmo);

        pPort->AddInputGizmo(pGizmo);
    }
    else if (pPort->IsIOType(IOType::Output))
    {
        mGizmosIn.append(pGizmo);

        pPort->AddOutputGizmo(pGizmo);
    }
}

// Add the temporary (ghost) port item.
// Will be used when attaching and dettaching gizmos to the ports.
void GraphNodeSlot::CreateGhostPort(IOType inputType, const QPointF& rScenePos)
{
    mpGhostPort = new GraphNodeSlot(nullptr, mDataType, inputType, mIndex);
    mpGhostPort->setPos(rScenePos);

    QGraphicsItem::scene()->addItem(mpGhostPort);
}
