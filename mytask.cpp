#include "mytask.h"

#include <QThread>
#include <QRandomGenerator>
#include <QDebug>

MyTask::MyTask() : m_stop{false}, m_count{0}
{

}

MyTask::~MyTask()
{
    qDebug() << QThread::currentThreadId() << "Destructor called";
}

void MyTask::doWork()
{
    QRandomGenerator rG;
    auto r = QRandomGenerator::global()->generate() % 1'00;
    if (r < 50)
    {
        qDebug() << QThread::currentThreadId() << r;
        throw std::runtime_error("Haste makes waste!");
    }
    for (uint i{0}; i < 10; ++i)
    {
        {
            QMutexLocker<QMutex> locker(&m_mutex);
            if (m_stop)
            {
                break;
            }
        }
        QThread::currentThread()->msleep(r);
        m_count++;
    }
    qDebug() << QThread::currentThreadId() << m_count;
    emit(finished(m_count));
}

void MyTask::stop()
{
    QMutexLocker<QMutex> locker(&m_mutex);
    m_stop = true;
    qDebug() << QThread::currentThreadId() << "Stop called";
}

uint MyTask::count() const
{
    return m_count;
}
