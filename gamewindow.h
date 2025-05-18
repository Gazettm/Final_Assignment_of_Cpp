#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "gomokuboard.h"

class GameWindow : public QMainWindow {
    Q_OBJECT  // Qt 元对象系统支持

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override = default;

protected:
    void paintEvent(QPaintEvent *event) override;  // 重绘事件
    void mousePressEvent(QMouseEvent *event) override;  // 鼠标事件

private:
    GomokuBoard m_board;      // 棋盘逻辑（组合）
    GomokuBoard::Piece m_currentPiece;  // 当前玩家棋子

    void drawBoard(QPainter &painter);  // 绘制棋盘
    void drawPieces(QPainter &painter); // 绘制棋子
};

#endif
