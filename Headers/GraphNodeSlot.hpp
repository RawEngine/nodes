
#pragma once

class GraphNode;

enum class GraphPortType
{
    Integer,
    Float
};

class GraphNodeSlot : public QGraphicsItem
{
public:
    enum class IOType
    {
        Input,
        Output
    };

    GraphNodeSlot(QGraphicsScene& rScene, GraphNode& rParentNode, GraphPortType dataType, IOType ioType, int index);

    void UpdatePosition();

    QRectF boundingRect() const override;

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    GraphNode&          mParentNode;

    const GraphPortType mDataType;
    const IOType        mIOType;
    const int           mIndex;
};
