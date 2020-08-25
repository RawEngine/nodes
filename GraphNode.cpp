
#include "PCH.hpp"

#include "GraphNode.hpp"
#include "GraphNodeSlot.hpp"

#include <QBrush>
#include <QPainter>

GraphNode::GraphNode(QGraphicsScene& rScene, const QString& rName)
    : QGraphicsItem()
    , mName(rName)
{
    rScene.addItem(this);

    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable);


    // Drop some shadow.
    mShadowEffect.setBlurRadius(18);
    mShadowEffect.setXOffset(0.0);
    mShadowEffect.setYOffset(6.0);

    QGraphicsItem::setGraphicsEffect(&mShadowEffect);

    //-----------------------------------------------
    mInputSlots.append(new GraphNodeSlot(rScene));
}

QRectF GraphNode::boundingRect() const
{
    return {0.f, 0.f, 100.f, 100.f};
}

void GraphNode::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    QRectF rect(this->boundingRect());

    QBrush brush(Qt::white);

    // Draw the background.
//    pPainter->setBrush(brush);
//    pPainter->fillRoundedRect(rect, brush, 4, 4);

    QPainterPath path;
    path.addRoundedRect(rect, 4, 4);

    QPen pen(mIsSelected ? GraphNode::BorderColorSelected : GraphNode::BorderColorDefault, 1);
    pPainter->setPen(pen);

    pPainter->fillPath(path, GraphNode::BackgroundColor);
    pPainter->drawPath(path);


    // Draw the border.
//    pPainter->setPen(mIsSelected ? Qt::red : Qt::green);
//    pPainter->drawRoundedRect(rect, 4, 4);

//    pPainter->setPen(QPen(Qt::gray));
 //   pPainter->drawText(QRectF(1, 1, 2, 2), "Woop");
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
