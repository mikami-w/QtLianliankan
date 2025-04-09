#include "options.h"
#include "gamewindow.h"
#include "ui_options.h"

Options::Options(
    QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Options)
{
    ui->setupUi(this);
    connect(ui->CBFullWindow, &QCheckBox::toggled, static_cast<GameWindow*>(parent), &GameWindow::handleFullWindowToggle);
    connect(ui->BtnBack, &QPushButton::clicked, static_cast<GameWindow*>(parent), &GameWindow::onBtnBackClicked);
}

Options::~Options()
{
    delete ui;
}

void Options::changeToThis()
{
    // static_cast<GameWindow*>(parent())->setWindowTitle("欢乐连连看");
    static_cast<GameWindow*>(parent())->paintBackground(":/res/img/fruit_bg.bmp");
    this->show();
}

