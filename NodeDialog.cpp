
#include "PCH.hpp"

#include "NodeDialog.hpp"

NodeDialog::NodeDialog()
    : QDialog()
{
    mUI.setupUi(this);

    // Get rid of "help" button.
    auto flags = QDialog::windowFlags();

    flags &= ~Qt::WindowContextHelpButtonHint;

    QDialog::setWindowFlags(flags);
}
