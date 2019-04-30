#pragma once

#include <QMainWindow>
#include <QLabel>

namespace Ui
{
class ChatWindow;
}

class ChatWindow: public QMainWindow
{
Q_OBJECT

public:
    explicit ChatWindow(ChatConnection *connection, QWidget *parent = nullptr);
    ~ChatWindow() override;

private slots:
    void on_send_clicked();
    void on_message_returnPressed();
    void on_message_textChanged(const QString &text);

private:
    QLabel *createMessageLabel(const QString &text);
    void onChatMessage(const QJsonObject &data);

    ChatConnection *connection;
    Ui::ChatWindow *ui;
};