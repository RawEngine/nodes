
#include "PCH.hpp"

#include "Headers/GraphNode.hpp"
#include "Headers/GraphNodeSlot.hpp"

GraphNode::GraphNode(QGraphicsScene& rScene, const QString& rName, const QPointF& rPosition)
    : QGraphicsItem()
    , mScene(rScene)
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

    // TEMP.
    this->AddInputPort(GraphPortType::Integer);
    this->AddInputPort(GraphPortType::Float);
    this->AddInputPort(GraphPortType::Integer);

    this->AddOutputPort(GraphPortType::Float);
    this->AddOutputPort(GraphPortType::Integer);
}

void GraphNode::AddInputPort(GraphPortType type)
{
    const int index = mInputPorts.size();

    mInputPorts.append(new GraphNodeSlot(mScene, *this, type, GraphNodeSlot::IOType::Input, index));
}

void GraphNode::AddOutputPort(GraphPortType type)
{
    const int index = mOutputPorts.size();

    mOutputPorts.append(new GraphNodeSlot(mScene, *this, type, GraphNodeSlot::IOType::Output, index));
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
