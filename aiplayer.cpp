#include <QRandomGenerator>
#include <cmath>
#include "aiplayer.h"
QPoint AiPlayer::calculateAIMove(GomokuBoard m_board) {
	int bestScore = -1;
	QVector<QPoint> bestMoves;  // 存储所有最高分位置
	
	for (int x = 0; x < m_board.size(); ++x) {
		for (int y = 0; y < m_board.size(); ++y) {
			if (m_board.pieceAt(x, y) == GomokuBoard::Empty) {
				int score = evaluatePosition(x, y, GomokuBoard::White,m_board);
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
	
	// 随机选择一个最优位置
	if (!bestMoves.isEmpty()) {
		int randomIndex = QRandomGenerator::global()->bounded(bestMoves.size());
		return bestMoves[randomIndex];
	}
	return QPoint(-1, -1); // 棋盘已满
}
int AiPlayer::evaluatePosition(int x, int y, GomokuBoard::Piece aiPiece,GomokuBoard m_board) {
	GomokuBoard::Piece humanPiece = (aiPiece == GomokuBoard::White) ? GomokuBoard::Black : GomokuBoard::White;
	int score = 0;
	
	const int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
	
	for (auto &dir : directions) {
		int dx = dir[0], dy = dir[1];
		int aiCount = 1, humanCount = 1;
		bool aiBlocked = false, humanBlocked = false;
		
		// 检测AI连子
		// 正向检测
		for (int i = 1; i < 5; ++i) {
			int nx = x + dx * i, ny = y + dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == aiPiece) {
				aiCount++;
			} else {
				if (m_board.pieceAt(nx, ny) == humanPiece) aiBlocked = true;
				break;
			}
		}
		// 反向检测
		for (int i = 1; i < 5; ++i) {
			int nx = x - dx * i, ny = y - dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == aiPiece) {
				aiCount++;
			} else {
				if (m_board.pieceAt(nx, ny) == humanPiece) aiBlocked = true;
				break;
			}
		}
		//检测人类连子
		// 正向检测
		for (int i = 1; i < 5; ++i) {
			int nx = x + dx * i, ny = y + dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == humanPiece) {
				humanCount++;
			} else {
				if (m_board.pieceAt(nx, ny) == aiPiece) humanBlocked = true;
				break;
			}
		}
		// 反向检测
		for (int i = 1; i < 5; ++i) {
			int nx = x - dx * i, ny = y - dy * i;
			if (nx < 0 || nx >= m_board.size() || ny < 0 || ny >= m_board.size()) break;
			if (m_board.pieceAt(nx, ny) == humanPiece) {
				humanCount++;
			} else {
				if (m_board.pieceAt(nx, ny) == aiPiece) humanBlocked = true;
				break;
			}
		}
		
		// 评分规则（防守优先）
		// 人类威胁评分
		if (humanCount >= 4) score += 100000;    // 必须堵住四连
		else if (humanCount == 3 && !humanBlocked) score += 10000; // 堵活三
		else if (humanCount == 2 && !humanBlocked) score += 1000;  // 堵活二
		
		// AI进攻评分
		if(aiCount == 5) score += 999999;		//能形成五连时绝杀
		else if (aiCount >= 4) score += 5000;       // 形成四连
		else if (aiCount == 3 && !aiBlocked) score += 5000;  // 活三
		else if (aiCount == 2 && !aiBlocked) score += 500;   // 活二
	}
	
	// 中心区域加分（避免总选角落）
	int center = m_board.size() / 2;
	int distanceToCenter = std::abs(x - center) + std::abs(y - center);
	score += (m_board.size() - distanceToCenter) * 10;  // 离中心越近分越高
	
	return score;
}
