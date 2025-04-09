#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include "ui_debugwindow.h"
#include <QWidget>

class GameWindow;

struct DebugOptions
{
    bool debugMode = false;
    bool goldenFinger = false;
};

namespace Ui {
class DebugWindow;
}

class DebugWindow : public QWidget
{
    Q_OBJECT
    friend class GameWindow;
public:
    enum Mode: char
    {
        GoldenFinger = 1,
    };
    explicit DebugWindow(QWidget *parent = nullptr);
    ~DebugWindow();

    void setDebugMode(bool mode) { debugOptions.debugMode = mode; }
    bool inDebugMode() const { return getDebugOptions().debugMode; }
    int getGoldenFingerValue() const { return ui->SBItemNum->value(); }
    const DebugOptions& getDebugOptions() const { return debugOptions; }
    void setMode(bool value, Mode mode);

    void closeEvent(QCloseEvent *event) override;


private:
    Ui::DebugWindow *ui;
    DebugOptions debugOptions;
};

#endif // DEBUGWINDOW_H
