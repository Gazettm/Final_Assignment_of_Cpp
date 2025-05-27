#include "gamewindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QtMath>

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
        QMessageBox::information(this, "游戏结束", "黑棋获胜！");
        m_board.reset();
        update();
    } else if (winner == GomokuBoard::White) {
        QMessageBox::information(this, "游戏结束", "白棋获胜！");
        m_board.reset();
        update();
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
                    QPoint aiMove = calculateAIMove();
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
QPoint GameWindow::calculateAIMove() {
    int bestScore = -1;
    QVector<QPoint> bestMoves;  // 存储所有最高分位置

    for (int x = 0; x < m_board.size(); ++x) {
        for (int y = 0; y < m_board.size(); ++y) {
            if (m_board.pieceAt(x, y) == GomokuBoard::Empty) {
                int score = evaluatePosition(x, y, GomokuBoard::White);
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
int GameWindow::evaluatePosition(int x, int y, GomokuBoard::Piece aiPiece) {
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

        // 检测人类玩家连子（新增完整逻辑）
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
        if (aiCount >= 4) score += 100001;       // 形成四连
        else if (aiCount == 3 && !aiBlocked) score += 5000;  // 活三
        else if (aiCount == 2 && !aiBlocked) score += 500;   // 活二
    }

    // 中心区域加分（避免总选角落）
    int center = m_board.size() / 2;
    int distanceToCenter = std::abs(x - center) + std::abs(y - center);
    score += (m_board.size() - distanceToCenter) * 10;  // 离中心越近分越高

    return score;
}void GameWindow::drawBoard(QPainter &painter) {
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
