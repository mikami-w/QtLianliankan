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
    // installEventFilter(this);
}

Help::~Help()
{
    delete ui;
}

bool Help::eventFilter(QObject *watched, QEvent *event)
{
    // qDebug()<<"in bool Help::eventFilter(QObject *watched, QEvent *event)";
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug()<<"QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);";
        handleKeyboardDebug(keyEvent);
    }

    return false;
}

void Help::handleKeyboardDebug(QKeyEvent* keyEvent)
{
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
        // static_cast<GameWindow *>(parent())->getDebugWindow()->setDebugMode(true);
        static_cast<GameWindow *>(parent())->getDebugWindow()->show();
    }
}

void Help::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    this->installEventFilter(this);
    this->setFocus();
    this->activateWindow();
    this->raise();
}

void Help::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    this->removeEventFilter(this);
}

void Help::changeToThis()
{
    static_cast<GameWindow *>(parent())->paintBackground(":/res/img/fruit_bg.bmp");
    this->show();
}

