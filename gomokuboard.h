#ifndef GOMOKU_BOARD_H
#define GOMOKU_BOARD_H

#include <QVector>

class GomokuBoard {
public:
    enum Piece { Empty, Black, White };  // 棋子类型,枚举的数字结构，有三种情况

    GomokuBoard(int size = 15);          // 构造函数，默认15x15棋盘
    virtual ~GomokuBoard() = default;    // 虚析构函数（多态支持）

    // 核心方法
    bool placePiece(int x, int y, Piece piece);  // 落子,
    bool checkWin(int x, int y) const;           // 检查胜利（多态预留）
    void reset();                                // 重置棋盘

    // 访问器
    int size() const { return m_size; }
    Piece pieceAt(int x, int y) const { return m_board[x][y]; }//返回棋子的类型

protected:
    QVector<QVector<Piece>> m_board;     // 棋盘数据（封装）
    int m_size;                          // 棋盘大小
};

#endif
