
#pragma once

#include <QGraphicsView>

class GraphView final : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(QWidget* pParent);
    ~GraphView();
};
