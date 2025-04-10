#include "gamemap.h"
#include "ui_gamemap.h"
#include "itemgrid.h"
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>

GameMap::GameMap(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::GameMap),
    itemGrid(new ItemGrid(this))
{
    ui->setupUi(this);
    getTransparentPixmap();
}

GameMap::~GameMap()
{
    delete ui;
    delete itemGrid;
}

void GameMap::reset()
{
    itemGrid->itemFinished();
    itemGrid->reset();
    update();
}

void GameMap::getTransparentPixmap()
{
    QPixmap textureOrigin(":/img/fruit_element.bmp");
    QImage mask(":/img/fruit_mask.bmp");     // 加载 mask 为黑白位图

    // 确保贴图和mask图尺寸相同
    if (textureOrigin.size() != mask.size()) {
        throw std::runtime_error("贴图和遮罩图尺寸不匹配!");
    }

    mask.invertPixels(); //????为什么这里需要反转
    textureOrigin.setMask(QBitmap::fromImage(mask));

    for(int i=0; i<10; ++i)
    {// 逐个切取40*40的子图
        texture.push_back(textureOrigin.copy(0, 40 * i + i * 0.9, 40, 40)); // 难绷 给的贴图是歪的 手动矫正了一下
    }
}

void GameMap::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect area = event->rect();

    for(int row=0;row<10;++row)
    {
        for(int col=0;col<16;++col)
        {
            QRect fruitRect(col * 40, row * 40, 40, 40);
            if(!area.intersects(fruitRect)) continue; // 与给定区域不相交, 不重绘

            char type = itemGrid->grid[row][col];
            if(type)
            {
                painter.drawPixmap(fruitRect, texture[type - 1]);
            }
        }
    }

    // qDebug() << "in function GameMap::paintEvent():grid->needToPaintRect="
    //          << grid->needToPaintRect.col << ',' << grid->needToPaintRect.row;

    // 绘制对item的选择框
    painter.setPen(QPen(Qt::red, 2));  // 红色 2px 宽的线
    for (ItemPos& item : itemGrid->highlighted)
    {
        painter.drawRect(item.col * 40, item.row * 40, 40, 40);
    }

    // 绘制连接线
    painter.setPen(QPen(Qt::blue, 3));
    for (std::vector<ItemPos>& path : itemGrid->paths)
    {
        QPainterPath pathToDraw;
        QPoint start = path[0].getCenter();
        pathToDraw.moveTo(start);
        for (int i = 1; i < path.size(); ++i)
        {
            pathToDraw.lineTo(path[i].getCenter());
        }
        painter.drawPath(pathToDraw);
    }
}

void GameMap::mousePressEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    qDebug() << "clicked at: (" << point.x() << ", " << point.y() << ')';
    qDebug() << "position: (" << point.x() / 40 << ", " << point.y() / 40 << ')';
    itemGrid->clicked(ItemPos(point));
}
