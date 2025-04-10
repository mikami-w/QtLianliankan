#include "gamewindow.h"
#include "./ui_gamewindow.h"
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
    options(nullptr),
    help(nullptr)
{
    ui->setupUi(this);
    QString styleAppend = QString(R"(
    QPushButton { font-family: '%1'; }
    QLabel { font-family: '%2'; }
    )") .arg(getFontFamily(":/fonts/ZCOOLCanger.ttf"), getFontFamily(":/fonts/ZCOOLCangerBold.ttf"));
    this->setStyleSheet(styleSheet()+styleAppend);

    paintBackground(":/img/llk_main.bmp");

    startMenu->show();
    startMenu->raise();

}

GameWindow::~GameWindow()
{
    delete ui;
    delete debugWindow;
    delete startMenu;
    delete basicMode;
    delete gameMap;
    delete options;
    delete help;
}

void GameWindow::onBtnBasicClicked()
{
    startMenu->hide();
    if (!basicMode)
    {
        basicMode = new BasicMode(this);
    }

    setGameMap();
    basicMode->changeToThis();
}

void GameWindow::onBtnRestartClicked()
{
    setGameMap();
}

void GameWindow::onBtnBackClicked()
{
    if (gameMap) gameMap->hide();
    if (basicMode) basicMode->hide();
    if (options) options->hide();
    if (help) help->hide();

    prevPage->changeToThis();
}

void GameWindow::onBtnBackToMenuClicked()
{
    if (gameMap) gameMap->hide();
    if (basicMode) basicMode->hide();
    if (options) options->hide();
    if (help) help->hide();

    startMenu->changeToThis();
}

void GameWindow::onBtnOptionsClicked()
{
    startMenu->hide();
    if (basicMode) basicMode->hide();
    if (gameMap) gameMap->hide();

    if (!options)
    {
        options = new Options(this);
    }
    options->changeToThis();
}

void GameWindow::onBtnHelpClicked()
{
    startMenu->hide();
    if (basicMode) basicMode->hide();
    if (gameMap) gameMap->hide();

    if (!help)
    {
        help = new Help(this);
    }
    help->changeToThis();
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
