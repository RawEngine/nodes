
#include "GraphNode.hpp"

#include <QBrush>
#include <QPainter>

GraphNode::GraphNode(const QString& rName)
    : QGraphicsItem()
    , mName(rName)
{
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF GraphNode::boundingRect() const
{
    return {0.f, 0.f, 100.f, 100.f};
}

void GraphNode::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    QRectF rect(this->boundingRect());
    QBrush brush(Qt::green);

    if (mIsSelected)
        brush.setColor(Qt::red);

    pPainter->fillRect(rect, brush);
    pPainter->drawRect(rect);
}

void GraphNode::mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent)
{
    QGraphicsItem::mouseMoveEvent(pEvent);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
{
    mIsSelected = true;

    QGraphicsItem::mousePressEvent(pEvent);
    QGraphicsItem::update();
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
{
    mIsSelected = false;

    QGraphicsItem::mouseReleaseEvent(pEvent);
    QGraphicsItem::update();
}
