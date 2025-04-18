#include "infopanel.h"
#include "gamewindow.h"
#include "ui_infopanel.h"

InfoPanel::InfoPanel(QWidget *parent):
    QWidget(parent),
    ui(new Ui::InfoPanel),
    timer(new CountdownTimer())
{
    ui->setupUi(this);
    connect(timer, &CountdownTimer::pass1s, [this](){
        ui->LblTimer->setText(QString("剩余时间: %1 s").arg(timer->getRemainingTime()));
    });
    connect(timer, &CountdownTimer::timeout, static_cast<GameWindow*>(parent), &GameWindow::gameover);
}

InfoPanel::~InfoPanel()
{
    delete ui;
}

void InfoPanel::resetTimer(qint64 initTime)
{
    timer->reset(initTime);
}

void InfoPanel::stopTimer()
{
    timer->stop();
}

void InfoPanel::startTimer()
{
    timer->start();
}

void InfoPanel::setTimerEnabled(bool enable) noexcept
{
    timerEnabled = enable;
    if (!enable)
    {
        ui->LblTimer->setText("剩余时间: +∞ s");
        timer->reset(0);
    }
}

void InfoPanel::pause()
{
    if (isTimerEnabled())
    {
        stopTimer();
    }
}

void InfoPanel::resume()
{
    if (isTimerEnabled())
    {
        startTimer();
    }
}
