#ifndef BASICMODE_H
#define BASICMODE_H

#include "ReturnablePage.h"
#include "gamemap.h"
#include "infopanel.h"
#include <QWidget>

namespace Ui {
class BasicMode;
}

class BasicMode : public QWidget, public ReturnablePage
{
    Q_OBJECT

public:
    explicit BasicMode(QWidget *parent = nullptr);
    ~BasicMode();
    Ui::BasicMode* getUi() noexcept { return ui; }


    void changeToThis() override;

    void bindGameMap(GameMap *map);

    void bindInfoPanel(InfoPanel *panel);
private:
    Ui::BasicMode *ui;
    GameMap* gameMapWeak = nullptr; // 仅用于访问 gameMap
    InfoPanel* infoPanelWeak = nullptr;
};

#endif // BASICMODE_H
