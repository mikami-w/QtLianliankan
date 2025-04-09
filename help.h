#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include <deque>

namespace Ui {
class Help;
}

class Help : public QWidget
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = nullptr);
    ~Help();

    // void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void handleKeyboardDebug(QKeyEvent *keyEvent);
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

    void changeToThis();
private:
    Ui::Help *ui;
    std::deque<char> keyBuffer;
};

#endif // HELP_H
