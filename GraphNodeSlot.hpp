
#pragma once

class GraphNode;

class GraphNodeSlot : public QGraphicsItem
{
public:
    GraphNodeSlot(QGraphicsScene& rScene, GraphNode& rParentNode, int index);

    void UpdatePosition();

    QRectF boundingRect() const override;

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    GraphNode& mParentNode;

    const int mIndex;
};
