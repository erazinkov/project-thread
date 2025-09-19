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
    connect(m_fW, &QFutureWatcher<void>::finished, this, [&](){
        qDebug() << "Finished";
        m_list->clear();
        foreach (const auto t, m_myTasks)
        {
            m_list->push_back(t->count());
        }
        auto s{m_list->size()};
        if (s)
        {
            QString str;
            str.append(QString("[%1]").arg(m_list->size()));
            str.append("{");
            foreach (const uint &item, *m_list)
            {
                str.append(" ");
                str.append(QString::number(item));
            }
            str.append(" }");
            qDebug() << str;
        }
    });
    connect(m_buttonStop, &QPushButton::clicked, m_fW, &QFutureWatcher<void>::cancel);

    qDebug() << QThread::currentThreadId();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleClickedStart()
{
    m_fW->cancel();
    m_myTasks.clear();
    const int n{25};
    for (auto i{0}; i < n; ++i)
    {
        MyTask *myTask = new MyTask();
        m_myTasks.push_back(myTask);
        connect(m_buttonStop, &QPushButton::clicked, myTask, &MyTask::stop);
    }
//    auto future = QtConcurrent::mapped(m_myTasks, [](MyTask *myTask) {
//            myTask->doWork();
//            return myTask;
//    }).then([](QFuture<MyTask *> f) {
//        QList<uint> l;
//        for (auto r : f.results()) {
//            l.push_back(r->count());
//        }
//        return l;
//    });

    auto p = [](MyTask *myTask){
        myTask->doWork();
    };

    auto future = QtConcurrent::map(m_myTasks, p);
    m_fW->setFuture(future);
}

void MainWindow::handleClickedStop()
{

}

