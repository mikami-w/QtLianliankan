#include "help.h"
#include "gamewindow.h"
#include "ui_help.h"
#include <QKeyEvent>
#include <algorithm>

Help::Help(QWidget *parent)
    : QWidget(parent), ui(new Ui::Help)
{
    ui->setupUi(this);
    connect(ui->BtnBack, &QPushButton::clicked, static_cast<GameWindow*>(parent), &GameWindow::onBtnBackClicked);
}

Help::~Help()
{
    delete ui;
}

bool Help::eventFilter(QObject *watched, QEvent *event)
{// 防止子组件获取焦点导致键盘事件失效, 使用 filter
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug()<<"QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);";
        handleKeyboardDebug(keyEvent);
    }

    return false; // 使事件继续传播
}

void Help::handleKeyboardDebug(QKeyEvent* keyEvent)
{// 在帮助页面使用键盘输入 debug 序列打开 debug 窗口
    static const std::array<char, 5> debugSequence = {'d', 'e', 'b', 'u', 'g'};

    if (!keyEvent->text().isEmpty())
    {
        QChar ch = keyEvent->text().at(0).toLower();
        qDebug()<<ch;
        if (ch.unicode() >= 'a' && ch.unicode() <= 'z')
        {
            keyBuffer.push_back(static_cast<char>(ch.unicode()));
        }
    }

    // 限制 buffer 长度
    if (keyBuffer.size() > 5)
    {
        keyBuffer.pop_front();
    }

    if (keyBuffer.size() != 5)
    {
        return;
    }

    if (std::equal(keyBuffer.begin(), keyBuffer.end(), debugSequence.begin()))
    {
        keyBuffer.clear();
        static_cast<GameWindow *>(parent())->getDebugWindow()->show();
    }
}

void Help::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->installEventFilter(this);
    this->setFocus();
    this->activateWindow();
    this->raise();
}

void Help::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    this->removeEventFilter(this);
}

void Help::changeToThis()
{
    static_cast<GameWindow *>(parent())->paintBackground(":/res/img/fruit_bg.bmp");
    this->show();
}

