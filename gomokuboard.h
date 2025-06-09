#ifndef GOMOKU_BOARD_H
#define GOMOKU_BOARD_H

#include <QVector>

class GomokuBoard {
public:
    enum Piece { Empty, Black, White };

    GomokuBoard(int size = 15);
    ~GomokuBoard() = default;

    bool placePiece(int x, int y, Piece piece);
    bool checkWin(int x, int y) const;
    void reset(); 

    int size() const { return m_size; }
    Piece pieceAt(int x, int y) const { return m_board[x][y]; }

protected:
    QVector<QVector<Piece>> m_board;
    int m_size;
};

#endif
