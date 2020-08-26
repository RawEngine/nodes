
#include "PCH.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp"
#include "Headers/GraphGizmo.hpp"

#include <QGraphicsSceneMouseEvent>

GraphNodeSlot::GraphNodeSlot(GraphNode* pParentNode, GraphPortDataType dataType, IOType ioType, int index, bool isGhost /* = false */)
    : QGraphicsItem(pParentNode)
    , mDataType(dataType)
    , mIOType(ioType)
    , mIndex(index)
    , mIsGhost(isGhost)
    , mColor(dataType == GraphPortDataType::Integer ? Qt::green : Qt::blue)
{
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable);

    this->UpdatePosition();
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
#if 1

    if (mIOType == IOType::Output)
    {
        mpNewLine->UpdatePointB(QGraphicsItem::mapToScene(pEvent->pos()));
    }
    else if (mIOType == IOType::Input)
    {
        qDebug() << "TODO mouseMoveEvent INPUT";
    }
    else
        QGraphicsItem::mouseMoveEvent(pEvent);
#else
 //   qDebug() << "--- GraphNodeSlot::mouseMoveEvent ---";

    // When the "port" is moved - we will actually move the original "port".
    // But in it's place we will create a temporary "ghost" port.
    // This "ghost" port will be removed once the "mouse released" event is received.
    if (!mIsGhost && !mpGhostPort)
    {
        const auto deltaX = pEvent->pos().x() - mMousePressedPos.x();
        const auto deltaY = pEvent->pos().y() - mMousePressedPos.y();

        if (deltaX > 0 || deltaY > 0)
        {
            mpGhostPort = new GraphNodeSlot(mParentNode, mDataType, mIOType, mIndex, true);
            QGraphicsItem::scene()->addItem(mpGhostPort);

            // Show gizmo only when trying to connect. (Not disconnect)
       //     if (!mpTempPort)
            {
                mpGhostGizmo = new GraphGizmo(this, mpGhostPort);
                QGraphicsItem::scene()->addItem(mpGhostGizmo);
            }
        }
    }

    // If this is a "ghost" port, simply pass all the mouse move events.
    if (mIsGhost)
    {

        qDebug() << "--- GraphNodeSlot::mouseMoveEvent GHOST ---";

        QGraphicsItem::mouseMoveEvent(pEvent);

        auto pClosestPort = this->FindClosestPort();
        if (!pClosestPort)
            return;

        // Check if port is compatible.
        // And allow only "input-to-outputs" or "outputs-to-inputs". 
        // (No "inputs-to-inputs", "outputs-to-outputs")
        if (pClosestPort->GetDataType() != mDataType ||
            pClosestPort->GetIOType() == mIOType)
        {
            if (mpGhostGizmo)
                mpGhostGizmo->UpdateColor(Qt::red);

            return;
        }
        else
            if (mpGhostGizmo)
                mpGhostGizmo->UpdateColor(mColor);

        mpClosestPort = pClosestPort;
    }

    // If we've just created the "ghost" port - forward all the mouse move events to it.
    if (mpGhostPort)
    {
        mpGhostPort->mouseMoveEvent(pEvent);
    }
    else
        QGraphicsItem::mouseMoveEvent(pEvent);
#endif
}

void GraphNodeSlot::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
#if 1
    if (mIOType == IOType::Output)
    {
        const auto rect = this->boundingRect();

        auto portA = QPointF(rect.x() + rect.width() / 2, rect.y() + rect.height() / 2);
        portA = QGraphicsItem::mapToScene(portA);
        auto pointB(QGraphicsItem::mapToScene(pEvent->pos()));

        mpNewLine = new GraphLine(mColor, portA, pointB);
        QGraphicsItem::scene()->addItem(mpNewLine);
        mGizmosOut.append(mpNewLine);
    }
    else if (mIOType == IOType::Input)
    {
        qDebug() << "TODO: IOType::Input";
    }
    else
        QGraphicsItem::mousePressEvent(pEvent);
#else
    QGraphicsItem::mousePressEvent(pEvent);
/*

 //   qDebug() << "--- GraphNodeSlot::mousePressEvent ---";
 //   qDebug() << "--- Num source ports connected: " << mConnectedSrcPorts.size();

    // Check if there is a need to perform a disconnect.
    if (!mConnectedSrcPorts.isEmpty())
    {
        mpTempPort = mConnectedSrcPorts.takeLast();
        qDebug() << "PRICIUPOM!";
    }

    mMousePressedPos = pEvent->pos();
    mpClosestPort = nullptr;
*/
#endif
}

void GraphNodeSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
#if 1
    qDebug() << "--- mouseReleaseEvent ---";

    // Check if we've hit the DST port.
    auto pDstPort = dynamic_cast<GraphNodeSlot*>(QGraphicsItem::scene()->itemAt(pEvent->scenePos(), QTransform()));
    if (!pDstPort)
    {
        QGraphicsItem::mouseReleaseEvent(pEvent);
        return;
    }

    qDebug() << "pDstPort:" << pDstPort;

    if (mIOType == IOType::Output && pDstPort->IsIOType(IOType::Input))
    {
        mpNewLine->SetPorts(this, pDstPort);
        mpNewLine->UpdatePointB(pDstPort->pos());
    }
    else if (mIOType == IOType::Input && pDstPort->IsIOType(IOType::Output))
    {
    }
#else
    QGraphicsItem::mouseReleaseEvent(pEvent);


    if (mpGhostGizmo)
    {
        QGraphicsItem::scene()->removeItem(mpGhostGizmo);
        mpGhostGizmo = nullptr;
    }

    if (mpGhostPort)
    {
        // Re-set port back to the original position and remove the temporary "ghost" port.
     //   QGraphicsItem::setPos(mpGhostPort->pos());
        QGraphicsItem::scene()->removeItem(mpGhostPort);
        mpGhostPort = nullptr;
    }

    // Connect to a valid & the closest port that was found.
    if (mpClosestPort)
    {
        this->ConnectToPort(mpClosestPort);
        mpClosestPort = nullptr;
    }
#endif
}

GraphNodeSlot* GraphNodeSlot::FindClosestPort()
{
    GraphNodeSlot* pClosestPort = nullptr;

    qreal closestDistance = 30.0;

    // Returns a list of all items that collide with this item.
    auto collidingItems = QGraphicsItem::scene()->collidingItems(this);

    if (!collidingItems.isEmpty())
    {
        for (auto pItem : collidingItems)
        {
            // NOTE: 
            // Not using any signals/slots for "GraphNodeSlot" so there is no point in using "qobject_cast".
            // Using "dynamic_cast" to seek out the desired object and return it if possible. If it can't, it will return nullptr.
            auto pNodeSlot = dynamic_cast<GraphNodeSlot*>(pItem);

            // Make sure the "ghost" object gets ignored too. 
            if (!pNodeSlot || pNodeSlot == mpGhostPort)
                continue;

            qreal dist = (pNodeSlot->pos() - QGraphicsItem::pos()).manhattanLength();

            if (dist < closestDistance)
            {
                closestDistance = dist;
                pClosestPort = pNodeSlot;
            }
        }
    }

    return pClosestPort;
}

void GraphNodeSlot::ConnectToPort(GraphNodeSlot* pDstPort)
{
    auto pGizmo = new GraphGizmo(this, pDstPort);

    QGraphicsItem::scene()->addItem(pGizmo);

 //   mConnectedGizmos.append(pGizmo);
}
