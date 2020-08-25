
#include "PCH.hpp"

#include "GraphNode.hpp"
#include "GraphNodeSlot.hpp"

GraphNodeSlot::GraphNodeSlot(QGraphicsScene& rScene, GraphNode& rParentNode, int index)
    : QGraphicsItem()
    , mParentNode(rParentNode)
    , mIndex(index)
{
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable);

    rScene.addItem(this);

    this->UpdatePosition();
}

void GraphNodeSlot::UpdatePosition()
{
    const int topDistance = 20;
    const int dapDistance = 20;

    QGraphicsItem::setPos(
                mParentNode.pos().x() + mParentNode.boundingRect().right(),
                mParentNode.pos().y() + mParentNode.boundingRect().top() + topDistance + (dapDistance * mIndex));
}

QRectF GraphNodeSlot::boundingRect() const
{
    return {-5.0, -5.0, 10.0, 10.0};
}

void GraphNodeSlot::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    pPainter->setPen(Qt::black);
    pPainter->setBrush(Qt::green);
    pPainter->drawEllipse(this->boundingRect());
}

void GraphNodeSlot::mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseMoveEvent(pEvent);
}

void GraphNodeSlot::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
//    mIsSelected = true;

    QGraphicsItem::mousePressEvent(pEvent);
    QGraphicsItem::update();
}

void GraphNodeSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
//    mIsSelected = false;

    QGraphicsItem::mouseReleaseEvent(pEvent);
    QGraphicsItem::update();
}
