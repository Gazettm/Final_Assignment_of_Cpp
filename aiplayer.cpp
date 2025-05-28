#include "aiplayer.h"
#include <QRandomGenerator>
#include <cmath> // 用于std::abs

AIPlayer::AIPlayer(const GomokuBoard& board) 
: m_board(board) {}

QPoint AIPlayer::calculateMove(GomokuBoard::Piece aiPiece) {
	int bestScore = -1;
	QVector<QPoint> bestMoves;
	
	for (int x = 0; x < m_board.size(); ++x) {
		for (int y = 0; y < m_board.size(); ++y) {
			if (m_board.pieceAt(x, y) == GomokuBoard::Empty) {
				int score = evaluatePosition(x, y, aiPiece);
				if (score > bestScore) {
					bestScore = score;
					bestMoves.clear();
					bestMoves.append(QPoint(x, y));
				} else if (score == bestScore) {
					bestMoves.append(QPoint(x, y));
				}
			}
		}
	}
	
	if (!bestMoves.isEmpty()) {
		int randomIndex = QRandomGenerator::global()->bounded(bestMoves.size());
		return bestMoves[randomIndex];
	}
	return QPoint(-1, -1); // 棋盘已满
}

int AIPlayer::evaluatePosition(int x, int y, GomokuBoard::Piece aiPiece) const {
	GomokuBoard::Piece humanPiece = (aiPiece == GomokuBoard::White) ? GomokuBoard::Black : GomokuBoard::White;
	int score = 0;
	
	const int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
	
	for (auto &dir : directions) {
		int dx = dir[0], dy = dir[1];
		int aiCount = 1, humanCount = 1;
		bool aiBlocked = false, humanBlocked = false;
		
		// 正向检测
		for (int i = 1; i < 5; ++i) {
			int nx = x + dx * i, ny = y + dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == aiPiece) aiCount++;
			else {
				if (m_board.pieceAt(nx, ny) == humanPiece) aiBlocked = true;
				break;
			}
		}
		
		// 反向检测
		for (int i = 1; i < 5; ++i) {
			int nx = x - dx * i, ny = y - dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == aiPiece) aiCount++;
			else {
				if (m_board.pieceAt(nx, ny) == humanPiece) aiBlocked = true;
				break;
			}
		}
		
		// 人类威胁检测
		for (int i = 1; i < 5; ++i) {
			int nx = x + dx * i, ny = y + dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == humanPiece) humanCount++;
			else {
				if (m_board.pieceAt(nx, ny) == aiPiece) humanBlocked = true;
				break;
			}
		}
		for (int i = 1; i < 5; ++i) {
			int nx = x - dx * i, ny = y - dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == humanPiece) humanCount++;
			else {
				if (m_board.pieceAt(nx, ny) == aiPiece) humanBlocked = true;
				break;
			}
		}
		
		// 评分规则
		if (humanCount >= 4) score += 100000;
		else if (humanCount == 3 && !humanBlocked) score += 10000;
		else if (humanCount == 2 && !humanBlocked) score += 1000;
		
		if(aiCount == 5) score += 999999;
		else if (aiCount >= 4) score += 5000;
		else if (aiCount == 3 && !aiBlocked) score += 5000;
		else if (aiCount == 2 && !aiBlocked) score += 500;
	}
	
	// 中心区域加分
	int center = m_board.size() / 2;
	int distanceToCenter = std::abs(x - center) + std::abs(y - center);
	score += (m_board.size() - distanceToCenter) * 10;
	
	return score;
}
