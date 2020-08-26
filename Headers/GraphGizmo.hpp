
#pragma once

class GraphNodeSlot;

#if 1
class GraphLine final : public QGraphicsItem
{
public:
    GraphLine(const QColor& rColor, const QPointF& rPointA, const QPointF& rPointB);

    void SetPorts(GraphNodeSlot* pSrc, GraphNodeSlot* pDst);

    void UpdateColor(const QColor& rColor);
    void UpdatePointB(const QPointF& rPoint);

    QRectF boundingRect() const override;

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

//private:
public: // TEMP

    QColor  mColor;
    QPointF mPointA;
    QPointF mPointB;

    GraphNodeSlot* mpSrcPort = nullptr;
    GraphNodeSlot* mpDstPort = nullptr;
};
#endif

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
    GraphNodeSlot*  mPortOut;
    GraphNodeSlot*  mPortIn;
};

