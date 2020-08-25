
#pragma once

class GraphNodeSlot;

class GraphNode : public QGraphicsItem
{
public:
    GraphNode(QGraphicsScene& rScene, const QString& rName, const QPointF& rPosition);

    QRectF boundingRect() const override;

    const QString& GetName() const { return mName; }


protected:

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    static constexpr auto BackgroundColor = QColor(30, 30, 30);
    static constexpr auto BorderColorDefault = QColor(9, 9, 9);
    static constexpr auto BorderColorSelected = QColor(39, 214, 45);

    const QString mName;

    QGraphicsDropShadowEffect mShadowEffect;

public: // TEMP!

    QVector<GraphNodeSlot*> mInputSlots;
};
