
#include "PCH.hpp"

#include "Headers/GraphView.hpp"

#include <QWheelEvent>

const QColor GraphView::BackgroundColor = QColor(40, 40, 40);
const QColor GraphView::GridColor = QColor(48, 48, 48);
const int    GraphView::GridScale = 64;

GraphView::GraphView(QWidget* pParent)
    : QGraphicsView(pParent)
{
    QGraphicsView::setRenderHints(QPainter::Antialiasing);
    QGraphicsView::setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QGraphicsView::setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void GraphView::drawBackground(QPainter* pPainter, const QRectF& rRect)
{
    const qreal left = int(rRect.left()) - (int(rRect.left()) % GraphView::GridScale);
    const qreal top = int(rRect.top()) - (int(rRect.top()) % GraphView::GridScale);

    // Draw the background.
    pPainter->setBrush(QBrush(GraphView::BackgroundColor));
    pPainter->setPen(Qt::NoPen);
    pPainter->drawRect(rRect);

    // Draw the grid lines.
    QVarLengthArray<QLineF, 1024> lines;

    for (qreal x = left; x < rRect.right(); x += GraphView::GridScale)
        lines.append(QLineF(x, rRect.top(), x, rRect.bottom()));

    for (qreal y = top; y < rRect.bottom(); y += GraphView::GridScale)
        lines.append(QLineF(rRect.left(), y, rRect.right(), y));

    pPainter->setPen(QPen(GraphView::GridColor, 1, Qt::SolidLine));
    pPainter->drawLines(lines.data(), lines.size());
}

void GraphView::wheelEvent(QWheelEvent* pEvent)
{
    qreal inFactor = 1.25;
    qreal outFactor = 1 / inFactor;

    auto oldPos = QGraphicsView::mapToScene(pEvent->pos());

    qreal zoomFactor;

    if (pEvent->delta() > 0)
        zoomFactor = inFactor;
    else
        zoomFactor = outFactor;

    QGraphicsView::scale(zoomFactor, zoomFactor);

    auto newPos = QGraphicsView::mapToScene(pEvent->pos());
    auto delta = newPos - oldPos;

    QGraphicsView::translate(delta.x(), delta.y());
}

void GraphView::mouseMoveEvent(QMouseEvent* pEvent)
{
    if (mIsMouseDrag)
    {
#if 0
        QPointF oldp = QGraphicsView::mapToScene(mMouseDownPos);
        QPointF newP = QGraphicsView::mapToScene(pEvent->pos());
        QPointF translation = newP - oldp;

        translate(translation.x(), translation.y());
#else
        auto delta = (QGraphicsView::mapToScene(pEvent->pos()) - QGraphicsView::mapToScene(mMouseDownPos)) * -1.0;
        auto center = QPoint(QGraphicsView::viewport()->width() / 2 + delta.x(), QGraphicsView::viewport()->height() / 2 + delta.y());

        QGraphicsView::centerOn(QGraphicsView::mapToScene(center));
#endif
        mMouseDownPos = pEvent->pos();
        return;
    }

    QGraphicsView::mouseMoveEvent(pEvent);
}

void GraphView::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        QGraphicsView::setDragMode(QGraphicsView::DragMode::RubberBandDrag);
    }
    else if (pEvent->button() == Qt::MiddleButton)
    {
        QGraphicsView::setDragMode(QGraphicsView::DragMode::NoDrag);
        QGraphicsView::setCursor(Qt::SizeAllCursor);

        mMouseDownPos = pEvent->pos();
        mIsMouseDrag = true;
    }

    QGraphicsView::mousePressEvent(pEvent);
}

void GraphView::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (mIsMouseDrag)
    {
        mIsMouseDrag = false;
        QGraphicsView::setCursor(Qt::ArrowCursor);
    }

    QGraphicsView::mouseReleaseEvent(pEvent);
}
