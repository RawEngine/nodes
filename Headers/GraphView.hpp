
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

    static constexpr auto BackgroundColor = QColor(40, 40, 40);
    static constexpr auto GridMinorColor = QColor(48, 48, 48);
    static constexpr auto GridMajorColor = QColor(50, 50, 50);

    int mGridScale = 64;
};
