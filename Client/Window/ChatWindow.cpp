#include <QPushButton>
#include <QtWebSockets/QWebSocket>
#include <QtWidgets/QPlainTextEdit>
#include <Chat/ChatConnection.h>

#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(ChatConnection *connection, QWidget *parent)
    : QMainWindow(parent),
      connection(connection),
      ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    connect(connection, &ChatConnection::chatMessage, this, &ChatWindow::onChatMessage);

    this->findChild<QPushButton *>("send")->setEnabled(false);
    this->findChild<QLineEdit *>("message")->setFocus();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::onChatMessage(const QJsonObject &data)
{
    QString text;
    if (data["type"] == 0) {
        text = data["name"].toString() + ": " + data["text"].toString();
    } else if (data["type"] == 1) {
        text = data["name"].toString() + " has been connected";
    } else if (data["type"] == 2) {
        text = data["name"].toString() + " has been disconnected";
    } else {
        text = "Wrong message type";
    }

    auto *area = this->findChild<QWidget *>("messageArea");
    area->layout()->addWidget(
        createMessageLabel(text)
    );
}

void ChatWindow::on_send_clicked()
{
    auto *button = qobject_cast<QPushButton *>(sender());
    if (!button->isEnabled()) {
        return;
    }

    auto messageObject = this->findChild<QLineEdit *>("message");
    QString message = messageObject->text();
    messageObject->setText("");

    ChatWindow::connection->sendMessage(message);
}

QLabel *ChatWindow::createMessageLabel(const QString &text)
{
    auto *label = new QLabel(text);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    label->show();

    return label;
}

void ChatWindow::on_message_returnPressed()
{
    this->findChild<QPushButton *>("send")->animateClick();
}

void ChatWindow::on_message_textChanged(const QString &text)
{
    this->findChild<QPushButton *>("send")->setEnabled(!text.isEmpty());
}
