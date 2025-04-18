#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

class CountdownTimer : public QObject
{
    Q_OBJECT

public:
    explicit CountdownTimer(QObject *parent = nullptr);
    ~CountdownTimer();
    void start();
    void stop();
    void reset(qint64 initTime);

    bool isRunning() noexcept { return running; }
    qint64 getRemainingTime() noexcept { return time - 1; }

signals:
    void timeout();
    void pass1s();


private:
    qint64 time = 0;
    qint64 remainingTimeToNextSecond = 0; // 计时器暂停后到下一秒剩余的时间(ms)
    QTimer* qtimer;
    QElapsedTimer* elapsedTimer;
    bool running = false;
};

#endif // COUNTDOWNTIMER_H
