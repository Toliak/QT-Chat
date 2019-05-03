#include <QPushButton>
#include <QtWebSockets/QWebSocket>
#include <QPlainTextEdit>
#include <QJsonArray>

#include "Chat/ChatConnection.h"
#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(ChatConnection *connection, QWidget *parent)
    : QMainWindow(parent),
      connection(connection),
      ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    connect(connection, &ChatConnection::chatMessage, this, &ChatWindow::onChatMessage);
    connect(connection, &ChatConnection::errorMessage, this, &ChatWindow::onErrorMessage);

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
    if (data["type"] == 0) {        // Default message
        text = data["name"].toString() + ": " + data["text"].toString();
    } else if (data["type"] == 1) { // Connection message
        text = data["name"].toString() + " has been connected";
    } else if (data["type"] == 2) { // Disconnection message
        text = data["name"].toString() + " has been disconnected";
    } else if (data["type"] == 3) { // Available clients message
        // Transform array to string
        QJsonArray names = data["names"].toArray();     ///< Result string
        for (auto it = names.constBegin(); it != names.constEnd(); it++) {
            if (it != names.constBegin()) {
                text += ", ";
            }
            text += it->toString();
        }

        text += " online";
    } else {
        text = "Wrong message type";
    }

    auto *area = this->findChild<QWidget *>("messageArea");
    area->layout()->addWidget(
        createMessageLabel(text)
    );
}

void ChatWindow::onErrorMessage(const QJsonObject &data)
{
    QString text = "Error: " + data["text"].toString();

    QLabel *label = createMessageLabel(text);
    label->setStyleSheet(".QLabel {color: rgb(255, 0, 0)}");

    auto *area = this->findChild<QWidget *>("messageArea");
    area->layout()->addWidget(label);
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

ChatLabel *ChatWindow::createMessageLabel(const QString &text)
{
    auto *label = new ChatLabel(text);     ///< Result Label

    // Set vertical size to minimum
//    QSizePolicy sizePolicy = label->sizePolicy();
//    sizePolicy.setVerticalPolicy(QSizePolicy::Maximum);
//    label->setSizePolicy(sizePolicy);

    // Now text can be selected
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // Wrapping by splitter
    label->setWordWrap(true);

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

void ChatWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Wrap all labels
    QList<ChatLabel *> list = this->findChild<QWidget *>("messageArea")->findChildren<ChatLabel *>();
    for (ChatLabel *label : list) {
        label->fullWrap();
    }
}
