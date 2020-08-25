
#pragma once

#include "ui_MainWindow.h"

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

private:   
    Ui::MainWindow mUI;

    QGraphicsScene* mpScene;
};
