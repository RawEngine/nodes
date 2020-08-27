
#pragma once

// Forwward declarations.
enum class GraphPortDataType;
class GraphNodeSlot;

class GraphNode : public QGraphicsItem
{
public:
    GraphNode(const QString& rName, const QPointF& rPosition);

    GraphNodeSlot* AddInputPort(GraphPortDataType type);
    GraphNodeSlot* AddOutputPort(GraphPortDataType type);

    QRectF boundingRect() const override;

    QString GetName() const { return mName; }

    bool IsUsingInputs() const { return !mInputPorts.isEmpty(); }
    bool IsUsingOutputs() const { return !mOutputPorts.isEmpty(); }

    const auto& GetInputs() const  { return mInputPorts;  }
    const auto& GetOutputs() const { return mOutputPorts; }

protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    static const QColor BackgroundColor;
    static const QColor BorderColorDefault;
    static const QColor BorderColorSelected;

    const QString mName;

    QGraphicsDropShadowEffect mShadowEffect;

    QVector<GraphNodeSlot*> mInputPorts;
    QVector<GraphNodeSlot*> mOutputPorts;
};
