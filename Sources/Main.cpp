
#include "PCH.hpp"

#include "Headers/MainWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    return a.exec();
}
