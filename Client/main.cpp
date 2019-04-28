#include <iostream>
#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    std::cout << "Hello, World!" << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return QApplication::exec();
}