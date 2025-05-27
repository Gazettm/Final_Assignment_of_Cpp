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
        return false; // 空格不可能是胜利状态
    }

    // 检查横向
    int count = 1;
    for (int i = 1; i < 5; i++) {
        if (x + i < m_size && m_board[x + i][y] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    for (int i = 1; i < 5; i++) {
        if (x - i >= 0 && m_board[x - i][y] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    if (count >= 5) return true;

    // 检查纵向
    count = 1;
    for (int i = 1; i < 5; i++) {
        if (y + i < m_size && m_board[x][y + i] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    for (int i = 1; i < 5; i++) {
        if (y - i >= 0 && m_board[x][y - i] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    if (count >= 5) return true;

    // 检查主对角线（从左上到右下）
    count = 1;
    for (int i = 1; i < 5; i++) {
        if (x + i < m_size && y + i < m_size && m_board[x + i][y + i] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    for (int i = 1; i < 5; i++) {
        if (x - i >= 0 && y - i >= 0 && m_board[x - i][y - i] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    if (count >= 5) return true;

    // 检查副对角线（从右上到左下）
    count = 1;
    for (int i = 1; i < 5; i++) {
        if (x + i < m_size && y - i >= 0 && m_board[x + i][y - i] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    for (int i = 1; i < 5; i++) {
        if (x - i >= 0 && y + i < m_size && m_board[x - i][y + i] == currentPiece) {
            count++;
        } else {
            break;
        }
    }
    if (count >= 5) return true;

    return false; // 如果没有找到五子连珠
}
