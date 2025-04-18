#include "gamewindow.h"
#include "./ui_gamewindow.h"
#include "ui/ui_basicmode.h"
#include <QPalette>
#include <QBrush>
#include <QFontDatabase>
#include <QMessageBox>
#include <qevent.h>

QString GameWindow::getFontFamily(const QString &path)
{
    static QMap<QString, QString> fonts;
    if (fonts.contains(path))
        return fonts[path];

    int id = QFontDatabase::addApplicationFont(path);
    if (id == -1)
        return QString();

    return fonts[path] = QFontDatabase::applicationFontFamilies(id).value(0);
}

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow),
    debugWindow(nullptr),
    startMenu(new StartMenu(this)),
    basicMode(nullptr),
    gameMap(nullptr),
    infoPanel(nullptr),
    options(nullptr),
    help(nullptr)
{
    ui->setupUi(this);
    QString styleAppend = QString(R"(
    QPushButton { font-family: '%1'; }
    QLabel { font-family: '%2'; }
    )") .arg(getFontFamily(":/fonts/ZCOOLCanger.ttf"), getFontFamily(":/fonts/ZCOOLCangerBold.ttf"));
    this->setStyleSheet(styleSheet()+styleAppend);

    startMenu->changeToThis();
    startMenu->raise();

}

GameWindow::~GameWindow()
{
    delete ui;
    delete debugWindow;
    delete startMenu;
    delete basicMode;
    delete gameMap;
    delete infoPanel;
    delete options;
    delete help;
}

void GameWindow::onBtnBasicClicked()
{
    // startMenu->hide();
    hideAll();
    if (!basicMode)
    {
        basicMode = new BasicMode(this);
    }

    setGameMap();
    setInfoPanel();
    infoPanel->setTimer(true);
    infoPanel->restartTimer(InfoPanel::STANDARD_TIME);
    basicMode->changeToThis();
}

void GameWindow::onBtnZenClicked()
{

}

void GameWindow::onBtnRestartClicked()
{
    setGameMap();
    resume();
    if (infoPanel->isTimerEnabled())
    {
        infoPanel->restartTimer(InfoPanel::STANDARD_TIME);
    }
}

void GameWindow::onBtnBackClicked()
{
    hideAll();
    if (!paused && infoPanel)
        infoPanel->startTimer();

    prevPage->changeToThis();
}

void GameWindow::onBtnBackToMenuClicked()
{
    if (paused)
        resume();

    hideAll();

    startMenu->changeToThis();
}

void GameWindow::onBtnOptionsClicked()
{
    hideAll();

    if (!options)
    {
        options = new Options(this);
    }
    if (infoPanel) if (!paused && infoPanel->isTimerEnabled())
    {
        infoPanel->stopTimer();
    }

    options->changeToThis();
}

void GameWindow::onBtnHelpClicked()
{
    hideAll();

    if (!help)
    {
        help = new Help(this);
    }
    if (infoPanel) if (!paused && infoPanel->isTimerEnabled())
    {
        infoPanel->stopTimer();
    }

    help->changeToThis();
}

void GameWindow::onBtnPauseClicked()
{
    if (paused)
    {
        resume();
    }
    else
    {
        pause();
    }

}

void GameWindow::handleFullWindowToggle(bool checked)
{
    if (checked)
    {
        /*  */
        qDebug()<<"checked";
    }
    else
    {
        qDebug()<<"unchecked";
    }
}

void GameWindow::gameover()
{

    QMessageBox MsgGameover(QMessageBox::Information, "Gameover", "时间耗尽了. 已经是极限了吗?", QMessageBox::NoButton, this);
    auto BtnRestart = MsgGameover.addButton("重新开始", QMessageBox::AcceptRole);
    auto BtnStartMenu = MsgGameover.addButton("返回主菜单", QMessageBox::RejectRole);

    // 显示对话框, 同时阻断主事件循环
    MsgGameover.exec();


    if (MsgGameover.clickedButton() == BtnRestart)
    {
        onBtnRestartClicked();
    }
    else
    {
        onBtnBackToMenuClicked();
    }
}

void GameWindow::gameFinished()
{
    infoPanel->stopTimer();
    auto reply = QMessageBox::question(
        this, "Game Finished", "恭喜完成游戏!\n你想再来一局游戏来庆祝胜利吗?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        onBtnRestartClicked();
    }
    else if (reply == QMessageBox::No)
    {
        onBtnBackToMenuClicked();
    }
}

void GameWindow::paintBackground(QString image)
{
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(image)));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

void GameWindow::setGameMap()
{
    if (gameMap)
    {
        gameMap->reset();
        if(!gameMap->isVisible())
            gameMap->show();
    }
    else
    {
        gameMap = new GameMap(this);
        basicMode->bindGameMap(gameMap);
        gameMap->setGeometry(50,50,640,400);
        gameMap->show();
    }
}

void GameWindow::setInfoPanel()
{
    if (infoPanel)
    {
        if(!infoPanel->isVisible())
            infoPanel->show();
    }
    else
    {
        infoPanel = new InfoPanel(this);
        basicMode->bindInfoPanel(infoPanel);
        infoPanel->setGeometry(50,475,640,100);
        infoPanel->show();
    }
}

void GameWindow::hideAll()
{
    startMenu->hide();
    if (gameMap) gameMap->hide();
    if (infoPanel)
    {
        if (infoPanel->isTimerRunning())
            infoPanel->stopTimer();
        infoPanel->hide();
    }
    if (basicMode) basicMode->hide();
    if (options) options->hide();
    if (help) help->hide();
}

void GameWindow::pause()
{
    infoPanel->pause();
    basicMode->getUi()->BtnPause->setText("恢复");
    basicMode->getUi()->BtnHint->setEnabled(false);

    infoPanel->setEnabled(false);
    gameMap->setEnabled(false);
    paused = true;
}

void GameWindow::resume()
{
    infoPanel->resume();
    basicMode->getUi()->BtnPause->setText("暂停");
    basicMode->getUi()->BtnHint->setEnabled(true);

    infoPanel->setEnabled(true);
    gameMap->setEnabled(true);
    paused = false;
}

void GameWindow::closeEvent(QCloseEvent *event)
{
    auto reply = QMessageBox::question(this, "退出游戏", "确定要退出吗？",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        delete debugWindow;
        debugWindow = nullptr;
        event->accept();
    }
    else
        event->ignore();
}
