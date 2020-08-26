
#include "PCH.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp"
#include "Headers/GraphGizmo.hpp"

#include <QGraphicsSceneMouseEvent>

GraphNodeSlot::GraphNodeSlot(GraphNode& rParentNode, GraphPortDataType dataType, IOType ioType, int index, bool isGhost /* = false */)
    : QGraphicsItem()
    , mParentNode(rParentNode)
    , mDataType(dataType)
    , mIOType(ioType)
    , mIndex(index)
    , mIsGhost(isGhost)
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

    qreal posX = mParentNode.pos().x();
    qreal posY = mParentNode.pos().y() + mParentNode.boundingRect().top() + topDistance + gapDistance * mIndex;

    // Inputs are located on the LEFT, outputs on the RIGHT side of the Node.
    if (mIOType == IOType::Input)
        posX += mParentNode.boundingRect().left();
    else
        posX += mParentNode.boundingRect().right();

    QGraphicsItem::setPos(posX, posY);
}

QRectF GraphNodeSlot::boundingRect() const
{
    return {-(NodePortSize/2), -(NodePortSize/2), NodePortSize, NodePortSize};
}

void GraphNodeSlot::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /* = nullptr */)
{
//  pPainter->setPen(Qt::black); // Looks prettier without the border?
    pPainter->setBrush(mDataType == GraphPortDataType::Integer ? Qt::green : Qt::red);
    pPainter->drawEllipse(this->boundingRect());

    if (this->IsClosest)
    {
        pPainter->setBrush(Qt::white);
        pPainter->drawEllipse(this->boundingRect());
    }
}

void GraphNodeSlot::mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseMoveEvent(pEvent);

    if (!mIsGhost && !mpGhostPort)
    {
        const auto deltaX = pEvent->pos().x() - mMousePressedPos.x();
        const auto deltaY = pEvent->pos().y() - mMousePressedPos.y();

        if (!mpGhostPort && (deltaX > 0 || deltaY > 0))
        {
            mpGhostPort = new GraphNodeSlot(mParentNode, mDataType, mIOType, mIndex, true);
            QGraphicsItem::scene()->addItem(mpGhostPort);

            mpGhostGizmo = new GraphGizmo(mpGhostPort, this);
            QGraphicsItem::scene()->addItem(mpGhostGizmo);
        }
    }

#if 0
    auto pClosestPort = this->FindClosestPort();
    if (!pClosestPort)
        return;

    // TODO
    pClosestPort->IsClosest = true;

    this->ConnectToPort(pClosestPort);
#endif
}

void GraphNodeSlot::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mousePressEvent(pEvent);

    mMousePressedPos = pEvent->pos();

    qDebug() << "--- GraphNodeSlot::mousePressEvent[" << mMousePressedPos << "] ---";


}

void GraphNodeSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseReleaseEvent(pEvent);

    if (mpGhostGizmo)
    {
        QGraphicsItem::scene()->removeItem(mpGhostGizmo);
        mpGhostGizmo = nullptr;
    }

    if (mpGhostPort)
    {
        // Re-set port back to the original position and remove the temporary "ghost" port.
        QGraphicsItem::setPos(mpGhostPort->pos());
        QGraphicsItem::scene()->removeItem(mpGhostPort);
        mpGhostPort = nullptr;
    }
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
            auto pNodeSlot = reinterpret_cast<GraphNodeSlot*>(pItem);
            if (!pNodeSlot)
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

void GraphNodeSlot::ConnectToPort(GraphNodeSlot* pPort)
{
    // TODO: Check if port is compatible.

    auto pGizmo = new GraphGizmo(this, pPort);

    QGraphicsItem::scene()->addItem(pGizmo);

    mGizmos.append(pGizmo);
}
