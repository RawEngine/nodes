
#pragma once

// Forward declarations.
class GraphNode;
class GraphGizmo;

enum class GraphPortDataType
{
    Unknown,
    Integer,
    Float,
};

class GraphNodeSlot : public QGraphicsItem
{
public:
    enum class IOType
    {
        Input,
        Output
    };

    GraphNodeSlot(GraphNode* pParentNode, GraphPortDataType dataType, IOType ioType, int index);

    void AddConnection(GraphNodeSlot* pDstPort);

    void AddInputGizmo(GraphGizmo* pGizmo);
    void AddOutputGizmo(GraphGizmo* pGizmo);

    static GraphPortDataType DataTypeFromString(const QString& rStr);

    QString DataTypeToString() const;

    void UpdatePosition();

    QRectF boundingRect() const override;

    bool IsIOType(IOType e) const { return mIOType == e; }
    bool IsDataType(GraphPortDataType e) const { return mDataType == e; }

    auto GetDataType() const    { return mDataType; }
    auto GetIOType() const      { return mIOType;   }
    auto GetIndex() const       { return mIndex;    }
    auto GetColor() const       { return mColor;    }   

 
    const auto& GetInputGizmos() const  { return mGizmosIn;  }
    const auto& GetOutputGizmos() const { return mGizmosOut; }

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    GraphNodeSlot* FindClosestPort();

    void ConnectToPort(GraphNodeSlot* pDstPort, GraphGizmo* pGizmo);
    void CreateGhostPort(IOType inputType, const QPointF& rScenePos);

private:

    static constexpr auto   NodePortSize = 10.0;

    const GraphPortDataType mDataType;
    const IOType            mIOType;
    const int               mIndex;
    const QColor            mColor;

    QVector<GraphGizmo*>    mGizmosIn;
    QVector<GraphGizmo*>    mGizmosOut;

    GraphNodeSlot*          mpClosestPort   = nullptr;
    GraphNodeSlot*          mpGhostPort     = nullptr; // A temporary "port" object that will be used while mouse dragging the port to the destination.
    GraphGizmo*             mpGhostGizmo    = nullptr;
};
