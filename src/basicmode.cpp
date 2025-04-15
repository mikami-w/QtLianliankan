#include "ui/ui_basicmode.h"
#include "basicmode.h"
#include "gamewindow.h"

BasicMode::BasicMode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BasicMode)
{
    ui->setupUi(this);
    connect(ui->BtnRestart, &QPushButton::clicked, static_cast<GameWindow*>(parent), &GameWindow::onBtnRestartClicked);
    connect(ui->BtnBackToMenu, &QPushButton::clicked, static_cast<GameWindow*>(parent), &GameWindow::onBtnBackToMenuClicked);
    connect(ui->BtnOptions, &QPushButton::clicked, [this, parent]() {
        static_cast<GameWindow *>(parent)->onBtnOptionsClicked();
        static_cast<GameWindow *>(parent)->prevPage = static_cast<ReturnablePage*>(this);
    });
    connect(ui->BtnHelp, &QPushButton::clicked, [this, parent]() {
        static_cast<GameWindow *>(parent)->onBtnHelpClicked();
        static_cast<GameWindow *>(parent)->prevPage = static_cast<ReturnablePage*>(this);
    });
}

BasicMode::~BasicMode()
{
    delete ui;
}

void BasicMode::changeToThis()
{
    static_cast<GameWindow*>(parent())->setWindowTitle("欢乐连连看--基本模式");
    static_cast<GameWindow*>(parent())->paintBackground(":/img/fruit_bg.bmp");
    this->show();
    gameMapWeak->show();
}

void BasicMode::bindGameMap(GameMap *map)
{
    gameMapWeak = map;
    connect(ui->BtnHint, &QPushButton::clicked, gameMapWeak,
            &GameMap::onBtnHintClicked);
}
