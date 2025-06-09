#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QMessageBox>
#include "gomokuboard.h"
#include "aiplayer.h"
#include "Rating.h"
#include <QTimer>
#include <QApplication>
#include <QRandomGenerator>
#include <QPixmap>

class GameWindow : public QMainWindow {
    Q_OBJECT  // Qt 元对象系统支持

public:
    enum GameMode { HumanVsHuman, HumanVsAI };
    explicit GameWindow(QWidget *parent = nullptr);//防止了自动的类型转换
    ~GameWindow() override = default;//默认实现
    void ShowWinner(GomokuBoard::Piece winner);
    void exitGame();
    void WriteRatingY();
    void WriteRatingN();

protected:
    void paintEvent(QPaintEvent *event) override;  // 绘画事件
    void mousePressEvent(QMouseEvent *event) override;  // 鼠标事件
    void mouseMoveEvent(QMouseEvent *event) override;    // 鼠标移动事件
    void leaveEvent(QEvent *event) override;             // 鼠标离开事件

private:
    GomokuBoard m_board;      // 棋盘逻辑（组合）
    GomokuBoard::Piece m_currentPiece;  // 当前玩家棋子
    GameMode m_gameMode;  // 游戏模式成员
    AiPlayer m_aiplayer;  // ai工具成员
    Rating rating;
    QPoint m_hoverPos;    // 鼠标悬停位置
    
    void drawBoard(QPainter &painter);  // 绘制棋盘
    void drawPieces(QPainter &painter); // 绘制棋子
    void drawHoverIndicator(QPainter &painter); // 绘制悬停指示器
};

#endif
