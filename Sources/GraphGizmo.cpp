
#include "PCH.hpp"

#include "Headers/GraphGizmo.hpp"
#include "Headers/GraphNodeSlot.hpp"

GraphGizmo::GraphGizmo(GraphNodeSlot* pPortOut, GraphNodeSlot* pPortIn)
    : QGraphicsItem()
    , mColor(pPortOut->GetColor())
    , mPortOut(pPortOut)
    , mPortIn(pPortIn)
{ }

void GraphGizmo::UpdateColor(const QColor& rColor)
{
    mColor = rColor;
    QGraphicsItem::update();
}

QRectF GraphGizmo::boundingRect() const
{
    return { mPortOut->pos(), mPortIn->pos() };
}

void GraphGizmo::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    const auto& posIn(mPortIn->pos());
    const auto& posOut(mPortOut->pos());

    QPainterPath path(mPortOut->pos());

    path.cubicTo(
                (posIn.x() + posOut.x()) / 2.0, posOut.y(),
                (posIn.x() + posOut.x()) / 2.0, posIn.y(),
                 posIn.x(), posIn.y());

    pPainter->setPen(QPen(mColor, 3));
    pPainter->drawPath(path);

#if 1
    // Draw debug rect.
    pPainter->setBrush(Qt::transparent);
    pPainter->setPen(Qt::red);
    pPainter->drawRect(this->boundingRect());
#endif
}
