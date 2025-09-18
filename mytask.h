#ifndef MYTASK_H
#define MYTASK_H

#include <QObject>
#include <QRunnable>
#include <QMutex>

class MyTask : public QObject
{
    Q_OBJECT
public:
    MyTask();
    void doWork();
    uint count() const;

signals:
    void finished(uint c);
public slots:
    void stop();
private:
    QMutex m_mutex;
    bool m_stop;
    uint m_count;
};

#endif // MYTASK_H
