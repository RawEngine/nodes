
#pragma once

class GraphNodeSlot;

class GraphGizmo final : public QGraphicsItem
{
public:
    GraphGizmo(GraphNodeSlot* pFrom, GraphNodeSlot* pTo);

    void UpdateColor(const QColor& rColor);

    QRectF boundingRect() const override;

    GraphNodeSlot* PortSRC = nullptr;
    GraphNodeSlot* PortDST = nullptr;

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

private:

    QColor mColor;
};

