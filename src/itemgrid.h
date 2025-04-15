#ifndef ITEMGRID_H
#define ITEMGRID_H

// #include "gamemap.h"
#include "gamemap.h"
#include "gamewindow.h"
#include <deque>
#include <vector>
#include <tuple>
class GameMap;
class DebugWindow;

struct ItemPos
{
    int col;
    int row;
    ItemPos(int _col, int _row) : col(_col), row(_row) {}
    explicit ItemPos(const QPoint &point): col(point.x() / 40), row(point.y() / 40) {}
    bool operator==(const ItemPos obj) const {
        return col == obj.col && row == obj.row;
    }
    explicit operator QPoint() const { return QPoint(col * 40, row * 40); }
    QPoint getCenter() const { return QPoint(col * 40 + 20, row * 40 + 20); }
};

class ItemGrid
{
    friend class GameMap;
    friend class DebugWindow;
// public:
    using Path = std::vector<ItemPos>;
    static const constexpr char ROW_LENGTH = 10;
    static const constexpr char COL_LENGTH = 16;

    ItemGrid(GameMap *_parent);
    void reset();
    void clicked(ItemPos item);
    void itemFinished(){ first = INVALID_ITEM; second = INVALID_ITEM; highlighted.clear(); };
    bool findPath(ItemPos start, ItemPos end, bool doErase = true);
    bool getHint();

    char& operator[](ItemPos pos) { return grid[pos.row][pos.col]; }

    const static ItemPos INVALID_ITEM;

    GameMap* parent;
    GameWindow* topParentWeak;  // 仅用于访问顶级父类的指针
    unsigned int remaining;
    char grid[ROW_LENGTH][COL_LENGTH]{}; // 水果矩阵, 0表示空, 1~10代表10种水果

    ItemPos first = INVALID_ITEM;
    ItemPos second = INVALID_ITEM;
    std::vector<ItemPos> highlighted; // 存储需要绘制边框的水果
    std::deque<Path> paths;

    bool checkLine1(ItemPos a, ItemPos b) const; // 检查是否一条直线连通
    std::tuple<bool,ItemPos> checkLine2(ItemPos a, ItemPos b) const; // 检查是否两条直线连通(转折一次, 返回值为连通性与中转点)
    std::tuple<bool,ItemPos,ItemPos> checkLine3(ItemPos a, ItemPos b) const; // 检查是否两条直线连通(转折两次, 返回值为连通性与中转点, 且顺序为从a开始按参数顺序到b结束)
    void eraseCurrentItems() {
        if (first != INVALID_ITEM && second != INVALID_ITEM)
        {
            grid[first.row][first.col] = 0;
            grid[second.row][second.col] = 0;
            remaining -= 2;
        }
    }
};

#endif // ITEMGRID_H
