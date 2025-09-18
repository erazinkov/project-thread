#include "mytask.h"

#include <QThread>
#include <QRandomGenerator>
#include <QDebug>

MyTask::MyTask() : m_stop{false}, m_count{0}
{

}

void MyTask::doWork()
{
    QRandomGenerator rG;
    auto r = QRandomGenerator::global()->generate() % 1'000;
    for (uint i{0}; i < 1'000'000'000; ++i)
    {
        {
            QMutexLocker<QMutex> locker(&m_mutex);
            if (m_stop)
            {
                break;
            }
        }
        // QThread::currentThread()->msleep(r);
        m_count++;
    }
    qDebug() << QThread::currentThreadId() << m_count;
    emit(finished(m_count));
}

void MyTask::stop()
{
    QMutexLocker<QMutex> locker(&m_mutex);
    m_stop = true;
}

uint MyTask::count() const
{
    return m_count;
}
