#include <iostream>
#include <QObject>
#include <QThread>
#include <QApplication>

class Worker: public QObject
{
Q_OBJECT

public slots:
    void doWork(const QString &parameter)
    {
        QString result = parameter;

        result += " It works!!";


        emit resultReady(result);
    }

signals:
    void resultReady(const QString &result);
};

class Controller: public QObject
{
Q_OBJECT
    QThread workerThread;
public:
    Controller()
    {
        auto *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
//        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
        worker->doWork("Test string");
    }

    void start(const QString &input)
    {
        emit operate(input);
    }

    ~Controller() override
    {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &result)
    {
        std::cout << result.toStdString() << std::endl;
    }
signals:
    void operate(const QString &);
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto *controller = new Controller;

//    controller->start("Test string");

    QThread::sleep(10);

    controller->deleteLater();

}