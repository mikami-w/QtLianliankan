#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "basicmode.h"
#include "gamemap.h"
#include "help.h"
#include "options.h"
#include "startmenu.h"
#include "debugwindow.h"
#include "ReturnablePage.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    static QString getFontFamily(const QString& path);
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    DebugWindow *getDebugWindow() {
        if (!debugWindow)
            debugWindow = new DebugWindow(nullptr);
        return debugWindow;
    }

    void paintBackground(QString image);
    void setGameMap();

    void closeEvent(QCloseEvent *event) override;

    // 若跳转到的页面可能调用 void onBtnBackClicked() 函数则需要设置该字段值
    ReturnablePage* prevPage = nullptr;

public slots:
    void onBtnBasicClicked();
    void onBtnRestartClicked();
    void onBtnBackClicked();    // 用于所有返回的 back 按钮
    void onBtnBackToMenuClicked();  // 用于返回主菜单
    void onBtnOptionsClicked();
    void onBtnHelpClicked();
    void handleFullWindowToggle(bool checked);

private:
    Ui::GameWindow *ui;
    DebugWindow* debugWindow;
    StartMenu* startMenu;
    BasicMode* basicMode;
    GameMap* gameMap;
    Options* options;
    Help* help;
};
#endif // GAMEWINDOW_H
