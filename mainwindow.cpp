#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQueue>
#include <QThreadPool>
#include <QTextStream>
#include <QFuture>
#include <QtConcurrent>
#include <cstdio>

#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_buttonStart = new QPushButton("Start", this);
    m_buttonStop = new QPushButton("Stop", this);
    m_progressBar = new QProgressBar(this);

    m_list = new QList<uint>();
    m_fW = new QFutureWatcher<void>(this);

    ui->gridLayout->addWidget(m_buttonStart);
    ui->gridLayout->addWidget(m_buttonStop);
    ui->gridLayout->addWidget(m_progressBar);

    connect(m_buttonStart, &QPushButton::clicked, this, &MainWindow::handleClickedStart);
    connect(m_fW, &QFutureWatcher<void>::progressRangeChanged, m_progressBar, &QProgressBar::setRange);
    connect(m_fW, &QFutureWatcher<void>::progressValueChanged, m_progressBar, &QProgressBar::setValue);
//    connect(m_fW, &QFutureWatcher<void>::progressRangeChanged, [](int min, int max){
//        qDebug() << min << max;
//    });
//    connect(m_fW, &QFutureWatcher<void>::progressValueChanged, m_progressBar, &QProgressBar::setValue);
//    connect(m_progressBar, &QProgressBar::valueChanged, this, [&](int value){
//        auto s{m_list->size()};
//        if (s)
//        {
//            QString str;
//            str.append(QString("[%1]").arg(m_list->size()));
//            str.append("{");
//            foreach (const uint &item, *m_list)
//            {
//                str.append(" ");
//                str.append(QString::number(item));
//            }
//            str.append(" }");
//            qDebug() << str;
//        }
//    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleClickedStart()
{
    m_myTasks.clear();
//    QList<MyTask *> myTasks;
    const int n{5};
//    m_progressBar->setMinimum(0);
//    m_progressBar->setMaximum(n);
//    m_progressBar->setValue(0);
    for (auto i{0}; i < n; ++i)
    {
        MyTask *myTask = new MyTask();
        m_myTasks.push_back(myTask);
//        connect(m_buttonStop, &QPushButton::clicked, myTask, &MyTask::stop);
    }
//    auto process = [](int val) {
//        return val * 2;
//    };
//    QList<int> inputs { 1, 2, 3 };
//    auto goodFuture = QtConcurrent::mapped(inputs, process)
//                              .then([](QFuture<int> f) {
//                                  for (auto r : f.results()) {
//                                      qDebug() << r;
//                                  }
//                              });
//    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
//    auto future = QtConcurrent::mapped(myTasks, [](MyTask *myTask) {
//            myTask->doWork();
//            return myTask;
//        }).then([](QFuture<MyTask *> f) {
//        for (auto r : f.results()) {
//            qDebug() << r->count();
//        }
//    });
    auto future = QtConcurrent::map(m_myTasks, [](MyTask *myTask) {
            myTask->doWork();
        });
    m_fW->setFuture(future);

//    auto myTask = [&](){
//        QThread::currentThread()->msleep(5'000);
//        uint c{50};
//        m_list->push_back(c);
//        m_progressBar->setValue(c);
//    };
//    QFuture future = QtConcurrent::task(std::move(myTask)).spawn();

//    m_list->clear();
//    QQueue<MyTask *> myTasks;
//    const int n{32};
//    m_progressBar->setMinimum(0);
//    m_progressBar->setMaximum(n);
//    m_progressBar->setValue(0);
//    for (auto i{0}; i < n; ++i)
//    {
//        MyTask *myTask = new MyTask();
//        myTasks.enqueue(myTask);
//        myTask->setAutoDelete(true);

//    }
//    QThreadPool *threadPool = QThreadPool::globalInstance();
//    qDebug() << QThread::idealThreadCount();
//    threadPool->setMaxThreadCount(QThread::idealThreadCount());
//    for (const auto &item: myTasks)
//    {
//        MyTask *newTask = myTasks.dequeue();
//        threadPool->start(newTask);
//        connect(newTask, &MyTask::finished, this, [&](uint c){
//            auto v{m_progressBar->value()};
//            m_progressBar->setValue(++v);
//            m_list->push_back(c);
//        }, Qt::QueuedConnection);
//        connect(m_buttonStop, &QPushButton::clicked, newTask, &MyTask::stop);
//    }
}

void MainWindow::handleClickedStop()
{

}

