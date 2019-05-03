#include <QPushButton>
#include <QtWebSockets/QWebSocket>
#include <QPlainTextEdit>
#include <QJsonArray>
#include <QTextBrowser>
#include <QScrollBar>

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

    QWidget::findChild<QPushButton *>("send")->setEnabled(false);
    QWidget::findChild<QLineEdit *>("message")->setFocus();
    QWidget::findChild<QTextBrowser *>("messages")->clear();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::insertMessage(const QString &message)
{
    static const QString tagBegin = "<p style=\"margin-top:0px; margin-bottom:0px; margin-left:0px;"
                                    "margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
    static const QString tagEnd = "</p>";

    auto *browser = QWidget::findChild<QTextBrowser *>("messages");     ///< Messages
    if (browser->toPlainText().isEmpty()) {
        // Replace all html code, if there are nothing
        browser->setHtml(
            tagBegin + message + tagEnd
        );
    } else {
        // Append html
        QString html = browser->toHtml();           ///< Messages html
        int endIndex = html.indexOf("</body>");
        html.insert(
            endIndex,
            tagBegin + message + tagEnd
        );
        browser->setHtml(html);
    }

    // Scroll to bottom
    browser->verticalScrollBar()->setValue(
        browser->verticalScrollBar()->maximum()
    );
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

    ChatWindow::insertMessage(text);
}

void ChatWindow::onErrorMessage(const QJsonObject &data)
{
    QString text = "<a style=\"color: #f00\">Error: " + data["text"].toString() + "</a>";

    ChatWindow::insertMessage(text);
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

void ChatWindow::on_message_returnPressed()
{
    this->findChild<QPushButton *>("send")->animateClick();
}

void ChatWindow::on_message_textChanged(const QString &text)
{
    this->findChild<QPushButton *>("send")->setEnabled(!text.isEmpty());
}
