
#pragma once

// Forward declarations.
class GraphNode;
class GraphLine;
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

    GraphNodeSlot(GraphNode* pParentNode, GraphPortDataType dataType, IOType ioType, int index, bool isGhost = false);

    static GraphPortDataType DataTypeFromString(const QString& rStr);

    QString DataTypeToString() const;

    void UpdatePosition();

    QRectF boundingRect() const override;

    bool IsIOType(IOType e) const { return mIOType == e; }

    auto GetDataType() const    { return mDataType; }
    auto GetIOType() const      { return mIOType;   }
    auto GetColor() const       { return mColor;    }

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    GraphNodeSlot* FindClosestPort();

    void ConnectToPort(GraphNodeSlot* pDstPort);

private:

    static constexpr auto   NodePortSize = 10.0;

    const GraphPortDataType mDataType;
    const IOType            mIOType;
    const int               mIndex;
    const bool              mIsGhost;
    const QColor            mColor;

    // A single src "port" might be connected to the multiple dst ports. 
 //   QVector<GraphNodeSlot*> mConnectedDstPorts;
    QVector<GraphNodeSlot*> mConnectedSrcPorts;
    
    QVector<GraphLine*> mGizmosIn;
    QVector<GraphLine*> mGizmosOut;
    GraphLine* mpNewLine = nullptr;


    QPointF                 mMousePressedPos;

    GraphNodeSlot* mpTempPort = nullptr;

    GraphNodeSlot*          mpClosestPort   = nullptr;
    GraphNodeSlot*          mpGhostPort     = nullptr; // A temporary "port" object that will be used while mouse dragging the port to the destination.
    GraphGizmo*             mpGhostGizmo    = nullptr;
};
