#pragma once

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow: public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_sendMessage_clicked(bool what);

private:
    Ui::MainWindow *ui;
};