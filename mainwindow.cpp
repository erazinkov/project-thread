#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQueue>
#include <QThreadPool>
#include <QTextStream>

#include <cstdio>

#include "mytask.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_buttonStart = new QPushButton("Start", this);
    m_buttonStop = new QPushButton("Stop", this);
    m_progressBar = new QProgressBar(this);

    m_list = new QList<uint>();

    ui->gridLayout->addWidget(m_buttonStart);
    ui->gridLayout->addWidget(m_buttonStop);
    ui->gridLayout->addWidget(m_progressBar);

    connect(m_buttonStart, &QPushButton::clicked, this, &MainWindow::handleClickedStart);
    connect(m_progressBar, &QProgressBar::valueChanged, this, [&](int value){
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleClickedStart()
{
    m_list->clear();
    QQueue<MyTask *> myTasks;
    const int n{15};
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(n);
    m_progressBar->setValue(0);
    for (auto i{0}; i < n; ++i)
    {
        MyTask *myTask = new MyTask();
        myTasks.enqueue(myTask);
        myTask->setAutoDelete(true);
    }
    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->setMaxThreadCount(QThread::idealThreadCount());
    for (const auto &item : myTasks)
    {
        MyTask *newTask = myTasks.dequeue();
        threadPool->start(newTask);
        connect(newTask, &MyTask::finished, this, [&](uint c){
            auto v{m_progressBar->value()};
            m_progressBar->setValue(++v);
            m_list->push_back(c);
        }, Qt::QueuedConnection);
        connect(m_buttonStop, &QPushButton::clicked, newTask, &MyTask::stop, Qt::QueuedConnection);
    }
}

void MainWindow::handleClickedStop()
{

}

