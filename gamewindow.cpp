#include "gamewindow.h"
#include <QPainter>
#include <QMouseEvent>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), m_board(15), m_currentPiece(GomokuBoard::Black) {
    setWindowTitle("Gomoku Game");
    setFixedSize(600, 600);  // 窗口大小
}

void GameWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawBoard(painter);
    drawPieces(painter);
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    int gridSize = width() / m_board.size();
    int x = event->x() / gridSize;
    int y = event->y() / gridSize;

    if (m_board.placePiece(x, y, m_currentPiece)) {
        m_currentPiece = (m_currentPiece == GomokuBoard::Black) ? GomokuBoard::White : GomokuBoard::Black;
        update();  // 触发重绘
    }
}

void GameWindow::drawBoard(QPainter &painter) {
    painter.setPen(Qt::black);
    int gridSize = width() / m_board.size();

    for (int i = 0; i < m_board.size(); ++i) {
        painter.drawLine(i * gridSize, 0, i * gridSize, height());
        painter.drawLine(0, i * gridSize, width(), i * gridSize);
    }
}

void GameWindow::drawPieces(QPainter &painter) {
    int gridSize = width() / m_board.size();

    for (int x = 0; x < m_board.size(); ++x) {
        for (int y = 0; y < m_board.size(); ++y) {
            if (m_board.pieceAt(x, y) != GomokuBoard::Empty) {
                painter.setBrush(m_board.pieceAt(x, y) == GomokuBoard::Black ? Qt::black : Qt::white);
                painter.drawEllipse(x * gridSize + 2, y * gridSize + 2, gridSize - 4, gridSize - 4);
            }
        }
    }
}
