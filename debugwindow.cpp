#include "debugwindow.h"
#include "ui_debugwindow.h"
#include <qevent.h>

DebugWindow::DebugWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, false);
    connect(ui->CBEnabled, &QCheckBox::toggled, [this](bool checked) {
        debugOptions.debugMode = checked;
        ui->CBGoldenFinger->setEnabled(checked);
    });

    connect(ui->CBGoldenFinger, &QCheckBox::toggled, [this](bool checked) {
        debugOptions.goldenFinger = checked;
        ui->SBItemNum->setEnabled(checked);
    });

    connect(ui->BtnExitDebug, &QPushButton::clicked,
            [this]() { this->close(); });
}

DebugWindow::~DebugWindow()
{
    delete ui;
}

void DebugWindow::setMode(bool value, Mode mode)
{
    switch (mode)
    {
    case Mode::GoldenFinger: {
        debugOptions.goldenFinger = value;
        break;
    }

    default:
        assert(0);
    }
}

void DebugWindow::closeEvent(QCloseEvent *event)
{
    ui->CBEnabled->setCheckState(Qt::Unchecked);
    hide();
    event->ignore();
}

