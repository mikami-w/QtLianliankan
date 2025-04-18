#ifndef INFOPANEL_H
#define INFOPANEL_H

#include "countdowntimer.h"
#include <QWidget>

namespace Ui {
class InfoPanel;
}

class InfoPanel : public QWidget
{
    Q_OBJECT

public:
    explicit InfoPanel(QWidget *parent = nullptr);
    ~InfoPanel();

    void restartTimer(qint64 initTime); // 重置并启动计时器
    void stopTimer(); //暂停计时器, 保留上次的进度
    void startTimer(); // 从上次的进度启动计时器
    bool isTimerEnabled() const noexcept { return timerEnabled; }
    bool isTimerRunning() const noexcept { return timer->isRunning(); }
    void setTimer(bool enable) noexcept { timerEnabled = enable; }
    void pause();
    void resume();

    constexpr const static qint64 STANDARD_TIME = 200;

private:
    Ui::InfoPanel* ui;
    CountdownTimer* timer;
    bool timerEnabled = false;
};

#endif // INFOPANEL_H
