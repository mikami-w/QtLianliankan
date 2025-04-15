#ifndef GAMEMAP_H
#define GAMEMAP_H

// #include "itemgrid.h"
#include <QWidget>
#include <vector>
// #include <QPainter>

class ItemGrid;

namespace Ui {
class GameMap;
}

class GameMap : public QWidget
{
    Q_OBJECT

public:
    explicit GameMap(QWidget *parent = nullptr);
    ~GameMap();
    void reset();

public slots:
    void onBtnHintClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override ;

private:
    void getTransparentPixmap();

    Ui::GameMap *ui;
    std::vector<QPixmap> texture;
    ItemGrid* itemGrid;

};

#endif // GAMEMAP_H
