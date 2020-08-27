
#include "PCH.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp"

const QColor GraphNode::BackgroundColor = QColor(30, 30, 30);
const QColor GraphNode::BorderColorDefault = QColor(9, 9, 9);
const QColor GraphNode::BorderColorSelected = QColor(39, 214, 45);

GraphNode::GraphNode(const QString& rName, const QPointF& rPosition)
    : QGraphicsItem()
    , mName(rName)
{
    QGraphicsItem::setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    QGraphicsItem::setPos(rPosition);

    // Drop some shadow.
    mShadowEffect.setBlurRadius(18);
    mShadowEffect.setXOffset(0.0);
    mShadowEffect.setYOffset(6.0);

    QGraphicsItem::setGraphicsEffect(&mShadowEffect);
}

GraphNodeSlot* GraphNode::AddInputPort(GraphPortDataType type)
{
    Q_ASSERT(QGraphicsItem::scene());

    const int index = mInputPorts.size();

    auto pItem = new GraphNodeSlot(this, type, GraphNodeSlot::IOType::Input, index);

    // NOTE: No need, because "GraphNodeSlot" is a child of a GraphNode. (QGraphicsItem)
//  QGraphicsItem::scene()->addItem(pItem);

    mInputPorts.append(pItem);

    return pItem;
}

GraphNodeSlot* GraphNode::AddOutputPort(GraphPortDataType type)
{
    Q_ASSERT(QGraphicsItem::scene());

    const int index = mOutputPorts.size();

    auto pItem = new GraphNodeSlot(this, type, GraphNodeSlot::IOType::Output, index);

    // NOTE: No need, because "GraphNodeSlot" is a child of a GraphNode. (QGraphicsItem)
//  QGraphicsItem::scene()->addItem(pItem);

    mOutputPorts.append(pItem);

    return pItem;
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

    for (auto pInputPort : mInputPorts)
        pInputPort->UpdatePosition();

    for (auto pOutputPort : mOutputPorts)
        pOutputPort->UpdatePosition();
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
