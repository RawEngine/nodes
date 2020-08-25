
#include "PCH.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp"

GraphNodeSlot::GraphNodeSlot(QGraphicsScene& rScene, GraphNode& rParentNode, GraphPortType dataType, IOType ioType, int index)
    : QGraphicsItem()
    , mParentNode(rParentNode)
    , mDataType(dataType)
    , mIOType(ioType)
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

    qreal posX = mParentNode.pos().x();
    qreal posY = mParentNode.pos().y() + mParentNode.boundingRect().top() + topDistance + (dapDistance * mIndex);

    if (mIOType == IOType::Input)
        posX += mParentNode.boundingRect().left();
    else
        posX += mParentNode.boundingRect().right();

    QGraphicsItem::setPos(posX, posY);
}

QRectF GraphNodeSlot::boundingRect() const
{
    return {-5.0, -5.0, 10.0, 10.0};
}

void GraphNodeSlot::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /* = nullptr */)
{
//  pPainter->setPen(Qt::black); // Looks prettier without the border?
    pPainter->setBrush(mDataType == GraphPortType::Integer ? Qt::green : Qt::red);
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
