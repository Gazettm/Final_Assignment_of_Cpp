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
#include <cmath>

class GameWindow : public QMainWindow {
    Q_OBJECT 

public:
    enum GameMode { HumanVsHuman, HumanVsAI };
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override = default;
    void ShowWinner(GomokuBoard::Piece winner);
    void exitGame();
    void WriteRatingY();
    void WriteRatingN();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    GomokuBoard m_board;
    GomokuBoard::Piece m_currentPiece;
    GameMode m_gameMode;
    AiPlayer m_aiplayer;
    Rating rating;
    QPoint m_hoverPos;
    
    void drawBoard(QPainter &painter);
    void drawPieces(QPainter &painter);
    void drawHoverIndicator(QPainter &painter);
};

#endif
