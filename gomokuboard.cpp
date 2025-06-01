#include "gomokuboard.h"

GomokuBoard::GomokuBoard(int size) : m_size(size) {
	reset();
}

void GomokuBoard::reset() {
	m_board.resize(m_size);
	for (auto &row : m_board) {
		row.resize(m_size);
		row.fill(Empty);
	}
}//重置棋盘

bool GomokuBoard::placePiece(int x, int y, Piece piece) {
	if (x < 0 || x >= m_size || y < 0 || y >= m_size || m_board[x][y] != Empty) {
		return false;
	}
	m_board[x][y] = piece;
	return true;
}//落子,若超出范围或在已有棋子处,则返回false，重新落子.

bool GomokuBoard::checkWin(int x, int y) const {
    Piece currentPiece = m_board[x][y];
    if (currentPiece == Empty) {
        return false;
    }
    const int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
    
    for (auto &dir : directions) {
        int Count = 1;
        int dx = dir[0], dy = dir[1];
        
        // 正向检测
        for (int i = 1; i < 5; ++i) {
            int nx = x + dx * i, ny = y + dy * i;
            if (nx < 0 || nx >= size() || ny < 0 || ny >= size()) break;
            if (pieceAt(nx, ny) == currentPiece) {
                Count++;
            } else {
                break;  // Bug fixes
            }
        }
        // 反向检测
        for (int i = 1; i < 5; ++i) {
            int nx = x - dx * i, ny = y - dy * i;
            if (nx < 0 || nx >= size() || ny < 0 || ny >= size()) break;
            if (pieceAt(nx, ny) == currentPiece) {
                Count++;
            } else {
                break;  // Bug fixes
            }
        }
        if(Count >= 5) return true;
    }
    return false;
}
