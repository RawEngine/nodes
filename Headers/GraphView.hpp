
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

private:

    static const QColor BackgroundColor;
    static const QColor GridMinorColor;
    static const QColor GridMajorColor;

    int mGridScale = 64;
};
