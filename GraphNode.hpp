
#pragma once

class GraphNodeSlot;

class GraphNode : public QGraphicsItem
{
public:
    GraphNode(QGraphicsScene& rScene, const QString& rName);

protected:

    QRectF boundingRect() const override;

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    static constexpr QColor BackgroundColor = QColor(30, 30, 30);
    static constexpr QColor BorderColorDefault = QColor(9, 9, 9);
    static constexpr QColor BorderColorSelected = QColor(39, 214, 45);

    const QString& mName;


    bool mIsSelected = false;

    QGraphicsDropShadowEffect mShadowEffect;

    QVector<GraphNodeSlot*> mInputSlots;
};
