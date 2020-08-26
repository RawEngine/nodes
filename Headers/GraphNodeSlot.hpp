
#pragma once

// Forward declarations.
class GraphNode;
class GraphGizmo;

enum class GraphPortType
{
    Unknown,
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

    GraphNodeSlot(GraphNode& rParentNode, GraphPortType dataType, IOType ioType, int index);

    static GraphPortType DataTypeFromString(const QString& rStr);

    QString DataTypeToString() const;

    void UpdatePosition();

    QRectF boundingRect() const override;

    // TEMP.
    bool IsClosest = false;

    auto GetDataType() const { return mDataType; }

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    GraphNodeSlot* FindClosestPort();

    void ConnectToPort(GraphNodeSlot* pPort);

private:

    static constexpr auto NodePortSize = 20.0;

    GraphNode&          mParentNode;

    const GraphPortType mDataType;
    const IOType        mIOType;
    const int           mIndex;

    QVector<GraphGizmo*> mGizmos;
};
