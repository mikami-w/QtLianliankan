#ifndef BASICMODE_H
#define BASICMODE_H

#include "ReturnablePage.h"
#include "gamemap.h"
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

    void changeToThis() override;

    void bindGameMap(GameMap *map) { gameMapWeak = map; }
public slots:
    // void onBtnBackClicked();


private:
    Ui::BasicMode *ui;
    GameMap* gameMapWeak;
};

#endif // BASICMODE_H
