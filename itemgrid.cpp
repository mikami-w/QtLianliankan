#include "itemgrid.h"
#include "gamemap.h"
#include "gamewindow.h"
#include <algorithm>
#include <random>
#include <QTimer>
#include <tuple>
#include <QMessageBox>

const ItemPos ItemGrid::INVALID_ITEM{-1,-1};

ItemGrid::ItemGrid(GameMap *_parent)
    : parent(_parent),
    topParentWeak(static_cast<GameWindow *>(parent->parent())),
    remaining(0)
{
    reset();
}

void ItemGrid::reset()
{
    // 共 16*10 = 160 格, 先生成每种水果16个
    for (int i = 0; i < 10; i++)
        std::fill(grid[i], grid[i] + 16, i + 1);
    remaining = 16 * 10;

    // 再打乱
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(grid[0], grid[0] + 16 * 10, g);

    // grid[1][0]=grid[3][0]=1;
    // grid[2][0]=9;

    // grid[0][4]=grid[0][6]=2;
    // grid[0][5]=9;

    // grid[0][15]=grid[2][15]=3;
    // grid[1][15]=9;

    // grid[9][4]=grid[9][6]=4;
    // grid[9][5]=9;

    itemFinished();
}

void ItemGrid::clicked(ItemPos item)
{
    if (topParentWeak->getDebugWindow()->inDebugMode())
    {
        if (topParentWeak->getDebugWindow()->getDebugOptions().goldenFinger)
        {
            (*this)[item] = topParentWeak->getDebugWindow()->getGoldenFingerValue();
            parent->update();
        }
        return;
    }

    if (!(*this)[item])
    {// 点击空白, 清空状态
        itemFinished();
        parent->update();
        return;
    }

    if (first == INVALID_ITEM)
    {// 是两次点击中的第一次
        first = item;
        highlighted.push_back(item);
        parent->update(QRect(item.col * 40, item.row * 40, 40, 40)); // 只有这里可以局部重绘, 因为下面的update不仅要绘制新的边框还要去掉旧的边框
        return;
    }
    else if (second == INVALID_ITEM)
    {// 是两次点击中的第二次
        second = item;
    }
    else assert(0); // 怎么会事呢

    if (first != second && (*this)[first] == (*this)[second])
    {// 点击的不是同一个item且种类相同
        if (findPath()) {
            highlighted.push_back(item);
            // 绘制线条和框
            parent->update();
            // 500ms后删除当前线条
            QTimer::singleShot(500, [this]() {
                paths.pop_front();
                parent->update();
            });
        }
    }

    itemFinished();
    parent->update();

    if (!remaining)
    {
        auto reply = QMessageBox::question(static_cast<QWidget *>(parent->parent()),
                              "Game Finished",
                              "恭喜完成游戏!\n你想再来一局游戏来庆祝胜利吗?",
                              QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            topParentWeak->onBtnRestartClicked();
        }
    }
    // return [](){};
}

bool ItemGrid::findPath()
{
    if (checkLine1(first, second))
    {
        paths.emplace_back(std::initializer_list<ItemPos>{ first, second });
        eraseCurrentItems();
        return true;
    }

    {
        auto [validity, m1] = checkLine2(first, second);
        if (validity)
        {
            paths.emplace_back(std::initializer_list<ItemPos>{ first, m1, second });
            eraseCurrentItems();
            parent->update();
            return true;
        }
    }

    {
        auto [validity, m1, m2] = checkLine3(first, second);
        if (validity)
        {
            paths.emplace_back(std::initializer_list<ItemPos>{ first, m1, m2, second });
            eraseCurrentItems();
            parent->update();
            return true;
        }
    }

    return false;
}

bool ItemGrid::checkLine1(ItemPos a, ItemPos b) const
{// 一条直线连接, 即一定在同一行或列
    // assert(a!=b);
    if (a == b)
        return false;

    // 在同一行
    if (a.row == b.row)
    {
        int min = std::min(a.col, b.col);
        int max = std::max(a.col, b.col);
        for (int i = min + 1; i < max; ++i)
        {
            if (grid[a.row][i])
                return false;
        }
        return true;
    }

    // 在同一列
    if (a.col == b.col)
    {
        int min = std::min(a.row, b.row);
        int max = std::max(a.row, b.row);
        for (int i = min + 1; i < max; ++i)
        {
            if (grid[i][a.col])
                return false;
        }
        return true;
    }

    return false;
}

