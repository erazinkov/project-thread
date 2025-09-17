#ifndef MYTASK_H
#define MYTASK_H

#include <QObject>
#include <QRunnable>
#include <QMutex>

class MyTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    MyTask();

    void run() override;
signals:
    void finished(uint c);
public slots:
    void stop();
private:
    QMutex m_mutex;
    bool m_stop;
};

#endif // MYTASK_H
