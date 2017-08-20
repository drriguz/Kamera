#ifndef DECODE_THREAD_H
#define DECODE_THREAD_H

#include <QThread>

class DecodeThread: public QThread
{
    Q_OBJECT
public:
    explicit DecodeThread(QObject *parent = Q_NULLPTR);
    ~DecodeThread();
    void stop();
protected:
    void run();
private:
    volatile bool running;
};

#endif // DECODE_THREAD_H
