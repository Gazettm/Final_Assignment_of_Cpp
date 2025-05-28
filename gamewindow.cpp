#include "gamewindow.h"
#include <QPainter>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>

GameWindow::GameWindow(QWidget *parent) 
: QMainWindow(parent), 
m_board(15),
m_currentPiece(GomokuBoard::Black),
m_aiPlayer(m_board) {  // 初始化AIPlayer
	
	setWindowTitle("Gomoku Game");
	setFixedSize(600, 600);
	
	// 模式选择
	QMessageBox::StandardButton reply = QMessageBox::question(
		this, "游戏模式", "请选择游戏模式：\n是 - 人机对战\n否 - 双人对战",
		QMessageBox::Yes | QMessageBox::No
		);
	m_gameMode = (reply == QMessageBox::Yes) ? HumanVsAI : HumanVsHuman;
}

void GameWindow::ShowWinner(GomokuBoard::Piece winner) {
	QString winnerText = (winner == GomokuBoard::Black) ? "黑棋获胜！" : "白棋获胜！";
	
	QMessageBox msgBox;
	msgBox.setText(winnerText);
	msgBox.setInformativeText("点击确定退出游戏");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	
	connect(&msgBox, &QMessageBox::finished, this, &GameWindow::exitGame);
	msgBox.exec();
}

void GameWindow::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	int gridSize = width() / m_board.size();
	
	// 绘制棋盘
	painter.setPen(Qt::black);
	for (int i = 0; i < m_board.size(); ++i) {
		painter.drawLine(i * gridSize, 0, i * gridSize, height());
		painter.drawLine(0, i * gridSize, width(), i * gridSize);
	}
	
	// 绘制棋子
	for (int x = 0; x < m_board.size(); ++x) {
		for (int y = 0; y < m_board.size(); ++y) {
			if (m_board.pieceAt(x, y) != GomokuBoard::Empty) {
				painter.setBrush(m_board.pieceAt(x, y) == GomokuBoard::Black ? Qt::black : Qt::white);
				painter.drawEllipse(x * gridSize + 2, y * gridSize + 2, gridSize - 4, gridSize - 4);
			}
		}
	}
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
	// 人机对战且不是玩家回合时忽略点击
	if (m_gameMode == HumanVsAI && m_currentPiece != GomokuBoard::Black) {
		return;
	}
	
	int gridSize = width() / m_board.size();
	int x = event->x() / gridSize;
	int y = event->y() / gridSize;
	
	if (m_board.placePiece(x, y, m_currentPiece)) {
		if (m_board.checkWin(x, y)) {
			ShowWinner(m_currentPiece);
			return;
		}
		
		// 人机对战模式触发AI回合
		if (m_gameMode == HumanVsAI) {
			m_currentPiece = GomokuBoard::White;
			QTimer::singleShot(500, this, &GameWindow::handleAIMove);
		} else {
			// 双人对战切换玩家
			m_currentPiece = (m_currentPiece == GomokuBoard::Black) 
			? GomokuBoard::White : GomokuBoard::Black;
		}
		update();
	}
}

// 新增的AI移动处理函数
void GameWindow::handleAIMove() {
	QPoint aiMove = m_aiPlayer.calculateMove(GomokuBoard::White);
	if (aiMove.x() != -1) {
		m_board.placePiece(aiMove.x(), aiMove.y(), GomokuBoard::White);
		if (m_board.checkWin(aiMove.x(), aiMove.y())) {
			ShowWinner(GomokuBoard::White);
		} else {
			m_currentPiece = GomokuBoard::Black;
		}
		update();
	}
}

void GameWindow::exitGame() {
	QApplication::quit();
}
