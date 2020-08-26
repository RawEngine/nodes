
#include "PCH.hpp"

#include "Headers/GraphGizmo.hpp"
#include "Headers/GraphNodeSlot.hpp"

GraphLine::GraphLine(const QColor& rColor, const QPointF& rPointA, const QPointF& rPointB)
    : QGraphicsItem()
    , mColor(rColor)
    , mPointA(rPointA)
    , mPointB(rPointB)
{
    QGraphicsItem::setZValue(-1); // Don't draw on top.
}

void GraphLine::SetPorts(GraphNodeSlot* pSrc, GraphNodeSlot* pDst)
{
    mpSrcPort = pSrc;
    mpDstPort = pDst;
}

void GraphLine::UpdateColor(const QColor& rColor)
{
    mColor = rColor;
    QGraphicsItem::update();
}

void GraphLine::UpdatePointB(const QPointF& rPoint)
{
    mPointB = rPoint;
    QGraphicsItem::update();
}

QRectF GraphLine::boundingRect() const
{
    if (mpSrcPort && mpDstPort)
    {
        return { mpSrcPort->pos(), mpDstPort->pos() };
    }

    return { mPointA, mPointB };
}

void GraphLine::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /*= nullptr*/)
{
    QPointF posFrom;
    QPointF posTo;

    if (mpSrcPort && mpDstPort)
    {
        posFrom = mpSrcPort->pos();
        posTo = mpDstPort->pos();
    }
    else
    {
        posFrom = mPointA;
        posTo = mPointB;
    }

    QPainterPath path(mPointB);

    path.cubicTo(
        (posFrom.x() + posTo.x()) / 2.0, posTo.y(),
        (posFrom.x() + posTo.x()) / 2.0, posFrom.y(),
        posFrom.x(), posFrom.y());

    pPainter->setPen(QPen(mColor, 3));
    pPainter->drawPath(path);

#if 1
    // Draw debug rect.
    pPainter->setBrush(Qt::transparent);
    pPainter->setPen(Qt::red);
    pPainter->drawRect(this->boundingRect());
#endif
}

//------------------------------------------------------------------------

GraphGizmo::GraphGizmo(GraphNodeSlot* pPortOut, GraphNodeSlot* pPortIn)
    : QGraphicsItem()
    , mColor(pPortOut->GetColor())
    , mPortOut(pPortOut)
    , mPortIn(pPortIn)
{
    QGraphicsItem::setZValue(-1); // Don't draw on top.
}

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

#if 0
    // Draw debug rect.
    pPainter->setBrush(Qt::transparent);
    pPainter->setPen(Qt::red);
    pPainter->drawRect(this->boundingRect());
#endif
}