std::tuple<bool, ItemPos> ItemGrid::checkLine2(ItemPos a, ItemPos b) const
{// 转折一次
    // assert(a.col!=b.col && a.row!=b.row);
    if (a.col == b.col || a.row == b.row)
        return std::make_tuple(false, INVALID_ITEM);

    // 分别使用两个点的横纵坐标得到中转点(折线折的地方)
    if (!grid[a.row][b.col])
    {
        ItemPos m{b.col, a.row};
        if (checkLine1(a, m) && checkLine1(m, b))
            return std::make_tuple(true, m);
    }
    if (!grid[b.row][a.col])
    {
        ItemPos m{a.col, b.row};
        if (checkLine1(a, m) && checkLine1(m, b))
            return std::make_tuple(true, m);
    }
    // 无可用中转
    return std::make_tuple(false, INVALID_ITEM);
}

std::tuple<bool, ItemPos, ItemPos> ItemGrid::checkLine3(ItemPos a, ItemPos b) const
{
    if (a==b)
        return std::make_tuple(false, INVALID_ITEM, INVALID_ITEM);

    qDebug()<<"checkLine3";

    // 找到一个中转点使得 checkLine2 成立
    // 不同列不同行, 暴力遍历所有4个方向, 否则只遍历坐标相同的垂直方向
    // 在同一行或既不同行也不同列
    if (a.row == b.row || (a.col != b.col && a.row != b.row))
    {
        // 向下拓展
        for (int i = a.row + 1; i < ROW_LENGTH; ++i)
        {
            if (grid[i][a.col])
                break;

            ItemPos m1{a.col, i};
            auto [validity, m2] = checkLine2(m1, b);
            qDebug()<<"向下:";
            qDebug()<<"m1: "<<m1.col<<", "<<m1.row;
            qDebug()<<"m2: "<<m2.col<<", "<<m2.row;
            if (validity)
                return std::make_tuple(true, m1, m2);
        }

        // 向上拓展
        for (int i = a.row - 1; i >= 0; --i)
        {
            if (grid[i][a.col])
                break;

            ItemPos m1{a.col, i};
            auto [validity, m2] = checkLine2(m1, b);
            qDebug()<<"向上:";
            qDebug()<<"m1: "<<m1.col<<", "<<m1.row;
            qDebug()<<"m2: "<<m2.col<<", "<<m2.row;
            if (validity)
                return std::make_tuple(true, m1, m2);
        }

        // return std::make_tuple(false, INVALID_ITEM, INVALID_ITEM);
    }

    // 在同一列或既不同行也不同列
    if (a.col == b.col || (a.col != b.col && a.row != b.row))
    {
        // 向右拓展
        for (int i = a.col + 1; i < COL_LENGTH; ++i)
        {
            if (grid[a.row][i])
                break;

            ItemPos m1{i, a.row};
            auto [validity, m2] = checkLine2(m1, b);
            qDebug()<<"向右:";
            qDebug()<<"m1: "<<m1.col<<", "<<m1.row;
            qDebug()<<"m2: "<<m2.col<<", "<<m2.row;
            if (validity)
                return std::make_tuple(true, m1, m2);
        }

        // 向左拓展
        for (int i = a.col - 1; i >= 0; --i)
        {
            if (grid[a.row][i])
                break;

            ItemPos m1{i, a.row};
            auto [validity, m2] = checkLine2(m1, b);
            qDebug()<<"向左:";
            qDebug()<<"m1: "<<m1.col<<", "<<m1.row;
            qDebug()<<"m2: "<<m2.col<<", "<<m2.row;
            if (validity)
                return std::make_tuple(true, m1, m2);
        }

        // return std::make_tuple(false, INVALID_ITEM, INVALID_ITEM);
    }

    return std::make_tuple(false, INVALID_ITEM, INVALID_ITEM);
}
