#include "mytask.h"

#include <QThread>
#include <QRandomGenerator>
#include <QDebug>

MyTask::MyTask() : m_stop{false}
{

}

void MyTask::run()
{
    uint c{0};
    QRandomGenerator rG;
    auto r = QRandomGenerator::global()->generate() % 1000;
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
        c++;
    }
    emit(finished(c));
}

void MyTask::stop()
{
    QMutexLocker<QMutex> locker(&m_mutex);
    m_stop = true;
}
