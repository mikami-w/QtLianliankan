#include "countdowntimer.h"

CountdownTimer::CountdownTimer(QObject *parent):
    QObject{parent},
    qtimer(new QTimer(this)),
    elapsedTimer(new QElapsedTimer())
{
    connect(qtimer, &QTimer::timeout, [this]() {
        if (running) // 防止用户快速调用start后调用stop
        {
            emit pass1s();

            if (--time == 0)
            {
                stop();
                emit timeout();
            }
        }
    });
}

CountdownTimer::~CountdownTimer()
{
    delete qtimer;
    delete elapsedTimer;
}

void CountdownTimer::start()
{
    if (running)
        return;

    running = true;
    elapsedTimer->start();
    QTimer::singleShot(remainingTimeToNextSecond, [this](){
        if (running)
        {
            emit pass1s();

            if (--time == 0)
            {
                stop();
                emit timeout();
            }
            else
            {
                qtimer->start(1000);
            }
        }
    });
}

void CountdownTimer::stop()
{
    // qtimer->stop();
    // running = false;

    if (!running)
        return;

    qtimer->stop();
    remainingTimeToNextSecond = 1000 - elapsedTimer->elapsed() % 1000;
    running = false;
}

void CountdownTimer::reset(qint64 initTime)
{
    stop(); // 先停止再重置
    time = initTime;
    remainingTimeToNextSecond = 0;
}
