
#include "PCH.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp"
#include "Headers/GraphGizmo.hpp"

GraphNodeSlot::GraphNodeSlot(GraphNode& rParentNode, GraphPortDataType dataType, IOType ioType, int index)
    : QGraphicsItem()
    , mParentNode(rParentNode)
    , mDataType(dataType)
    , mIOType(ioType)
    , mIndex(index)
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

    auto pClosestPort = this->FindClosestPort();
    if (!pClosestPort)
        return;

    // TODO
    pClosestPort->IsClosest = true;

    this->ConnectToPort(pClosestPort);
}

void GraphNodeSlot::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mousePressEvent(pEvent);

}

void GraphNodeSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseReleaseEvent(pEvent);
}

GraphNodeSlot* GraphNodeSlot::FindClosestPort()
{
    GraphNodeSlot* pClosestPort = nullptr;

    qreal closestDistance = 30.0;

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
