
#pragma once

class GraphNode;

class GraphNodeSlot : public QGraphicsItem
{
public:
    enum class Type
    {
        Integer,
        Float
    };

    GraphNodeSlot(QGraphicsScene& rScene, GraphNode& rParentNode, Type type, int index);

    void UpdatePosition();

    QRectF boundingRect() const override;

    Type GetType() const { return mType; }

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    GraphNode&  mParentNode;

    const Type  mType;
    const int   mIndex;
};
