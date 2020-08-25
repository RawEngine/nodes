
#pragma once

#include "ui_NodeDialog.h"

class NodeDialog final : public QDialog
{
public:
    NodeDialog();

    QString GetNodeName() const;
    int GetNumInputs() const;
    int GetNumOutputs() const;

private:

    Ui::NodeDialog mUI;
};
