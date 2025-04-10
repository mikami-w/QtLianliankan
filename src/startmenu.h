#ifndef STARTMENU_H
#define STARTMENU_H

#include "ReturnablePage.h"
#include <QWidget>

namespace Ui {
class StartMenu;
}

class StartMenu : public QWidget, public ReturnablePage
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);
    ~StartMenu();

    void changeToThis() override;

private:
    Ui::StartMenu *ui;
};

#endif // STARTMENU_H
