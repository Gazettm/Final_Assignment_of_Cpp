#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include "gomokuboard.h"
#include "aiplayer.h"  // 包含新的AIPlayer类

class GameWindow : public QMainWindow {
	Q_OBJECT
	
public:
	enum GameMode { HumanVsHuman, HumanVsAI };
	
	explicit GameWindow(QWidget *parent = nullptr);
	~GameWindow() override = default;
	
	void ShowWinner(GomokuBoard::Piece winner);
	void exitGame();
	
protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	
private:
	GomokuBoard m_board;
	GomokuBoard::Piece m_currentPiece;
	GameMode m_gameMode;
	AIPlayer m_aiPlayer;  // 添加AIPlayer成员变量
	
	void handleAIMove();  // 新增处理AI移动的函数
};

#endif
