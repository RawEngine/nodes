
#pragma once

#include <QGraphicsItem>

class GraphNode : public QGraphicsItem
{
public:
    GraphNode(const QString& rName);

protected:

    QRectF boundingRect() const override;

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* pEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent) override;

private:

    const QString& mName;

    bool mIsSelected = false;
};
