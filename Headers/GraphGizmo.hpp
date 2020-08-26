
#pragma once

class GraphNodeSlot;

class GraphGizmo final : public QGraphicsItem
{
public:
    GraphGizmo(GraphNodeSlot* pPortOut, GraphNodeSlot* pPortIn);

    void UpdateColor(const QColor& rColor);

    QRectF boundingRect() const override;

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

private:

    QColor mColor;
    GraphNodeSlot* mPortOut;
    GraphNodeSlot* mPortIn;
};

