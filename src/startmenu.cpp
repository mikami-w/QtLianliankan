#include "startmenu.h"
#include "gamewindow.h"
#include "ui_startmenu.h"

StartMenu::StartMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartMenu)
{
    ui->setupUi(this);
    connect(ui->BtnBasic, &QPushButton::clicked, static_cast<GameWindow*>(parent), &GameWindow::onBtnBasicClicked);
    connect(ui->BtnZen, &QPushButton::clicked, static_cast<GameWindow*>(parent), &GameWindow::onBtnZenClicked);
    connect(ui->BtnOptions, &QPushButton::clicked, [this, parent]() {
        static_cast<GameWindow *>(parent)->onBtnOptionsClicked();
        static_cast<GameWindow *>(parent)->prevPage = static_cast<ReturnablePage*>(this);
    });
    connect(ui->BtnHelp, &QPushButton::clicked, [this, parent]() {
        static_cast<GameWindow *>(parent)->onBtnHelpClicked();
        static_cast<GameWindow *>(parent)->prevPage = static_cast<ReturnablePage*>(this);
    });
}

StartMenu::~StartMenu()
{
    delete ui;
}

void StartMenu::changeToThis()
{
    static_cast<GameWindow *>(parent())->setWindowTitle("欢乐连连看");
    static_cast<GameWindow *>(parent())->paintBackground(":/img/llk_main.bmp");
    this->show();
}
