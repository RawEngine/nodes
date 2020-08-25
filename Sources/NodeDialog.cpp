
#include "PCH.hpp"

#include "Headers/NodeDialog.hpp"

NodeDialog::NodeDialog()
    : QDialog()
{
    mUI.setupUi(this);

    // Get rid of "help" button.
    auto flags = QDialog::windowFlags();

    flags &= ~Qt::WindowContextHelpButtonHint;

    QDialog::setWindowFlags(flags);
}

QString NodeDialog::GetNodeName() const
{
    return mUI.editName->text();
}

int NodeDialog::GetNumInputs() const
{
    return mUI.spinNumInputs->value();
}

int NodeDialog::GetNumOutputs() const
{
    return mUI.spinNumOutputs->value();
}
