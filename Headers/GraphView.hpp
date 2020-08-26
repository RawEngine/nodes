
#pragma once

#include <QGraphicsView>

class GraphView final : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(QWidget* pParent);

protected:

    void drawBackground(QPainter* pPainter, const QRectF& rRect) override;

    void wheelEvent(QWheelEvent* pEvent) override;
    void mouseMoveEvent(QMouseEvent* pEvent) override;
    void mousePressEvent(QMouseEvent* pEvent) override;
    void mouseReleaseEvent(QMouseEvent* pEvent) override;

private:

    static const QColor BackgroundColor;
    static const QColor GridColor;
    static const int    GridScale;

    bool    mIsMouseDrag = false;
    QPoint  mMouseDownPos;
};
