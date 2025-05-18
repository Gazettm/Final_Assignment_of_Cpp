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
}

bool GomokuBoard::placePiece(int x, int y, Piece piece) {
    if (x < 0 || x >= m_size || y < 0 || y >= m_size || m_board[x][y] != Empty) {
        return false;
    }
    m_board[x][y] = piece;
    return true;
}

bool GomokuBoard::checkWin(int x, int y) const {
    // 实现五子连珠检查逻辑（此处简化为示例）
    return false;
}
