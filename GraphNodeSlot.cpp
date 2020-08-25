
#include "PCH.hpp"

#include "GraphNodeSlot.hpp"

#include <QBrush>
#include <QPainter>

GraphNodeSlot::GraphNodeSlot(QGraphicsScene& rScene)
    : QGraphicsItem()
{
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable);

    rScene.addItem(this);
}

GraphNodeSlot::~GraphNodeSlot()
{ }

QRectF GraphNodeSlot::boundingRect() const
{
    return {0.f, 0.f, 100.f, 100.f};
}

void GraphNodeSlot::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    pPainter->setPen(Qt::black);
    pPainter->setBrush(Qt::green);
    pPainter->drawEllipse(-5, -5, 10, 10);
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
