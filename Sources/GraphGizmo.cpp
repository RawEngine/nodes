
#include "PCH.hpp"

#include "Headers/GraphGizmo.hpp"
#include "Headers/GraphNodeSlot.hpp"

GraphGizmo::GraphGizmo(GraphNodeSlot* pFrom, GraphNodeSlot* pTo)
    : QGraphicsItem()
    , mColor(pFrom->GetColor())
    , PortSRC(pFrom)
    , PortDST(pTo)
{
    QGraphicsItem::setZValue(-1); // Don't draw on top.
}

void GraphGizmo::UpdateColor(const QColor& rColor)
{
    if (mColor == rColor)
        return;

    mColor = rColor;
    QGraphicsItem::update();
}

QRectF GraphGizmo::boundingRect() const
{
    return { this->PortSRC->scenePos(), this->PortDST->scenePos() };
}

void GraphGizmo::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    const auto& posFrom(this->PortSRC->scenePos());
    const auto& posTo(this->PortDST->scenePos());

    QPainterPath path(posFrom);

    path.cubicTo(
                (posTo.x() + posFrom.x()) / 2.0, posFrom.y(),
                (posTo.x() + posFrom.x()) / 2.0, posTo.y(),
                 posTo.x(), posTo.y());

    pPainter->setPen(QPen(mColor, 3));
    pPainter->drawPath(path);

#if 0
    // Draw debug rect.
    pPainter->setBrush(Qt::transparent);
    pPainter->setPen(Qt::red);
    pPainter->drawRect(this->boundingRect());
#endif
}
