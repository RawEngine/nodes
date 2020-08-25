
#include "PCH.hpp"

#include "GraphView.hpp"

GraphView::GraphView(QWidget* pParent)
    : QGraphicsView(pParent)
{ }

void GraphView::drawBackground(QPainter* pPainter, const QRectF& rRect)
{
    const qreal left = int(rRect.left()) - (int(rRect.left()) % mGridScale);
    const qreal top = int(rRect.top()) - (int(rRect.top()) % mGridScale);

    // Draw the background.
    pPainter->setBrush(QBrush(GraphView::BackgroundColor));
    pPainter->setPen(Qt::NoPen);
    pPainter->drawRect(rRect);

    // Draw the minor grid lines.
    {
        QVarLengthArray<QLineF, 1024> lines;

        for (qreal x = left; x < rRect.right(); x += mGridScale)
            lines.append(QLineF(x, rRect.top(), x, rRect.bottom()));

        for (qreal y = top; y < rRect.bottom(); y += mGridScale)
            lines.append(QLineF(rRect.left(), y, rRect.right(), y));

        pPainter->setPen(QPen(GraphView::GridMinorColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
        pPainter->drawLines(lines.data(), lines.size());
    }

    // Draw the major grid lines.
    {
        QVarLengthArray<QLineF, 1024> lines;

        for (qreal x = left; x < rRect.right(); x += mGridScale * 5)
            lines.append(QLineF(x, rRect.top(), x, rRect.bottom()));

        for (qreal y = top; y < rRect.bottom(); y += mGridScale * 5)
            lines.append(QLineF(rRect.left(), y, rRect.right(), y));

        pPainter->setPen(QPen(GraphView::GridMajorColor, 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
        pPainter->drawLines(lines.data(), lines.size());
    }
}

void GraphView::wheelEvent(QWheelEvent* pEvent)
{
    // TODO: Scale by mouse wheel. (pEvent->delta();)
}
