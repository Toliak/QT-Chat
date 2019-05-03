#include <QIcon>
#include <QPushButton>

#include "AuthDialog.h"
#include "ui_AuthDialog.h"
#include "Window/ChatWindow.h"

AuthDialog::AuthDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::AuthDialog)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(), this->height());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    this->findChild<QLabel *>("error")->hide();
    this->findChild<QDialogButtonBox *>("buttonBox")->button(QDialogButtonBox::Ok)->setEnabled(false);

    auto *address = this->findChild<QLineEdit *>("address");
    address->setFocus();

    connect(address, &QLineEdit::textChanged, this, &AuthDialog::onTextChanged);
    connect(this->findChild<QLineEdit *>("name"), &QLineEdit::textChanged, this, &AuthDialog::onTextChanged);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::accept()
{
    auto *buttonBox = qobject_cast<QDialogButtonBox *>(sender());
    buttonBox->setEnabled(false);

    auto *address = this->findChild<QLineEdit *>("address");
    auto *name = this->findChild<QLineEdit *>("name");

    auto *connection = new ChatConnection(
        address->text()
    );

    connect(connection, &ChatConnection::login, [this, connection]() {
        AuthDialog::onSuccess(connection);
    });
    connect(connection, &ChatConnection::fail, this, &AuthDialog::onFail);
    connect(connection, &ChatConnection::fail, connection, &QObject::deleteLater);

    connection->start(
        QJsonObject{
            {"name", name->text()},
        }
    );
}

void AuthDialog::onSuccess(ChatConnection *connection)
{
    auto *mainWindow = new ChatWindow(connection);
    mainWindow->show();

    QDialog::accept();
}

void AuthDialog::onFail(const QString &reason) const
{
    auto error = this->findChild<QLabel *>("error");
    error->setText("Failed: " + reason);
    error->show();

    auto *buttonBox = this->findChild<QDialogButtonBox *>("buttonBox");
    buttonBox->setEnabled(true);
}

void AuthDialog::onTextChanged(const QString &)
{
    bool state =
        !(this->findChild<QLineEdit *>("address")->text().isEmpty()
            || this->findChild<QLineEdit *>("name")->text().isEmpty());

    this->findChild<QDialogButtonBox *>("buttonBox")->button(QDialogButtonBox::Ok)->setEnabled(state);
}
