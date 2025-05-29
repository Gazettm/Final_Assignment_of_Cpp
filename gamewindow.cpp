#include "gamewindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QApplication>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), m_board(15), m_currentPiece(GomokuBoard::Black) {
    setWindowTitle("Gomoku Game");
    setFixedSize(600, 600);

    // 添加模式选择对话框
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "游戏模式",
        "请选择游戏模式：\n是 - 人机对战\n否 - 双人对战",
        QMessageBox::Yes | QMessageBox::No
    );
    m_gameMode = (reply == QMessageBox::Yes) ? HumanVsAI : HumanVsHuman;
}
void GameWindow::ShowWinner(GomokuBoard::Piece winner) {
    if (winner == GomokuBoard::Black) {
        // 黑棋获胜时弹出消息框
        QMessageBox msgBox;
        msgBox.setText("黑棋获胜！");
        msgBox.setInformativeText("点击确定退出游戏");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        // 设置消息框按钮点击后的槽函数
        connect(&msgBox, &QMessageBox::finished, this, &GameWindow::exitGame);

        msgBox.exec();  // 弹出消息框，等待用户点击
    } else if (winner == GomokuBoard::White) {
        // 白棋获胜时弹出消息框
        QMessageBox msgBox;
        msgBox.setText("白棋获胜！");
        msgBox.setInformativeText("点击确定退出游戏");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        connect(&msgBox, &QMessageBox::finished, this, &GameWindow::exitGame);

        msgBox.exec();  // 弹出消息框，等待用户点击
    }
}
void GameWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawBoard(painter);
    drawPieces(painter);
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
        } else {
            // 人机对战模式触发AI回合
            if (m_gameMode == HumanVsAI) {
                m_currentPiece = GomokuBoard::White;
                QTimer::singleShot(500, this, [this]() {  // 添加0.5秒延迟
                    QPoint aiMove = m_aiplayer.calculateAIMove(m_board);
                    if (aiMove.x() != -1) {
                        m_board.placePiece(aiMove.x(), aiMove.y(), GomokuBoard::White);
                        if (m_board.checkWin(aiMove.x(), aiMove.y())) {
                            ShowWinner(GomokuBoard::White);
                        } else {
                            m_currentPiece = GomokuBoard::Black;
                        }
                        update();
                    }
                });
            } else {
                m_currentPiece = (m_currentPiece == GomokuBoard::Black) ? GomokuBoard::White : GomokuBoard::Black;
            }
            update();
        }
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
void GameWindow::exitGame() {
    QApplication::quit();  // 退出整个应用
}
