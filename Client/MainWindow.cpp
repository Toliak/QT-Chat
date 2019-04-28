#include <iostream>
#include <QtWebSockets/QWebSocket>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendMessage_clicked(bool what)
{
    auto messageObject = this->findChild<QPlainTextEdit*>("message");
    QString message = messageObject->toPlainText();

    std::cout << message.toStdString() << std::endl;
}