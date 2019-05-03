#include <QApplication>

#include "Window/AuthDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AuthDialog authWindow;
    authWindow.show();

    return QApplication::exec();
}