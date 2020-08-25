
#pragma once

class GraphNodeSlot : public QGraphicsItem
{
public:
    GraphNodeSlot(QGraphicsScene& rScene);
    ~GraphNodeSlot();

protected:

    QRectF boundingRect() const override;

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:
};
