#pragma once

#include <QDialog>

#include "Chat/ChatConnection.h"

namespace Ui
{
class AuthDialog;
}

class AuthDialog: public QDialog
{
Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    Q_DISABLE_COPY(AuthDialog);
    
    ~AuthDialog() override;

public slots:
    void accept() override;

private:
    void onTextChanged(const QString &);

    void onSuccess(ChatConnection *connection);
    void onFail(const QString &reason) const;

    Ui::AuthDialog *ui;
};