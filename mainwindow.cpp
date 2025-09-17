#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQueue>
#include <QThreadPool>

#include "mytask.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_buttonStart = new QPushButton("Start", this);
    m_buttonStop = new QPushButton("Stop", this);
    m_progressBar = new QProgressBar(this);

    ui->gridLayout->addWidget(m_buttonStart);
    ui->gridLayout->addWidget(m_buttonStop);
    ui->gridLayout->addWidget(m_progressBar);

    connect(m_buttonStart, &QPushButton::clicked, this, &MainWindow::handleClickedStart);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleClickedStart()
{
    QQueue<MyTask *> myTasks;
    const int n{10};
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
    for (const auto &item : myTasks)
    {
        MyTask *newTask = myTasks.dequeue();
        threadPool->start(newTask);
        connect(newTask, &MyTask::finished, this, [&](uint c){
            auto v{m_progressBar->value()};
            m_progressBar->setValue(++v);
            qDebug() << "Finished" << c;
        }, Qt::QueuedConnection);
        connect(m_buttonStop, &QPushButton::clicked, newTask, &MyTask::stop, Qt::QueuedConnection);
    }
}

void MainWindow::handleClickedStop()
{

}

