
#include "PCH.hpp"

#include "GraphNode.hpp"
#include "GraphNodeSlot.hpp"

GraphNode::GraphNode(QGraphicsScene& rScene, const QString& rName, const QPointF& rPosition)
    : QGraphicsItem()
    , mName(rName)
{
    rScene.addItem(this);

    QGraphicsItem::setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    QGraphicsItem::setPos(rPosition);

    // Drop some shadow.
    mShadowEffect.setBlurRadius(18);
    mShadowEffect.setXOffset(0.0);
    mShadowEffect.setYOffset(6.0);

    QGraphicsItem::setGraphicsEffect(&mShadowEffect);

    //-----------------------------------------------
    mInputSlots.append(new GraphNodeSlot(rScene, *this, GraphNodeSlot::Type::Integer, 0));
    mInputSlots.append(new GraphNodeSlot(rScene, *this, GraphNodeSlot::Type::Float, 1));
    mInputSlots.append(new GraphNodeSlot(rScene, *this, GraphNodeSlot::Type::Integer, 2));
}

QRectF GraphNode::boundingRect() const
{
    return {0.0, 0.0, 100.0, 200.0};
}

void GraphNode::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    QRectF rect(this->boundingRect());


    QPainterPath path;
    path.addRoundedRect(rect, 4, 4);

    QPen pen(QGraphicsItem::isSelected() ? GraphNode::BorderColorSelected : GraphNode::BorderColorDefault, 1);
    pPainter->setPen(pen);

    pPainter->fillPath(path, GraphNode::BackgroundColor);
    pPainter->drawPath(path);


    // Draw the header text.
    pPainter->setPen(Qt::white);
    pPainter->setFont(QFont("Verdana", 12, QFont::Bold));
    pPainter->drawText(rect, Qt::AlignCenter, mName);
}

void GraphNode::mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseMoveEvent(pEvent);

    for (auto pSlot : mInputSlots)
        pSlot->UpdatePosition();
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mousePressEvent(pEvent);
//    QGraphicsItem::update();
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseReleaseEvent(pEvent);
//    QGraphicsItem::update();
}